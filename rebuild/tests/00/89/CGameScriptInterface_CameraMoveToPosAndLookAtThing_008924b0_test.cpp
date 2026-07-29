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

class CGSICameraMoveToThing_Interpolator
{
public:
    CGSICameraMoveToThing_Interpolator* Construct(
        const C3DVector& position,
        void* pTarget);
};

class CGSICameraMoveToThing_ModeArguments
{
public:
    void Construct(
        CGSICameraMoveToThing_Interpolator* pInterpolator);
};

class CGSICameraMoveToThing_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraMoveToThing_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveToPosAndLookAtThing(
        const C3DVector& position,
        const CScriptThing& target,
        float duration) const;
};

struct CGSICameraMoveToThing_ThingVftable
{
    unsigned char m_Pad00[0x2C];
    void* m_GetTarget;
    unsigned char m_Pad30[0x12C - 0x30];
    void* m_IsValid;
};

struct CGSICameraMoveToThing_ThingStorage
{
    CGSICameraMoveToThing_ThingVftable* m_pVftable;
};

struct CGSICameraMoveToThing_Target
{
    unsigned char m_Pad00[0x6C];
    unsigned char m_Flags;
};

struct CGSICameraMoveToThing_ResolverVftable
{
    unsigned char m_Pad00[0x24];
    void* m_ResolveReceiver;
};

struct CGSICameraMoveToThing_Resolver
{
    CGSICameraMoveToThing_ResolverVftable* m_pVftable;
};

struct CGSICameraMoveToThing_InterfaceOwner
{
    unsigned char m_Pad00[0x34];
    CGSICameraMoveToThing_Resolver* m_pResolver;
};

struct CGSICameraMoveToThing_InterfaceStorage
{
    unsigned char m_Pad00[0x04];
    CGSICameraMoveToThing_InterfaceOwner* m_pOwner;
};

struct CGSICameraMoveToThing_ArgumentStorage
{
    CGSICameraMoveToThing_Interpolator* m_pInterpolator;
    void* m_pCallback;
};

static bool g_IsThingValid;
static CGSICameraMoveToThing_Target* g_pTarget;
static CGSICameraMoveToThing_Receiver* g_pReceiver;
static const C3DVector* g_pExpectedPosition;
static CGSICameraMoveToThing_Interpolator* g_pInterpolatorStorage;
static void* g_pCallbackValue;
static unsigned long g_IsValidCalls;
static unsigned long g_GetTargetCalls;
static unsigned long g_ResolverCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_ConstructCalls;
static unsigned long g_ModeArgumentCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_AllocationSize;
static void* g_pConstructTarget;
static CGSICameraMoveToThing_Interpolator* g_pReceivedInterpolator;
static void* g_pReceivedCallback;

static bool __fastcall IsThingValid(CScriptThing*)
{
    ++g_IsValidCalls;
    return g_IsThingValid;
}

static CGSICameraMoveToThing_Target* __fastcall GetTarget(
    CScriptThing*)
{
    ++g_GetTargetCalls;
    return g_pTarget;
}

static void __cdecl ResolveReceiverHelper(
    CGSICameraMoveToThing_Resolver* pResolver,
    CGSICameraMoveToThing_Receiver** ppReceiver)
{
    if (pResolver == 0 || ppReceiver == 0)
        std::printf("resolver arguments mismatch\n");
    ++g_ResolverCalls;
    *ppReceiver = g_pReceiver;
}

static void __declspec(naked) ResolveReceiverThunk()
{
    __asm
    {
        push dword ptr [esp + 4]
        push ecx
        call ResolveReceiverHelper
        add esp, 8
        ret 4
    }
}

void* __cdecl CGSICameraMoveToThing_Allocate(unsigned long size)
{
    ++g_AllocateCalls;
    g_AllocationSize = size;
    return g_pInterpolatorStorage;
}

CGSICameraMoveToThing_Interpolator*
CGSICameraMoveToThing_Interpolator::Construct(
    const C3DVector& position,
    void* pTarget)
{
    ++g_ConstructCalls;
    if (this != g_pInterpolatorStorage ||
        &position != g_pExpectedPosition)
        std::printf("interpolator arguments mismatch\n");
    g_pConstructTarget = pTarget;
    return this;
}

void CGSICameraMoveToThing_ModeArguments::Construct(
    CGSICameraMoveToThing_Interpolator* pInterpolator)
{
    ++g_ModeArgumentCalls;
    CGSICameraMoveToThing_ArgumentStorage* const pThis =
        reinterpret_cast<CGSICameraMoveToThing_ArgumentStorage*>(this);
    pThis->m_pInterpolator = pInterpolator;
    pThis->m_pCallback =
        pInterpolator != 0 ? g_pCallbackValue : 0;
}

