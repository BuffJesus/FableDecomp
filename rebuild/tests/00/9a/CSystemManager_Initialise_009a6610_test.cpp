#include <new>
#include <stdio.h>
#include <string.h>

#include "fable_system_manager.h"
#include "test_support/bank_manager_member_stubs.h"
#include "test_support/profile_manager_member_stubs.h"
#include "test_support/base_class_stubs.h"

// CSystemManager::Initialise (0x9A6610) against recorders for its real
// callees. Every callee appends a token to one trace, so each scenario checks
// both what ran and the order it ran in.

extern "C" __declspec(dllimport) void* FABLE_STDCALL ConvertThreadToFiber(
    void* parameter);
extern "C" __declspec(dllimport) void FABLE_STDCALL InitializeCriticalSection(
    void* criticalSection);
extern "C" __declspec(dllimport) void* FABLE_STDCALL GetCurrentProcess();
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(
    void* address,
    unsigned long size,
    unsigned long protect,
    unsigned long* oldProtect);

extern "C" char __ImageBase;

namespace
{
    fable_u32 g_managerStorage[0x180 / 4];
    fable_u32 g_initStorage[0xC8 / 4];
    fable_u32 g_arena[0x1000];
    fable_u32 g_arenaUsed;

    char g_trace[1024];
    bool g_baseResult;
    bool g_windowResult;
    ESystemCode g_driveResult;
    bool g_coloursMarksActive;

    CCharStringData g_fontData;
    CCharStringData g_streamingFontData;
    unsigned long g_shaderPathDataRaw_[4];
    CWideStringData& g_shaderPathData = *reinterpret_cast<CWideStringData*>(g_shaderPathDataRaw_);

    const void* g_fiberParameter;
    const void* g_criticalSection;
    const char* g_guidName;
    long g_windowWidth;
    long g_windowHeight;
    long g_scratchSize;
    const CSystemManager* g_displayOwner;
    void* g_displayObject;
    const void* g_displayInitShaderPath;
    void* g_displayInitWindow;
    bool g_displayInitSkipConfig;
    long g_displayInitWidth;
    bool g_displayExclusive;
    const void* g_inputInit;
    bool g_inputExclusive;
    void* g_inputObject;
    const void* g_soundInit;
    bool g_soundExclusive;
    void* g_soundObject;
    const void* g_fontBank;
    const void* g_streamingFontBank;
    void* g_fontObject;
    void* g_graphicBankObject;
    void* g_meshBankObject;
    CDeviceResetCallback* g_resetCallback;
    const CDisplayManager* g_callbackDisplay;
    void* g_profileObject;
    const CDriveManager* g_driveObject;
    const CSystemManager* g_driveSystem;
    const void* g_wideAssignSource[2];
    fable_u32 g_wideAssigns;

    typedef void* (FABLE_STDCALL* FiberFunction)(void*);
    typedef void (FABLE_STDCALL* CriticalSectionFunction)(void*);
    typedef void* (FABLE_STDCALL* ProcessFunction)();

    FiberFunction g_realFiber;
    CriticalSectionFunction g_realCriticalSection;
    ProcessFunction g_realProcess;

    void Trace(const char* token)
    {
        if (g_trace[0] != 0)
            strcat(g_trace, " ");
        strcat(g_trace, token);
    }

    CSystemManager& Manager()
    {
        return *reinterpret_cast<CSystemManager*>(g_managerStorage);
    }

    fable_u8* ManagerBytes()
    {
        return reinterpret_cast<fable_u8*>(g_managerStorage);
    }

    CSystemManagerInit& Init()
    {
        return *reinterpret_cast<CSystemManagerInit*>(g_initStorage);
    }

    // Names a counted-pointer slot by its offset in the manager.
    void TraceSlot(const char* prefix, const void* slot)
    {
        char token[32];
        const fable_u8* at = static_cast<const fable_u8*>(slot);
        if (at >= ManagerBytes() && at < ManagerBytes() + 0x180)
            sprintf(token, "%s%X", prefix, static_cast<unsigned>(at - ManagerBytes()));
        else
            sprintf(token, "%sPClass", prefix);
        Trace(token);
    }

