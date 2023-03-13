//
// Created by Admin on 22/02/2023.
//

#include "Observable.hpp"

namespace SignalSystem
{
    /*template<typename Type>
    Observable<Type>::Observable(const Type &value)
            : m_value(value)
    {}

    template<typename Type>
    Observable<Type>::Observable(const Observable &other)
            : m_value(other.m_value)
    {}

    template<typename Type>
    Observable<Type> &Observable<Type>::operator=(const Observable<Type> &other)
    {
        m_value = other.m_value;
        notify();
        return *this;
    }

    template<typename Type>
    Observable<Type> &Observable<Type>::operator=(const Type &value)
    {
        m_value = value;
        notify();
        return *this;
    }

    template<typename Type>
    void Observable<Type>::connect(std::function<void(const Type &)> callback)
    {
        m_scopedConnection = m_signal.connectScoped(callback);
    }

    template<typename Type>
    void Observable<Type>::notify()
    {
        m_signal(m_value);
    }*/
}