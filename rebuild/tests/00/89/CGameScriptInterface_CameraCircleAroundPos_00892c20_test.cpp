#include <cstdio>
#include <cstring>

class C3DVector
{
public:
    unsigned long m_X;
    unsigned long m_Y;
    unsigned long m_Z;
};

class CGSICameraCirclePos_ModuleBase
{
public:
    void Construct();
};

class CGSICameraCirclePos_Receiver
{
public:
    void SetScriptedCameraMode(
        void* pModule,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraCircleAroundPos(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        float speed) const;
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
    unsigned long m_FirstX;
    unsigned long m_FirstY;
    unsigned long m_FirstZ;
    unsigned long m_SecondX;
    unsigned long m_SecondY;
    unsigned long m_SecondZ;
    unsigned long m_SpeedBits;
};

struct CallbackState
{
    long m_Kind;
    void* m_pCallback;
    void* m_pModule;
};

void* CGSICameraCirclePos_ModuleVftable[1];
unsigned char CGSICameraCirclePos_Callback;

static CGSICameraCirclePos_Receiver* g_pReceiver;
static ModuleOverlay* g_pModuleStorage;
static CallbackState* g_pCallbackStorage;
static unsigned long g_ResolveCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_BaseConstructCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_FirstSize;
static unsigned long g_SecondSize;
static void* g_pReceivedModule;
static void* g_pReceivedCallback;

static void __cdecl ResolveHelper(
    Resolver* pResolver,
    CGSICameraCirclePos_Receiver** ppReceiver)
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

void* __cdecl CGSICameraCirclePos_Allocate(unsigned long size)
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

void CGSICameraCirclePos_ModuleBase::Construct()
{
    if (this != reinterpret_cast<CGSICameraCirclePos_ModuleBase*>(
            g_pModuleStorage))
        std::printf("base this mismatch\n");
    ++g_BaseConstructCalls;
}

void CGSICameraCirclePos_Receiver::SetScriptedCameraMode(
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
    std::printf("FSE2_00892c20_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCalls()
{
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
    ResolverVftable resolverVftable = {};
    Resolver resolver;
    Owner owner = {};
    InterfaceStorage storage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    C3DVector first = {0x11223344, 0x55667788, 0x99AABBCC};
    C3DVector second = {0x10203040, 0x50607080, 0x90A0B0C0};
    ModuleOverlay module = {};
    CallbackState callback = {};

    resolverVftable.m_Resolve = reinterpret_cast<void*>(ResolveThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    storage.m_pOwner = &owner;
    g_pReceiver =
        reinterpret_cast<CGSICameraCirclePos_Receiver*>(0x12345678);

    ResetCalls();
    g_pModuleStorage = 0;
    g_pCallbackStorage = &callback;
    interfaceObject.CGameScriptInterface::CameraCircleAroundPos(
        first, second, FloatWithBits(0x7FC12345));
    if (g_AllocateCalls != 1 || g_FirstSize != 0x68 ||
        g_BaseConstructCalls != 0 || g_SetModeCalls != 1 ||
        g_pReceivedModule != 0 || g_pReceivedCallback != 0)
        return Fail("module allocation fallback");

    ResetCalls();
    g_pModuleStorage = &module;
    g_pCallbackStorage = 0;
    interfaceObject.CGameScriptInterface::CameraCircleAroundPos(
        first, second, FloatWithBits(0x80000000));
    if (g_AllocateCalls != 2 || g_SecondSize != 0x0C ||
        g_BaseConstructCalls != 1 || g_SetModeCalls != 1 ||
        g_pReceivedModule != &module || g_pReceivedCallback != 0)
        return Fail("callback allocation fallback");
    if (module.m_pVftable != CGSICameraCirclePos_ModuleVftable ||
        module.m_FirstX != first.m_X ||
        module.m_FirstY != first.m_Y ||
        module.m_FirstZ != first.m_Z ||
        module.m_SecondX != second.m_X ||
        module.m_SecondY != second.m_Y ||
        module.m_SecondZ != second.m_Z ||
        module.m_SpeedBits != 0x80000000)
        return Fail("module fields");

    ResetCalls();
    g_pCallbackStorage = &callback;
    callback.m_Kind = 0;
    callback.m_pCallback = 0;
    callback.m_pModule = 0;
    interfaceObject.CGameScriptInterface::CameraCircleAroundPos(
        first, second, FloatWithBits(0x7F800000));
    if (g_SetModeCalls != 1 || g_pReceivedCallback != &callback ||
        callback.m_Kind != 1 ||
        callback.m_pCallback != &CGSICameraCirclePos_Callback ||
        callback.m_pModule != &module ||
        module.m_SpeedBits != 0x7F800000)
        return Fail("successful callback or raw speed");
    if (g_ResolveCalls != 3)
        return Fail("resolver calls");

    std::printf("FSE2_00892c20_TEST PASS\n");
    return 0;
}
