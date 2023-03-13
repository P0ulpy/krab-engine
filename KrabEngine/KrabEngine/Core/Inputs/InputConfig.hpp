//
// Created by Admin on 30/01/2023.
//

#pragma once

#include <vector>
#include <utility>

#include "Binding.hpp"
#include "../../Vendor/mINI/ini.hpp"
#include "../Logger/Logger.hpp"

namespace SignalSystem
{
    class InputConfig {
        using ConfigKey = Key;
        using ConfigType = Type;
    public:
        static InputConfig* Get()
        {
            static InputConfig m_inputConfig;

            return &m_inputConfig;
        }

        void LoadConfig(const std::string& path)
        {
            mINI::INIFile file(path);

            mINI::INIStructure ini;
            file.read(ini);
            auto keys = ini["keys"];
            for (auto& key : keys)
            {
                auto binding = BindingsMapKey.at(key.second);
                m_bindingskeys.emplace(key.first, binding);
            }

            auto mouse = ini["mouse"];
            for (auto& key : mouse)
            {
                auto binding = BindingsMapMouse.at(key.second);
                m_bindingsmouse.emplace(key.first, binding);
            }

            auto events = ini["events"];
            for (auto& key : events)
            {
                auto binding = BindingsMapEvent.at(key.second);
                m_bindingsevents.emplace(key.first, binding);
            }

            if(m_bindingskeys.empty() || m_bindingsmouse.empty() || m_bindingsevents.empty())
                Engine::Logger::Err("InputConfig path is incorrect");

            isLoad = true;
            m_path = path;
        }

        [[nodiscard]] ConfigType GetKeyBinding(const ConfigKey& eventType) const { return m_bindingskeys.at(eventType); }
        [[nodiscard]] ConfigType GetMouseBinding(const ConfigKey& eventType) const { return m_bindingsmouse.at(eventType); }
        [[nodiscard]] std::vector<ConfigKey> GetKeyBindingName(const ConfigType& event, const ConfigType& binding) const
        {
            if(!IsLoad())
                Engine::Logger::Err("InputConfig is not load");

            std::vector<ConfigKey> names;

            if(m_bindingskeys.empty())
                return names;

            if(event != sf::Event::EventType::KeyPressed && event != sf::Event::EventType::KeyReleased)
                return names;

            for (auto& key : m_bindingskeys)
            {
                if (key.second == binding)
                    if(m_bindingsevents.at(key.first) == event)
                        names.push_back(key.first);
            }

            return names;
        }
        [[nodiscard]] std::vector<ConfigKey> GetMouseBindingName(const ConfigType& event, const ConfigType& binding) const
        {
            if(!IsLoad())
                Engine::Logger::Err("InputConfig is not load");

            std::vector<ConfigKey> names;

            if(m_bindingsmouse.empty())
                return names;

            if (
                event == sf::Event::EventType::MouseMoved
                || event == sf::Event::EventType::MouseEntered
                || event == sf::Event::EventType::MouseLeft
            )
            {
                for (auto& key : m_bindingsmouse)
                {
                    if (key.second == event)
                        names.push_back(key.first);
                }
                return names;
            }

            if (
                event != sf::Event::EventType::MouseWheelMoved
                && event != sf::Event::EventType::MouseWheelScrolled
                && event != sf::Event::EventType::MouseButtonPressed
                && event != sf::Event::EventType::MouseButtonReleased
            )
                return names;

            for (auto& key : m_bindingsmouse)
            {
                if (key.second == binding)
                    if(m_bindingsevents.at(key.first) == event)
                        names.push_back(key.first);
            }
            return names;
        }
        [[nodiscard]] ConfigKey GetEventBindingName(const ConfigType& event) const
        {
            if(!IsLoad())
                Engine::Logger::Err("InputConfig is not load");

            if(m_bindingsevents.empty())
                return "";

            if(event == sf::Event::EventType::KeyPressed || event == sf::Event::EventType::KeyReleased
               || event == sf::Event::EventType::MouseWheelMoved || event == sf::Event::EventType::MouseWheelScrolled
               || event == sf::Event::EventType::MouseButtonPressed || event == sf::Event::EventType::MouseButtonReleased
               || event == sf::Event::EventType::MouseMoved || event == sf::Event::EventType::MouseEntered
               || event == sf::Event::EventType::MouseLeft)
                return "";

            for (auto& key : m_bindingsevents)
            {
                if (key.second == event)
                    return key.first;
            }
            return "";
        }

        [[nodiscard]] bool IsLoad() const { return isLoad; }
        [[nodiscard]] const std::unordered_map<ConfigKey, ConfigType>& GetBindingsKeys() const { return m_bindingskeys; }
        [[nodiscard]] const std::unordered_map<ConfigKey, ConfigType>& GetBindingsMouse() const { return m_bindingsmouse; }

        void SetKeyBinding(const ConfigKey& key, const ConfigType& binding) {
            ChangeValueFile(key, binding);
            m_bindingskeys[key] = binding;
        }
        void SetMouseBinding(const ConfigKey& key, const ConfigType& binding) {
            ChangeValueFile(key, binding);
            m_bindingsmouse[key] = binding;
        }
        void SetEventBinding(const ConfigKey& key, const ConfigType& binding) {
            ChangeValueFile(key, binding);
            m_bindingsevents[key] = binding;
        }

    private:
        explicit InputConfig() = default;

        void ChangeValueFile(const ConfigKey& key, const ConfigType& value)
        {
            if(!IsLoad())
                Engine::Logger::Err("InputConfig is not load");

            if(m_path.empty())
                Engine::Logger::Err("InputConfig path is empty");

            mINI::INIFile file(m_path);

            mINI::INIStructure ini;
            file.read(ini);

            if(m_bindingskeys.find(key) != m_bindingskeys.end())
            {
                auto v = GetKeyByValue(value);
                if(!v.empty())
                    ini["keys"][key] = v;
            }
            else if(m_bindingsmouse.find(key) != m_bindingsmouse.end())
            {
                auto v = GetMouseByValue(value);
                if(!v.empty())
                    ini["mouse"][key] = v;
            }
            else if(m_bindingsevents.find(key) != m_bindingsevents.end())
            {
                auto v = GetEventByValue(value);
                if(!v.empty())
                    ini["events"][key] = v;
            }
            else
            {
                Engine::Logger::Err("InputConfig Key is not found");
            }

            file.write(ini);
        }

        bool isLoad = false;
        std::string m_path;

        std::unordered_map<ConfigKey, ConfigType> m_bindingskeys;
        std::unordered_map<ConfigKey, ConfigType> m_bindingsmouse;
        std::unordered_map<ConfigKey, ConfigType> m_bindingsevents;
    };
}
