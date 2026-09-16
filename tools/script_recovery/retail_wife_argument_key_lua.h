#pragma once
#include "retail_wife_argument_key.h"
#include <sol/sol.hpp>
#include <memory>

inline void RegisterRetailWifeArgumentKey(sol::state& lua) {
    auto type=lua.new_usertype<RetailWifeArgumentKey>("RetailWifeArgumentKey",sol::no_constructor);
    type["Exists"]=&RetailWifeArgumentKey::Exists;
    type["ResetToFirst"]=&RetailWifeArgumentKey::ResetToFirst;
}

inline bool WithRetailWifeArgumentKey(CGameScriptInterfaceBase* game,int number,
    sol::protected_function callback,WifeArgumentKeyAPIs api) {
    auto key=std::make_shared<RetailWifeArgumentKey>(game,number,api);
    auto result=callback(key);
    if (!result.valid()) {
        sol::error primary=result;
        try { key->Close(); } catch (...) {}
        throw primary;
    }
    key->Close();
    if (result.get_type()!=sol::type::boolean)
        throw std::runtime_error("Wife argument-key callback must return continuation boolean");
    return result.get<bool>();
}
