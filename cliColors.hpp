#pragma once
#include <array>
#include <string>

enum Colors : uint8_t {
    red = 0,
    orange,
    yellow,
    green,
    cyan,
    blue,
    purple,
    pink,
    brightgreen,
    brightred,
    black,
    gray,
    brightgray,
    white,
    reset,
    _default,
};

class cliColors {
   private:
    static const std::array<const char*, 16> m_colors;

   public:
    [[nodiscard]] inline static const std::string
    Colorize(const std::string& str, Colors c) {
        return m_colors[c] + str + m_colors[reset];
    }

    [[nodiscard]] inline static const std::string
    GetColor(Colors color) {
        return m_colors[color];
    }

    [[nodiscard]] inline static const std::string
    Reset() {
        return m_colors[reset];
    }
};

const std::array<const char*, 16> cliColors::m_colors = {
    "\e[1;31m",  //    red = 0
    "\e[1;33m",  //    orange
    "\e[1;93m",  //    yellow
    "\e[1;32m",  //    green
    "\e[1;36m",  //    cyan
    "\e[1;34m",  //    blue
    "\e[1;35m",  //    purple
    "\e[1;95m",  //    pink
    "\e[1;92m",  //    brightgreen
    "\e[1;91m",  //    brightred
    "\e[1;30m",  //    black
    "\e[1;90m",  //    gray
    "\e[1;37m",  //    brightgray
    "\e[1;97m",  //    white
    "\e[0m",     //    reset
    ""           //    default
};