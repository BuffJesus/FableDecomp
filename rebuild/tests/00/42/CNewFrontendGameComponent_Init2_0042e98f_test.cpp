#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fable_game.h"
#include "fable_nui_manager.h"
#include "fable_particle_engine.h"
#include "fable_game_text_bank.h"
#include "fable_definition_manager.h"
#include "fable_display_viewport.h"
#include "fable_surface.h"

// Behaviour fixture for CNewFrontendGameComponent::Init2 @ 0x0042E98F.
// Retail offsets: m_pFrontEndGraphicBank +0x40, m_pMeshBank +0x48,
// m_pTextBank +0x60, m_pInputManager +0x68, m_pFrontEndManager +0xB4,
// unknown144_ +0x144. CFrontEndManager: PGameComponent +0x1C,
// m_bQuitFromGame +0xC0. Every callee is a recording stand-in; the text bank
// is a fake object (its ctor is aliased below) whose vtable slot 1 is
// CBankFile::OpenReadOnly.

enum
{
    EV_GETINSTANCE = 1, EV_GRAPHICS, EV_SOUNDS, EV_INPUT, EV_SETBANK, EV_NEW,
    EV_BANKCTOR, EV_RESET, EV_BANKNAME, EV_OPEN, EV_NAMEDTOR, EV_CLEAR,
    EV_DEFGET, EV_MESHCOPY, EV_GRAPHICCOPY, EV_INIT, EV_BUILD, EV_DISPLAYMODE
};

static int g_events[64];
static int g_eventCount;
static void Record(int ev) { if (g_eventCount < 64) g_events[g_eventCount] = ev; ++g_eventCount; }

static unsigned char g_component[0x148];
static unsigned char g_frontEnd[0xE0];
static unsigned char g_defManager[0x40];
static unsigned char g_display[0x2C8];
static unsigned char g_graphicBank[0x10];
static unsigned char g_meshBank[0x10];
static unsigned char g_inputManager[0x10];
static CCPPointerInfo g_graphicInfo(0, g_graphicBank);
static CCPPointerInfo g_meshInfo(0, g_meshBank);

bool g_FableFrontendFirstRun_01375448;
CDisplayManager* g_FableDisplayManager_013B8390 = reinterpret_cast<CDisplayManager*>(g_display);

// --- the text bank ----------------------------------------------------
struct FakeBank { void** vtable; unsigned char rest[0x210]; };
static void* g_bankVtable[4];
static FakeBank g_fallbackBank;
static bool g_newFails;
static size_t g_newSize;
static void* g_newResult;
static void* g_bankCtorThis;
static void* g_resetThis;
static void* g_resetArg;
static void* g_openThis;
static const void* g_openName;
static unsigned long g_openFlags;
static const void* g_nameObject;
static const void* g_nameDtorThis;

void* operator new(size_t size)
{
    Record(EV_NEW);
    g_newSize = size;
    g_newResult = g_newFails ? 0 : malloc(size);
    return g_newResult;
}
void operator delete(void* p) { free(p); }

static bool __fastcall FakeOpenReadOnly(void* self, void*, const CCharString* name, unsigned long flags)
{
    Record(EV_OPEN);
    g_openThis = self; g_openName = name; g_openFlags = flags;
    return true;
}

extern "C" void* __fastcall FakeDataBankCtor(void* self)
{
    Record(EV_BANKCTOR);
    g_bankCtorThis = self;
    static_cast<FakeBank*>(self)->vtable = g_bankVtable;
    return self;
}
#pragma comment(linker, "/alternatename:??0CDataBank@NGameText@@QAE@XZ=@FakeDataBankCtor@4")

template<>
void CCountedPointer<NGameText::CDataBank>::Reset(NGameText::CDataBank* data)
{
    Record(EV_RESET);
    g_resetThis = this; g_resetArg = data;
    // A failed allocation still has to be openable for the fixture to go on.
    g_fallbackBank.vtable = g_bankVtable;
    Data = data ? data : reinterpret_cast<NGameText::CDataBank*>(&g_fallbackBank);
}

CCharString::CCharString() : storage_(0) { g_nameObject = this; }
CCharString::~CCharString()
{
    Record(EV_NAMEDTOR);
    g_nameDtorThis = this;
}

CCharString FABLE_FASTCALL NLocalisation::GetTextBankName()
{
    Record(EV_BANKNAME);
    return CCharString();
}

// --- managers -----------------------------------------------------------
NUISystem::CFrontEndManager* FABLE_FASTCALL NUISystem::CFrontEndManager::GetInstance()
{
    Record(EV_GETINSTANCE);
    return reinterpret_cast<NUISystem::CFrontEndManager*>(g_frontEnd);
}

