#include <cstdio>
#include <cstring>

class C3DVector
{
public:
    unsigned long m_Marker;
};

class CScriptThing
{
};

class CGSICameraLookingAtThing_Interpolator
{
public:
    CGSICameraLookingAtThing_Interpolator* Construct(
        void* pTarget,
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        long scaledDuration,
        float blend);
};

class CGSICameraLookingAtThing_ModeArguments
{
public:
    void Construct(
        CGSICameraLookingAtThing_Interpolator* pInterpolator);
};

class CGSICameraLookingAtThing_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraLookingAtThing_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveBetweenLookingAt(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        const CScriptThing& target,
        float duration,
        float blend) const;
};

struct ThingVftable
{
    unsigned char m_Pad00[0x2C];
    void* m_GetTarget;
    unsigned char m_Pad30[0x12C - 0x30];
    void* m_IsValid;
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

struct ArgumentStorage
{
    CGSICameraLookingAtThing_Interpolator* m_pInterpolator;
    void* m_pCallback;
};

float CGSICameraLookingAtThing_ClampThreshold = -1.0f;
long CGSICameraLookingAtThing_Scale = 15;

static bool g_IsValid;
static void* g_pTarget;
static CGSICameraLookingAtThing_Receiver* g_pReceiver;
static CGSICameraLookingAtThing_Interpolator* g_pInterpolatorStorage;
static const C3DVector* g_pExpectedFirst;
static const C3DVector* g_pExpectedSecond;
static long g_ExpectedScaledDuration;
static unsigned long g_ExpectedBlendBits;
static void* g_pCallbackValue;
static unsigned long g_IsValidCalls;
static unsigned long g_GetTargetCalls;
static unsigned long g_ResolveCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_ConstructCalls;
static unsigned long g_ModeArgumentCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_AllocationSize;
static CGSICameraLookingAtThing_Interpolator* g_pReceivedInterpolator;
static void* g_pReceivedCallback;

static bool __fastcall IsValid(CScriptThing*)
{
    ++g_IsValidCalls;
    return g_IsValid;
}

static void* __fastcall GetTarget(CScriptThing*)
{
    ++g_GetTargetCalls;
    return g_pTarget;
}

static void __cdecl ResolveHelper(
    Resolver* pResolver,
    CGSICameraLookingAtThing_Receiver** ppReceiver)
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

void* __cdecl CGSICameraLookingAtThing_Allocate(unsigned long size)
{
    ++g_AllocateCalls;
    g_AllocationSize = size;
    return g_pInterpolatorStorage;
}

long __declspec(naked) __cdecl
CGSICameraLookingAtThing_FloatToLong()
{
    __asm
    {
        sub esp, 4
        fistp dword ptr [esp]
        mov eax, dword ptr [esp]
        add esp, 4
        ret
    }
}

CGSICameraLookingAtThing_Interpolator*
CGSICameraLookingAtThing_Interpolator::Construct(
    void* pTarget,
    const C3DVector& firstPosition,
    const C3DVector& secondPosition,
    long scaledDuration,
    float blend)
{
    ++g_ConstructCalls;
    unsigned long blendBits;
    std::memcpy(&blendBits, &blend, sizeof(blendBits));
    if (this != g_pInterpolatorStorage ||
        pTarget != g_pTarget ||
        &firstPosition != g_pExpectedFirst ||
        &secondPosition != g_pExpectedSecond ||
        scaledDuration != g_ExpectedScaledDuration ||
        blendBits != g_ExpectedBlendBits)
    {
        std::printf("constructor arguments mismatch\n");
    }
    return this;
}

void CGSICameraLookingAtThing_ModeArguments::Construct(
    CGSICameraLookingAtThing_Interpolator* pInterpolator)
{
    ++g_ModeArgumentCalls;
    ArgumentStorage* const pThis =
        reinterpret_cast<ArgumentStorage*>(this);
    pThis->m_pInterpolator = pInterpolator;
    pThis->m_pCallback =
        pInterpolator != 0 ? g_pCallbackValue : 0;
}

void CGSICameraLookingAtThing_Receiver::SetScriptedCameraMode(
    CGSICameraLookingAtThing_Interpolator* pInterpolator,
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
    std::printf("FSE2_008926d0_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCalls()
{
    g_IsValidCalls = 0;
    g_GetTargetCalls = 0;
    g_ResolveCalls = 0;
    g_AllocateCalls = 0;
    g_ConstructCalls = 0;
    g_ModeArgumentCalls = 0;
    g_SetModeCalls = 0;
    g_AllocationSize = 0;
    g_pReceivedInterpolator = 0;
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
    C3DVector first = {0x11};
    C3DVector second = {0x22};
    unsigned char interpolatorBytes[0x90] = {};

    thingVftable.m_IsValid = reinterpret_cast<void*>(IsValid);
    thingVftable.m_GetTarget = reinterpret_cast<void*>(GetTarget);
    thingStorage.m_pVftable = &thingVftable;
    resolverVftable.m_Resolve = reinterpret_cast<void*>(ResolveThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    storage.m_pOwner = &owner;
    g_pTarget = reinterpret_cast<void*>(0x3456789A);
    g_pReceiver =
        reinterpret_cast<CGSICameraLookingAtThing_Receiver*>(
            0x12345678);
    g_pExpectedFirst = &first;
    g_pExpectedSecond = &second;
    g_pCallbackValue = reinterpret_cast<void*>(0x87654321);

    ResetCalls();
    g_IsValid = false;
    interfaceObject.CGameScriptInterface::CameraMoveBetweenLookingAt(
        first, second, scriptThing, 2.0f, -1.0f);
    if (g_IsValidCalls != 1 || g_GetTargetCalls != 0 ||
        g_ResolveCalls != 0 || g_SetModeCalls != 0)
        return Fail("invalid thing guard");

    ResetCalls();
    g_IsValid = true;
    g_pInterpolatorStorage = 0;
    interfaceObject.CGameScriptInterface::CameraMoveBetweenLookingAt(
        first, second, scriptThing, 2.0f, -1.0f);
    if (g_GetTargetCalls != 1 || g_ResolveCalls != 1 ||
        g_AllocateCalls != 1 || g_AllocationSize != 0x90 ||
        g_ConstructCalls != 0 || g_ModeArgumentCalls != 1 ||
        g_SetModeCalls != 1 || g_pReceivedInterpolator != 0 ||
        g_pReceivedCallback != 0)
        return Fail("allocation fallback");

    ResetCalls();
    g_pInterpolatorStorage =
        reinterpret_cast<CGSICameraLookingAtThing_Interpolator*>(
            interpolatorBytes);
    g_ExpectedScaledDuration = 30;
    g_ExpectedBlendBits = 0x3E4CCCCD;
    interfaceObject.CGameScriptInterface::CameraMoveBetweenLookingAt(
        first, second, scriptThing, 2.0f, -1.0f);
    if (g_ConstructCalls != 1 || g_ModeArgumentCalls != 1 ||
        g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != g_pInterpolatorStorage ||
        g_pReceivedCallback != g_pCallbackValue)
        return Fail("clamped successful dispatch");

    ResetCalls();
    g_ExpectedScaledDuration = -30;
    g_ExpectedBlendBits = 0x7FC12345;
    interfaceObject.CGameScriptInterface::CameraMoveBetweenLookingAt(
        first, second, scriptThing, -2.0f,
        FloatWithBits(g_ExpectedBlendBits));
    if (g_ConstructCalls != 1 || g_SetModeCalls != 1)
        return Fail("NaN blend or negative scale");

    std::printf("FSE2_008926d0_TEST PASS\n");
    return 0;
}
