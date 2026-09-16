#pragma once
#include "FableAPI.h"
#include "GameInterface.h"
#include <exception>
#include <stdexcept>

// Owns native quest fields +104 (ambient conversation) and +108 (barrel watch).
// Attach to the quest owner before Init; destroy before its speech-list owner.
class RetailOakvaleTimers {
public:
    using InterfaceSource=CGameScriptInterfaceBase*(*)();
private:
    InterfaceSource source;
    int ambient=0,watch=0;
    bool ambientLive=false,watchLive=false,closed=false;
    static CGameScriptInterfaceBase* CurrentInterface() {
        return *ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
    }
    CGameScriptInterfaceBase* Game() {
        auto* game=source();
        if(!game || !*reinterpret_cast<void***>(game))
            throw std::runtime_error("Oakvale timer interface unavailable");
        return game;
    }
    int Register() {
        auto* game=Game();
        auto method=reinterpret_cast<tRegisterTimer>((*reinterpret_cast<void***>(game))[0x15c/4]);
        if(!method)throw std::runtime_error("Oakvale timer registration unavailable");
        return method(game);
    }
    void Deregister(int id) {
        auto* game=Game();
        auto method=reinterpret_cast<tDeregisterTimer>((*reinterpret_cast<void***>(game))[0x160/4]);
        if(!method)throw std::runtime_error("Oakvale timer deregistration unavailable");
        method(game,id);
    }
public:
    explicit RetailOakvaleTimers(InterfaceSource provider=&CurrentInterface):source(provider) {
        if(!source)throw std::runtime_error("Oakvale timer source unavailable");
        try {
            ambient=Register();ambientLive=true;
            watch=Register();watchLive=true;
        } catch(...) {try{Close();}catch(...){}throw;}
    }
    RetailOakvaleTimers(const RetailOakvaleTimers&)=delete;
    RetailOakvaleTimers& operator=(const RetailOakvaleTimers&)=delete;
    ~RetailOakvaleTimers(){try{Close();}catch(...) {}}
    int Ambient() const {
        if(closed)throw std::runtime_error("Oakvale timers closed");
        return ambient;
    }
    int Watch() const {
        if(closed)throw std::runtime_error("Oakvale timers closed");
        return watch;
    }
    void Close() {
        if(closed)return;
        closed=true;
        std::exception_ptr failure;
        if(watchLive){watchLive=false;try{Deregister(watch);}catch(...){failure=std::current_exception();}}
        if(ambientLive){ambientLive=false;try{Deregister(ambient);}catch(...){if(!failure)failure=std::current_exception();}}
        if(failure)std::rethrow_exception(failure);
    }
};
