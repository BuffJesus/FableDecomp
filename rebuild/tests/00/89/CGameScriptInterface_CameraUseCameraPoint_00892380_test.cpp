#include <cstdio>
#include <cstring>

class CCharString
{
public:
    unsigned long m_Marker;
};

class C3DVector
{
public:
    unsigned long m_Marker;
};

class CRightHandedSet
{
public:
    unsigned long m_Marker;
};

struct CGSICameraUseCameraPointPose_Temporary
{
    void* m_pVftable;
    unsigned long m_Field04;
    void* m_pReference;
};

struct CGSICameraUseCameraPointPose_Reference
{
    long m_RefCount;
    void* m_Release;
    void* m_pContext;
};

void* CGSICameraUseCameraPointPose_ScriptThingVftable[1];

class CGSICameraUseCameraPointPose_Base
{
public:
    void Destroy();
};

class CGameScriptInterface
{
public:
    virtual void CameraUseCameraPoint(
        const CCharString& cameraPointName,
        const C3DVector* pPosition,
        const CRightHandedSet* pOrientation,
        float transitionTime,
        long firstMode,
        long secondMode) const;
};

struct CGSICameraUseCameraPointPose_Vftable
{
    unsigned char m_Pad00[0x120];
    void* m_ResolveCameraPoint;
    unsigned char m_Pad124[0x66C - 0x124];
    void* m_UseCameraPoint;
};

struct CGSICameraUseCameraPointPose_InterfaceStorage
{
    CGSICameraUseCameraPointPose_Vftable* m_pVftable;
};

static const CGameScriptInterface* g_pExpectedInterface;
static const CCharString* g_pExpectedName;
static const C3DVector* g_pExpectedPosition;
static const CRightHandedSet* g_pExpectedOrientation;
static unsigned long g_ExpectedFloatBits;
static long g_ExpectedFirstMode;
static long g_ExpectedSecondMode;
static CGSICameraUseCameraPointPose_Reference* g_pResolveReference;
static CGSICameraUseCameraPointPose_Temporary* g_pResolvedTemporary;
static unsigned long g_ResolveCalls;
static unsigned long g_UseCalls;
static unsigned long g_ReleaseCalls;
static unsigned long g_FreeCalls;
static unsigned long g_BaseDestroyCalls;
static void* g_pExpectedReleaseContext;

static void __cdecl ResolveCameraPointHelper(
    const CGameScriptInterface* pInterface,
    CGSICameraUseCameraPointPose_Temporary* pResult,
    const CCharString* pName)
{
    ++g_ResolveCalls;
    if (pInterface != g_pExpectedInterface || pName != g_pExpectedName)
    {
        std::printf("resolve arguments mismatch\n");
        return;
    }
    pResult->m_pVftable = reinterpret_cast<void*>(0x11111111);
    pResult->m_Field04 = 0x22222222;
    pResult->m_pReference = g_pResolveReference;
}

static void __declspec(naked) ResolveCameraPointThunk()
{
    __asm
    {
        push dword ptr [esp + 8]
        push dword ptr [esp + 8]
        push ecx
        call ResolveCameraPointHelper
        add esp, 0Ch
        ret 8
    }
}

static void __cdecl UseCameraPointHelper(
    const CGameScriptInterface* pInterface,
    CGSICameraUseCameraPointPose_Temporary* pResolved,
    const C3DVector* pPosition,
    const CRightHandedSet* pOrientation,
    unsigned long transitionBits,
    long firstMode,
    long secondMode)
{
    ++g_UseCalls;
    if (pInterface != g_pExpectedInterface ||
        pResolved->m_pReference != g_pResolveReference ||
        pPosition != g_pExpectedPosition ||
        pOrientation != g_pExpectedOrientation ||
        transitionBits != g_ExpectedFloatBits ||
        firstMode != g_ExpectedFirstMode ||
        secondMode != g_ExpectedSecondMode)
    {
        std::printf("use arguments mismatch\n");
    }
    g_pResolvedTemporary = pResolved;
}

