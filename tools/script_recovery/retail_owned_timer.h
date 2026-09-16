#pragma once
#include "GameInterface.h"
#include <memory>
#include <stdexcept>
#include <sol/sol.hpp>

// Retail CTimer uses the current global interface at every boundary, including
// destruction. An ID of zero or -1 is still the ID returned by registration.
class RetailOwnedTimer {
    int m_id=0;
    bool m_open=false;
    static CGameScriptInterfaceBase* Current() {
        auto* game=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!game || !*reinterpret_cast<void***>(game))
            throw std::runtime_error("Retail timer interface unavailable");
        return game;
    }
    template<class T> static T Slot(CGameScriptInterfaceBase* game,unsigned offset) {
        auto target=(*reinterpret_cast<void***>(game))[offset/sizeof(void*)];
        if(!target)throw std::runtime_error("Retail timer slot unavailable");
        return reinterpret_cast<T>(target);
    }
    void CheckOpen() const {if(!m_open)throw std::runtime_error("Retail timer is closed");}
public:
    RetailOwnedTimer() {
        auto* game=Current();m_id=Slot<tRegisterTimer>(game,0x15C)(game);m_open=true;
    }
    RetailOwnedTimer(const RetailOwnedTimer&)=delete;
    RetailOwnedTimer& operator=(const RetailOwnedTimer&)=delete;
    ~RetailOwnedTimer() {try {Close();} catch(...) {}}
    void Set(int value) {
        CheckOpen();auto* game=Current();Slot<tSetTimer>(game,0x164)(game,m_id,value);
    }
    int Get() {
        CheckOpen();auto* game=Current();return Slot<tGetTimer>(game,0x168)(game,m_id);
    }
    void Close() {
        if(!m_open)return;
        m_open=false;
        auto* game=Current();Slot<tDeregisterTimer>(game,0x160)(game,m_id);
    }
};

inline void RegisterRetailOwnedTimer(sol::state& lua) {
    auto type=lua.new_usertype<RetailOwnedTimer>("RetailOwnedTimer",sol::no_constructor);
    type["Set"]=&RetailOwnedTimer::Set;
    type["Get"]=&RetailOwnedTimer::Get;
}

inline bool WithRetailOwnedTimer(sol::protected_function callback) {
    auto timer=std::make_shared<RetailOwnedTimer>();
    auto result=callback(timer);
    if(!result.valid()) {
        sol::error primary=result;
        try {timer->Close();}catch(...) {}
        throw primary;
    }
    timer->Close();
    if(result.get_type()!=sol::type::boolean)
        throw std::runtime_error("Retail timer callback must return continuation boolean");
    return result.get<bool>();
}
