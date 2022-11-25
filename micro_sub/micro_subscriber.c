/******************************************************************************
(C) Copyright 2020-2022 Real-Time Innovations, Inc.  All rights reserved.

The use of this software is governed by the terms specified in the 
RTI Labs License Agreement, available at https://www.rti.com/terms/RTILabs. 

By accessing, downloading, or otherwise using this software, you agree to 
be bound by those terms.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Application.h"
#include "rti_me_c.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "res/types/data/sensing/Camera_t.h"
#include "res/types/data/sensing/Camera_tSupport.h"
#include "res/types/data/sensing/Camera_tPlugin.h"

/* See Wire Protocol Specification on http://www.omg.org/spec/DDSI-RTPS/2.2/
 * for more details about CDR encapsulation.
 */

/*ci \brief Unsigned Long type size */
#define UNSIGNED_LONG_SIZE  4

void
dds_sensing_CameraSubscriber_on_data_available(void *listener_data,
        DDS_DataReader * reader)
{
    dds_sensing_CameraDataReader *hw_reader = dds_sensing_CameraDataReader_narrow(reader);
    DDS_ReturnCode_t retcode;
    struct DDS_SampleInfo *sample_info = NULL;
    dds_sensing_Camera *sample = NULL;

    struct DDS_SampleInfoSeq info_seq = 
        DDS_SEQUENCE_INITIALIZER;
    struct dds_sensing_CameraSeq sample_seq =
        DDS_SEQUENCE_INITIALIZER;

    const DDS_Long TAKE_MAX_SAMPLES = 32;
    DDS_Long i;

    retcode = dds_sensing_CameraDataReader_take(hw_reader,
            &sample_seq, &info_seq, TAKE_MAX_SAMPLES, 
            DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode != DDS_RETCODE_OK)
    {
        printf("failed to take data, retcode(%d)\n", retcode);
        goto done;
    }

    /* Print each valid sample taken */
    for (i = 0; i < dds_sensing_CameraSeq_get_length(&sample_seq); ++i)
    {
        sample_info = DDS_SampleInfoSeq_get_reference(&info_seq, i);

        if (sample_info->valid_data)
        {

            sample = dds_sensing_CameraSeq_get_reference(&sample_seq, i);
            printf("\nValid sample received\n");
            /* TODO read and process sample attributes here */ 

        }
        else
        {
            printf("\nSample received\n\tINVALID DATA\n");
        }
    }

    dds_sensing_CameraDataReader_return_loan(hw_reader, &sample_seq, &info_seq);

done:
    dds_sensing_CameraSeq_finalize(&sample_seq);
    DDS_SampleInfoSeq_finalize(&info_seq);
}

void
dds_sensing_CameraSubscriber_on_subscription_matched(void *listener_data,
        DDS_DataReader * reader,
        const struct
        DDS_SubscriptionMatchedStatus
        *status)
{
    if (status->current_count_change > 0)
    {
        printf("Matched a publisher\n");
    }
    else if (status->current_count_change < 0)
    {
        printf("Unmatched a publisher\n");
    }
}

#ifdef USE_SAMPLE_FILTER
#ifdef FILTER_ON_DESERIALIZE
/*i
 * \brief Helper function to deserialize an unsigned long
 *
 * \param[in]  src_buffer      Pointer to CDR stream buffer
 * \param[in]  need_byte_swap  Indicates whether it is needed to swap bytes
 * \param[out] instance        Deserialized unsigned long
 */
void
dds_sensing_CameraSubscriber_deserialize_unsigned_long(char **src_buffer,
        RTI_BOOL need_byte_swap,
        DDS_UnsignedLong *instance)
{
    RTI_INT32 i;
    if (need_byte_swap)
    {
        for (i = 3; i >= 0; --i)
        {
            *((RTI_INT8*)instance + i) = *((*src_buffer)++);
        }
    }
    else
    {
        *instance = *(RTI_UINT32*)(*src_buffer);
        (*src_buffer) += CDR_LONG_SIZE;
    }
}

/*i
 * \brief Implementation of \ref DDS_DataReaderListener::on_before_sample_deserialize
 */
