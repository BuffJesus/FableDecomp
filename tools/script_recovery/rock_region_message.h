#pragma once
#include "LuaRetailResources.h"
#include <exception>

// Retail default constructor, not the literal constructor or zero initialization:
// 99E4B0 also increments the native CCharString instance counter.
using RockDefaultStringCtor = CCharString* (__thiscall*)(CCharString*);
class RockRegionMessage {
public:
    RockRegionMessage(CGameScriptInterfaceBase* game, RockDefaultStringCtor ctor,
                      tMsgOnRegionLoaded poll, tCCharString_Destructor destroy)
        : game_(game), poll_(poll), destroy_(destroy) {
        if (!game || !ctor || !poll || !destroy)
            throw std::runtime_error("Region message APIs unavailable");
        ctor(&value_);
        open_ = true;
    }
    RockRegionMessage(const RockRegionMessage&) = delete;
    RockRegionMessage& operator=(const RockRegionMessage&) = delete;
    ~RockRegionMessage() noexcept { try { Close(); } catch (...) {} }
    bool Poll() {
        if (!open_) throw std::runtime_error("Region message scope closed");
        return poll_(game_, &value_); // AL only; true+empty and false+populated matter.
    }
    void Close() {
        if (!open_) return;
        open_ = false;
        destroy_(&value_); // Native destructor is required even for an empty buffer.
    }
private:
    CGameScriptInterfaceBase* game_;
    tMsgOnRegionLoaded poll_;
    tCCharString_Destructor destroy_;
    CCharString value_{};
    bool open_ = false;
};
inline void RegisterRockRegionMessage(sol::state& lua) {
    auto type = lua.new_usertype<RockRegionMessage>("RockRegionMessage", sol::no_constructor);
    type["Poll"] = &RockRegionMessage::Poll;
}
inline void WithRockRegionMessage(CGameScriptInterfaceBase* game, sol::protected_function callback,
    RockDefaultStringCtor ctor, tMsgOnRegionLoaded poll, tCCharString_Destructor destroy) {
    auto scope = std::make_shared<RockRegionMessage>(game, ctor, poll, destroy);
    auto result = callback(scope);
    if (!result.valid()) {
        sol::error primary = result;
        try { scope->Close(); } catch (...) {} // Preserve the original Lua/engine-double error.
        throw primary;
    }
    scope->Close();
}