static void* g_graphicsThis;
static void* g_soundsThis;
static void* g_inputThis;
bool CNewFrontendGameComponent::InitialiseGraphics() { Record(EV_GRAPHICS); g_graphicsThis = this; return false; }
void CNewFrontendGameComponent::InitialiseSounds() { Record(EV_SOUNDS); g_soundsThis = this; }
void CNewFrontendGameComponent::InitialiseInput() { Record(EV_INPUT); g_inputThis = this; }

static const void* g_setBankArg;
void FABLE_FASTCALL NParticleEngine::SetGraphicBank(const CGraphicDataBank& bank)
{
    Record(EV_SETBANK);
    g_setBankArg = &bank;
}

void FABLE_FASTCALL NUISystem::CManager::Clear() { Record(EV_CLEAR); }

CStartupDefinitionManager& FABLE_FASTCALL CStartupDefinitionManager::Get()
{
    Record(EV_DEFGET);
    return *reinterpret_cast<CStartupDefinitionManager*>(g_defManager);
}

static const void* g_meshCopySource;
static const void* g_graphicCopySource;
template<>
CCountedPointer<CMeshDataBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    Record(EV_MESHCOPY);
    g_meshCopySource = &other;
    if (Info) ++Info->RefCount;
}
template<>
CCountedPointer<CGraphicDataBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    Record(EV_GRAPHICCOPY);
    g_graphicCopySource = &other;
    if (Info) ++Info->RefCount;
}

static void* g_initThis;
static void* g_initGraphic;
static void* g_initMesh;
static unsigned long g_initGraphicRefs;
static unsigned long g_initMeshRefs;
static CInputManager* g_initInput;
static CDefinitionManager* g_initDefs;
void NUISystem::CFrontEndManager::Init(CCountedPointer<CGraphicDataBank> graphicBank,
                                       CCountedPointer<CMeshDataBank> meshBank,
                                       CInputManager* inputManager,
                                       CDefinitionManager* definitionManager)
{
    Record(EV_INIT);
    g_initThis = this;
    g_initGraphic = graphicBank.Data;
    g_initMesh = meshBank.Data;
    g_initGraphicRefs = graphicBank.Info ? graphicBank.Info->RefCount : 0;
    g_initMeshRefs = meshBank.Info ? meshBank.Info->RefCount : 0;
    g_initInput = inputManager;
    g_initDefs = definitionManager;
    // The by-value counted pointers are released here, as retail's callee does.
    graphicBank.Info = 0;
    meshBank.Info = 0;
    if (g_graphicInfo.RefCount) --g_graphicInfo.RefCount;
    if (g_meshInfo.RefCount) --g_meshInfo.RefCount;
}

static void* g_buildThis;
static int g_buildArg;
void NUISystem::CFrontEndManager::BuildComponents(bool flag)
{
    Record(EV_BUILD);
    g_buildThis = this; g_buildArg = flag ? 1 : 0;
}

static void* g_modeThis;
static long g_modeX, g_modeY;
static unsigned long g_modeDepth, g_modeRefresh;
static int g_modeMultisample;
CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetDisplayMode(
    const C2DExtentsI& dimensions, unsigned long bitDepth,
    ESurfaceMultisampleType multisample, unsigned long refreshRate)
{
    Record(EV_DISPLAYMODE);
    g_modeThis = this;
    g_modeX = dimensions.X; g_modeY = dimensions.Y;
    g_modeDepth = bitDepth; g_modeMultisample = (int)multisample; g_modeRefresh = refreshRate;
    return CREATE_DEVICE_SUCCESS;
}

// --------------------------------------------------------------------------
static void Put(unsigned char* base, int offset, const void* value)
{
    memcpy(base + offset, &value, 4);
}

