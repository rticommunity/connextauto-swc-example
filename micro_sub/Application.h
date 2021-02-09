
#ifndef Application_h
#define Application_h

#include "rti_me_c.h"

/*e \dref_Example_Config_UseReliableQos
* Define USE_RELIABLE_QOS to use reliable
* reliability on the DataReader; otherwise, use
* best-effort reliability by default.
*/

#define USE_RELIABLE_QOS

/*e \dref_Example_Config_UseSampleFilter
* Define USE_SAMPLE_FILTER to filter samples
* using call-backs on the DataReader's listener
* and, in case the first field of your type is not
* a DDS_Long called "id", modify
* "CameraImageSubscriber_filter_sample",
* "CameraImageSubscriber_on_before_sample_deserialize",
* "CameraImageSubscriber_deserialize_unsigned_long", and
* "CameraImageSubscriber_filter_sample" functions to
* correctly filter by the field you want to use for filtering.
*/

/*#define USE_SAMPLE_FILTER*/

/*e \dref_Example_Config_FilterOnDeserialize
* Define FILTER_ON_DESERIALIZE to enable
* filtering on call-back on_before_sample_deserialize;
* otherwise use call-back on_before_sample_commit
* by default. */

/*#define FILTER_ON_DESERIALIZE*/

struct Application {
    DDS_DomainParticipant *participant;
    char topic_name[255];
    char type_name[255];
    DDS_Long sleep_time;
    DDS_Long count;
    DDS_Topic *topic;
};

extern void 
Application_help(char *appname);

extern struct Application*
Application_create(const char *local_participant_name,
const char *remote_participant_name,
DDS_Long domain_id, char *udp_intf, char *peer, 
DDS_Long sleep_time, DDS_Long count);

extern DDS_ReturnCode_t
Application_enable(struct Application *application);

extern void
Application_delete(struct Application *application);

#endif
