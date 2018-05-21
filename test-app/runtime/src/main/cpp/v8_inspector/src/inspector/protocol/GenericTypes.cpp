// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/GenericTypes.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace GenericTypes {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "GenericTypes";
const char Metainfo::commandPrefix[] = "GenericTypes.";
const char Metainfo::version[] = "1.3";

// ------------- Enum values from params.


// ------------- Frontend notifications.

void Frontend::flush() {
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification) {
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
    public:
        DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
            : DispatcherBase(frontendChannel)
            , m_backend(backend)
            , m_fallThroughForNotFound(fallThroughForNotFound) {
        }
        ~DispatcherImpl() override { }
        DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
        HashMap<String, String>& redirects() {
            return m_redirects;
        }

    protected:
        using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
        using DispatchMap = protocol::HashMap<String, CallHandler>;
        DispatchMap m_dispatchMap;
        HashMap<String, String> m_redirects;


        Backend* m_backend;
        bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) {
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound) {
            return DispatchResponse::kFallThrough;
        }
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend) {
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("GenericTypes", std::move(dispatcher));
}

} // GenericTypes
} // namespace v8_inspector
} // namespace protocol
