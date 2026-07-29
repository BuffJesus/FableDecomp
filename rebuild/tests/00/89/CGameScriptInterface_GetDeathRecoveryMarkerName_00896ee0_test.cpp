// Real-object behavior harness for
// CGameScriptInterface::GetDeathRecoveryMarkerName @ 0x00896EE0.
// The candidate body is supplied by the separately compiled source object.

#include <cstdio>
#include <cstring>

struct CCharString
{
    void* data;
};

struct DeathRecoveryInterface
{
    char pad_000[0x170];
    CCharString markerName;
};

struct TCInterfaceEntry
{
    int key;
    DeathRecoveryInterface* value;
};

struct TCInterfaceMap
{
    TCInterfaceEntry* begin;
    TCInterfaceEntry* end;
};

struct PlayerCreature
{
    char pad_00[0x20];
    unsigned char flags20;
    char pad_21[0x23];
    TCInterfaceMap interfaces;
    char pad_4c[0x45];
    unsigned char flags91;
};

struct CGameScriptInterfaceLayout
{
    char pad_00[0x14];
    void* playerSlot;
};

extern "C" CCharString* __fastcall
CGameScriptInterface_GetDeathRecoveryMarkerName(
    const CGameScriptInterfaceLayout* self,
    void*,
    CCharString* result);

extern "C" const char FableGsiGetDeathRecoveryEmpty[] = "";

static void* g_playerHandle = (void*)0x11112222;
static PlayerCreature* g_creature;
static int g_getHandleCalls;
static int g_getCreatureCalls;
static int g_lowerBoundCalls;
static int g_copyCalls;
static int g_literalCalls;
static void* g_getHandleThis;
static void* g_getCreatureThis;
static TCInterfaceMap* g_lowerBoundThis;
static int g_lowerBoundKey;
static TCInterfaceEntry* g_lowerBoundResult;
static const CCharString* g_copySource;
static const char* g_literalText;
static int g_literalLength;

extern "C" void* __fastcall
FableGsiGetDeathRecoveryPlayerHandle(void* playerSlot, void*)
{
    ++g_getHandleCalls;
    g_getHandleThis = playerSlot;
    return g_playerHandle;
}

extern "C" void* __fastcall
FableGsiGetDeathRecoveryCreature(void* playerHandle, void*)
{
    ++g_getCreatureCalls;
    g_getCreatureThis = playerHandle;
    return g_creature;
}

extern "C" void* __fastcall
FableGsiGetDeathRecoveryLowerBound(
    void* map,
    void*,
    const int* key)
{
    ++g_lowerBoundCalls;
    g_lowerBoundThis = (TCInterfaceMap*)map;
    g_lowerBoundKey = *key;
    return g_lowerBoundResult;
}

extern "C" void __fastcall
FableGsiGetDeathRecoveryCopy(
    CCharString* result,
    void*,
    const CCharString* source)
{
    ++g_copyCalls;
    g_copySource = source;
    result->data = source->data;
}

extern "C" void __fastcall
FableGsiGetDeathRecoveryLiteral(
    CCharString* result,
    void*,
    const char* text,
    int length)
{
    ++g_literalCalls;
    g_literalText = text;
    g_literalLength = length;
    result->data = 0;
}

static void ResetCalls()
{
    g_getHandleCalls = 0;
    g_getCreatureCalls = 0;
    g_lowerBoundCalls = 0;
    g_copyCalls = 0;
    g_literalCalls = 0;
    g_getHandleThis = 0;
    g_getCreatureThis = 0;
    g_lowerBoundThis = 0;
    g_lowerBoundKey = -1;
    g_copySource = 0;
    g_literalText = 0;
    g_literalLength = 0;
}