    void* FABLE_STDCALL TestConvertThreadToFiber(void* parameter)
    {
        Trace("fiber");
        g_fiberParameter = parameter;
        return reinterpret_cast<void*>(0xF1B0F1B0);
    }

    void FABLE_STDCALL TestInitializeCriticalSection(void* section)
    {
        Trace("critsec");
        g_criticalSection = section;
    }

    void* FABLE_STDCALL TestGetCurrentProcess()
    {
        Trace("process");
        return reinterpret_cast<void*>(-1);
    }

    long FABLE_STDCALL TestCoInitialize(void* reserved)
    {
        Trace(reserved == 0 ? "coinit" : "coinit-bad");
        return 0;
    }

    void** FindImport(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                    return &slots[i];
            }
        }
        return 0;
    }

    bool Patch(const char* name, void* replacement, void** original)
    {
        void** slot = FindImport(name);
        unsigned long oldProtect = 0;
        if (slot == 0 || !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
            return false;
        *original = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }

    bool InstallImports()
    {
        return
            Patch(
                "ConvertThreadToFiber",
                reinterpret_cast<void*>(&TestConvertThreadToFiber),
                reinterpret_cast<void**>(&g_realFiber)) &&
            Patch(
                "InitializeCriticalSection",
                reinterpret_cast<void*>(&TestInitializeCriticalSection),
                reinterpret_cast<void**>(&g_realCriticalSection)) &&
            Patch(
                "GetCurrentProcess",
                reinterpret_cast<void*>(&TestGetCurrentProcess),
                reinterpret_cast<void**>(&g_realProcess));
    }

    bool SameFunction(bool (FABLE_FASTCALL* value)(ESystemCode), bool expectDefault)
    {
        // DefaultErrorFunc is private; the default is the one pointer that
        // answers false for every code without being the custom one.
        return expectDefault ? value != 0 && !value(kSystemCodeSuccess) : true;
    }

    bool FABLE_FASTCALL CustomErrorFunc(ESystemCode)
    {
        return true;
    }

    void Reset()
    {
        memset(g_managerStorage, 0xCD, sizeof(g_managerStorage));
        memset(g_initStorage, 0, sizeof(g_initStorage));
        memset(g_arena, 0, sizeof(g_arena));
        g_arenaUsed = 0;
        g_trace[0] = 0;
        g_baseResult = true;
        g_windowResult = true;
        g_driveResult = kSystemCodeSuccess;
        g_coloursMarksActive = false;
        g_fiberParameter = 0;
        g_criticalSection = 0;
        g_guidName = 0;
        g_windowWidth = 0;
        g_windowHeight = 0;
        g_scratchSize = 0;
        g_displayOwner = 0;
        g_displayObject = 0;
        g_displayInitShaderPath = 0;
        g_displayInitWindow = 0;
        g_displayInitSkipConfig = false;
        g_displayInitWidth = 0;
        g_displayExclusive = false;
        g_inputInit = 0;
        g_inputExclusive = false;
        g_inputObject = 0;
        g_soundInit = 0;
        g_soundExclusive = false;
        g_soundObject = 0;
        g_fontBank = 0;
        g_streamingFontBank = 0;
        g_fontObject = 0;
        g_graphicBankObject = 0;
        g_meshBankObject = 0;
        g_resetCallback = 0;
        g_callbackDisplay = 0;
        g_profileObject = 0;
        g_driveObject = 0;
        g_driveSystem = 0;
        g_wideAssignSource[0] = 0;
        g_wideAssignSource[1] = 0;
        g_wideAssigns = 0;
        CSystemManager::NoInstances = 0;
        memset(&g_fontData, 0, sizeof(g_fontData));
        memset(&g_streamingFontData, 0, sizeof(g_streamingFontData));

        // The constructor leaves the counted pointers empty; the rest of the
        // object stays poisoned so unwritten fields show up.
        CSystemManager& manager = Manager();
        manager.ApplicationActive = false;
        manager.Restore = false;
        manager.PInputManager.Data = 0;
        manager.PDisplayManager.Data = 0;
        manager.PGraphicBankManager.Data = 0;
        manager.PMeshBankManager.Data = 0;
        manager.PSoundManager.Data = 0;
        manager.PFontManager.Data = 0;
        manager.ProfileManager.Data = 0;

        CSystemManagerInit& init = Init();
        init.InstanceHandle = reinterpret_cast<void*>(0x11110000);
        init.CommandLine = reinterpret_cast<char*>(0x22220000);
        init.WindowsShow = 5;
        init.ExclusiveMode = true;
        init.PreferPrimaryDisplayDevice = true;
        init.Flags = 0;
        init.UseAccurateTimer = true;
        init.AttachToExternalWindow = false;
        init.ApplicationWindowHandle = reinterpret_cast<void*>(0x33330000);
        init.PreventExclusiveModeChanges = true;
        init.AppReinitFunc = reinterpret_cast<void (*)(void*)>(0x44440000);
        init.AppReinitContext = reinterpret_cast<void*>(0x55550000);
        init.WaitWhileInactive = true;
        init.SkipConfigDetection = true;
        init.DisplayManagerInit.Width = 640;
        init.DisplayManagerInit.Height = 480;
        init.DisplayManagerInit.WindowHandle = reinterpret_cast<void*>(0x66660000);
        init.DisplayManagerInit.SkipConfigDetection = false;
        *reinterpret_cast<CWideStringData**>(&init.DisplayManagerInit.ShaderResourcePath) =
            &g_shaderPathData;
        strcpy(init.Identifier, "FableIdentifier");
        init.ScratchBufferSize = 0;
        init.IMEMessageFunc = reinterpret_cast<FableIMEMessageFunction>(0x77770000);
        init.MessageFilter = reinterpret_cast<FableWindowMessageFilter>(0x88880000);
    }

    void SetFonts(long fontLength, long streamingLength)
    {
        g_fontData.unknown04 = fontLength;
        g_streamingFontData.unknown04 = streamingLength;
        *reinterpret_cast<CCharStringData**>(&Init().FontBankHandle) = &g_fontData;
        *reinterpret_cast<CCharStringData**>(&Init().StreamingFontBankHandle) =
            &g_streamingFontData;
    }

    // Fields the prologue writes whatever the base-class result is.
    bool CheckPrologue()
    {
        const CSystemManager& manager = Manager();
        return
            !manager.DDrawInitialised &&
            !manager.WindowValid &&
            !manager.RestoreExclusive &&
            manager.WaitWhileInactive &&
            manager.MouseZMove == 0 &&
            !manager.ForceMusicPlay &&
            manager.ForceMusicPlayOffset == 0.0f &&
            manager.PreventExclusiveModeChanges &&
            manager.ExclusiveMode == Init().ExclusiveMode &&
            g_wideAssignSource[0] == &Init().TempPath &&
            manager.IMEMessageFunc == Init().IMEMessageFunc &&
            manager.MessageFilter == Init().MessageFilter &&
            manager.EverBeenActive &&
            manager.ApplicationActive &&
            CSystemManager::NoInstances == 1;
    }

    // The base class refuses: only the always-built tail runs.
    bool CheckBaseRefused()
    {
        Reset();
        g_baseResult = false;
        Init().Flags = 0x15;
        const ESystemCode result = Manager().Initialise(&Init());
        return
            result == kSystemCodeSuccess &&
            strcmp(g_trace,
                "wassign164 base new78 profile resetF8 maths vectormaths "
                "colours appname") == 0 &&
            CheckPrologue() &&
            !Manager().WindowInitialised &&
            !Manager().Restore &&
            Manager().ProfileManager.Data == g_profileObject;
    }

    bool CheckWindowFailure()
    {
        Reset();
        Init().Flags = 4;
        g_windowResult = false;
        const ESystemCode result = Manager().Initialise(&Init());
        return
            result == kSystemCodeWindowInitialisationFailed &&
            strcmp(g_trace,
                "wassign164 base fiber coinit critsec wassignA0 guid window "
                "uninitialise") == 0 &&
            g_windowWidth == 640 &&
            g_windowHeight == 480 &&
            !Manager().WindowInitialised &&
            CSystemManager::NoInstances == 0;
    }

    bool CheckDriveFailure()
    {
        Reset();
        g_driveResult = static_cast<ESystemCode>(7);
        const ESystemCode result = Manager().Initialise(&Init());
        return
            result == static_cast<ESystemCode>(7) &&
            strcmp(g_trace,
                "wassign164 base fiber coinit critsec wassignA0 guid process "
                "new8 resetPClass drive uninitialise") == 0 &&
            g_driveObject == &CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get() &&
            g_driveSystem == &Manager() &&
            CSystemManager::NoInstances == 0;
    }

    // Everything, with the manager's own window and no font banks.
    bool CheckFullOwnWindow()
    {
        Reset();
        Init().Flags = 0x15;
        Init().ScratchBufferSize = 0x4000;
        Init().ExclusiveMode = false;
        SetFonts(0, 0);
        const ESystemCode result = Manager().Initialise(&Init());
        const CSystemManager& manager = Manager();
        return
            result == kSystemCodeSuccess &&
            strcmp(g_trace,
                "wassign164 base fiber coinit critsec wassignA0 guid window "
                "process scratch wcopy new2C8 display reset60 displayinit "
                "wdestroy new84 input reset58 new8 resetPClass drive "
                "new80 sound reset7C new8 graphicbank reset6C newC meshbank "
                "reset74 callback new78 profile resetF8 maths vectormaths "
                "colours appname") == 0 &&
            CheckPrologue() &&
            g_fiberParameter == &manager &&
            g_criticalSection == &manager.CriticalSection &&
            manager.MainFibre == reinterpret_cast<void*>(0xF1B0F1B0) &&
            manager.PreferPrimaryDevice &&
            manager.WinInstanceHandle == Init().InstanceHandle &&
            manager.WinCommandLine == Init().CommandLine &&
            manager.WinShow == 5 &&
            g_wideAssignSource[1] == &Init().AppName &&
            g_guidName == Init().Identifier &&
            SameFunction(manager.ErrorFunc, true) &&
            manager.InitFlags == 0x15 &&
            manager.AppReinitFunc == Init().AppReinitFunc &&
            manager.AppReinitContext == Init().AppReinitContext &&
            manager.UsingAccurateTimer &&
            manager.WindowInitialised &&
            g_scratchSize == 0x4000 &&
            g_displayOwner == &manager &&
            manager.PDisplayManager.Data == g_displayObject &&
            g_displayInitWidth == 640 &&
            g_displayInitShaderPath == &g_shaderPathData &&
            g_displayInitWindow == manager.ApplicationWinHandle &&
            g_displayInitSkipConfig &&
            !g_displayExclusive &&
            g_inputInit == &Init().InputManagerInit &&
            !g_inputExclusive &&
            manager.PInputManager.Data == g_inputObject &&
            g_soundInit == &Init().SoundManagerInit &&
            !g_soundExclusive &&
            manager.PSoundManager.Data == g_soundObject &&
            manager.PGraphicBankManager.Data == g_graphicBankObject &&
            manager.PMeshBankManager.Data == g_meshBankObject &&
            g_callbackDisplay == manager.PDisplayManager.Data &&
            reinterpret_cast<fable_u8*>(g_resetCallback) ==
                static_cast<fable_u8*>(g_meshBankObject) + 4 &&
            manager.ProfileManager.Data == g_profileObject &&
            *reinterpret_cast<void* const*>(g_driveObject) != 0;
    }

    // An external window, a custom error function, the streaming font bank
    // only, and no display, input or sound flags.
    bool CheckExternalWindowNoDisplay()
    {
        Reset();
        Init().AttachToExternalWindow = true;
        Init().IsErrorFailureFunc = &CustomErrorFunc;
        Init().Flags = 0;
        SetFonts(0, 3);
        g_coloursMarksActive = true;
        const ESystemCode result = Manager().Initialise(&Init());
        const CSystemManager& manager = Manager();
        return
            result == kSystemCodeSuccess &&
            strcmp(g_trace,
                "wassign164 base fiber coinit critsec wassignA0 guid process "
                "new8 resetPClass drive new3C font reset84 new8 graphicbank "
                "reset6C newC meshbank reset74 new78 profile resetF8 maths "
                "vectormaths colours appname") == 0 &&
            manager.ErrorFunc == &CustomErrorFunc &&
            manager.AttachedToExternalWindow &&
            !manager.WindowInitialised &&
            g_fontBank == &Init().FontBankHandle &&
            g_streamingFontBank == &Init().StreamingFontBankHandle &&
            manager.PFontManager.Data == g_fontObject &&
            manager.Restore;
    }

    // With the display flag, an external window is taken from the init;
    // exclusive mode reaches the display and sound managers.
    bool CheckExternalWindowDisplay()
    {
        Reset();
        Init().AttachToExternalWindow = true;
        Init().Flags = 0x14;
        SetFonts(2, 0);
        const ESystemCode result = Manager().Initialise(&Init());
        const CSystemManager& manager = Manager();
        return
            result == kSystemCodeSuccess &&
            strcmp(g_trace,
                "wassign164 base fiber coinit critsec wassignA0 guid process "
                "wcopy new2C8 display reset60 displayinit wdestroy new8 "
                "resetPClass drive new80 sound reset7C new3C font reset84 "
                "new8 graphicbank "
                "reset6C newC meshbank reset74 callback new78 profile resetF8 "
                "maths vectormaths colours appname") == 0 &&
            manager.WindowInitialised &&
            manager.ApplicationWinHandle == Init().ApplicationWindowHandle &&
            g_displayInitWindow == Init().ApplicationWindowHandle &&
            g_displayExclusive &&
            g_soundExclusive &&
            !manager.Restore;
    }
}

