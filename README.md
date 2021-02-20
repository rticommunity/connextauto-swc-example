(c) Copyright, Real-Time Innovations, 2020.  All rights reserved.
RTI grants Licensee a license to use, modify, compile, and create derivative
works of the software solely for use with RTI Connext DDS. Licensee may
redistribute copies of the software provided that all such copies are subject
to this license. The software is provided "as is", with no warranty of any
type, including any warranty for fitness for any purpose. RTI is under no
obligation to maintain or support the software. RTI shall not be liable for
any incidental or consequential damages arising out of the use or inability
to use the software.

# Example Components

This example show how to build example components (i.e. applications) using a 
single CMakeLists.txt file for various RTI Connext SDKs

- Connext Professional (*pro*)
- Connext Micro 3+ (*micro*)
- Connext Micro 2 (*micro2*)

The example consists of the following applications

- A publisher that uses *pro*, on a topic
- A subscriber that uses *micro* or *micro2* on the same topic


# Building the applications

For an *integrated build* (default), please change the working directory
to the *top-level* system project directory and follow the instructions
in `DATABUS_PROJECT_DIR`/[README](../../databus/README.md#building).

For a *standalone build* (advanced), please change the working directory
to *this* component project directory and follow the instructions
in `DATABUS_PROJECT_DIR`/[README](../../databus/README.md#building).

For more details, please refer to the *top-level* system
project [README](../README.md).


# Running the applications

If you did an *integrated build* (default), please change the working directory
to the *top-level* system project directory and follow the instructions
in `DATABUS_PROJECT_DIR`/[README](../../databus/README.md#running).

If you did a *standalone build* (advanced), please change the working directory
to *this* component project directory and follow the instructions
in `DATABUS_PROJECT_DIR`/[README](../../databus/README.md#running).

Assuming the working directory is the *top-level* system project directory,
you can run the applications as follows:

1. (Pro) In terminal #1, start a publisher built with Connext Pro SDK:
   e.g.

        databus/bin/run build/pro/x64Darwin17clang9.0/Debug/pro_publisher

   You should see output like:

        DATABUS_PROJECT_DIR: /Users/rajive/rti/connext-auto/databus
        Running : /Users/rajive/rti/connext-auto/build/pro/x64Darwin17clang9.0/Debug/pro_publisher ...
        DATABUS_PROJECT_DIR=/Users/rajive/rti/connext-auto/databus
        NDDS_QOS_PROFILES=/Users/rajive/rti/connext-auto/databus/res/qos/data/snippets/factory_qos.xml;/Users/rajive/rti/connext-auto/databus/res/qos/data/snippets/participant_qos.xml;/Users/rajive/rti/connext-auto/databus/res/qos/data/snippets/endpoint_qos.xml;/Users/rajive/rti/connext-auto/databus/res/qos/data/doa_qos.xml;/Users/rajive/rti/connext-auto/databus/res/qos/services/Drive_qos.xml;/Users/rajive/rti/connext-auto/databus/res/qos/systems/Drive-default_qos.xml;/Users/rajive/rti/connext-auto/databus/if/Drive_bus.xml;/Users/rajive/rti/connext-auto/databus/if/Sensing_svc.xml;/Users/rajive/rti/connext-auto/databus/if/Perception_svc.xml;/Users/rajive/rti/connext-auto/databus/if/Planning_svc.xml;/Users/rajive/rti/connext-auto/databus/if/Actuation_svc.xml;/Users/rajive/rti/connext-auto/databus/if/HMI_svc.xml;/Users/rajive/rti/connext-auto/databus/if/CAN_svc.xml
        Writing Camera, count 0
        Writing Camera, count 1
        Writing Camera, count 3
        :

2. (Micro2) In terminal #2, start a subscriber built with Connext Micro 2 SDK: 
   e.g. 

        build/micro2/x64Darwin17clang9.0/Debug/micro_subscriber

   You should see output like:

        Running for 24 hours, press Ctrl-C to exit
        Matched a publisher

        Valid sample received

        Valid sample received
        
        :


3. (Micro) In terminal #2, start a subscriber built with Connext Micro[3] SDK: 
   e.g. 

        build/micro/x64Darwin17clang9.0/Debug/micro_subscriber

   You should see output like:

        Running for 24 hours, press Ctrl-C to exit
        Matched a publisher

        Valid sample received

        Valid sample received

        :

        
Press ^C to terminate the apps.


# Observing the dataflows using RTI Tools

Start *RTI Admin Console*. You should see the pub;isher and subscriber applications.

Start *rtiddsspy*. You should be able to subscribe to the data from the publisher.
