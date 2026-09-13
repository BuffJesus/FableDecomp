// Use the actual vendor sol/Lua conversion, rather than a Python/Lua test double.
#include "sol/sol.hpp"
#include <iostream>

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Expected generated call-site checks");
        sol::state lua; lua.open_libraries(sol::lib::base);
        lua["operands"] = [](bool first, bool second) {
            return std::string(first ? "1" : "0") + (second ? "1" : "0");
        };
        lua.script("assert(operands(0, 0) == '11'); assert(operands(false, false) == '00')");
        auto result = lua.safe_script_file(argv[1]);
        if (!result.valid()) { sol::error error = result; throw error; }
        std::cout << "PASS: numeric zero is truthy; actual conversation call sites pass false bool operands\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
