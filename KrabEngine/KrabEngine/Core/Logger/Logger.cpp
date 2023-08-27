//
// Created by Flo on 28/11/2022.
//

#include "Logger.hpp"

#include <iostream>
#include <iomanip>

namespace KrabEngine
{
    std::mutex Logger::_mutex {};
    std::map<std::thread::id, std::string> Logger::_threadsLabels {};

    void Logger::SetThreadLabel(const std::string_view &label)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Logger::_threadsLabels[std::this_thread::get_id()] = label;
    }

    void Logger::SetThreadLabel(const std::string_view &label, std::thread::id threadID)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Logger::_threadsLabels[threadID] = label;
    }

    std::string Logger::CurrentDateTimeToString()
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::stringstream formatBuffer;
        formatBuffer << std::put_time(std::localtime(&now), "%H:%M:%S");

        return formatBuffer.str();
    }

} // KrabEngine