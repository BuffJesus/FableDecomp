#include <cstdio>
#include <cstring>

class CCharString
{
public:
    unsigned long m_Marker;
};

class CScriptThing
{
public:
    unsigned long m_Marker;
};

struct CGSICameraUseCameraPoint_Temporary
{
    void* m_pVftable;
    unsigned long m_Field04;
    void* m_pReference;
};

struct CGSICameraUseCameraPoint_Reference
{
    long m_RefCount;
    void* m_Release;
    void* m_pContext;
};

void* CGSICameraUseCameraPoint_ScriptThingVftable[1];

class CGSICameraUseCameraPoint_Base
{
public:
    void Destroy();
};

class CGameScriptInterface
{
public:
    virtual void CameraUseCameraPoint(
        const CCharString& cameraPointName,
        const CScriptThing& target,
        float transitionTime,
        long firstMode,
        long secondMode) const;
};

struct CGSICameraUseCameraPoint_Vftable
{
    unsigned char m_Pad00[0x120];
    void* m_ResolveCameraPoint;
    unsigned char m_Pad124[0x674 - 0x124];
    void* m_UseCameraPoint;
};

struct CGSICameraUseCameraPoint_InterfaceStorage
{
    CGSICameraUseCameraPoint_Vftable* m_pVftable;
};

static const CGameScriptInterface* g_pExpectedInterface;
static const CCharString* g_pExpectedName;
static const CScriptThing* g_pExpectedTarget;
static unsigned long g_ExpectedFloatBits;
static long g_ExpectedFirstMode;
static long g_ExpectedSecondMode;
static CGSICameraUseCameraPoint_Reference* g_pResolveReference;
static CGSICameraUseCameraPoint_Temporary* g_pResolvedTemporary;
static unsigned long g_ResolveCalls;
static unsigned long g_UseCalls;
static unsigned long g_ReleaseCalls;
static unsigned long g_FreeCalls;
static unsigned long g_BaseDestroyCalls;
static void* g_pExpectedReleaseContext;

static void __cdecl ResolveCameraPointHelper(
    const CGameScriptInterface* pInterface,
    CGSICameraUseCameraPoint_Temporary* pResult,
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
    CGSICameraUseCameraPoint_Temporary* pResolved,
    const CScriptThing* pTarget,
    unsigned long transitionBits,
    long firstMode,
    long secondMode)
{
    ++g_UseCalls;
    if (pInterface != g_pExpectedInterface ||
        pResolved->m_pReference != g_pResolveReference ||
        pTarget != g_pExpectedTarget ||
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
        push dword ptr [esp + 14h]
        push dword ptr [esp + 14h]
        push dword ptr [esp + 14h]
        push dword ptr [esp + 14h]
        push dword ptr [esp + 14h]
        push ecx
        call UseCameraPointHelper
        add esp, 18h
        ret 14h
    }
}

static void __fastcall ReleaseReference(void* pContext)
{
    if (pContext != g_pExpectedReleaseContext)
        std::printf("release context mismatch\n");
    ++g_ReleaseCalls;
}

void __cdecl CGSICameraUseCameraPoint_Free(void* pMemory)
{
    if (pMemory != g_pResolveReference)
        std::printf("free pointer mismatch\n");
    ++g_FreeCalls;
}

void CGSICameraUseCameraPoint_Base::Destroy()
{
    CGSICameraUseCameraPoint_Temporary* const pThis =
        reinterpret_cast<CGSICameraUseCameraPoint_Temporary*>(this);
    if (pThis != g_pResolvedTemporary ||
        pThis->m_Field04 != 0 || pThis->m_pReference != 0 ||
        pThis->m_pVftable !=
            CGSICameraUseCameraPoint_ScriptThingVftable)
    {
        std::printf("base destruction state mismatch\n");
    }
    ++g_BaseDestroyCalls;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_008922f0_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void SetFloatBits(float& value, unsigned long bits)
{
    std::memcpy(&value, &bits, sizeof(bits));
}

int main()
{
    CGSICameraUseCameraPoint_Vftable vftable = {};
    CGSICameraUseCameraPoint_InterfaceStorage storage;
    CGameScriptInterface& interfaceObject =
        reinterpret_cast<CGameScriptInterface&>(storage);
    CCharString name;
    CScriptThing target;
    CGSICameraUseCameraPoint_Reference reference;
    float transition;

    vftable.m_ResolveCameraPoint =
        reinterpret_cast<void*>(ResolveCameraPointThunk);
    vftable.m_UseCameraPoint =
        reinterpret_cast<void*>(UseCameraPointThunk);
    storage.m_pVftable = &vftable;
    g_pExpectedInterface = &interfaceObject;
    g_pExpectedName = &name;
    g_pExpectedTarget = &target;
    name.m_Marker = 0xA1A2A3A4;
    target.m_Marker = 0xB1B2B3B4;

    reference.m_RefCount = 3;
    reference.m_Release = reinterpret_cast<void*>(ReleaseReference);
    reference.m_pContext = reinterpret_cast<void*>(0x13572468);
    g_pExpectedReleaseContext = reference.m_pContext;
    g_pResolveReference = &reference;
    g_ExpectedFloatBits = 0xFFC12345;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = static_cast<long>(0x81234567);
    g_ExpectedSecondMode = 0x76543210;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, target, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 1 || g_UseCalls != 1 ||
        g_ReleaseCalls != 0 || g_FreeCalls != 0 ||
        g_BaseDestroyCalls != 1 || reference.m_RefCount != 2)
        return Fail("retained reference path");

    reference.m_RefCount = 1;
    g_ExpectedFloatBits = 0x80000000;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = -1;
    g_ExpectedSecondMode = 0;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, target, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 2 || g_UseCalls != 2 ||
        g_ReleaseCalls != 1 || g_FreeCalls != 1 ||
        g_BaseDestroyCalls != 2 || reference.m_RefCount != 0)
        return Fail("released reference path");

    g_pResolveReference = 0;
    g_ExpectedFloatBits = 0x7F800000;
    SetFloatBits(transition, g_ExpectedFloatBits);
    g_ExpectedFirstMode = 0x10203040;
    g_ExpectedSecondMode = -2;
    interfaceObject.CGameScriptInterface::CameraUseCameraPoint(
        name, target, transition,
        g_ExpectedFirstMode, g_ExpectedSecondMode);

    if (g_ResolveCalls != 3 || g_UseCalls != 3 ||
        g_ReleaseCalls != 1 || g_FreeCalls != 1 ||
        g_BaseDestroyCalls != 3)
        return Fail("null reference path");

    std::printf("FSE2_008922f0_TEST PASS\n");
    return 0;
}
