#pragma once

namespace SignalSystem
{
    template<typename... Args>
    Connection<Args...>::Connection(Callback callback, StateType state, SignalPtr signal)
            : m_callback(callback), m_state(state), m_signal(signal)
    {}

    template<typename... Args>
    ScopedConnection<Args...>::ScopedConnection()
            : m_connection(nullptr)
    {}

    template<typename... Args>
    ScopedConnection<Args...>::ScopedConnection(ScopedConnection&& other) {
        std::swap(m_connection, other.m_connection);
    }

    template<typename... Args>
    ScopedConnection<Args...>& ScopedConnection<Args...>::operator=(ScopedConnection&& other) {
        std::swap(m_connection, other.m_connection);
        return *this;
    }

    template<typename... Args>
    ScopedConnection<Args...>::~ScopedConnection() {
        disconnect();
    }

    template<typename... Args>
    void ScopedConnection<Args...>::disconnect() const {
        if (nullptr != m_connection)
            m_connection->m_signal->Disconnect(m_connection);

    }

    template<typename... Args>
    SlotConnection<Args...>::SlotConnection(Signal<Args...> &signal, const typename Signal<Args...>::Callback &callback)
            : m_scopedConnection(signal.connectScoped([this, &callback]() {
        if (!m_semaphore.isDisabled())
            return;

        callback();
    })),
              m_semaphore{}
    {}

    template<typename... Args>
    void SlotConnection<Args...>::block() {
        m_semaphore.addBlocker();
    }

    template<typename... Args>
    void SlotConnection<Args...>::unblock() {
        m_semaphore.removeBlocker();
    }

    template<typename... Args>
    auto SlotConnection<Args...>::scopedBlock() {
        struct BlockScoped
        {
            explicit BlockScoped(SlotConnection<Args...>& owner) : m_connection(owner)
            {
                m_connection.block();
            }

            ~BlockScoped()
            {
                m_connection.unblock();
            }

            SlotConnection<Args...>& m_connection;
        };

        return BlockScoped{ *this };
    }
}