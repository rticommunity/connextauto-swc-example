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



# Building the example applications

The executables are built as part of a top-level build. 
Please refer to the top-level [README](../../README.md).


# Running the example applications

- Pro: 
        
        ./build/pro/x64Darwin17clang9.0/Debug/pro_publisher

- Micro: 
        
        ./build/micro/x64Darwin17clang9.0/Debug/micro_subscriber

- Micro2: 
        
        ./build/micro2/x64Darwin17clang9.0/Debug/micro_subscriber


# Observing the example applications using RTI Tools

Start *RTI Admin Console*. You should see the pub;isher and subscriber applications.

Start *rtiddsspy*. You should be able to subscribe to the data from the publisher.
