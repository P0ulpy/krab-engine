//
// Created by Admin on 22/02/2023.
//

#pragma once

#include <functional>
#include "InputSignal.hpp"

namespace SignalSystem
{
    template<typename Type>
    class Observable {
    public:
        Observable() = default;

        explicit Observable(const Type& value);
        Observable(const Observable& other);

        Observable& operator=(const Observable& other);
        Observable& operator=(const Type& value);

        void connect(std::function<void(const Type&)> callback);
        void forceDisconnect();

    private:
        void notify();

        Type m_value;
        Signal<Type> m_signal;
        ScopedConnection<Type> m_scopedConnection;
    };
}

#include "Observable.tpp"
