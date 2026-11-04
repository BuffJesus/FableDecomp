#include "fable_definition_manager.h"
#include "fable_game.h"
#include "fable_rgb_colour.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

// Real symbols the reconstructed Shutdown references.
CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
CFontManager* g_FableFontManager_013B838C;
CASoundManager* g_FableSoundManager_013B8394;
CDisplayManager* g_FableDisplayManager_013B8390;

static int seq;
static bool ok;
static void* frontend;
static int defUnlockAt, setAt, cleanAt, slot16At, slot18At, updAt, clrAt, xmvAt,
    namesAt, timesAt, clearRtAt, swapAt;
static long slot18Arg;
static unsigned char clearColour[4];
static unsigned long clearFlags;

// Def pointee: slot 1 runs when the count reaches zero.
struct TestDef : CDefPointeeBase
{
    virtual void DeleteCIVCountedPointeeBase() const { defUnlockAt = ++seq; }
};
// The def base is CResource (retail); its out-of-line pieces.
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
CResource::CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) {}
void CResourceList::RemoveResource(CResource&) {}

void FABLE_FASTCALL CStartupDefinitionManager::Set(CStartupDefinitionManager* manager)
{
    setAt = ++seq;
    ok = ok && manager == 0 &&
        *reinterpret_cast<void**>(static_cast<char*>(frontend) + 0x3C) == 0;
}

void CNewFrontendGameComponent::CleanUpInterfaces()
{
    cleanAt = ++seq;
    ok = ok && this == frontend && g_FableFrontendGameComponent_013B871C == 0 &&
        reinterpret_cast<unsigned char*>(g_FableFontManager_013B838C)[0x38] == 0;
}

void CNewFrontendGameComponent::ReleaseXMVCode()
{
    xmvAt = ++seq;
    ok = ok && this == frontend;
}

// Sound manager: a hand-built vtable of thiscall stand-ins.
static unsigned long soundObject[0x40];
static void* soundVtable[60];
static void FABLE_FASTCALL Slot16(void* self, void*)
{
    slot16At = ++seq;
    ok = ok && self == soundObject;
}
static void FABLE_FASTCALL Slot18(void* self, void*, long arg)
{
    slot18At = ++seq;
    slot18Arg = arg;
    ok = ok && self == soundObject;
}
static void FABLE_FASTCALL SlotBad(void*, void*) { ok = false; }
void CASoundManager::UnknownUpdateEntries_00991750()
{
    updAt = ++seq;
    ok = ok && static_cast<void*>(this) == soundObject;
}
void CASoundManager::UnknownClearEntries_009918F0()
{
    clrAt = ++seq;
    ok = ok && static_cast<void*>(this) == soundObject;
}

template <> CWideString* _STL::vector<CWideString>::erase(CWideString* first, CWideString* last)
{
    namesAt = ++seq;
    ok = ok && static_cast<void*>(this) == static_cast<char*>(frontend) + 0x6C &&
        first == reinterpret_cast<CWideString*>(0x1000) &&
        last == reinterpret_cast<CWideString*>(0x1010);
    return first;
}
template <> CDateAndTime* _STL::vector<CDateAndTime>::erase(CDateAndTime* first, CDateAndTime* last)
{
    timesAt = ++seq;
    ok = ok && static_cast<void*>(this) == static_cast<char*>(frontend) + 0x78 &&
        first == reinterpret_cast<CDateAndTime*>(0x2000) &&
        last == reinterpret_cast<CDateAndTime*>(0x2038);
    return first;
}

static unsigned long displayObject[0x2C8 / 4];
void CDisplayManager::ClearRenderTarget(const CRGBColour& colour, unsigned long flags)
{
    clearRtAt = ++seq;
    memcpy(clearColour, &colour, 4);
    clearFlags = flags;
    ok = ok && static_cast<void*>(this) == displayObject;
}
bool CDisplayManager::SwapScreens()
{
    swapAt = ++seq;
    ok = ok && static_cast<void*>(this) == displayObject;
    return true;
}

static unsigned long fontObject[0x3C / 4];

