#pragma once
#include "arena_round_snapshot.h"
#include <cstring>

// Included after NoviCCharStringToStd in NoviUnitBindings.h. Strings in these
// definition records are CCharString values, not CDefString conversion tokens.
struct NoviArenaDefinitionReader {
    std::uint32_t Word(std::uint32_t address) const {
        std::uint32_t value;
        std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(value));
        return value;
    }
    std::int32_t Signed(std::uint32_t address) const {
        std::int32_t value;
        std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(value));
        return value;
    }
    unsigned char Byte(std::uint32_t address) const {
        return *reinterpret_cast<const unsigned char*>(address);
    }
    std::string String(std::uint32_t address) const {
        return NoviCCharStringToStd(reinterpret_cast<const CCharString*>(address));
    }
};

inline void RegisterNoviArenaRounds(sol::usertype<LuaQuestState>& quest) {
    quest["InitialiseArenaRounds"] = [](LuaQuestState& self) {
        static_assert(sizeof(void*) == 4, "Arena definitions require the retail x86 ABI");
        if (!CCharString_ToConstChar_API)
            throw std::runtime_error("Arena definition-string API unavailable");
        NoviArenaDefinitionReader memory;
        const auto definitions = *ASLR<std::uint32_t*>(0x0143e90c);
        const auto snapshot = arena_rounds::Read(memory, definitions);
        arena_rounds::Store(self, snapshot);
    };
}