// ---- Real callees, as recorders ----------------------------------------

fable_u8 CSystemManager::NoInstances = 0;
#include "test_support/scratch_buffer_storage.h"

extern "C" long (FABLE_STDCALL* FixtureCoInitializeImport)(void*) =
    &TestCoInitialize;
// ole32 is not linked here; the reconstruction's CoInitialize import slot
// resolves to the recorder instead.
#pragma comment(linker, "/alternatename:__imp__CoInitialize@4=_FixtureCoInitializeImport")

void* operator new(size_t size)
{
    char token[16];
    sprintf(token, "new%X", static_cast<unsigned>(size));
    Trace(token);
    void* result = &g_arena[g_arenaUsed];
    g_arenaUsed += static_cast<fable_u32>((size + 3) / 4);
    return result;
}

void operator delete(void*)
{
}

bool CInitBaseClass::Initialise()
{
    Trace("base");
    return g_baseResult;
}

CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_)
{
    Trace("wcopy");
}

CWideString::~CWideString()
{
    Trace("wdestroy");
}

CWideString& CWideString::operator=(const CWideString& other)
{
    TraceSlot("wassign", this);
    if (g_wideAssigns < 2)
        g_wideAssignSource[g_wideAssigns] = &other;
    ++g_wideAssigns;
    return *this;
}

