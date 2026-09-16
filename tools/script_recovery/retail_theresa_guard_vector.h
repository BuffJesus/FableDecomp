#pragma once
// Unapplied proposal. The resource scope must own this object until explicit Close.
#include "FableAPI.h"
#include "GameInterface.h"
#include <stdexcept>

class RetailTheresaGuardVector {
public:
    struct Vector { CScriptThing* begin=nullptr; CScriptThing* end=nullptr; CScriptThing* capacity=nullptr; };
    static_assert(sizeof(Vector)==12,"Retail Thing vectors require x86");
    using Getter=int(__thiscall*)(CGameScriptInterfaceBase*,const CCharString*,Vector*);
    using Remove=void(__fastcall*)(CGameScriptInterfaceBase*,Vector*,bool);
    using Destroy=void(__thiscall*)(Vector*);
    struct APIs { Getter get; Remove remove; Destroy destroy; };
    static APIs NativeAPIs() {
        return {reinterpret_cast<Getter>(GetAllThingsWithScriptName_API),
                ASLR<Remove>(0x00CBED82),ASLR<Destroy>(0x008AC970)};
    }
    explicit RetailTheresaGuardVector(CGameScriptInterfaceBase* game,APIs apis=NativeAPIs())
        : game(game),apis(apis) {
        if(!game || !apis.get || !apis.remove || !apis.destroy || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Theresa guard vector unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,"NOVI_Guard",-1);
        try {apis.get(game,&key,&vector);}
        catch(...) {
            try {CCharString_Destroy(&key);}catch(...) {}
            try {apis.destroy(&vector);}catch(...) {}
            throw;
        }
        try {CCharString_Destroy(&key);}
        catch(...) {try {apis.destroy(&vector);}catch(...) {} throw;}
    }
    RetailTheresaGuardVector(const RetailTheresaGuardVector&)=delete;
    RetailTheresaGuardVector& operator=(const RetailTheresaGuardVector&)=delete;
    ~RetailTheresaGuardVector(){try {Close();}catch(...) {}}
    void RemoveLivingGuards() {
        if(closed)throw std::runtime_error("Theresa guard vector is closed");
        apis.remove(game,&vector,false);
    }
    void Close() {
        if(closed)return;
        closed=true;
        apis.destroy(&vector);
    }
private:
    CGameScriptInterfaceBase* game;
    APIs apis;
    Vector vector{};
    bool closed=false;
};
