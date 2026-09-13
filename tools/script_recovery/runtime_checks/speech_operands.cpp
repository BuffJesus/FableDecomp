// Compile the actual Speak registration with real sol, forwarding into a double.
#include "FableAPI.h"
#include "EntityScriptingAPI.h"
#include "sol/sol.hpp"
#include <iostream>
#include <memory>
#include <string>

struct SpeechProbe {
    CScriptThing* me = nullptr;
    CScriptThing* hero = nullptr;
    std::string last;
    bool Speak_Blocking(CScriptThing* speaker, const std::shared_ptr<CScriptThing>& target,
                        const std::string& key, int method, bool listen, bool sound2d, bool overFade) {
        if (speaker != me || target.get() != hero || key != "OPERAND_PROBE")
            throw std::runtime_error("Speak actor/key forwarding mismatch");
        last = std::to_string(method) + ":" + (listen ? "1" : "0") + (sound2d ? "1" : "0") + (overFade ? "1" : "0");
        return true;
    }
};

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Expected speech call-site fixture");
        sol::state lua; lua.open_libraries(sol::lib::base);
        CScriptThing me{}; auto hero = std::make_shared<CScriptThing>();
        SpeechProbe probe; probe.me = &me; probe.hero = hero.get();
        auto* entityAPI = &probe;
        auto cscriptThing_type = lua.new_usertype<CScriptThing>("Thing", sol::no_constructor);
        // Verbatim current three-overload registration from LuaManager.cpp.
#include "speech-registration.inc"
        lua["me"] = &me; lua["hero"] = hero;
        lua["last"] = [&probe]() { return probe.last; };
        lua.script("quest = {GetHero = function() return hero end}");
        lua.script_file(argv[1]);
        std::cout << "PASS: actual Speak overloads preserve actors, selection and 010 speech flags\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