void CGSICameraMoveToThing_Receiver::SetScriptedCameraMode(
    CGSICameraMoveToThing_Interpolator* pInterpolator,
    void* pCallbackState)
{
    if (this != g_pReceiver)
        std::printf("receiver this mismatch\n");
    ++g_SetModeCalls;
    g_pReceivedInterpolator = pInterpolator;
    g_pReceivedCallback = pCallbackState;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_008924b0_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCounts()
{
    g_IsValidCalls = 0;
    g_GetTargetCalls = 0;
    g_ResolverCalls = 0;
    g_AllocateCalls = 0;
    g_ConstructCalls = 0;
    g_ModeArgumentCalls = 0;
    g_SetModeCalls = 0;
    g_AllocationSize = 0;
    g_pConstructTarget = 0;
    g_pReceivedInterpolator = 0;
    g_pReceivedCallback = 0;
}

static void SetFloatBits(float& value, unsigned long bits)
{
    std::memcpy(&value, &bits, sizeof(bits));
}

int main()
{
    CGSICameraMoveToThing_ThingVftable thingVftable = {};
    CGSICameraMoveToThing_ThingStorage thingStorage;
    CGSICameraMoveToThing_Target target = {};
    CGSICameraMoveToThing_ResolverVftable resolverVftable = {};
    CGSICameraMoveToThing_Resolver resolver;
    CGSICameraMoveToThing_InterfaceOwner owner = {};
    CGSICameraMoveToThing_InterfaceStorage interfaceStorage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(interfaceStorage);
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(thingStorage);
    C3DVector position = {1, 2, 3};
    unsigned char interpolatorBytes[0x6C] = {};
    float duration;

    thingVftable.m_IsValid = reinterpret_cast<void*>(IsThingValid);
    thingVftable.m_GetTarget = reinterpret_cast<void*>(GetTarget);
    thingStorage.m_pVftable = &thingVftable;
    resolverVftable.m_ResolveReceiver =
        reinterpret_cast<void*>(ResolveReceiverThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    interfaceStorage.m_pOwner = &owner;
    g_pTarget = &target;
    g_pReceiver =
        reinterpret_cast<CGSICameraMoveToThing_Receiver*>(0x12345678);
    g_pExpectedPosition = &position;
    g_pCallbackValue = reinterpret_cast<void*>(0x87654321);

    ResetCounts();
    g_IsThingValid = false;
    SetFloatBits(duration, 0x7FC12345);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtThing(position, scriptThing, duration);
    if (g_IsValidCalls != 1 || g_GetTargetCalls != 0 ||
        g_ResolverCalls != 0 || g_SetModeCalls != 0)
        return Fail("invalid thing guard");

    ResetCounts();
    g_IsThingValid = true;
    target.m_Flags = 0;
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtThing(position, scriptThing, duration);
    if (g_GetTargetCalls != 1 || g_ResolverCalls != 0 ||
        g_SetModeCalls != 0)
        return Fail("target flag guard");

    ResetCounts();
    target.m_Flags = 4;
    g_pInterpolatorStorage = 0;
    SetFloatBits(duration, 0x80000000);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtThing(position, scriptThing, duration);
    if (g_GetTargetCalls != 2 || g_ResolverCalls != 1 ||
        g_AllocateCalls != 1 || g_AllocationSize != 0x6C ||
        g_ConstructCalls != 0 || g_ModeArgumentCalls != 1 ||
        g_SetModeCalls != 1 || g_pReceivedInterpolator != 0 ||
        g_pReceivedCallback != 0)
        return Fail("allocation fallback");

    ResetCounts();
    g_pInterpolatorStorage =
        reinterpret_cast<CGSICameraMoveToThing_Interpolator*>(
            interpolatorBytes);
    SetFloatBits(duration, 0x7F800000);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtThing(position, scriptThing, duration);
    if (g_GetTargetCalls != 2 || g_ResolverCalls != 1 ||
        g_ConstructCalls != 1 || g_pConstructTarget != &target ||
        g_ModeArgumentCalls != 1 || g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != g_pInterpolatorStorage ||
        g_pReceivedCallback != g_pCallbackValue)
        return Fail("successful dispatch");

    std::printf("FSE2_008924b0_TEST PASS\n");
    return 0;
}
