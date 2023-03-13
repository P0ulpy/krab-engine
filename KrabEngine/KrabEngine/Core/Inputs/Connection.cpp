//
// Created by Admin on 30/01/2023.
//

#include "Connection.hpp"

#include <utility>
#include "InputSignal.hpp"

namespace SignalSystem
{
    ConnectionSignal::ConnectionSignal(ConnectionSignal::Callback callback, ConnectionSignal::StateType state, InputSignal* signal)
            : m_callback(std::move(callback)), m_state(state), m_signal(signal)

    {}

    ScopedConnectionSignal::ScopedConnectionSignal() {

    }

    ScopedConnectionSignal::ScopedConnectionSignal(ScopedConnectionSignal&& other) {
        std::swap(m_connection, other.m_connection);
    }

    ScopedConnectionSignal& ScopedConnectionSignal::operator=(ScopedConnectionSignal&& other) {
        std::swap(m_connection, other.m_connection);
        return *this;
    }

    ScopedConnectionSignal::~ScopedConnectionSignal() {
        disconnect();
    }

    void ScopedConnectionSignal::disconnect() const {
        if (nullptr != m_connection)
            m_connection->m_signal->Disconnect(m_connection);

    }

    ScopedConnectionSignal::ScopedConnectionSignal(ScopedConnectionSignal::EventType  eventType, ConnectionSignal *connection)
        : m_eventType(std::move(eventType))
        , m_connection(connection)
    {}

    SlotConnectionSignal::SlotConnectionSignal(InputSignal& signal, const EventType& eventType, const ConnectionSignal::Callback& callback) :
            m_scopedConnection(signal.connectScoped(eventType, [this, &callback]() {
                if (!m_semaphore.isDisabled())
                    return;

                callback();
            })),
            m_semaphore{}
    {}

    void SlotConnectionSignal::block() {
        m_semaphore.addBlocker();
    }

    void SlotConnectionSignal::unblock() {
        m_semaphore.removeBlocker();
    }

    auto SlotConnectionSignal::scopedBlock() {
        struct BlockScoped
        {
            explicit BlockScoped(SlotConnectionSignal& owner) : m_connection(owner)
            {
                m_connection.block();
            }

            ~BlockScoped()
            {
                m_connection.unblock();
            }

            SlotConnectionSignal& m_connection;
        };

        return BlockScoped{ *this };
    }
}


