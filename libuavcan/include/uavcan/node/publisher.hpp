/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#pragma once

#include <uavcan/node/generic_publisher.hpp>

namespace uavcan
{

template <typename DataType_>
class Publisher : protected GenericPublisher<DataType_, DataType_>
{
    typedef GenericPublisher<DataType_, DataType_> BaseType;

public:
    typedef DataType_ DataType;

    explicit Publisher(INode& node, MonotonicDuration tx_timeout = getDefaultTxTimeout(),
                       MonotonicDuration max_transfer_interval = TransferSender::getDefaultMaxTransferInterval())
        : BaseType(node, tx_timeout, max_transfer_interval)
    {
#if UAVCAN_DEBUG
        assert(getTxTimeout() == tx_timeout);  // Making sure default values are OK
#endif
        StaticAssert<DataTypeKind(DataType::DataTypeKind) == DataTypeKindMessage>::check();
    }

    int broadcast(const DataType& message)
    {
        return BaseType::publish(message, TransferTypeMessageBroadcast, NodeID::Broadcast);
    }

    int broadcast(const DataType& message, TransferID tid)
    {
        return BaseType::publish(message, TransferTypeMessageBroadcast, NodeID::Broadcast, tid);
    }

    int unicast(const DataType& message, NodeID dst_node_id)
    {
        if (!dst_node_id.isUnicast())
        {
            assert(0);
            return -ErrInvalidParam;
        }
        return BaseType::publish(message, TransferTypeMessageUnicast, dst_node_id);
    }

    static MonotonicDuration getDefaultTxTimeout() { return MonotonicDuration::fromMSec(5); } // 5 ms --> 200 Hz max

    using BaseType::init;
    using BaseType::getTransferSender;
    using BaseType::getMinTxTimeout;
    using BaseType::getMaxTxTimeout;
    using BaseType::getTxTimeout;
    using BaseType::setTxTimeout;
    using BaseType::getNode;
};

}