static bool RunCase(const char* name, bool firstRun, unsigned char flag144, bool newFails)
{
    g_eventCount = 0;
    memset(g_events, 0, sizeof(g_events));
    g_newFails = newFails;
    g_graphicInfo.RefCount = 5;
    g_meshInfo.RefCount = 7;
    g_FableFrontendFirstRun_01375448 = firstRun;
    memset(g_bankVtable, 0, sizeof(g_bankVtable));
    g_bankVtable[1] = (void*)&FakeOpenReadOnly;

    memset(g_component, 0xA7, sizeof(g_component));
    memset(g_frontEnd, 0x3C, sizeof(g_frontEnd));
    Put(g_component, 0x40, g_graphicBank);
    Put(g_component, 0x44, &g_graphicInfo);
    Put(g_component, 0x48, g_meshBank);
    Put(g_component, 0x4C, &g_meshInfo);
    Put(g_component, 0x60, 0);
    Put(g_component, 0x64, 0);
    Put(g_component, 0x68, g_inputManager);
    g_component[0x144] = flag144;

    reinterpret_cast<CNewFrontendGameComponent*>(g_component)->Init2();

    static const int expect[] = {
        EV_GETINSTANCE, EV_GRAPHICS, EV_SOUNDS, EV_INPUT, EV_SETBANK, EV_NEW,
        EV_BANKCTOR, EV_RESET, EV_BANKNAME, EV_OPEN, EV_NAMEDTOR, EV_CLEAR,
        EV_DEFGET, EV_MESHCOPY, EV_GRAPHICCOPY, EV_INIT, EV_BUILD, EV_DISPLAYMODE };
    static const int expectNoAlloc[] = {
        EV_GETINSTANCE, EV_GRAPHICS, EV_SOUNDS, EV_INPUT, EV_SETBANK, EV_NEW,
        EV_RESET, EV_BANKNAME, EV_OPEN, EV_NAMEDTOR, EV_CLEAR,
        EV_DEFGET, EV_MESHCOPY, EV_GRAPHICCOPY, EV_INIT, EV_BUILD, EV_DISPLAYMODE };
    const int* want = newFails ? expectNoAlloc : expect;
    int wantCount = newFails ? (int)(sizeof(expectNoAlloc) / sizeof(int)) : (int)(sizeof(expect) / sizeof(int));

    bool ok = g_eventCount == wantCount;
    for (int i = 0; ok && i < wantCount; ++i)
        ok = g_events[i] == want[i];

    void* stored;
    memcpy(&stored, g_component + 0xB4, 4);
    ok = ok && stored == g_frontEnd;
    memcpy(&stored, g_frontEnd + 0x1C, 4);
    ok = ok && stored == g_component;
    ok = ok && g_frontEnd[0xC0] == (firstRun ? 0 : 1);
    ok = ok && g_graphicsThis == g_component && g_soundsThis == g_component && g_inputThis == g_component;
    ok = ok && g_setBankArg == g_graphicBank;
    ok = ok && g_newSize == 0x214;
    if (!newFails)
        ok = ok && g_bankCtorThis == g_newResult && g_resetArg == g_newResult && g_openThis == g_newResult;
    else
        ok = ok && g_resetArg == 0 && g_openThis == &g_fallbackBank;
    ok = ok && g_resetThis == g_component + 0x60;
    ok = ok && g_openName == g_nameObject && g_nameDtorThis == g_nameObject && g_openFlags == 0x100;
    ok = ok && g_meshCopySource == g_component + 0x48 && g_graphicCopySource == g_component + 0x40;
    ok = ok && g_initThis == g_frontEnd && g_initGraphic == g_graphicBank && g_initMesh == g_meshBank;
    ok = ok && g_initGraphicRefs == 6 && g_initMeshRefs == 8;
    ok = ok && g_graphicInfo.RefCount == 5 && g_meshInfo.RefCount == 7;
    ok = ok && g_initInput == reinterpret_cast<CInputManager*>(g_inputManager);
    ok = ok && g_initDefs == reinterpret_cast<CDefinitionManager*>(g_defManager);
    ok = ok && g_buildThis == g_frontEnd && g_buildArg == (flag144 ? 0 : 1);
    ok = ok && g_modeThis == g_display && g_modeX == 1024 && g_modeY == 768;
    ok = ok && g_modeDepth == 16 && g_modeMultisample == 0 && g_modeRefresh == 60;

    // Only the manager pointer (+0xB4) and the text bank (+0x60) change.
    for (int i = 0; i < 0x148; ++i)
    {
        if (i >= 0x40 && i < 0x50) continue;
        if (i >= 0x60 && i < 0x6C) continue;
        if (i >= 0xB4 && i < 0xB8) continue;
        if (i == 0x144) continue;
        if (g_component[i] != 0xA7) { ok = false; break; }
    }
    // The frontend manager: only +0x1C and +0xC0 written.
    for (int i = 0; i < 0xE0; ++i)
    {
        if ((i >= 0x1C && i < 0x20) || i == 0xC0) continue;
        if (g_frontEnd[i] != 0x3C) { ok = false; break; }
    }
    if (!ok)
    {
        printf("FAIL %s events:", name);
        for (int i = 0; i < g_eventCount && i < 64; ++i) printf(" %d", g_events[i]);
        printf("\n");
    }
    return ok;
}

int main()
{
    bool ok = true;
    ok &= RunCase("first-run", true, 1, false);
    ok &= RunCase("returning", false, 0, false);
    ok &= RunCase("alloc-fails", true, 0, true);
    if (ok) printf("INIT2_0042E98F_TEST PASS\n");
    return ok ? 0 : 1;
}