static bool Run(bool createGame, bool haveSound, bool haveDisplay, bool haveDef, unsigned long defCount)
{
    unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    unsigned char* b = reinterpret_cast<unsigned char*>(storage);
    frontend = storage;

    TestDef def;
    def.IVRefCount = defCount;
    void* defPtr = haveDef ? static_cast<CDefPointeeBase*>(&def) : 0;
    memcpy(b + 0x3C, &defPtr, 4);
    b[0x29] = createGame ? 1 : 0;
    unsigned long v[4] = { 0x1000, 0x1010, 0x2000, 0x2038 };
    memcpy(b + 0x6C, &v[0], 8);
    memcpy(b + 0x78, &v[2], 8);
    memcpy(expected, storage, sizeof(storage));
    memset(expected + 0x3C, 0, 4);

    memset(fontObject, 0, sizeof(fontObject));
    g_FableFontManager_013B838C = reinterpret_cast<CFontManager*>(fontObject);
    for (int i = 0; i < 60; ++i) soundVtable[i] = (void*)&SlotBad;
    soundVtable[0x40 / 4] = (void*)&Slot16;
    soundVtable[0x48 / 4] = (void*)&Slot18;
    soundObject[0] = (unsigned long)soundVtable;
    g_FableSoundManager_013B8394 = haveSound ? reinterpret_cast<CASoundManager*>(soundObject) : 0;
    g_FableDisplayManager_013B8390 = haveDisplay ? reinterpret_cast<CDisplayManager*>(displayObject) : 0;
    g_FableFrontendGameComponent_013B871C = reinterpret_cast<CNewFrontendGameComponent*>(storage);

    seq = defUnlockAt = setAt = cleanAt = slot16At = slot18At = updAt = clrAt = xmvAt =
        namesAt = timesAt = clearRtAt = swapAt = 0;
    slot18Arg = -1;
    clearFlags = 0;
    memset(clearColour, 0x55, 4);
    ok = true;

    reinterpret_cast<CNewFrontendGameComponent*>(storage)->CNewFrontendGameComponent::Shutdown();

    int n = 1;
    bool unlocks = haveDef && defCount == 1;
    ok = ok && (unlocks ? defUnlockAt == n++ : defUnlockAt == 0);
    ok = ok && (!haveDef || def.IVRefCount == defCount - 1);
    ok = ok && setAt == n++ && cleanAt == n++;
    ok = ok && g_FableFrontendGameComponent_013B871C == 0;
    ok = ok && reinterpret_cast<unsigned char*>(fontObject)[0x38] == 1;
    bool sound = !createGame && haveSound;
    if (sound)
        ok = ok && slot16At == n++ && slot18At == n++ && slot18Arg == 0 && updAt == n++ && clrAt == n++;
    else
        ok = ok && slot16At == 0 && slot18At == 0 && updAt == 0 && clrAt == 0;
    ok = ok && xmvAt == n++ && namesAt == n++ && timesAt == n++;
    if (haveDisplay)
    {
        static const unsigned char black[4] = { 0, 0, 0, 0xFF };
        ok = ok && clearRtAt == n++ && swapAt == n++ && clearFlags == 7 &&
            memcmp(clearColour, black, 4) == 0;
    }
    else
        ok = ok && clearRtAt == 0 && swapAt == 0;
    ok = ok && memcmp(expected, storage, sizeof(storage)) == 0;
    return ok;
}

// VC7.1 treats these out-of-class specializations of an in-class-defined
// member as inline COMDATs; reference them so the test object emits them.
typedef CWideString* (_STL::vector<CWideString>::*EraseNames)(CWideString*, CWideString*);
typedef CDateAndTime* (_STL::vector<CDateAndTime>::*EraseTimes)(CDateAndTime*, CDateAndTime*);
EraseNames volatile g_eraseNames = &_STL::vector<CWideString>::erase;
EraseTimes volatile g_eraseTimes = &_STL::vector<CDateAndTime>::erase;

int main()
{
    if (!Run(false, true, true, true, 1)) { puts("case1"); return 1; }
    if (!Run(true, true, true, true, 3)) { puts("case2"); return 1; }
    if (!Run(false, false, false, false, 0)) { puts("case3"); return 1; }
    if (!Run(false, true, false, true, 2)) { puts("case4"); return 1; }
    puts("FABLETLC_NEW_FRONTEND_SHUTDOWN PASS");
    return 0;
}