void CSystemManager::InitGUIDFromName(const char* name)
{
    Trace("guid");
    g_guidName = name;
}

bool CSystemManager::InitWindow(long width, long height)
{
    Trace("window");
    g_windowWidth = width;
    g_windowHeight = height;
    return g_windowResult;
}

void CSystemManager::Uninitialise()
{
    Trace("uninitialise");
}

bool FABLE_FASTCALL CSystemManager::DefaultErrorFunc(ESystemCode)
{
    return false;
}

void CSystemManager::InitSystemColours()
{
    Trace(NoInstances == 0 ? "colours" : "colours-late");
    if (g_coloursMarksActive)
        EverBeenActive = true;
}

void CSystemManager::InitApplicationName()
{
    // The instance count is bumped between the colours and the name.
    Trace(NoInstances == 1 ? "appname" : "appname-early");
}

bool CScratchBuffer::Initialise(long size)
{
    Trace(this == &GScratchBuffer ? "scratch" : "scratch-bad");
    g_scratchSize = size;
    return true;
}

void FABLE_FASTCALL GFInitMaths()
{
    Trace("maths");
}

void NVectorMath::GFInitVectorMath()
{
    Trace("vectormaths");
}

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    TraceSlot("reset", this);
    Data = data;
}

template void CCountedPointer<CDisplayManager>::Reset(CDisplayManager*);
template void CCountedPointer<CInputManager>::Reset(CInputManager*);
template void CCountedPointer<CDriveManager>::Reset(CDriveManager*);
template void CCountedPointer<CASoundManager>::Reset(CASoundManager*);
template void CCountedPointer<CFontManager>::Reset(CFontManager*);
template void CCountedPointer<CGraphicBankManager>::Reset(CGraphicBankManager*);
template void CCountedPointer<CMeshBankManager>::Reset(CMeshBankManager*);
template void CCountedPointer<CProfileManager>::Reset(CProfileManager*);

