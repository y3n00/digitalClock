#include <fmt/format-inl.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Digits {
   private:
    static const std::vector<std::vector<std::string>> m_digits;

   public:
    static inline const std::string get(const char digit, size_t at) {
        auto& row = m_digits.at(at);

        switch (digit) {
            case '0':
                return row[0];
            case '1':
                return row[1];
            case '2':
                return row[2];
            case '3':
                return row[3];
            case '4':
                return row[4];
            case '5':
                return row[5];
            case '6':
                return row[6];
            case '7':
                return row[7];
            case '8':
                return row[8];
            case '9':
                return row[9];
        }
        return row[10];
    }
};
const std::vector<std::vector<std::string>> Digits::m_digits = {
    {" ┏━┓ ", "  ╻  ", " ┏━┓ ", " ┏━┓ ", " ╻ ╻ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", " ┏━┓ ", "   "},
    {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", " ╻ "},
    {" ┃ ┃ ", "  ┃  ", "   ┃ ", "   ┃ ", " ┃ ┃ ", " ┃   ", " ┃   ", "   ┃ ", " ┃ ┃ ", " ┃ ┃ ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┏━┛ ", " ┣━┫ ", " ┗━┫ ", " ┗━┓ ", " ┣━┓ ", "   ┃ ", " ┣━┫ ", " ┗━┫ ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", "   "},
    {" ┃ ┃ ", "  ┃  ", " ┃   ", "   ┃ ", "   ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ┃ ┃ ", "   ┃ ", " ╹ "},
    {" ┗━┛ ", "  ╹  ", " ┗━━ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   ╹ ", " ┗━┛ ", " ┗━┛ ", "   "},
};

class DigitalClock {
   private:
    static auto getDigits(std::string time) {
        std::stringstream lines;

        for (size_t row = 0; row < 7; ++row) {
            for (auto c : time)

                lines << Digits::get(c, row);

            lines << '\n';
        }

        return lines.str();
    }

   public:
    static void displayTime() {
        while (true) {
            auto retCode = system("clear");
            if (retCode != 0)
                return;

            auto t = std::time(nullptr);
            auto now = std::localtime(&t);
            int h{now->tm_hour}, m{now->tm_min}, s{now->tm_sec};

            auto time = fmt::format("{:02}:{:02}:{:02}", h, m, s);
            std::cout << getDigits(time) << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // affects accuracy relative to the system clock
        }
    }
};