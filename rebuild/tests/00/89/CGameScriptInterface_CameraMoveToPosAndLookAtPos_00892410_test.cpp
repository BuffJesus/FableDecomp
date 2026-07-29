#include <cstdio>
#include <cstring>

class C3DVector
{
public:
    unsigned long m_X;
    unsigned long m_Y;
    unsigned long m_Z;
};

class CGSICameraMove_Interpolator
{
public:
    CGSICameraMove_Interpolator* Construct(
        const C3DVector& position,
        const C3DVector& lookAtPosition);
};

class CGSICameraMove_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraMove_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveToPosAndLookAtPos(
        const C3DVector& position,
        const C3DVector& lookAtPosition,
        float duration) const;
};

struct CGSICameraMove_CallbackState
{
    long m_Kind;
    void* m_pCallback;
    CGSICameraMove_Interpolator* m_pInterpolator;
};

struct CGSICameraMove_ResolverVftable
{
    unsigned char m_Pad00[0x24];
    void* m_ResolveReceiver;
};

struct CGSICameraMove_Resolver
{
    CGSICameraMove_ResolverVftable* m_pVftable;
};

struct CGSICameraMove_InterfaceOwner
{
    unsigned char m_Pad00[0x34];
    CGSICameraMove_Resolver* m_pResolver;
};

struct CGSICameraMove_InterfaceStorage
{
    unsigned char m_Pad00[0x04];
    CGSICameraMove_InterfaceOwner* m_pOwner;
};

unsigned char CGSICameraMove_Callback;

static const C3DVector* g_pExpectedPosition;
static const C3DVector* g_pExpectedLookAt;
static CGSICameraMove_Receiver* g_pReceiver;
static CGSICameraMove_Interpolator* g_pInterpolatorStorage;
static CGSICameraMove_CallbackState* g_pCallbackStorage;
static unsigned long g_ResolverCalls;
static unsigned long g_AllocateCalls;
static unsigned long g_ConstructCalls;
static unsigned long g_SetModeCalls;
static unsigned long g_FirstAllocationSize;
static unsigned long g_SecondAllocationSize;
static CGSICameraMove_Interpolator* g_pReceivedInterpolator;
static void* g_pReceivedCallback;

static void __cdecl ResolveReceiverHelper(
    CGSICameraMove_Resolver* pResolver,
    CGSICameraMove_Receiver** ppReceiver)
{
    if (pResolver == 0 || ppReceiver == 0)
    {
        std::printf("resolver arguments mismatch\n");
        return;
    }
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

void* __cdecl CGSICameraMove_Allocate(unsigned long size)
{
    ++g_AllocateCalls;
    if (g_AllocateCalls == 1)
    {
        g_FirstAllocationSize = size;
        return g_pInterpolatorStorage;
    }
    g_SecondAllocationSize = size;
    return g_pCallbackStorage;
}

CGSICameraMove_Interpolator*
CGSICameraMove_Interpolator::Construct(
    const C3DVector& position,
    const C3DVector& lookAtPosition)
{
    ++g_ConstructCalls;
    if (this != g_pInterpolatorStorage ||
        &position != g_pExpectedPosition ||
        &lookAtPosition != g_pExpectedLookAt)
    {
        std::printf("constructor arguments mismatch\n");
    }
    return this;
}

void CGSICameraMove_Receiver::SetScriptedCameraMode(
    CGSICameraMove_Interpolator* pInterpolator,
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
    std::printf("FSE2_00892410_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCallState()
{
    g_AllocateCalls = 0;
    g_ConstructCalls = 0;
    g_SetModeCalls = 0;
    g_FirstAllocationSize = 0;
    g_SecondAllocationSize = 0;
    g_pReceivedInterpolator = 0;
    g_pReceivedCallback = 0;
}

static void SetFloatBits(float& value, unsigned long bits)
{
    std::memcpy(&value, &bits, sizeof(bits));
}

int main()
{
    CGSICameraMove_ResolverVftable resolverVftable = {};
    CGSICameraMove_Resolver resolver;
    CGSICameraMove_InterfaceOwner owner = {};
    CGSICameraMove_InterfaceStorage storage = {};
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    C3DVector position = {1, 2, 3};
    C3DVector lookAt = {4, 5, 6};
    unsigned char interpolatorBytes[0x6C] = {};
    CGSICameraMove_CallbackState callbackState = {};
    float duration;

    resolverVftable.m_ResolveReceiver =
        reinterpret_cast<void*>(ResolveReceiverThunk);
    resolver.m_pVftable = &resolverVftable;
    owner.m_pResolver = &resolver;
    storage.m_pOwner = &owner;
    g_pReceiver =
        reinterpret_cast<CGSICameraMove_Receiver*>(0x12345678);
    g_pExpectedPosition = &position;
    g_pExpectedLookAt = &lookAt;
    g_pInterpolatorStorage =
        reinterpret_cast<CGSICameraMove_Interpolator*>(
            interpolatorBytes);

    ResetCallState();
    g_pInterpolatorStorage = 0;
    g_pCallbackStorage = &callbackState;
    SetFloatBits(duration, 0x7FC12345);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtPos(position, lookAt, duration);
    if (g_AllocateCalls != 1 || g_FirstAllocationSize != 0x6C ||
        g_ConstructCalls != 0 || g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != 0 || g_pReceivedCallback != 0)
        return Fail("interpolator allocation failure");

    ResetCallState();
    g_pInterpolatorStorage =
        reinterpret_cast<CGSICameraMove_Interpolator*>(
            interpolatorBytes);
    g_pCallbackStorage = 0;
    SetFloatBits(duration, 0x80000000);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtPos(position, lookAt, duration);
    if (g_AllocateCalls != 2 || g_FirstAllocationSize != 0x6C ||
        g_SecondAllocationSize != 0x0C ||
        g_ConstructCalls != 1 || g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != g_pInterpolatorStorage ||
        g_pReceivedCallback != 0)
        return Fail("callback allocation failure");

    ResetCallState();
    callbackState.m_Kind = 0;
    callbackState.m_pCallback = 0;
    callbackState.m_pInterpolator = 0;
    g_pCallbackStorage = &callbackState;
    SetFloatBits(duration, 0x7F800000);
    interfaceObject.CGameScriptInterface::
        CameraMoveToPosAndLookAtPos(position, lookAt, duration);
    if (g_AllocateCalls != 2 || g_ConstructCalls != 1 ||
        g_SetModeCalls != 1 ||
        g_pReceivedInterpolator != g_pInterpolatorStorage ||
        g_pReceivedCallback != &callbackState)
        return Fail("successful setup");
    if (callbackState.m_Kind != 1 ||
        callbackState.m_pCallback != &CGSICameraMove_Callback ||
        callbackState.m_pInterpolator != g_pInterpolatorStorage)
        return Fail("callback state");
    if (g_ResolverCalls != 3)
        return Fail("resolver calls");

    std::printf("FSE2_00892410_TEST PASS\n");
    return 0;
}