template<typename T>
CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;

CDriveManager::~CDriveManager()
{
}

ESystemCode CDriveManager::Initialise(CSystemManager* systemManager)
{
    Trace("drive");
    g_driveObject = this;
    g_driveSystem = systemManager;
    return g_driveResult;
}

// Fixture boundary: device-creation defaults are outside this coordinator test.


CDisplayManager::CDisplayManager(CSystemManager& systemManager)
{
    Trace("display");
    g_displayOwner = &systemManager;
    g_displayObject = this;
}

void CDisplayManager::Init(const CDisplayManagerInit& init, bool exclusiveMode)
{
    Trace("displayinit");
    g_displayInitWidth = init.Width;
    g_displayInitShaderPath =
        *reinterpret_cast<CWideStringData* const*>(&init.ShaderResourcePath);
    g_displayInitWindow = init.WindowHandle;
    g_displayInitSkipConfig = init.SkipConfigDetection;
    g_displayExclusive = exclusiveMode;
}

void CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback* callback)
{
    Trace("callback");
    g_callbackDisplay = this;
    g_resetCallback = callback;
}

// The base's constructor and destructor are out of line in retail; the
// fixture only needs them to exist (no member is touched here).
CInputManager::CInputManager(const CInputManagerInit&, bool) : Checksum(0, 0, 0) {}
CInputManager::~CInputManager() {}
// CInputManager's members (the package file and event vectors) bring these
// in through its constructor/destructor; none is reached.
class CFileInformation { public: long Words[4]; };
CAFile::CAFile() {}
CInputEvent::~CInputEvent() {}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() {}
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation f = { 0 }; return f; }

