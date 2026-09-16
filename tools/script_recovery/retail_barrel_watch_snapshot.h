#pragma once
// Proposed scope-owned retained vector for WatchBarrels, DBE890..DBEB17.
#include "FableAPI.h"
#include "GameInterface.h"
#include <cstdint>
#include <stdexcept>

class RetailBarrelWatchSnapshot {
public:
    struct Vector { CScriptThing* begin=nullptr; CScriptThing* end=nullptr; CScriptThing* capacity=nullptr; };
    static_assert(sizeof(Vector)==12 && sizeof(CScriptThing)==12,"Retail barrel snapshot requires x86");
    using Getter=int(__thiscall*)(CGameScriptInterfaceBase*,const CCharString*,Vector*);
    using Destroy=void(__thiscall*)(Vector*);
    struct APIs { Getter get; Destroy destroy; };
    static APIs NativeAPIs() {
        return {reinterpret_cast<Getter>(GetAllThingsWithScriptName_API),ASLR<Destroy>(0x008AC970)};
    }
    explicit RetailBarrelWatchSnapshot(CGameScriptInterfaceBase* game,APIs apis=NativeAPIs()):game(game),apis(apis) {
        if(!game || !apis.get || !apis.destroy || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail barrel snapshot APIs unavailable");
    }
    RetailBarrelWatchSnapshot(const RetailBarrelWatchSnapshot&)=delete;
    RetailBarrelWatchSnapshot& operator=(const RetailBarrelWatchSnapshot&)=delete;
    ~RetailBarrelWatchSnapshot(){try {Close();}catch(...) {}}
    int Refresh() {
        CheckOpen();
        CCharString key{};
        CCharString_Construct_Literal(&key,"NOVI_Barrel",-1);
        int result;
        try {result=apis.get(game,&key,&vector);}
        catch(...) {try {CCharString_Destroy(&key);}catch(...) {} throw;}
        CCharString_Destroy(&key);
        return result;
    }
    int Count() const {
        CheckOpen();
        // The native caller counts vector bytes independently of Refresh's result.
        auto bytes=static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(vector.end)-reinterpret_cast<std::uintptr_t>(vector.begin));
        return bytes/12;
    }
    void Close() {
        if(closed)return;
        closed=true;
        apis.destroy(&vector);
    }
private:
    void CheckOpen() const {if(closed)throw std::runtime_error("Retail barrel snapshot is closed");}
    CGameScriptInterfaceBase* game;
    APIs apis;
    Vector vector{};
    bool closed=false;
};
