#******************************************************************************
# (C) Copyright 2020-2022 Real-Time Innovations, Inc.  All rights reserved.
#
# The use of this software is governed by the terms specified in the 
# RTI Labs License Agreement, available at https://www.rti.com/terms/RTILabs. 
#
# By accessing, downloading, or otherwise using this software, you agree to 
# be bound by those terms.
#*****************************************************************************

cmake_minimum_required(VERSION 3.20)
project(connextauto-swc-example)

if(NOT databus_SOURCE_DIR) # "databus" project is NOT defined in current build
    include(${DATABUS_PROJECT_DIR}/build-helpers/cmake/init.cmake)
endif()

#-----------------------------------------------------------------------------

set(APPLICATIONS)

if("${RTI_CONNEXT_SDK}" MATCHES "pro")
    #-------------------------------------------------------------------------
    # Connext Version: Professional
    #-------------------------------------------------------------------------
    set(APPLICATIONS pro_publisher)

    add_executable(pro_publisher
        "${CMAKE_CURRENT_SOURCE_DIR}/pro_pub/pro_publisher.cxx"
        )

    target_link_libraries(pro_publisher
        PRIVATE
        C++_${RTI_CONNEXT_SDK}_types_lib
        )

elseif("${RTI_CONNEXT_SDK}" MATCHES "micro")
    #-------------------------------------------------------------------------
    # Connext Version: Micro
    #-------------------------------------------------------------------------
    set(APPLICATIONS micro_subscriber)

    add_executable(micro_subscriber
        "${CMAKE_CURRENT_SOURCE_DIR}/micro_sub/micro_subscriber.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/micro_sub/Application.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/micro_sub/Application.h"
        )

    target_link_libraries(micro_subscriber
        PRIVATE
        C_${RTI_CONNEXT_SDK}_types_lib
        RTIConnextDDSMicro::discdpse
        RTIConnextDDSMicro::discdpde
        RTIConnextDDSMicro::whsm
        RTIConnextDDSMicro::rhsm
        )

else()

    message(FATAL_ERROR "Variable RTI_CONNEXT_SDK needs to be either 'pro*' or 'micro*'")

endif()

#-----------------------------------------------------------------------------

install(
    TARGETS ${APPLICATIONS}
    CONFIGURATIONS Debug Release
    RUNTIME DESTINATION "${INSTALL_RUNTIME_OUTPUT_DIRECTORY}"
    ARCHIVE DESTINATION "${INSTALL_ARCHIVE_OUTPUT_DIRECTORY}"
    LIBRARY DESTINATION "${INSTALL_LIBRARY_OUTPUT_DIRECTORY}"
    )

#-----------------------------------------------------------------------------
