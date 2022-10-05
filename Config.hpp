#pragma once
#include <fmt/format.h>

#include <fstream>
#include <nlohmann/json.hpp>

#include "cliColors.hpp"

using namespace cliColors;
using namespace nlohmann;

class Config {
   private:
    static const std::string filename;
    json jConfig;
    struct CFG {
        bool timeOnNewLine = false;
        bool timeEnabled = true, dateEnabled = true;

        Colors
            timeH{_default},
            timeM{_default},
            timeS{_default},
            dateD{_default},
            dateM{_default},
            dateY{_default};

        std::string timeFormat = "H:M:S", dateFormat = "d-m-y";
        std::string separator = "          ";
    } cfg;

    void Helpers() {
        json& jMain = jConfig["Helpers"];

        jMain["Colors"] = {
            "Red = 0",
            "Orange = 1",
            "Yellow = 2",
            "Green = 3",
            "Cyan = 4",
            "Blue = 5",
            "Purple = 6",
            "Pink = 7",
            "Brightgree = 8",
            "Brightred = 9",
            "Black = 10",
            "Gray = 11",
            "Brightgray = 12",
            "White = 13",
            "Reset = 14",
            "Default = 15"};

        jMain["Separators"] = {{"Date and time separators", {":", "-", " "}},
                               {"To change single line clock separator change this in settings", "singleLineSep"}};
    }

   public:
    Config() { std::filesystem::exists(filename) ? load() : init(); }

    Config(Config&&) = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    void init() {
        Helpers();
        json& jSettings = jConfig["Settings"];

        jSettings["timeOnNewLine"] = cfg.timeOnNewLine;
        jSettings["singleLineSep"] = cfg.separator;

        json& jTime = jSettings["Time"];
        json& jTimeColor = jTime["Colors"];
        jTime["Enabled"] = cfg.timeEnabled;
        jTime["Format"] = cfg.timeFormat;
        jTimeColor["Hours"] = cfg.timeH;
        jTimeColor["Minutes"] = cfg.timeM;
        jTimeColor["Seconds"] = cfg.timeS;

        json& jDate = jSettings["Date"];
        json& jDateColor = jDate["Colors"];
        jDate["Enabled"] = cfg.dateEnabled;
        jDate["Format"] = cfg.dateFormat;
        jDateColor["day"] = cfg.dateD;
        jDateColor["month"] = cfg.dateM;
        jDateColor["year"] = cfg.dateY;

        std::ofstream(filename) << jConfig;
    }

    void load() {
        jConfig.clear();
        std::ifstream(filename) >> jConfig;

        json jSettings = jConfig["Settings"];

        json& jTime = jSettings["Time"];
        json& jTimeColor = jTime["Colors"];
        cfg.timeOnNewLine = jSettings["timeOnNewLine"].get<bool>();
        cfg.timeEnabled = jTime["Enabled"].get<bool>();
        cfg.timeFormat = jTime["Format"].get<std::string>();
        cfg.timeH = jTimeColor["Hours"].get<Colors>();
        cfg.timeM = jTimeColor["Minutes"].get<Colors>();
        cfg.timeS = jTimeColor["Seconds"].get<Colors>();

        json& jDate = jSettings["Date"];
        json& jDateColor = jDate["Colors"];
        cfg.dateEnabled = jDate["Enabled"].get<bool>();
        cfg.dateFormat = jDate["Format"].get<std::string>();
        cfg.dateD = jDateColor["day"].get<Colors>();
        cfg.dateM = jDateColor["month"].get<Colors>();
        cfg.dateY = jDateColor["year"].get<Colors>();
        cfg.separator = jSettings["singleLineSep"].get<std::string>();
    }

    const auto get() const { return &cfg; }
};
const std::string Config::filename = "config.json";
