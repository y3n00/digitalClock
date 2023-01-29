#pragma once
#include <fmt/format-inl.h>

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include "Config.hpp"

#define __Q(x) #x[0]

#define _CASE(x)          \
    case __Q(x):          \
        retVal = _row[x]; \
        break;

#define ERROR(x) \
    std::cerr << cliColors::Colorize(x, Colors::red);

class Digits {
   private:
    static const std::vector<std::vector<std::string>> m_digits;

   public:
    [[nodiscard]] static const auto
    size() {
        return m_digits.size();
    }

    [[nodiscard]] static const auto
    get(char digit, size_t row) {
        std::string retVal;
        auto& _row = m_digits.at(row);

        switch (digit) {
            _CASE(0);
            _CASE(1);
            _CASE(2);
            _CASE(3);
            _CASE(4);
            _CASE(5);
            _CASE(6);
            _CASE(7);
            _CASE(8);
            _CASE(9);
            case ':':
                retVal = _row[10];
                break;
            case ' ':
                retVal = _row[11];
                break;
            case '-':
                retVal = _row[12];
                break;
            case '$':
                retVal = _row[13];
                break;
        }
        return retVal;
    }
};

const std::vector<std::vector<std::string>> Digits::m_digits = {
    {" ┏━┓ ", "  ╻  ", " ┏━┓ ", " ┏━┓ ", " ╻ ╻ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", "   ", "   ", "   ", "   "},
    {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", " ╻ ", "   ", "   ", " $ "},
    {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", "   ", "   ", "   ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┏━┛ ", " ┣━┫ ", " ┗━┫ ", " ┗━┓ ", " ┣━┓ ", "   ┃ ", " ┣━┫ ", " ┗━┫ ", "   ", "   ", " ━ ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", "   ", "   ", "   ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ╹ ", "   ", "   ", " $ "},
    {" ┗━┛ ", "  ╹  ", " ┗━━ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   ", "   ", "   ", "   "},
};

class Clock {
   private:
    static Config cfg;

    [[nodiscard]] static const auto
    Time(tm* now, size_t row) {
        std::string retVal;
        if (cfg.time.Enabled) {
            std::string _sec = fmt::format("{:02}", now->tm_sec);
            std::string _min = fmt::format("{:02}", now->tm_min);
            std::string _hour = fmt::format("{:02}", now->tm_hour);

            for (auto c : cfg.time.Format) {
                retVal += ' ';
                switch (c) {
                    case 'H':
                        for (auto num : _hour)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.time.colors[0]);
                        break;
                    case 'M':
                        for (auto num : _min)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.time.colors[1]);
                        break;
                    case 'S':
                        for (auto num : _sec)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.time.colors[2]);
                        break;
                    default:
                        retVal += Digits::get(c, row);
                        break;
                }
            }
        }
        return retVal;
    }

    [[nodiscard]] static const auto
    Date(tm* now, size_t row) {
        std::string retVal;
        if (cfg.date.Enabled) {
            const std::string _day = fmt::format("{:02}", now->tm_mday);
            const std::string _month = fmt::format("{:02}", now->tm_mon + 1);
            const std::string _year = fmt::format("{}", now->tm_year + 1900);
            for (auto c : cfg.date.Format) {
                switch (c) {
                    case 'd':
                        for (auto num : _day)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.date.colors[0]);
                        break;
                    case 'm':
                        for (auto num : _month)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.date.colors[1]);
                        break;
                    case 'y':
                        for (auto num : _year)
                            retVal += cliColors::Colorize(Digits::get(num, row), cfg.date.colors[2]);
                        break;

                    default:
                        retVal += Digits::get(c, row);
                        break;
                }
            }
        }
        return retVal;
    }

   public:
    static void run() {
        Config::init();

        while (true) {
            if (system("clear") != 0) {
                ERROR("Output error\n");
                return;
            }

            time_t t = std::time(nullptr);
            tm* now = std::localtime(&t);

            if (cfg.timeOnNewLine) {
                for (size_t row = 0; row < Digits::size(); ++row)
                    std::cout << Date(now, row) << '\n';

                std::cout << std::endl;

                for (size_t row = 0; row < Digits::size(); ++row)
                    std::cout << Time(now, row) << '\n';

            } else {
                for (size_t row = 0; row < Digits::size(); ++row) {
                    std::cout << Date(now, row);
                    std::cout << cfg.separator;
                    std::cout << Time(now, row);
                    std::cout << '\n';
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(cfg.deltaTime));  // accuracy relative to the system clock
        }
    }
};

#undef __Q
#undef _CASE