DDS_Boolean
dds_sensing_CameraSubscriber_on_before_sample_deserialize(
        void *listener_data,
        DDS_DataReader *reader,
        struct NDDS_Type_Plugin *plugin,
        struct CDR_Stream_t *stream,
        DDS_Boolean *dropped)
{
    DDS_Boolean result = DDS_BOOLEAN_FALSE;
    DDS_Long id = 0;
    RTI_BOOL need_byte_swap = DDS_BOOLEAN_FALSE;
    char *src_buffer = NULL;

    need_byte_swap = stream->need_byte_swap;
    src_buffer = CDR_Stream_get_current_position_ptr(stream);

    /* deserialize only field 'id', instead of the whole sample */

    if (!CDR_Stream_check_size(stream, UNSIGNED_LONG_SIZE))
    {
        printf("Failed to deserialize id. The stream is too short, missing data\n");
        goto done;
    }

    /* Note primitive types must be aligned to their length in the CDR stream.
     * For example, a long must start on a 4-byte boundary. The boundaries are
     * counted from the start of the CDR stream.
     * As the sample 'id' is the first data in the stream it is already aligned.
     * Position 0 (beginning of the stream) is aligned to 4 (size of long).
     *
     * NOTE: If you want to use a different field for filtering (e.g. you type does
     * not have a field called id as first field), you will need to reimplement this
     * function and dds_sensing_CameraSubscriber_deserialize_unsigned_long
     * to match your type.
     */
    dds_sensing_CameraSubscriber_deserialize_unsigned_long(&src_buffer,
            need_byte_swap,
            (DDS_UnsignedLong*)&id);

    /* filter the sample */
    *dropped = (id % 2 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;

    if (*dropped)
    {
        printf("\nSample filtered, before deserialize...\n\tDROPPED - id: %d\n",
                id);
    }

    result = DDS_BOOLEAN_TRUE;
done:
    return result;
}

#else

/*i
 * \brief Helper function to filter an dds_sensing_Camera sample
 *
 * \param[in]  sample       A dds_sensing_Camera data sample to filter
 * \param[out] drop_sample  Out parameter determining whether the sample
 *                          should be filtered out or not.
 */
void 
dds_sensing_CameraSubscriber_filter_sample(dds_sensing_Camera *sample,
        DDS_Boolean *drop_sample)
{
    /* Example filter: drop samples with even-numbered count in id */
    /* NOTE: If field "id" does not exist in your data type, change "id"
     * by the field you want to use for filtering.
     */
    *drop_sample = (sample->id % 2 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;
}

/*i
 * \brief Implementation of \ref DDS_DataReaderListener::on_before_sample_commit
 */
DDS_Boolean
dds_sensing_CameraSubscriber_on_before_sample_commit(
        void *listener_data,
        DDS_DataReader *reader,
        const void *const sample,
        const struct DDS_SampleInfo *const sample_info,
        DDS_Boolean *dropped)
{
    dds_sensing_Camera *hw_sample = (dds_sensing_Camera *)sample;

    dds_sensing_CameraSubscriber_filter_sample(hw_sample, dropped);

    if (*dropped)
    {
        printf("\nSample filtered, before commit...\n");
    }

    return DDS_BOOLEAN_TRUE;
}
#endif /* FILTER_ON_DESERIALIZE */
#endif /* USE_SAMPLE_FILTER */

int
subscriber_main_w_args(DDS_Long domain_id, char *udp_intf, char *peer,
        DDS_Long sleep_time, DDS_Long count)
{
    DDS_Subscriber *subscriber;
    DDS_DataReader *datareader;
    struct DDS_DataReaderQos dr_qos = DDS_DataReaderQos_INITIALIZER;
    DDS_ReturnCode_t retcode;
    struct DDS_DataReaderListener dr_listener =
        DDS_DataReaderListener_INITIALIZER;
    struct Application *application;

    application = Application_create(
            "Example : Camera (micro)", 
            "Example : Camera",
            domain_id, udp_intf, peer, sleep_time, count);

    if (application == NULL)
    {
        return 0;
    }

    subscriber =
        DDS_DomainParticipant_create_subscriber(application->participant,
                &DDS_SUBSCRIBER_QOS_DEFAULT,
                NULL, DDS_STATUS_MASK_NONE);
    if (subscriber == NULL)
    {
        printf("subscriber == NULL\n");
        goto done;
    }

#ifdef USE_SAMPLE_FILTER
    /* choose one callback to enable */
#ifdef FILTER_ON_DESERIALIZE
    dr_listener.on_before_sample_deserialize =
        dds_sensing_CameraSubscriber_on_before_sample_deserialize;
#else
    dr_listener.on_before_sample_commit = 
        dds_sensing_CameraSubscriber_on_before_sample_commit;
#endif

#endif

    dr_listener.on_data_available = dds_sensing_CameraSubscriber_on_data_available;
    dr_listener.on_subscription_matched =
        dds_sensing_CameraSubscriber_on_subscription_matched;

    /* Publisher sends samples with id = 0 or id = 1, so 2 instances maximum.
     * But in case filtering is done, all samples with 'id = 0' are
     * filtered so only one instance is needed.
     */
#ifdef USE_SAMPLE_FILTER
    dr_qos.resource_limits.max_instances = 1;
#else
    dr_qos.resource_limits.max_instances = 2;
#endif

    dr_qos.resource_limits.max_samples_per_instance = 32;
    dr_qos.resource_limits.max_samples = dr_qos.resource_limits.max_instances *
        dr_qos.resource_limits.max_samples_per_instance;
    /* if there are more remote writers, you need to increase these limits */
    dr_qos.reader_resource_limits.max_remote_writers = 10;
    dr_qos.reader_resource_limits.max_remote_writers_per_instance = 10;
    dr_qos.history.depth = 32;

    /* Reliability QoS */
#ifdef USE_RELIABLE_QOS
    dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
#else
    dr_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
#endif

    datareader = DDS_Subscriber_create_datareader(subscriber,
            DDS_Topic_as_topicdescription
            (application->topic), &dr_qos,
            &dr_listener,
            DDS_DATA_AVAILABLE_STATUS |
            DDS_SUBSCRIPTION_MATCHED_STATUS);

    if (datareader == NULL)
    {
        printf("datareader == NULL\n");
        goto done;
    }

    retcode = Application_enable(application);
    if (retcode != DDS_RETCODE_OK)
    {
        printf("failed to enable application\n");
        goto done;
    }

    if (application->count != 0)
    {
        printf("Running for %d seconds, press Ctrl-C to exit\n",
                application->count);
        OSAPI_Thread_sleep(application->count * 1000);
    }
    else
    {
        int sleep_loop_count =  (24 * 60 * 60) / 2000;
        int sleep_loop_left = (24 * 60 * 60) % 2000;

        printf("Running for 24 hours, press Ctrl-C to exit\n");

        while (sleep_loop_count)
        {
            OSAPI_Thread_sleep(2000  * 1000); /* sleep is in ms */
            --sleep_loop_count;
        }

        OSAPI_Thread_sleep(sleep_loop_left * 1000);
    }

done:

    Application_delete(application);

    DDS_DataReaderQos_finalize(&dr_qos);

    return 0;
}

#if !(defined(RTI_VXWORKS) && !defined(__RTP__))
int
main(int argc, char **argv)
{
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = NULL;
    char *udp_intf = NULL;
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;

    for (i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-domain"))
        {
            ++i;
            if (i == argc)
            {
                printf("-domain <domain_id>\n");
                return -1;
            }
            domain_id = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-udp_intf"))
        {
            ++i;
            if (i == argc)
            {
                printf("-udp_intf <interface>\n");
                return -1;
            }
            udp_intf = argv[i];
        }
        else if (!strcmp(argv[i], "-peer"))
        {
            ++i;
            if (i == argc)
            {
                printf("-peer <address>\n");
                return -1;
            }
            peer = argv[i];
        }
        else if (!strcmp(argv[i], "-sleep"))
        {
            ++i;
            if (i == argc)
            {
                printf("-sleep_time <sleep_time>\n");
                return -1;
            }
            sleep_time = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-count"))
        {
            ++i;
            if (i == argc)
            {
                printf("-count <count>\n");
                return -1;
            }
            count = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-h"))
        {
            Application_help(argv[0]);
            return 0;
        }
        else
        {
            printf("unknown option: %s\n", argv[i]);
            return -1;
        }
    }

    return subscriber_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#elif defined(RTI_VXWORKS)
int
subscriber_main(void)
{
    /* Explicitly configure args below */
    DDS_Long domain_id = 44;
    char *peer = "10.10.65.103";
    char *udp_intf = NULL;
    DDS_Long sleep_time = 1000;
    DDS_Long count = 0;

    return subscriber_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#endif
