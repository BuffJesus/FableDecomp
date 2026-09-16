#pragma once
#include "FableAPI.h"
#include "GameInterface.h"
#include <stdexcept>

// Retail 99F570 takes two CCharStrings, unlike the existing char* left-hand
// operator overload. It constructs result; callers must not default-construct it.
using WifeConcatStrings = CCharString* (__fastcall*)(CCharString*, const CCharString*, const CCharString*);

struct WifeArgumentKeyAPIs {
    tGFIntToCharString number;
    tCCharString_Constructor_Literal literal;
    WifeConcatStrings concat;
    tCCharString_Destructor destroy;
    tCCharString_AssignmentLiteral assign;
    tTextEntryExists exists;
    tAddLineToConversation line;
};

class RetailWifeArgumentKey {
public:
    RetailWifeArgumentKey(CGameScriptInterfaceBase* game, int number, WifeArgumentKeyAPIs api)
        : game_(game), api_(api) {
        if (!game || !api.number || !api.literal || !api.concat || !api.destroy ||
            !api.assign || !api.exists || !api.line)
            throw std::runtime_error("Wife argument-key APIs unavailable");
        CCharString suffix{}, prefix{};
        bool suffixLive=false, prefixLive=false;
        try {
            api_.number(&suffix, number); suffixLive=true;
            api_.literal(&prefix, "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_", -1); prefixLive=true;
            api_.concat(&key_, &prefix, &suffix); open_=true;
            prefixLive=false; api_.destroy(&prefix);
            suffixLive=false; api_.destroy(&suffix);
        } catch (...) {
            if (prefixLive) try { api_.destroy(&prefix); } catch (...) {}
            if (suffixLive) try { api_.destroy(&suffix); } catch (...) {}
            try { Close(); } catch (...) {}
            throw;
        }
    }
    RetailWifeArgumentKey(const RetailWifeArgumentKey&)=delete;
    RetailWifeArgumentKey& operator=(const RetailWifeArgumentKey&)=delete;
    ~RetailWifeArgumentKey() noexcept { try { Close(); } catch (...) {} }
    bool Exists() { CheckOpen(); return api_.exists(game_, &key_); }
    void ResetToFirst() {
        CheckOpen(); api_.assign(&key_, "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10");
    }
    void AddLine(int conversation, const CScriptThing* speaker, const CScriptThing* listener) {
        CheckOpen(); api_.line(game_, conversation, &key_, false, speaker, listener);
    }
    void Close() {
        if (!open_) return;
        open_=false; api_.destroy(&key_); // Also required when key has empty data.
    }
private:
    void CheckOpen() const { if (!open_) throw std::runtime_error("Wife argument-key scope closed"); }
    CGameScriptInterfaceBase* game_;
    WifeArgumentKeyAPIs api_;
    CCharString key_{};
    bool open_=false;
};
