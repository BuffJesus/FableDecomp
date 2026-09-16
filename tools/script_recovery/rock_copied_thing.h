#pragma once
#include "LuaRetailResources.h"

// Ownership core only. Native EC52F0 copies all three words and increments Info
// independently of Data. This is NOT the retained scheduler argument itself.
class RockCopiedThing {
public:
    RockCopiedThing(CGameScriptInterfaceBase* game,const CScriptThing* source):game_(game) {
        if(!game || !source || !RetailThing_Destroy_API)
            throw std::runtime_error("Copied Thing requires source and native destructor");
        thing_=*source;
        if(thing_.pImp.Info)++thing_.pImp.Info->RefCount;
    }
    RockCopiedThing(const RockCopiedThing&)=delete;
    RockCopiedThing& operator=(const RockCopiedThing&)=delete;
    ~RockCopiedThing(){try{Close();}catch(...){}}
    void Close(){if(open_){open_=false;RetailThing_Destroy_API(&thing_);}}
    float GetHealth(){
        if(!open_)throw std::runtime_error("Copied Thing scope is closed");
        if(!GetHealth_API)throw std::runtime_error("Health API unavailable");
        return GetHealth_API(game_,&thing_);
    }
private:
    CGameScriptInterfaceBase* game_;
    CScriptThing thing_{};
    bool open_=true;
};
inline void RegisterRockCopiedThing(sol::state& lua){
    auto type=lua.new_usertype<RockCopiedThing>("RockCopiedThing",sol::no_constructor);
    type["GetHealth"]=&RockCopiedThing::GetHealth;
}
inline void WithRockCopiedThing(CGameScriptInterfaceBase* game,const CScriptThing* source,sol::protected_function callback){
    auto scope=std::make_shared<RockCopiedThing>(game,source);
    auto result=callback(scope);
    if(!result.valid()){
        sol::error primary=result;
        try{scope->Close();}catch(...){}
        throw primary;
    }
    scope->Close();
}
