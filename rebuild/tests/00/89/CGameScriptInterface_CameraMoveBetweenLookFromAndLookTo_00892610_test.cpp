#include <cstdio>

class C3DVector
{
public:
    unsigned long m_Marker;
};

class CGSICameraLookFromTo_Interpolator
{
public:
    CGSICameraLookFromTo_Interpolator* Construct(
        const C3DVector& thirdPosition,
        const C3DVector& firstPosition,
        const C3DVector& fourthPosition,
        const C3DVector& secondPosition,
        long scaledDuration);
};

class CGSICameraLookFromTo_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraLookFromTo_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveBetweenLookFromAndLookTo(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        const C3DVector& thirdPosition,
        const C3DVector& fourthPosition,
        float duration) const;
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

struct CallbackState
{
    long m_Kind;
    void* m_pCallback;
    CGSICameraLookFromTo_Interpolator* m_pInterpolator;
};

long CGSICameraLookFromTo_Scale = 15;
unsigned char CGSICameraLookFromTo_Callback;

static CGSICameraLookFromTo_Receiver* g_pReceiver;
static CGSICameraLookFromTo_Interpolator* g_pInterpolatorStorage;
static CallbackState* g_pCallbackStorage;
static const C3DVector* g_pExpectedFirst;
static const C3DVector* g_pExpectedSecond;
static const C3DVector* g_pExpectedThird;
static const C3DVector* g_pExpectedFourth;
static long g_ExpectedScaledDuration;
static unsigned long g_ResolveCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_ConstructCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_FirstSize;
static unsigned long g_SecondSize;
static CGSICameraLookFromTo_Interpolator* g_pReceivedInterpolator;
static void* g_pReceivedCallback;

static void __cdecl ResolveHelper(
    Resolver* pResolver,
    CGSICameraLookFromTo_Receiver** ppReceiver)
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

void* __cdecl CGSICameraLookFromTo_Allocate(unsigned long size)
{
    ++g_AllocateCalls;
    if (g_AllocateCalls == 1)
    {
        g_FirstSize = size;
        return g_pInterpolatorStorage;
    }
    g_SecondSize = size;
    return g_pCallbackStorage;
}

long __declspec(naked) __cdecl
CGSICameraLookFromTo_FloatToLong()
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

CGSICameraLookFromTo_Interpolator*
CGSICameraLookFromTo_Interpolator::Construct(
    const C3DVector& thirdPosition,
    const C3DVector& firstPosition,
    const C3DVector& fourthPosition,
    const C3DVector& secondPosition,
    long scaledDuration)
{
    ++g_ConstructCalls;
    if (this != g_pInterpolatorStorage ||
        &firstPosition != g_pExpectedFirst ||
        &secondPosition != g_pExpectedSecond ||
        &thirdPosition != g_pExpectedThird ||
        &fourthPosition != g_pExpectedFourth ||
        scaledDuration != g_ExpectedScaledDuration)
    {
        std::printf("constructor arguments mismatch\n");
    }
    return this;
}

void CGSICameraLookFromTo_Receiver::SetScriptedCameraMode(
    CGSICameraLookFromTo_Interpolator* pInterpolator,
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
    std::printf("FSE2_00892610_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCalls()
{
    g_AllocateCalls = 0;
    g_ConstructCalls = 0;
    g_SetModeCalls = 0;
    g_FirstSize = 0;
    g_SecondSize = 0;
    g_pReceivedInterpolator = 0;
    g_pReceivedCallback = 0;
}

int main()
{
    ResolverVftable resolverVftable = {};
    Resolver resolver;
    Owner owner = {};
    InterfaceStorage storage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    C3DVector first = {0x11};
    C3DVector second = {0x22};
    C3DVector third = {0x33};
    C3DVector fourth = {0x44};
    unsigned char interpolatorBytes[0x80] = {};
    CallbackState callback = {};

    resolverVftable.m_Resolve = reinterpret_cast<void*>(ResolveThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    storage.m_pOwner = &owner;
    g_pReceiver =
        reinterpret_cast<CGSICameraLookFromTo_Receiver*>(
            0x12345678);
    g_pExpectedFirst = &first;
    g_pExpectedSecond = &second;
    g_pExpectedThird = &third;
    g_pExpectedFourth = &fourth;

    ResetCalls();
    g_pInterpolatorStorage = 0;
    g_pCallbackStorage = &callback;
    interfaceObject.CGameScriptInterface::
        CameraMoveBetweenLookFromAndLookTo(
            first, second, third, fourth, 2.0f);
    if (g_AllocateCalls != 1 || g_FirstSize != 0x80 ||
        g_ConstructCalls != 0 || g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != 0 || g_pReceivedCallback != 0)
        return Fail("interpolator allocation fallback");

    ResetCalls();
    g_pInterpolatorStorage =
        reinterpret_cast<CGSICameraLookFromTo_Interpolator*>(
            interpolatorBytes);
    g_pCallbackStorage = 0;
    g_ExpectedScaledDuration = -30;
    interfaceObject.CGameScriptInterface::
        CameraMoveBetweenLookFromAndLookTo(
            first, second, third, fourth, -2.0f);
    if (g_AllocateCalls != 2 || g_FirstSize != 0x80 ||
        g_SecondSize != 0x0C || g_ConstructCalls != 1 ||
        g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != g_pInterpolatorStorage ||
        g_pReceivedCallback != 0)
        return Fail("callback allocation fallback");

    ResetCalls();
    g_pCallbackStorage = &callback;
    callback.m_Kind = 0;
    callback.m_pCallback = 0;
    callback.m_pInterpolator = 0;
    g_ExpectedScaledDuration = 45;
    interfaceObject.CGameScriptInterface::
        CameraMoveBetweenLookFromAndLookTo(
            first, second, third, fourth, 3.0f);
    if (g_ConstructCalls != 1 || g_SetModeCalls != 1 ||
        g_pReceivedCallback != &callback ||
        callback.m_Kind != 1 ||
        callback.m_pCallback != &CGSICameraLookFromTo_Callback ||
        callback.m_pInterpolator != g_pInterpolatorStorage)
        return Fail("successful callback state");
    if (g_ResolveCalls != 3)
        return Fail("resolver calls");

    std::printf("FSE2_00892610_TEST PASS\n");
    return 0;
}
