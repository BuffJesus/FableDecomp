#pragma once
#include "FableAPI.h"
#include "sol/sol.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

// Quest-owned native map<CCharString,bool>. Retail MazeResearch constructs this
// header/sentinel inline at EA7716..EA773B and destroys it through 7ADD50.
// Lua references from separate VMs share the C++ object, never a sol::object.
class LuaRetailFlags {
public:
    LuaRetailFlags() {
        static_assert(sizeof(void*) == 4, "Retail flags require x86");
        if (!Game_malloc || !Game_free || !StdMap_Bool_OperatorBracket_API ||
            !StdMap_Bool_Destroy_API || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail boolean-map APIs are unavailable");
        m_map.head = Game_malloc(0x18);
        if (!m_map.head) throw std::bad_alloc();
        std::memset(m_map.head, 0, 0x18);
        auto links = static_cast<void**>(m_map.head);
        links[2] = m_map.head;
        links[3] = m_map.head;
    }
    ~LuaRetailFlags() { StdMap_Bool_Destroy_API(&m_map); }
    LuaRetailFlags(const LuaRetailFlags&) = delete;
    LuaRetailFlags& operator=(const LuaRetailFlags&) = delete;
    bool Get(const std::string& name) { return *Entry(name); }
    void Set(const std::string& name, bool value) { *Entry(name) = value; }
    void* NativeMap() { return &m_map; }
private:
    bool* Entry(const std::string& name) {
        FableString key(name.c_str());
        auto value = StdMap_Bool_OperatorBracket_API(&m_map, key);
        if (!value) throw std::runtime_error("Retail boolean-map insertion failed");
        return value;
    }
    struct { void* head = nullptr; unsigned count = 0; } m_map;
};

inline void RegisterRetailFlags(sol::state& lua) {
    auto type = lua.new_usertype<LuaRetailFlags>("RetailFlags", sol::no_constructor);
    type["Get"] = &LuaRetailFlags::Get;
    type["Set"] = &LuaRetailFlags::Set;
}