static int CheckEmptyResult(
    CGameScriptInterfaceLayout& self,
    PlayerCreature* creature,
    int failureBase)
{
    ResetCalls();
    g_creature = creature;
    CCharString result;
    result.data = (void*)0xffffffff;

    CCharString* returned =
        CGameScriptInterface_GetDeathRecoveryMarkerName(&self, 0, &result);
    if (returned != &result)
        return failureBase;
    if (g_getHandleCalls != 1 || g_getCreatureCalls != 1)
        return failureBase + 1;
    if (g_getHandleThis != self.playerSlot ||
        g_getCreatureThis != g_playerHandle)
        return failureBase + 2;
    if (g_lowerBoundCalls != 0 || g_copyCalls != 0)
        return failureBase + 3;
    if (g_literalCalls != 1 || g_literalLength != -1)
        return failureBase + 4;
    if (g_literalText == 0 ||
        std::strcmp(g_literalText, "") != 0 ||
        result.data != 0)
        return failureBase + 5;
    return 0;
}

int main()
{
    CGameScriptInterfaceLayout self;
    std::memset(&self, 0, sizeof(self));
    self.playerSlot = (void*)0x33334444;

    PlayerCreature creature;
    std::memset(&creature, 0, sizeof(creature));

    int failure = CheckEmptyResult(self, 0, 1);
    if (failure != 0)
        return failure;

    creature.flags20 = 0x10;
    creature.flags91 = 1;
    failure = CheckEmptyResult(self, &creature, 10);
    if (failure != 0)
        return failure;

    creature.flags20 = 0;
    creature.flags91 = 0;
    failure = CheckEmptyResult(self, &creature, 20);
    if (failure != 0)
        return failure;

    char recoveryStorage[sizeof(DeathRecoveryInterface) * 2];
    DeathRecoveryInterface* recovery =
        (DeathRecoveryInterface*)recoveryStorage;
    std::memset(recoveryStorage, 0, sizeof(recoveryStorage));
    recovery[0].markerName.data = (void*)0x12345678;
    recovery[1].markerName.data = (void*)0x87654321;

    TCInterfaceEntry entries[2];
    entries[0].key = 4;
    entries[0].value = &recovery[0];
    entries[1].key = 7;
    entries[1].value = &recovery[1];
    creature.flags20 = 0x10;
    creature.flags91 = 0;
    creature.interfaces.begin = entries;
    creature.interfaces.end = entries + 1;

    ResetCalls();
    g_creature = &creature;
    g_lowerBoundResult = entries;
    CCharString exact;
    exact.data = 0;
    CCharString* returned =
        CGameScriptInterface_GetDeathRecoveryMarkerName(&self, 0, &exact);
    if (returned != &exact ||
        g_lowerBoundCalls != 1 ||
        g_lowerBoundThis != &creature.interfaces ||
        g_lowerBoundKey != 4)
        return 30;
    if (g_copyCalls != 1 ||
        g_copySource != &recovery[0].markerName ||
        exact.data != recovery[0].markerName.data)
        return 31;
    if (g_literalCalls != 0)
        return 32;

    ResetCalls();
    g_creature = &creature;
    g_lowerBoundResult = entries + 1;
    CCharString atEnd;
    atEnd.data = 0;
    CGameScriptInterface_GetDeathRecoveryMarkerName(&self, 0, &atEnd);
    if (g_copyCalls != 1 ||
        g_copySource != &recovery[1].markerName ||
        atEnd.data != recovery[1].markerName.data)
        return 40;

    ResetCalls();
    g_creature = &creature;
    g_lowerBoundResult = entries;
    entries[0].key = 5;
    CCharString aboveKey;
    aboveKey.data = 0;
    CGameScriptInterface_GetDeathRecoveryMarkerName(&self, 0, &aboveKey);
    if (g_copyCalls != 1 ||
        g_copySource != &recovery[1].markerName ||
        aboveKey.data != recovery[1].markerName.data)
        return 50;

    std::printf("FSE2_00896ee0_TEST PASS\n");
    return 0;
}
