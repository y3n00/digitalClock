#include <array>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "cliColors.hpp"
using namespace nlohmann;

#define saveVal(val, name, to) to[#name] = val;
#define loadVal(val, name, from) from.at(#name).get_to(val);
struct values {
    values(std::string_view format)
        : Format{format} {}
    bool Enabled = true;
    std::string Format;
    Colors colors[3] = {_default, _default, _default};

    [[nodiscard]] json
    save(json& j) const {
        saveVal(Enabled, Enabled, j);
        saveVal(Format, Format, j);
        saveVal(colors[0], 1stVal, j);
        saveVal(colors[1], 2ndVal, j);
        saveVal(colors[2], 3rdVal, j);
        return j;
    }

    void load(const json& j) {
        loadVal(Enabled, Enabled, j);
        loadVal(Format, Format, j);
        loadVal(colors[0], 1stVal, j);
        loadVal(colors[1], 2ndVal, j);
        loadVal(colors[2], 3rdVal, j);
    }
};

class Config {
   private:
    static const std::string filename;
    static json mainJson;

   public:
    static size_t deltaTime;
    static bool timeOnNewLine;
    static std::string separator;
    static values time, date;

    static void init() {
        std::filesystem::exists(filename) ? load() : save();
    }

    static void save() {
        saveVal(deltaTime, deltaTime, mainJson);
        saveVal(separator, separator, mainJson);
        saveVal(timeOnNewLine, timeOnNewLine, mainJson);

        time.save(mainJson["time"]);
        date.save(mainJson["date"]);
        std::ofstream(filename) << mainJson;
    }

    static void load() {
        std::ifstream(filename) >> mainJson;
        loadVal(deltaTime, deltaTime, mainJson);
        loadVal(separator, separator, mainJson);
        loadVal(timeOnNewLine, timeOnNewLine, mainJson);
        time.load(mainJson["time"]);
        date.load(mainJson["date"]);
    }
};

const std::string Config::filename = "digitalClock.json";
json Config::mainJson;
size_t Config::deltaTime = 50;
bool Config::timeOnNewLine;
std::string Config::separator;
values Config::time("H:M:S");
values Config::date("d m y");

#undef saveVal
#undef loadVal
#undef loadValAs