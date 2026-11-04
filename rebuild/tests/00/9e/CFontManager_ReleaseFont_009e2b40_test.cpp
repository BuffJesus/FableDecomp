#include <stdio.h>
#include <string.h>
#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "fable_symbol_map.h"

static CSymbolMap* staticSymbols = reinterpret_cast<CSymbolMap*>(0x11000);
static CSymbolMap* streamingSymbols = reinterpret_cast<CSymbolMap*>(0x22000);
static CFontDataBank* staticBank = reinterpret_cast<CFontDataBank*>(0x33000);
static CStreamingFontDataBank* streamingBank = reinterpret_cast<CStreamingFontDataBank*>(0x44000);
static const CCharString* expectedName;
static bool hasStatic, hasStreaming, canUnload;
static int existsStatic, existsStreaming, indexStatic, indexStreaming;
static int bankStatic, bankStreaming, canCalls, unloadCalls;
static bool FABLE_FASTCALL CanUnload(void*) { ++canCalls; return canUnload; }
static void FABLE_FASTCALL Unload(void*) { ++unloadCalls; }
static unsigned long staticEntry[1], fakeStreamingEntry[1];
struct CFontManagerTestAccess {
    static void set(CFontManager& manager) {
        manager.PSymbols.Data = staticSymbols;
        manager.PStreamingSymbols.Data = streamingSymbols;
        manager.PFontDataBank.Data = staticBank;
        manager.PStreamingFontDataBank.Data = streamingBank;
    }
};
bool CSymbolMap::SymbolExists(const CCharString& name) const {
    if (&name != expectedName) return false;
    if (this == staticSymbols) { ++existsStatic; return hasStatic; }
    if (this == streamingSymbols) { ++existsStreaming; return hasStreaming; }
    return false;
}
long CSymbolMap::GetSymbolSLONG(const CCharString& name) const {
    if (&name != expectedName) return -1;
    if (this == staticSymbols) { ++indexStatic; return 7; }
    if (this == streamingSymbols) { ++indexStreaming; return 11; }
    return -1;
}
CFontDataBankEntry* CFontDataBank::GetFontInfo(unsigned long index) const {
    if (this != staticBank || index != 7) return 0;
    ++bankStatic; return reinterpret_cast<CFontDataBankEntry*>(&staticEntry);
}
CStreamingFontDataBankEntry* CStreamingFontDataBank::GetFontInfo(unsigned long index) const {
    if (this != streamingBank || index != 11) return 0;
    ++bankStreaming; return reinterpret_cast<CStreamingFontDataBankEntry*>(&fakeStreamingEntry);
}
static void resetCalls() { existsStatic=existsStreaming=indexStatic=indexStreaming=bankStatic=bankStreaming=canCalls=unloadCalls=0; }
int main() {
    unsigned long storage[32]; memset(storage, 0, sizeof(storage));
    CFontManager& manager=*reinterpret_cast<CFontManager*>(storage);
    CFontManagerTestAccess::set(manager);
    void* methods[4] = {0, 0, (void*)&CanUnload, (void*)&Unload};
    staticEntry[0] = reinterpret_cast<unsigned long>(methods);
    fakeStreamingEntry[0] = reinterpret_cast<unsigned long>(methods);
    unsigned long nameStorage=0;
    const CCharString& name=*reinterpret_cast<CCharString*>(&nameStorage);
    expectedName=&name;
    hasStatic=true;hasStreaming=true;canUnload=true;resetCalls();manager.ReleaseFont(name);
    if (existsStatic!=1||existsStreaming||indexStatic!=1||indexStreaming||bankStatic!=1||bankStreaming||canCalls!=1||unloadCalls!=1) return 1;
    hasStatic=false;hasStreaming=true;resetCalls();manager.ReleaseFont(name);
    if (existsStatic!=1||existsStreaming!=1||indexStatic||indexStreaming!=1||bankStatic||bankStreaming!=1||canCalls!=1||unloadCalls!=1) return 2;
    hasStatic=false;hasStreaming=false;resetCalls();manager.ReleaseFont(name);
    if (existsStatic!=1||existsStreaming!=1||indexStatic||indexStreaming||bankStatic||bankStreaming||canCalls||unloadCalls) return 3;
    hasStatic=true;hasStreaming=false;canUnload=false;resetCalls();manager.ReleaseFont(name);
    if (existsStatic!=1||existsStreaming||indexStatic!=1||bankStatic!=1||canCalls!=1||unloadCalls) return 4;
    puts("FONT_RELEASE_009e2b40_PASS");return 0;
}