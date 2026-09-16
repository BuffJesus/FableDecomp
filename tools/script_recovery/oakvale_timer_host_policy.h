#pragma once
#include "sol/sol.hpp"
#include "retail_oakvale_timers.h"
#include <memory>
#include <string>

// Registry metadata, never inferred from quest names or file paths.
enum class NativeQuestLifetime { None, NewOakValeIntro };
inline NativeQuestLifetime ParseNativeQuestLifetime(const sol::object& value) {
    if(!value.valid() || value.get_type()==sol::type::nil)return NativeQuestLifetime::None;
    if(!value.is<std::string>())throw std::runtime_error("nativeLifetime must be a string");
    if(value.as<std::string>()=="NewOakValeIntro")return NativeQuestLifetime::NewOakValeIntro;
    throw std::runtime_error("Unknown nativeLifetime policy");
}
struct NativeQuestSlot {
    std::string file;
    NativeQuestLifetime lifetime=NativeQuestLifetime::None;
};
class NativeQuestTimerOwner {
    bool configured=false;
    std::unique_ptr<RetailOakvaleTimers> timers;
public:
    void Configure(NativeQuestLifetime policy, RetailOakvaleTimers::InterfaceSource provider=nullptr) {
        if(configured)throw std::runtime_error("Native quest lifetime already configured");
        configured=true;
        if(policy==NativeQuestLifetime::NewOakValeIntro) {
            if(provider)timers=std::make_unique<RetailOakvaleTimers>(provider);
            else timers=std::make_unique<RetailOakvaleTimers>();
        } else if(policy!=NativeQuestLifetime::None)throw std::runtime_error("Unknown native quest lifetime");
    }
    bool Read(const std::string& key,int& value) const {
        if(!timers)return false;
        if(key=="TalkIntermittentTimer"){value=timers->Ambient();return true;}
        if(key=="WatchTimer"){value=timers->Watch();return true;}
        return false;
    }
    void RejectOwnedWrite(const std::string& key) const {
        int ignored;
        if(Read(key,ignored))throw std::runtime_error("Native timer IDs are transient read-only fields");
    }
};
