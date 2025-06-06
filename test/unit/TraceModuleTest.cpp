
// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#include "aws/iotfleetwise/TraceModule.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace Aws
{
namespace IoTFleetWise
{

TEST( TraceModuleTest, TraceModulePrint )
{
    // Make sure to clear any previous data since this is a singleton
    TraceModule::get().startNewObservationWindow();

    TraceModule::get().setVariable( TraceVariable::READ_SOCKET_FRAMES_0, 10 );
    TraceModule::get().setVariable( TraceVariable::READ_SOCKET_FRAMES_0, 20 );
    TraceModule::get().setVariable( TraceVariable::READ_SOCKET_FRAMES_0, 15 );

    // S&F Metrics
    TraceModule::get().setVariable( TraceVariable::DATA_STORE_BYTES, 15 );
    TraceModule::get().setVariable( TraceVariable::DATA_STORE_SIGNAL_COUNT, 1 );
    TraceModule::get().setVariable( TraceVariable::DATA_FORWARD_BYTES, 5 );
    TraceModule::get().setVariable( TraceVariable::DATA_FORWARD_SIGNAL_COUNT, 1 );
    TraceModule::get().setVariable( TraceVariable::DATA_EXPIRED_BYTES, 5 );
    TraceModule::get().setVariable( TraceVariable::DATA_DROPPED_BYTES, 5 );
    TraceModule::get().setVariable( TraceVariable::DATA_STORE_ERROR, 1 );
    TraceModule::get().setVariable( TraceVariable::DATA_FORWARD_ERROR, 1 );

    TraceModule::get().sectionBegin( TraceSection::BUILD_MQTT );
    std::this_thread::sleep_for( std::chrono::milliseconds( 4 ) );
    TraceModule::get().sectionEnd( TraceSection::BUILD_MQTT );

    std::this_thread::sleep_for( std::chrono::milliseconds( 9 ) );

    TraceModule::get().sectionBegin( TraceSection::BUILD_MQTT );
    std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    TraceModule::get().sectionEnd( TraceSection::BUILD_MQTT );

    std::this_thread::sleep_for( std::chrono::milliseconds( 11 ) );

    TraceModule::get().sectionBegin( TraceSection::BUILD_MQTT );
    std::this_thread::sleep_for( std::chrono::milliseconds( 6 ) );
    TraceModule::get().sectionEnd( TraceSection::BUILD_MQTT );

    ASSERT_EQ( TraceModule::get().getVariableMax( TraceVariable::READ_SOCKET_FRAMES_0 ), 20 );
    TraceModule::get().print();
    TraceModule::get().startNewObservationWindow();
    TraceModule::get().print();
    TraceModule::get().startNewObservationWindow();
}

} // namespace IoTFleetWise
} // namespace Aws
