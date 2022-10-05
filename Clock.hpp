#include <fmt/format-inl.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "Config.hpp"

#define __Q(x) #x[0]

#define _CASE(x)          \
    case __Q(x):          \
        retVal = _row[x]; \
        break;

#define ERROR(x) \
    std::cerr << ColorTxt::Colorize(x, Colors::red);

struct Digits {
   private:
    const std::vector<std::vector<std::string>> m_digits = {
        {" ┏━┓ ", "  ╻  ", " ┏━┓ ", " ┏━┓ ", " ╻ ╻ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", "   ", "   ", "   "},
        {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", " ╻ ", "   ", "   "},
        {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", "   ", "   ", "   "},
        {" ┃ ┃ ", "  ┃  ", " ┏━┛ ", " ┣━┫ ", " ┗━┫ ", " ┗━┓ ", " ┣━┓ ", "   ┃ ", " ┣━┫ ", " ┗━┫ ", "   ", "   ", " ━ "},
        {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", "   ", "   ", "   "},
        {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ╹ ", "   ", "   "},
        {" ┗━┛ ", "  ╹  ", " ┗━━ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   ", "   ", "   "},
    };

   public:
    inline std::string get(char digit, size_t row) {
        auto& _row = m_digits.at(row);
        std::string retVal;

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
        }
        return retVal;
    }
};

class DigitalClock {
   private:
    Config config;
    Digits digits;

    const auto Time(tm* now, size_t row) {
        static auto cfg = config.get();

        std::string _sec = fmt::format("{:02}", now->tm_sec);
        std::string _min = fmt::format("{:02}", now->tm_min);
        std::string _hour = fmt::format("{:02}", now->tm_hour);

        std::string retVal;
        if (cfg->timeEnabled)
            for (auto c : cfg->timeFormat) {
                retVal += ' ';
                switch (c) {
                    case 'H':
                        for (auto num : _hour)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->timeH);
                        break;

                    case 'M':
                        for (auto num : _min)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->timeM);
                        break;

                    case 'S':
                        for (auto num : _sec)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->timeS);
                        break;

                    default:
                        retVal += digits.get(c, row);
                        break;
                }
            }

        return retVal;
    }

    const auto Date(tm* now, size_t row) {
        static auto cfg = config.get();

        std::string _day = fmt::format("{:02}", now->tm_mday);
        std::string _month = fmt::format("{:02}", now->tm_mon + 1);
        std::string _year = fmt::format("{}", now->tm_year + 1900);

        std::string retVal;
        if (cfg->dateEnabled) {
            for (auto c : cfg->dateFormat) {
                switch (c) {
                    case 'd':
                        for (auto num : _day)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->dateD);
                        break;

                    case 'm':
                        for (auto num : _month)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->dateM);
                        break;

                    case 'y':
                        for (auto num : _year)
                            retVal += ColorTxt::Colorize(digits.get(num, row), cfg->dateY);
                        break;

                    default:
                        retVal += digits.get(c, row);
                        break;
                }
            }
        }
        return retVal;
    }

   public:
    void displayTime() {
        while (true) {
            static auto cfg = config.get();

            if (system("clear") != 0) {
                ERROR("Output error\n");
                return;
            }

            auto t = std::time(nullptr);
            auto now = std::localtime(&t);

            if (cfg->timeOnNewLine) {
                for (size_t row = 0; row < 7; ++row) {
                    std::cout << Date(now, row) << '\n';
                }
                std::cout << std::endl;
                for (size_t row = 0; row < 7; ++row) {
                    std::cout << Time(now, row) << '\n';
                }
            } else {
                for (size_t row = 0; row < 7; ++row) {
                    std::cout << Date(now, row);
                    std::cout << cfg->separator;
                    std::cout << Time(now, row);
                    std::cout << '\n';
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // affects accuracy relative to the system clock
        }
    }
};

#undef __Q
#undef _CASE