#include <array>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "cliColors.hpp"
using namespace nlohmann;

#define saveVal(val, to) to[#val] = val;
#define loadVal(val, from) val = from[#val].get_to(val);
#define saveColors(colors, to) \
    to["Color_1"] = colors[0]; \
    to["Color_2"] = colors[1]; \
    to["Color_3"] = colors[2];
#define loadColors(colors, from)       \
    from["Color_1"].get_to(colors[0]); \
    from["Color_2"].get_to(colors[1]); \
    from["Color_3"].get_to(colors[2]);

struct Values {
    bool Enabled = true;
    std::string Format;

    Colors colors[3]{_default, _default, _default};

    json save(json& j) const {
        saveVal(Enabled, j);
        saveVal(Format, j);
        saveColors(colors, j);
        return j;
    }

    void load(const json& j) {
        loadVal(Enabled, j);
        loadVal(Format, j);
        loadColors(colors, j);
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
    static Values time, date;

    static void init() {
        std::filesystem::exists(filename) ? load() : save();
    }

    static void save() {
        time.Format = "H:M:S";
        date.Format = "d m y";
        saveVal(deltaTime, mainJson);
        saveVal(separator, mainJson);
        saveVal(timeOnNewLine, mainJson);
        time.save(mainJson["time"]);
        date.save(mainJson["date"]);
        std::ofstream(filename) << mainJson;
    }

    static void load() {
        std::ifstream(filename) >> mainJson;
        loadVal(deltaTime, mainJson);
        loadVal(separator, mainJson);
        loadVal(timeOnNewLine, mainJson);
        time.load(mainJson["time"]);
        date.load(mainJson["date"]);
    }
};

const std::string Config::filename{"digitalClock.json"};
json Config::mainJson{};
size_t Config::deltaTime{50};
bool Config::timeOnNewLine{true};
std::string Config::separator{};
Values Config::time;
Values Config::date;

#undef saveVal
#undef saveColors
#undef loadVal
#undef loadColors