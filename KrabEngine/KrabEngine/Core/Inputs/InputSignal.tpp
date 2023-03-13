#pragma once

namespace SignalSystem
{
    template<typename... Args>
    Signal<Args...>::~Signal()
    {
        for (auto& connection : m_observers) {
            match(connection->m_state,
                  [&](typename ConnectionType::Managed) { delete connection; },
                  [&](typename ConnectionType::Scoped) { connection->m_state = typename ConnectionType::Zombified{}; },
                  [&](typename ConnectionType::Zombified) { delete connection; });
        }
    }


    template<typename... Args>
    void Signal<Args...>::connect(const Signal::Callback &callback) const
    {
        auto* connection = new ConnectionType(callback,
                                              typename ConnectionType::StateType{ typename ConnectionType::Managed {} },
                                              const_cast<Signal<Args...>*>(this));
        m_observers.push_back(connection);
    }

    template<typename... Args>
    template<typename Type>
    void Signal<Args...>::connect(Type *instance, void (Type::*fn)(Args...)) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        connect(callback);
    }

    template<typename... Args>
    template<typename Type>
    void Signal<Args...>::connect(Type *instance, void (Type::*fn)(Args...) const) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        connect(callback);
    }

    template<typename... Args>
    typename Signal<Args...>::ScopedConnectionType Signal<Args...>::connectScoped(const Callback &callback) const
    {
        auto* connection = new ConnectionType (callback,
                                               typename ConnectionType::StateType{ typename ConnectionType::Scoped {} },
                                               const_cast<Signal<Args...>*>(this));
        m_observers.push_back(connection);
        return ScopedConnectionType{ connection };
    }

    template<typename... Args>
    template<typename Type>
    typename Signal<Args...>::ScopedConnectionType Signal<Args...>::connectScoped(Type *instance, void (Type::*fn)(Args...)) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        return connectScoped(callback);
    }

    template<typename... Args>
    template<typename Type>
    typename Signal<Args...>::ScopedConnectionType Signal<Args...>::connectScoped(Type *instance, void (Type::*fn)(Args...) const) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        return connectScoped(callback);
    }

    template<typename... Args>
    typename Signal<Args...>::SlotConnectionType Signal<Args...>::connectSlot(const Signal::Callback &callback) const
    {
        auto signal = const_cast<Signal<Args...>*>(this);
        return SlotConnectionType{ *signal, callback };
    }

    template<typename... Args>
    template<typename Type>
    typename Signal<Args...>::SlotConnectionType Signal<Args...>::connectSlot(Type *instance, void (Type::*fn)(Args...)) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        return connectSlot(callback);
    }

    template<typename... Args>
    template<typename Type>
    typename Signal<Args...>::SlotConnectionType Signal<Args...>::connectSlot(Type *instance, void (Type::*fn)(Args...) const) const
    {
        auto callback = [instance, fn](Args... args) { (instance->*fn)(args...); };
        return connectSlot(callback);
    }

    template<typename... Args>
    void Signal<Args...>::Disconnect(Signal::ConnectionType *connection) const
    {
        auto connectionIt = std::find_if(m_observers.begin(), m_observers.end(),
                                         [&](ConnectionType* c) { return c == connection; });

        if (connectionIt == m_observers.end())
            throw std::runtime_error("Connection is not attached to this signal");

        match(connection->m_state,
              [&](typename ConnectionType::Managed) { throw std::runtime_error("Connection is managed typed"); },
              [&](typename ConnectionType::Scoped) { m_observers.erase(connectionIt); },
              [&](typename ConnectionType::Zombified) {});

        delete connection;
        connection = nullptr;
    }

    template<typename... Args>
    void Signal<Args...>::Emit(const Args &... args) const
    {
        auto connections = m_observers;
        for (auto& connection : connections)
            connection->m_callback(args...);
    }

    template<typename... Args>
    void Signal<Args...>::operator()(const Args &... args) const
    {
        Emit(args...);
    }
}