CInputManagerDX::CInputManagerDX(const CInputManagerInit& init, bool exclusiveMode)
    : CInputManager(init, exclusiveMode)
{
    Trace("input");
    g_inputInit = &init;
    g_inputExclusive = exclusiveMode;
    g_inputObject = static_cast<CInputManager*>(this);
}

// CInputManager's own virtual slots (retail vtable 0x0129D620); never reached here.
bool CInputManager::SetExclusiveMode(bool) { return false; }
void CInputManager::Update() {}

CSoundManagerLH::CSoundManagerLH(const CSoundManagerInit& init, bool exclusiveMode)
{
    Trace("sound");
    g_soundInit = &init;
    g_soundExclusive = exclusiveMode;
    g_soundObject = static_cast<CASoundManager*>(this);
}

CFontManager::CFontManager(
    const CCharString& fontBankHandle,
    const CCharString& streamingFontBankHandle)
{
    Trace("font");
    g_fontBank = &fontBankHandle;
    g_streamingFontBank = &streamingFontBankHandle;
    g_fontObject = this;
}
CFontManager::~CFontManager() {}

CGraphicBankManager::CGraphicBankManager()
{
    Trace("graphicbank");
    g_graphicBankObject = this;
}

CMeshBankManager::CMeshBankManager()
{
    Trace("meshbank");
    g_meshBankObject = this;
}

CProfileManager::CProfileManager()
{
    Trace("profile");
    g_profileObject = this;
}

// Bases the recorders' constructors and the drive manager's vtable need.
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}

void CDeviceResetCallback::OnPreDeviceReset()
{
}

bool CDeviceResetCallback::OnPostDeviceReset()
{
    return true;
}

void CMeshBankManager::OnPreDeviceReset() {}
bool CMeshBankManager::OnPostDeviceReset() { return true; }

