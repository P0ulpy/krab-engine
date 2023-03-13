//
// Created by Admin on 30/01/2023.
//

#pragma once

#include <functional>
#include <utility>
#include <string>
#include <string_view>
#include <variant>

namespace SignalSystem
{
    class InputSignal;

    template<typename... Args>
    class Signal;

    enum class EventType;

    template<typename... Types>
    struct Overload : Types... {
        using Types::operator()...;
    };

    template<typename... Types>
    Overload(Types...)->Overload<Types...>;

    template<typename Variant, typename... Matchers>
    auto match(Variant&& variant, Matchers&&... matchers) {
        return std::visit(Overload{ std::forward<Matchers>(matchers)... }, variant);
    }

    struct Semaphore
    {
        Semaphore() : m_counter(0) {}

        void addBlocker() { m_counter++; }
        void removeBlocker() { --m_counter; }
        bool isDisabled() const { return m_counter == 0; }

    private:
        size_t m_counter;
    };

    struct ConnectionSignal {
        using Callback = std::function<void()>;
        using Signal = InputSignal;
        using SignalPtr = Signal*;

        struct Managed {};
        struct Scoped {};
        struct Zombified {};

        using StateType = std::variant<Managed, Scoped, Zombified>;

        ConnectionSignal(Callback callback, StateType state, SignalPtr signal);

        Callback m_callback;
        StateType m_state;
        SignalPtr m_signal;
    };

    struct ScopedConnectionSignal
    {
        friend class InputSignal;

        using EventType = std::string;

        ScopedConnectionSignal();
        ScopedConnectionSignal(const ScopedConnectionSignal& other) = default;
        ScopedConnectionSignal(ScopedConnectionSignal&& other);
        ScopedConnectionSignal& operator=(const ScopedConnectionSignal& other) = default;
        ScopedConnectionSignal& operator=(ScopedConnectionSignal&& other);
        ~ScopedConnectionSignal();

    private:
        explicit ScopedConnectionSignal(EventType eventType, ConnectionSignal* connection);

        void disconnect() const;
        mutable ConnectionSignal* m_connection = nullptr;
        mutable EventType m_eventType;
    };

    struct SlotConnectionSignal
    {
        friend class InputSignal;

        using EventType = ScopedConnectionSignal::EventType;

        void block();
        void unblock();
        auto scopedBlock();

        SlotConnectionSignal(InputSignal& signal, const EventType& eventType, const ConnectionSignal::Callback& callback);
        SlotConnectionSignal(const SlotConnectionSignal& other) = delete;
        SlotConnectionSignal(SlotConnectionSignal&& other) = delete;
        SlotConnectionSignal& operator=(const SlotConnectionSignal& other) = delete;
        SlotConnectionSignal& operator=(SlotConnectionSignal&& other) = delete;

    private:
        ScopedConnectionSignal m_scopedConnection;
        Semaphore m_semaphore;
    };

    template<typename... Args>
    struct Connection {
        using Callback = std::function<void(Args...)>;
        using SignalT = Signal<Args...>;
        using SignalPtr = SignalT*;

        struct Managed {};
        struct Scoped {};
        struct Zombified {};

        using StateType = std::variant<Managed, Scoped, Zombified>;

        Connection(Callback callback, StateType state, SignalPtr signal);

        Callback m_callback;
        StateType m_state;
        SignalPtr m_signal;
    };

    template<typename... Args>
    struct ScopedConnection
    {
        friend class Signal<Args...>;

        ScopedConnection();
        ScopedConnection(const ScopedConnection& other) = default;
        ScopedConnection(ScopedConnection&& other);
        ScopedConnection& operator=(const ScopedConnection& other) = default;
        ScopedConnection& operator=(ScopedConnection&& other);
        ~ScopedConnection();

    private:
        explicit ScopedConnection(Connection<Args...>* connection)
                : m_connection(connection)
        {}

        void disconnect() const;
        mutable Connection<Args...>* m_connection = nullptr;
    };

    template<typename... Args>
    struct SlotConnection
    {
        void block();
        void unblock();
        auto scopedBlock();

        SlotConnection(Signal<Args...>& signal, const typename Signal<Args...>::Callback& callback);
        SlotConnection(const SlotConnection& other) = delete;
        SlotConnection(SlotConnection&& other) = delete;
        SlotConnection& operator=(const SlotConnection& other) = delete;
        SlotConnection& operator=(SlotConnection&& other) = delete;

    private:
        ScopedConnection<Args...> m_scopedConnection;
        Semaphore m_semaphore;
    };
}

#include "Connection.tpp"
