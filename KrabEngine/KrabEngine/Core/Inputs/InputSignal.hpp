//
// Created by Admin on 22/01/2023.
//

#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <functional>
#include <map>
#include <tuple>
#include <vector>
#include <variant>

#include "Connection.hpp"
#include "InputConfig.hpp"
#include <stdexcept>

namespace SignalSystem
{
    class InputSignal {
        using ConnectionType = ConnectionSignal;
        using ScopedConnectionType = ScopedConnectionSignal;
        using SlotConnectionType = SlotConnectionSignal;

        using EventType = ScopedConnectionType::EventType;
        using Callback = typename ConnectionType::Callback;
        using Observers = std::vector<ConnectionType*>;
        using ObserversMap = std::map<EventType, Observers>;

    public:
        static InputSignal* Get() {
            static InputSignal m_inputSignal;

            return &m_inputSignal;
        }

        void connect(EventType eventType, const Callback& callback) const
        {
            auto* connection = new ConnectionType(callback,
                                                  typename ConnectionType::StateType{ typename ConnectionType::Managed {} },
                                                  const_cast<InputSignal*>(this));
            m_observers[eventType].push_back(connection);
        }
        template<typename Type>
        void connect(EventType eventType, Type* instance, void (Type::* fn)()) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            connect(eventType, callback);
        }
        template<typename Type>
        void connect(EventType eventType, Type* instance, void (Type::* fn)() const) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            connect(eventType, callback);
        }

        ScopedConnectionType connectScoped(EventType eventType, const Callback& callback) const
        {
            auto* connection = new ConnectionType (callback,
                                                   typename ConnectionType::StateType{ typename ConnectionType::Scoped {} },
                                                   const_cast<InputSignal*>(this));
            m_observers[eventType].push_back(connection);
            return ScopedConnectionType{ eventType, connection };
        }
        template<typename Type>
        ScopedConnectionType connectScoped(const EventType& eventType, Type* instance, void (Type::* fn)()) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            return connectScoped(eventType, callback);
        }
        template<typename Type>
        ScopedConnectionType connectScoped(const EventType& eventType, Type* instance, void (Type::* fn)() const) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            return connectScoped(eventType, callback);
        }

        SlotConnectionType connectSlot(EventType eventType, const Callback& callback) const
        {
            auto signal = const_cast<InputSignal*>(this);
            return SlotConnectionType{ *signal, eventType, callback };
        }
        template<typename Type>
        SlotConnectionType connectSlot(EventType eventType, Type* instance, void (Type::* fn)()) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            return connectSlot(eventType, callback);
        }
        template<typename Type>
        SlotConnectionType connectSlot(EventType eventType, Type* instance, void (Type::* fn)() const) const
        {
            auto callback = [instance, fn]() { (instance->*fn)(); };
            return connectSlot(eventType, callback);
        }

        void Disconnect(const EventType& eventType, ConnectionType* connection) const {
            auto it = m_observers.find(eventType);
            if (it != m_observers.end())
            {
                auto connectionIt = std::find_if(it->second.begin(), it->second.end(),
                                                 [&](ConnectionType* c) { return c == connection; });

                if (connectionIt == it->second.end())
                    throw std::runtime_error("Connection is not attached to this signal");

                match(connection->m_state,
                      [&](typename ConnectionType::Managed) { throw std::runtime_error("Connection is managed typed"); },
                      [&](typename ConnectionType::Scoped) { it->second.erase(connectionIt); },
                      [&](typename ConnectionType::Zombified) {});

                delete connection;
                connection = nullptr;
            }
        }

        void Disconnect(ConnectionType* connection) const {
            for (auto& observer : m_observers) {
                auto connectionIt = std::find_if(observer.second.begin(), observer.second.end(),
                                                 [&](ConnectionType* c) { return c == connection; });

                if (connectionIt != observer.second.end()) {
                    match(connection->m_state,
                          [&](typename ConnectionType::Managed) { throw std::runtime_error("Connection is managed typed"); },
                          [&](typename ConnectionType::Scoped) { observer.second.erase(connectionIt); },
                          [&](typename ConnectionType::Zombified) {});

                    delete connection;
                    connection = nullptr;
                    break;
                }
            }
        }

        void Emit(const EventType& eventType) const {
            if(IsLock())
                return;

            if(eventType.empty())
                return;

            auto it = m_observers.find(eventType);
            if (it != m_observers.end())
            {
                auto observers = it->second;
                for (auto observer : observers)
                    observer->m_callback();
            }
        }

        void operator()(const EventType& eventType) const {
            Emit(eventType);
        }

        void Lock() { isLock = true; }
        void Unlock() { isLock = false; }
        bool IsLock() const { return isLock; }

    private:
        InputSignal() = default;
        ~InputSignal()
        {
            for (auto& observer : m_observers) {
                for (auto& connection : observer.second) {
                    match(connection->m_state,
                          [&](typename ConnectionType::Managed) { delete connection; },
                          [&](typename ConnectionType::Scoped) { connection->m_state = typename ConnectionType::Zombified{}; },
                          [&](typename ConnectionType::Zombified) { delete connection; });
                }
            }
        }


    private:
        mutable ObserversMap m_observers;
        bool isLock = false;

    };

    template<typename... Args>
    class Signal {
        using ConnectionType = Connection<Args...>;
        using ScopedConnectionType = ScopedConnection<Args...>;
        using SlotConnectionType = SlotConnection<Args...>;

        using Callback = typename ConnectionType::Callback;
        using Observers = std::vector<ConnectionType*>;

    public:
        Signal() = default;
        ~Signal();

        void connect(const Callback& callback) const;
        template<typename Type>
        void connect(Type* instance, void (Type::* fn)(Args...)) const;
        template<typename Type>
        void connect(Type* instance, void (Type::* fn)(Args...) const) const;

        ScopedConnectionType connectScoped(const Callback& callback) const;
        template<typename Type>
        ScopedConnectionType connectScoped(Type* instance, void (Type::* fn)(Args...)) const;
        template<typename Type>
        ScopedConnectionType connectScoped(Type* instance, void (Type::* fn)(Args...) const) const;

        SlotConnectionType connectSlot(const Callback& callback) const;
        template<typename Type>
        SlotConnectionType connectSlot(Type* instance, void (Type::* fn)(Args...)) const;
        template<typename Type>
        SlotConnectionType connectSlot(Type* instance, void (Type::* fn)(Args...) const) const;

        void Disconnect(ConnectionType* connection) const;
        void Emit(const Args&... args) const;
        void operator()(const Args&... args) const;

    private:
        mutable Observers m_observers;

    };
}

#include "InputSignal.tpp"