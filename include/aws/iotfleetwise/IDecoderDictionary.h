// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aws/iotfleetwise/IDecoderManifest.h"
#include <unordered_map>
#include <unordered_set>

namespace Aws
{
namespace IoTFleetWise
{

/**
 * @brief This struct define the necessary information required to decode CAN frame
 *
 * nodeID: VSS ID of the CAN node. This is not the physical Channel ID.
 * collectType: specify whether the message is intended to be decoded or kept as raw or both
 * format: CAN message format specifying the frame ID, number of bytes and whether it's Multiplexed.
 * Note the format only contains the signals intended to be collected.
 */
struct CANMessageDecoderMethod
{
    CANMessageFormat format;
};

/**
 * @brief Base class of Decoder Dictionary. It contains a set of signal IDs to collect
 */
struct DecoderDictionary
{
    DecoderDictionary() = default;
    DecoderDictionary( std::unordered_set<SignalID> signalIDs )
        : signalIDsToCollect( std::move( signalIDs ) )
    {
    }
    virtual ~DecoderDictionary() = default;
    std::unordered_set<SignalID> signalIDsToCollect;
};

/**
 * @brief CAN decoder dictionary to be used to decode CAN Frame message to signals. This dictionary comes from
 * CollectionScheme Management
 *
 * canMessageDecoderMethod is a two dimension map. The top layer index is CANChannelNumericID; second layer index is
 * CAN Frame ID which is the CAN Arbitration ID
 * signalIDsToCollect is an unordered_set to specify which SignalID to be collected based on the CollectionScheme
 * pidMessageDecoderMethod is a map from OBD-II PID to its decoding method
 */
struct CANDecoderDictionary : DecoderDictionary
{
    using CANMsgDecoderMethodType =
        std::unordered_map<CANChannelNumericID, std::unordered_map<CANRawFrameID, CANMessageDecoderMethod>>;
    CANDecoderDictionary() = default;
    CANDecoderDictionary( CANMsgDecoderMethodType canMsgDecoderMethodIn,
                          std::unordered_set<SignalID> signalIDsToCollectIn )
        : DecoderDictionary( std::move( signalIDsToCollectIn ) )
        , canMessageDecoderMethod( std::move( canMsgDecoderMethodIn ) )
    {
    }
    CANMsgDecoderMethodType canMessageDecoderMethod;
};

/**
 * @brief define shared pointer type for decoder dictionary
 */
using ConstDecoderDictionaryConstPtr = const std::shared_ptr<const DecoderDictionary>;

#ifdef FWE_FEATURE_VISION_SYSTEM_DATA
/**
 * @brief In one decoder manifest never more complex types can be listed
 */
const uint32_t MAX_COMPLEX_TYPES = 5000;

/**
 * @brief Complex Data Decoder Dictionary can be used to decode multiple data source that can also send
 *          bigger messages. A tree is used in this decoding dictionary. To decode for example a flat binary
 *          buffer the  tree can be traversed in a depth first manner and every time a leaf is found the next byte(s)
 *          of the buffer can be consumed.
 */
struct ComplexDataDecoderDictionary : DecoderDictionary
{
    using ComplexDataDecoderMethodType =
        std::unordered_map<InterfaceID, std::unordered_map<ComplexDataMessageId, ComplexDataMessageFormat>>;

    ComplexDataDecoderDictionary() = default;
    ComplexDataDecoderDictionary( ComplexDataDecoderMethodType complexMsgDecoderMethod,
                                  std::unordered_set<SignalID> signalIDsToCollectIn )
        : DecoderDictionary( std::move( signalIDsToCollectIn ) )
        , complexMessageDecoderMethod( std::move( complexMsgDecoderMethod ) )
    {
    }
    ComplexDataDecoderMethodType complexMessageDecoderMethod;
};
#endif

struct CustomDecoderDictionary : DecoderDictionary
{
    CustomDecoderDictionary() = default;
    using CustomDecoderMethodType =
        std::unordered_map<InterfaceID, std::unordered_map<CustomSignalDecoder, CustomSignalDecoderFormat>>;
    CustomDecoderMethodType customDecoderMethod;
};

} // namespace IoTFleetWise
} // namespace Aws