static void __declspec(naked) UseCameraPointThunk()
{
    __asm
    {
        push dword ptr [esp + 18h]
        push dword ptr [esp + 18h]
        push dword ptr [esp + 18h]
        push dword ptr [esp + 18h]
        push dword ptr [esp + 18h]
        push dword ptr [esp + 18h]
        push ecx
        call UseCameraPointHelper
        add esp, 1Ch
        ret 18h
    }
}

static void __fastcall ReleaseReference(void* pContext)
{
    if (pContext != g_pExpectedReleaseContext)
        std::printf("release context mismatch\n");
    ++g_ReleaseCalls;
}

void __cdecl CGSICameraUseCameraPointPose_Free(void* pMemory)
{
    if (pMemory != g_pResolveReference)
        std::printf("free pointer mismatch\n");
    ++g_FreeCalls;
}

void CGSICameraUseCameraPointPose_Base::Destroy()
{
    CGSICameraUseCameraPointPose_Temporary* const pThis =
        reinterpret_cast<CGSICameraUseCameraPointPose_Temporary*>(this);
    if (pThis != g_pResolvedTemporary ||
        pThis->m_Field04 != 0 || pThis->m_pReference != 0 ||
        pThis->m_pVftable !=
            CGSICameraUseCameraPointPose_ScriptThingVftable)
    {
        std::printf("base destruction state mismatch\n");
    }
    ++g_BaseDestroyCalls;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00892380_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void SetFloatBits(float& value, unsigned long bits)
{
    std::memcpy(&value, &bits, sizeof(bits));
}

int main()
{
    CGSICameraUseCameraPointPose_Vftable vftable = {};
    CGSICameraUseCameraPointPose_InterfaceStorage storage;
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    CCharString name;
    C3DVector position;
    CRightHandedSet orientation;
    CGSICameraUseCameraPointPose_Reference reference;
    float transition;

    vftable.m_ResolveCameraPoint =
        reinterpret_cast<void*>(ResolveCameraPointThunk);
    vftable.m_UseCameraPoint =
        reinterpret_cast<void*>(UseCameraPointThunk);
    storage.m_pVftable = &vftable;
    g_pExpectedInterface = &interfaceObject;
    g_pExpectedName = &name;
    g_pExpectedPosition = &position;
    g_pExpectedOrientation = &orientation;
    name.m_Marker = 0xA1A2A3A4;
    position.m_Marker = 0xB1B2B3B4;
    orientation.m_Marker = 0xC1C2C3C4;

    reference.m_RefCount = 4;
    reference.m_Release = reinterpret_cast<void*>(ReleaseReference);
    reference.m_pContext = reinterpret_cast<void*>(0x24681357);
    g_pExpectedReleaseContext = reference.m_pContext;
    g_pResolveReference = &reference;
    g_ExpectedFloatBits = 0x7FC12345;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = static_cast<long>(0x87654321);
    g_ExpectedSecondMode = 0x12345678;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, &position, &orientation, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 1 || g_UseCalls != 1 ||
        g_ReleaseCalls != 0 || g_FreeCalls != 0 ||
        g_BaseDestroyCalls != 1 || reference.m_RefCount != 3)
        return Fail("retained reference path");

    reference.m_RefCount = 1;
    g_pExpectedPosition = 0;
    g_pExpectedOrientation = 0;
    g_ExpectedFloatBits = 0x80000000;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = -1;
    g_ExpectedSecondMode = 0;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, 0, 0, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 2 || g_UseCalls != 2 ||
        g_ReleaseCalls != 1 || g_FreeCalls != 1 ||
        g_BaseDestroyCalls != 2 || reference.m_RefCount != 0)
        return Fail("released reference path");

    g_pResolveReference = 0;
    g_pExpectedPosition = &position;
    g_pExpectedOrientation = &orientation;
    g_ExpectedFloatBits = 0x7F800000;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = 0x10203040;
    g_ExpectedSecondMode = -2;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, &position, &orientation, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 3 || g_UseCalls != 3 ||
        g_ReleaseCalls != 1 || g_FreeCalls != 1 ||
        g_BaseDestroyCalls != 3)
        return Fail("null reference path");

    std::printf("FSE2_00892380_TEST PASS\n");
    return 0;
}
