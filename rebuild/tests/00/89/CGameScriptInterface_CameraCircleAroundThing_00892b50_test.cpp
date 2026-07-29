#include <cstdio>
#include <cstring>

class C3DVector
{
public:
    unsigned long m_X;
    unsigned long m_Y;
    unsigned long m_Z;
};

class CScriptThing
{
};

class CGSICameraCircleThing_ModuleBase
{
public:
    void Construct();
};

class CGSICameraCircleThing_Receiver
{
public:
    void SetScriptedCameraMode(
        void* pModule,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraCircleAroundThing(
        const CScriptThing& target,
        const C3DVector& offset,
        float speed) const;
};

struct ThingVftable
{
    unsigned char m_Pad00[0x2C];
    void* m_GetTarget;
};

struct ThingStorage
{
    ThingVftable* m_pVftable;
};

struct ResolverVftable
{
    unsigned char m_Pad00[0x24];
    void* m_Resolve;
};

struct Resolver
{
    ResolverVftable* m_pVftable;
};

struct Owner
{
    unsigned char m_Pad00[0x34];
    Resolver* m_pResolver;
};

struct InterfaceStorage
{
    unsigned char m_Pad00[0x04];
    Owner* m_pOwner;
};

struct ModuleOverlay
{
    void* m_pVftable;
    unsigned char m_Pad04[0x48];
    void* m_pTarget;
    unsigned long m_X;
    unsigned long m_Y;
    unsigned long m_Z;
    unsigned long m_SpeedBits;
};

struct CallbackState
{
    long m_Kind;
    void* m_pCallback;
    void* m_pModule;
};

void* CGSICameraCircleThing_ModuleVftable[1];
unsigned char CGSICameraCircleThing_Callback;

static void* g_pTarget;
static CGSICameraCircleThing_Receiver* g_pReceiver;
static ModuleOverlay* g_pModuleStorage;
static CallbackState* g_pCallbackStorage;
static unsigned long g_GetTargetCalls;
static unsigned long g_ResolveCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_BaseConstructCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_FirstSize;
static unsigned long g_SecondSize;
static void* g_pReceivedModule;
static void* g_pReceivedCallback;

static void* __fastcall GetTarget(CScriptThing*)
{
    ++g_GetTargetCalls;
    return g_pTarget;
}

static void __cdecl ResolveHelper(
    Resolver* pResolver,
    CGSICameraCircleThing_Receiver** ppReceiver)
{
    if (pResolver == 0 || ppReceiver == 0)
        std::printf("resolver arguments mismatch\n");
    ++g_ResolveCalls;
    *ppReceiver = g_pReceiver;
}

static void __declspec(naked) ResolveThunk()
{
    __asm
    {
        push dword ptr [esp + 4]
        push ecx
        call ResolveHelper
        add esp, 8
        ret 4
    }
}

void* __cdecl CGSICameraCircleThing_Allocate(unsigned long size)
{
    ++g_AllocateCalls;
    if (g_AllocateCalls == 1)
    {
        g_FirstSize = size;
        return g_pModuleStorage;
    }
    g_SecondSize = size;
    return g_pCallbackStorage;
}

void CGSICameraCircleThing_ModuleBase::Construct()
{
    if (this != reinterpret_cast<CGSICameraCircleThing_ModuleBase*>(
            g_pModuleStorage))
        std::printf("base this mismatch\n");
    ++g_BaseConstructCalls;
}

void CGSICameraCircleThing_Receiver::SetScriptedCameraMode(
    void* pModule,
    void* pCallbackState)
{
    if (this != g_pReceiver)
        std::printf("receiver this mismatch\n");
    ++g_SetModeCalls;
    g_pReceivedModule = pModule;
    g_pReceivedCallback = pCallbackState;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00892b50_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCalls()
{
    g_GetTargetCalls = 0;
    g_AllocateCalls = 0;
    g_BaseConstructCalls = 0;
    g_SetModeCalls = 0;
    g_FirstSize = 0;
    g_SecondSize = 0;
    g_pReceivedModule = 0;
    g_pReceivedCallback = 0;
}

static float FloatWithBits(unsigned long bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

int main()
{
    ThingVftable thingVftable = {};
    ThingStorage thingStorage;
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(thingStorage);
    ResolverVftable resolverVftable = {};
    Resolver resolver;
    Owner owner = {};
    InterfaceStorage storage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    C3DVector offset = {0x11223344, 0x55667788, 0x99AABBCC};
    ModuleOverlay module = {};
    CallbackState callback = {};

    thingVftable.m_GetTarget = reinterpret_cast<void*>(GetTarget);
    thingStorage.m_pVftable = &thingVftable;
    resolverVftable.m_Resolve = reinterpret_cast<void*>(ResolveThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    storage.m_pOwner = &owner;
    g_pTarget = reinterpret_cast<void*>(0x3456789A);
    g_pReceiver =
        reinterpret_cast<CGSICameraCircleThing_Receiver*>(0x12345678);

    ResetCalls();
    g_pModuleStorage = 0;
    g_pCallbackStorage = &callback;
    interfaceObject.CGameScriptInterface::CameraCircleAroundThing(
        scriptThing, offset, FloatWithBits(0x7FC12345));
    if (g_AllocateCalls != 1 || g_FirstSize != 0x60 ||
        g_GetTargetCalls != 0 || g_BaseConstructCalls != 0 ||
        g_SetModeCalls != 1 || g_pReceivedModule != 0 ||
        g_pReceivedCallback != 0)
        return Fail("module allocation fallback");

    ResetCalls();
    g_pModuleStorage = &module;
    g_pCallbackStorage = 0;
    interfaceObject.CGameScriptInterface::CameraCircleAroundThing(
        scriptThing, offset, FloatWithBits(0x80000000));
    if (g_AllocateCalls != 2 || g_SecondSize != 0x0C ||
        g_GetTargetCalls != 1 || g_BaseConstructCalls != 1 ||
        g_SetModeCalls != 1 || g_pReceivedModule != &module ||
        g_pReceivedCallback != 0)
        return Fail("callback allocation fallback");
    if (module.m_pVftable != CGSICameraCircleThing_ModuleVftable ||
        module.m_pTarget != g_pTarget ||
        module.m_X != offset.m_X || module.m_Y != offset.m_Y ||
        module.m_Z != offset.m_Z ||
        module.m_SpeedBits != 0x80000000)
        return Fail("module fields");

    ResetCalls();
    g_pCallbackStorage = &callback;
    callback.m_Kind = 0;
    callback.m_pCallback = 0;
    callback.m_pModule = 0;
    interfaceObject.CGameScriptInterface::CameraCircleAroundThing(
        scriptThing, offset, FloatWithBits(0x7F800000));
    if (g_SetModeCalls != 1 || g_pReceivedCallback != &callback ||
        callback.m_Kind != 1 ||
        callback.m_pCallback != &CGSICameraCircleThing_Callback ||
        callback.m_pModule != &module ||
        module.m_SpeedBits != 0x7F800000)
        return Fail("successful callback or raw speed");
    if (g_ResolveCalls != 3)
        return Fail("resolver calls");

    std::printf("FSE2_00892b50_TEST PASS\n");
    return 0;
}
