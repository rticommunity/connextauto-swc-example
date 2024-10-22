# Software Component : Example

This repo uses the common data model, interfaces, build system, and component
launcher provided by the standalone **databus** project repo.

It's primary purpose to exercise and illustrate how to build  components (i.e. 
applications) for various RTI Connext SDKs using a single `CMakeLists.txt` file.

- RTI Connext DDS Professional (*pro*)
- RTI Connext DDS Micro (*micro*)

The example consists of the following applications

- `pro_publisher` : publishes dummy data to the `Camera` topic using *pro*
- `micro_subscriber` : subscribes to the `Camera` topic using *micro*

## Assumptions

- `$DATABUSHOME` refers to the location of the **databus** project repository

## Files Overview

The files in this repo are modified example files, generated using the `rtiddsgen` utility for the datatype `dds::sensing::Camera`, defined in `$DATABUSHOME/res/types/data/sensing/Camera_t.idl`.

### Applications

[pro_publisher.cxx](pro_pub/pro_publisher.cxx):

- the C++ publisher example generated by *RTI Connext DDS Professional*'s `rtiddsgen`
- the generated example has been modified to use the constants defined in `$DATABUSHOME/res/types/services/Drive_t.idl` for the `Camera` type and topic.

[micro_subscriber.c](micro_sub/micro_subscriber.c):

- the C subscriber example generated by *RTI Connext DDS Micro*'s `rtiddsgen`
- the generated example has been modified to use the constants defined in `$DATABUSHOME/res/types/services/Drive_t.idl` for the `Camera` type and topic.

# Building the Applications

This repo uses the *common build system* provided by the databus project repo located at `$DATABUSHOME`.

## Generate the build system for an RTI Connext SDK

From this git repo's top-level directory:

- To generate the build system for *RTI Connext DDS Professional*, e.g.

      $DATABUSHOME/bin/build-gen.sh pro x64Linux4gcc7.3.0 Debug

- To generate the build system for *RTI Connext DDS Micro*, e.g.

      $DATABUSHOME/bin/build-gen.sh micro x64Linux4gcc7.3.0 Release

This step creates a `build/` directory in the project top-level directory. The `build/` directory contains a shell script to build for the specified target platform and build kind.

For more details, please refer to the documentation on the *common build system* located at `$DATABUSHOME/doc/Build.md`.

## Building this repo using the generated build system

From this git repo's top-level directory, run the generated build script to build the datatypes:

- To build for *RTI Connext DDS Professional*, e.g.:

      ./build/pro-x64Linux4gcc7.3.0-Debug.sh

- To build for *RTI Connext DDS Micro*, e.g.:

      ./build/micro-x64Linux4gcc7.3.0-Release.sh



# Running the Applications

This repo uses the *common component launcher* provided by the databus project repo located at `$DATABUSHOME`. For documentation on the `$DATABUSHOME/bin/run` launcher utility, please refer to the documentation located at `$DATABUSHOME/doc/Run.md`.

Open two terminal windows...

- Launch the `pro_publisher` in terminal #1 

      $DATABUSHOME/bin/run Drive build/pro/x64Linux4gcc7.3.0/Debug/pro_publisher [domain_id] [sample_count]

   You should see output like:

        :
        Writing Camera, count 0
        Writing Camera, count 1
        Writing Camera, count 3
        :

- Launch the `micro_subscriber` in terminal #2

      $DATABUSHOME/bin/run Drive build/micro/x64Linux4gcc7.3.0/Release/micro_subscriber [-domain <id>]

   You should see output like:

        Running for 24 hours, press Ctrl-C to exit
        Matched a publisher

        Valid sample received

        Valid sample received
        
        :
        
Press ^C in a terminal window to terminate.

---
(C) Copyright 2020-2022 Real-Time Innovations, Inc.  All rights reserved.

The use of this software is governed by the terms specified in the RTI Labs License Agreement, available at https://www.rti.com/terms/RTILabs. 

By accessing, downloading, or otherwise using this software, you agree to be bound by those terms.
