//
// Created by Flo on 02/12/2022.
//

#ifndef PATHFINDER_CONSOLECOLOR_HPP
#define PATHFINDER_CONSOLECOLOR_HPP

#include <iostream>

#ifdef WIN32

#include <Windows.h>

namespace ConsoleColor
{
    inline std::ostream& blue(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
                                         |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& red(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& green(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_GREEN|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& yellow(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& white(std::ostream &s)
    {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,
                                FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
        return s;
    }

    inline std::ostream& reset(std::ostream &s) { return white(s); }

    struct color
    {
        explicit color(WORD attribute)
            : m_color(attribute)
        {};

        WORD m_color;
    };

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits>&
    operator<<(std::basic_ostream<Elem, Traits>& i, color& c)
    {
        HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout,c.m_color);
        return i;
    }
}

#endif

#ifdef linux

namespace ConsoleColor
{
    inline std::ostream& blue(std::ostream &s)
    {
        return s << "\033[1;34m";
    }

    inline std::ostream& red(std::ostream &s)
    {
        return s << "\033[1;31m";
    }

    inline std::ostream& green(std::ostream &s)
    {
        return s << "\033[1;32m";
    }

    inline std::ostream& yellow(std::ostream &s)
    {
        return s << "\033[1;33m";
    }

    inline std::ostream& white(std::ostream &s)
    {
        return s << "\033[1;97m";
    }

    inline std::ostream& reset(std::ostream &s) { return s << "\033[0m"; }

    struct color
    {
        explicit color(std::string& attribute)
            : m_color(attribute)
        {};

        std::string m_color;
    };

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits>&
    operator<<(std::basic_ostream<Elem, Traits>& i, color& c)
    {
        return i << c;
    }
}

#endif

#endif //PATHFINDER_CONSOLECOLOR_HPP