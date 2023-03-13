#pragma once

#include <unordered_map>

#include <SFML/Window/Event.hpp>
#include "SFML/Window/Mouse.hpp"

namespace SignalSystem
{
    using KeyEnum = sf::Keyboard::Key;
    using MouseButtonEnum = sf::Mouse::Button;
    using MouseWheelEnum = sf::Mouse::Wheel;
    using SensorEnum = sf::Sensor::Type;

    using Key = std::string;
    using Type = int64_t;

    static const std::unordered_map<Key, Type> BindingsMapKey
            {
                    { "Unknown", -1 },
                    { "A", KeyEnum::A },
                    { "B", KeyEnum::B },
                    { "C", KeyEnum::C },
                    { "D", KeyEnum::D },
                    { "E", KeyEnum::E },
                    { "F", KeyEnum::F },
                    { "G", KeyEnum::G },
                    { "H", KeyEnum::H },
                    { "I", KeyEnum::I },
                    { "J", KeyEnum::J },
                    { "K", KeyEnum::K },
                    { "L", KeyEnum::L },
                    { "M", KeyEnum::M },
                    { "N", KeyEnum::N },
                    { "O", KeyEnum::O },
                    { "P", KeyEnum::P },
                    { "Q", KeyEnum::Q },
                    { "R", KeyEnum::R },
                    { "S", KeyEnum::S },
                    { "T", KeyEnum::T },
                    { "U", KeyEnum::U },
                    { "V", KeyEnum::V },
                    { "W", KeyEnum::W },
                    { "X", KeyEnum::X },
                    { "Y", KeyEnum::Y },
                    { "Z", KeyEnum::Z },
                    { "Num0", KeyEnum::Num0 },
                    { "Num1", KeyEnum::Num1 },
                    { "Num2", KeyEnum::Num2 },
                    { "Num3", KeyEnum::Num3 },
                    { "Num4", KeyEnum::Num4 },
                    { "Num5", KeyEnum::Num5 },
                    { "Num6", KeyEnum::Num6 },
                    { "Num7", KeyEnum::Num7 },
                    { "Num8", KeyEnum::Num8 },
                    { "Num9", KeyEnum::Num9 },
                    { "Escape", KeyEnum::Escape },
                    { "LControl", KeyEnum::LControl },
                    { "LShift", KeyEnum::LShift },
                    { "LAlt", KeyEnum::LAlt },
                    { "LSystem", KeyEnum::LSystem },
                    { "RControl", KeyEnum::RControl },
                    { "RShift", KeyEnum::RShift },
                    { "RAlt", KeyEnum::RAlt },
                    { "RSystem", KeyEnum::RSystem },
                    { "Menu", KeyEnum::Menu },
                    { "LBracket", KeyEnum::LBracket },
                    { "RBracket", KeyEnum::RBracket },
                    { "SemiColon", KeyEnum::SemiColon },
                    { "Comma", KeyEnum::Comma },
                    { "Period", KeyEnum::Period },
                    { "Quote", KeyEnum::Quote },
                    { "Slash", KeyEnum::Slash },
                    { "BackSlash", KeyEnum::BackSlash },
                    { "Tilde", KeyEnum::Tilde },
                    { "Equal", KeyEnum::Equal },
                    { "Dash", KeyEnum::Dash },
                    { "Space", KeyEnum::Space },
                    { "Return", KeyEnum::Return },
                    { "BackSpace", KeyEnum::BackSpace },
                    { "Tab", KeyEnum::Tab },
                    { "PageUp", KeyEnum::PageUp },
                    { "PageDown", KeyEnum::PageDown },
                    { "End", KeyEnum::End },
                    { "Home", KeyEnum::Home },
                    { "Insert", KeyEnum::Insert },
                    { "Delete", KeyEnum::Delete },
                    { "Add", KeyEnum::Add },
                    { "Subtract", KeyEnum::Subtract },
                    { "Multiply", KeyEnum::Multiply },
                    { "Divide", KeyEnum::Divide },
                    { "Left", KeyEnum::Left },
                    { "Right", KeyEnum::Right },
                    { "Up", KeyEnum::Up },
                    { "Down", KeyEnum::Down },
                    { "Numpad0", KeyEnum::Numpad0 },
                    { "Numpad1", KeyEnum::Numpad1 },
                    { "Numpad2", KeyEnum::Numpad2 },
                    { "Numpad3", KeyEnum::Numpad3 },
                    { "Numpad4", KeyEnum::Numpad4 },
                    { "Numpad5", KeyEnum::Numpad5 },
                    { "Numpad6", KeyEnum::Numpad6 },
                    { "Numpad7", KeyEnum::Numpad7 },
                    { "Numpad8", KeyEnum::Numpad8 },
                    { "Numpad9", KeyEnum::Numpad9 },
                    { "F1", KeyEnum::F1 },
                    { "F2", KeyEnum::F2 },
                    { "F3", KeyEnum::F3 },
                    { "F4", KeyEnum::F4 },
                    { "F5", KeyEnum::F5 },
                    { "F6", KeyEnum::F6 },
                    { "F7", KeyEnum::F7 },
                    { "F8", KeyEnum::F8 },
                    { "F9", KeyEnum::F9 },
                    { "F10", KeyEnum::F10 },
                    { "F11", KeyEnum::F11 },
                    { "F12", KeyEnum::F12 },
                    { "F13", KeyEnum::F13 },
                    { "F14", KeyEnum::F14 },
                    { "F15", KeyEnum::F15 },
                    { "Pause", KeyEnum::Pause },
            };