void CASoundManager::GenerateLipSyncData(
    const CWideString&,
    LHA::CBinaryDataWriter&)
{
}

#define FABLE_UNKNOWN_SLOT(n) void CASoundManager::UnknownSlot##n() {}
FABLE_UNKNOWN_SLOT(01) FABLE_UNKNOWN_SLOT(02) FABLE_UNKNOWN_SLOT(03)
FABLE_UNKNOWN_SLOT(04) FABLE_UNKNOWN_SLOT(05) FABLE_UNKNOWN_SLOT(06)
FABLE_UNKNOWN_SLOT(07) FABLE_UNKNOWN_SLOT(08) FABLE_UNKNOWN_SLOT(09)
FABLE_UNKNOWN_SLOT(10) FABLE_UNKNOWN_SLOT(11) FABLE_UNKNOWN_SLOT(12)
FABLE_UNKNOWN_SLOT(13) FABLE_UNKNOWN_SLOT(14) FABLE_UNKNOWN_SLOT(15)
FABLE_UNKNOWN_SLOT(16) FABLE_UNKNOWN_SLOT(17) void CASoundManager::UnknownSlot18(long) {}
FABLE_UNKNOWN_SLOT(19) FABLE_UNKNOWN_SLOT(20) FABLE_UNKNOWN_SLOT(21)
FABLE_UNKNOWN_SLOT(22) FABLE_UNKNOWN_SLOT(23) FABLE_UNKNOWN_SLOT(24)
FABLE_UNKNOWN_SLOT(25) FABLE_UNKNOWN_SLOT(26) FABLE_UNKNOWN_SLOT(27)
FABLE_UNKNOWN_SLOT(28) FABLE_UNKNOWN_SLOT(29) FABLE_UNKNOWN_SLOT(30)
FABLE_UNKNOWN_SLOT(31) FABLE_UNKNOWN_SLOT(32) FABLE_UNKNOWN_SLOT(33)
FABLE_UNKNOWN_SLOT(34) FABLE_UNKNOWN_SLOT(35) FABLE_UNKNOWN_SLOT(36)
FABLE_UNKNOWN_SLOT(37) FABLE_UNKNOWN_SLOT(38) FABLE_UNKNOWN_SLOT(39)
FABLE_UNKNOWN_SLOT(40) FABLE_UNKNOWN_SLOT(41) FABLE_UNKNOWN_SLOT(42)
FABLE_UNKNOWN_SLOT(43) FABLE_UNKNOWN_SLOT(44) FABLE_UNKNOWN_SLOT(45)
FABLE_UNKNOWN_SLOT(48) FABLE_UNKNOWN_SLOT(50)
void CASoundManager::UnknownSlot46(unsigned long, float, float) {}
float CASoundManager::UnknownSlot47(unsigned long) { return 0.0f; }
bool CASoundManager::UnknownSlot49(float&) { return false; }

int main()
{
    if (!InstallImports())
    {
        printf("FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR FAIL imports\n");
        return 1;
    }

    const bool baseRefused = CheckBaseRefused();
    const bool windowFailure = CheckWindowFailure();
    const bool driveFailure = CheckDriveFailure();
    const bool fullOwnWindow = CheckFullOwnWindow();
    const bool externalNoDisplay = CheckExternalWindowNoDisplay();
    const bool externalDisplay = CheckExternalWindowDisplay();
    const bool passed =
        baseRefused &&
        windowFailure &&
        driveFailure &&
        fullOwnWindow &&
        externalNoDisplay &&
        externalDisplay;
    if (!passed)
    {
        printf(
            "FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR FAIL "
            "base=%u window=%u drive=%u full=%u external=%u externalDisplay=%u\n"
            "last trace: %s\n",
            baseRefused,
            windowFailure,
            driveFailure,
            fullOwnWindow,
            externalNoDisplay,
            externalDisplay,
            g_trace);
        return 1;
    }

    printf("FABLETLC_SYSTEM_MANAGER_INITIALISE_BEHAVIOR PASS\n");
    return 0;
}

#include "test_support/display_ownership_boundary.h"
CWideString::CWideString() : storage_(0) {}
