#pragma once

namespace KrabEngine
{
    template<typename FirstMessage, typename ...Args>
    void Logger::BuildMessage(std::stringstream& out, const FirstMessage& first, Args... args)
    {
        out << first;

        if constexpr (sizeof...(args) > 0)
            BuildMessage(out, args...);
    }

    template <LogType logType>
    LogEntry Logger::ConstructEntry()
    {
        LogEntry logEntry;
        logEntry.type = logType;
        logEntry.threadId = std::this_thread::get_id();

        std::stringstream headerBuffer;
        headerBuffer << "["
                     << CurrentDateTimeToString()
                     << "] "
                     << LogTypeLabel<logType>::value
                     << ": ";

        logEntry.message = headerBuffer.str();

        return logEntry;
    }

    template<LogType logType, typename... Args>
    LogEntry Logger::PrintMessage(std::ostream& ostream, Args... args)
    {
        LogEntry logEntry = ConstructEntry<logType>();

        std::stringstream messageBuffer;
        BuildMessage<Args...>(messageBuffer, args...);
        logEntry.message += messageBuffer.str();

        std::string_view threadLabel = (_threadsLabels.contains(logEntry.threadId))
                                       ? _threadsLabels.at(logEntry.threadId)
                                       : std::string_view("");

        ostream << LogTypeColor<logType>::value
                << '[';

        if(!threadLabel.empty())
            ostream << threadLabel;
        else
            ostream << logEntry.threadId;

        ostream << ']'
                << logEntry.message
                << ConsoleColor::reset
                << std::endl;

        return logEntry;
    }

    template<typename ...Args>
    void Logger::Log(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_INFO, Args...>(std::cout, args...);

        //messages.push_back(logEntry);
    }

    template<typename ...Args>
    void Logger::Warn(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_WARNING, Args...>(std::cout, args...);

        //messages.push_back(logEntry);
    }

    template<typename ...Args>
    void Logger::Err(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_ERROR, Args...>(std::cerr, args...);

        //messages.push_back(logEntry);
    }
}