    static const std::unordered_map<Key, Type> BindingsMapMouse
            {
                    //Mouse
                    { "MouseLeft", MouseButtonEnum::Left },
                    { "MouseRight", MouseButtonEnum::Right },
                    { "MouseMiddle", MouseButtonEnum::Middle },
                    { "MouseXButton1", MouseButtonEnum::XButton1 },
                    { "MouseXButton2", MouseButtonEnum::XButton2 },
            };

    static const std::unordered_map<Key, Type> BindingsMapWheel
            {
                    //Wheel
                    { "WheelVerticalWheel", MouseWheelEnum::VerticalWheel },
                    { "WheelHorizontalWheel", MouseWheelEnum::HorizontalWheel },
            };

    static const std::unordered_map<Key, Type> BindingsMapSensor
            {
                    //Sensor
                    { "SensorAccelerometer", SensorEnum::Accelerometer },
                    { "SensorGyroscope", SensorEnum::Gyroscope },
                    { "SensorMagnetometer", SensorEnum::Magnetometer },
                    { "SensorGravity", SensorEnum::Gravity },
                    { "SensorUserAcceleration", SensorEnum::UserAcceleration },
                    { "SensorOrientation", SensorEnum::Orientation },
                    { "SensorCount", SensorEnum::Count },
            };

    static const std::unordered_map<Key, Type> BindingsMapEvent
            {
                    //Event
                    { "EventClosed", sf::Event::Closed },
                    { "EventResized", sf::Event::Resized },
                    { "EventLostFocus", sf::Event::LostFocus },
                    { "EventGainedFocus", sf::Event::GainedFocus },
                    { "EventTextEntered", sf::Event::TextEntered },
                    { "EventKeyPressed", sf::Event::KeyPressed },
                    { "EventKeyReleased", sf::Event::KeyReleased },
                    { "EventMouseWheelMoved", sf::Event::MouseWheelMoved },
                    { "EventMouseWheelScrolled", sf::Event::MouseWheelScrolled },
                    { "EventMouseButtonPressed", sf::Event::MouseButtonPressed },
                    { "EventMouseButtonReleased", sf::Event::MouseButtonReleased },
                    { "EventMouseMoved", sf::Event::MouseMoved },
                    { "EventMouseEntered", sf::Event::MouseEntered },
                    { "EventMouseLeft", sf::Event::MouseLeft },
                    { "EventJoystickButtonPressed", sf::Event::JoystickButtonPressed },
                    { "EventJoystickButtonReleased", sf::Event::JoystickButtonReleased },
                    { "EventJoystickMoved", sf::Event::JoystickMoved },
                    { "EventJoystickConnected", sf::Event::JoystickConnected },
                    { "EventJoystickDisconnected", sf::Event::JoystickDisconnected },
                    { "EventTouchBegan", sf::Event::TouchBegan },
                    { "EventTouchMoved", sf::Event::TouchMoved },
                    { "EventTouchEnded", sf::Event::TouchEnded },
                    { "EventSensorChanged", sf::Event::SensorChanged }
            };

    static Key GetKeyByValue(const Type& value)
    {
        for( auto [k, t] : SignalSystem::BindingsMapKey)
        {
            if(t == value) {
                return k;
            }
        }
        return "";
    }
    static Key GetMouseByValue(const Type& value)
    {
        for( auto [k, t] : SignalSystem::BindingsMapMouse)
        {
            if(t == value) {
                return k;
            }
        }
        return "";
    }
    static Key GetEventByValue(const Type& value)
    {
        for( auto [k, t] : SignalSystem::BindingsMapEvent)
        {
            if(t == value) {
                return k;
            }
        }
        return "";
    }
    static Key GetSensorByValue(const Type& value)
    {
        for( auto [k, t] : SignalSystem::BindingsMapSensor)
        {
            if(t == value) {
                return k;
            }
        }
        return "";
    }
    static Key GetWheelByValue(const Type& value)
    {
        for( auto [k, t] : SignalSystem::BindingsMapWheel)
        {
            if(t == value) {
                return k;
            }
        }
        return "";
    }

}