// Behaviour fixture for CNewFrontendGameComponent::Run @ 0x0042EC7C.
// Every callee is a recording stand-in; each scenario compares the exact call
// log (receivers, arguments, order) and the field/global writes.
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <new>

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_rgb_colour.h"
#include "fable_profile.h"
#include "fable_nui_manager.h"
#include "fable_console.h"
#include "fable_file_installer.h"
#include "fable_resource_directories.h"
#include "engine/CDisplayEngine.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"

class CFileInformation
{
};

namespace NLocalisation
{
    unsigned long FABLE_FASTCALL GetLanguage();
}

// ---------------------------------------------------------------- log
static char g_log[16384];
static void Log(const char* format, ...)
{
    char line[512];
    va_list args;
    va_start(args, format);
    _vsnprintf(line, sizeof(line) - 1, format, args);
    va_end(args);
    line[sizeof(line) - 1] = 0;
    strcat(g_log, line);
    strcat(g_log, ";");
}

// ---------------------------------------------------------------- objects
static unsigned char g_componentStorage[0x148];
static CNewFrontendGameComponent* const g_component =
    reinterpret_cast<CNewFrontendGameComponent*>(g_componentStorage);
static unsigned char g_gameStorage[sizeof(CGame)];
static CGame* const g_game = reinterpret_cast<CGame*>(g_gameStorage);
static unsigned char g_systemStorage[sizeof(CSystemManager)];
static CSystemManager* const g_system = reinterpret_cast<CSystemManager*>(g_systemStorage);
static unsigned char g_displayStorage[16];
static unsigned char g_renderStorage[16];
static unsigned char g_fontManagerStorage[0x3C];
static unsigned char g_inputStorage[16];
static unsigned char g_frontEndManagerStorage[0xE0];
static NUISystem::CFrontEndManager* const g_frontEndManager =
    reinterpret_cast<NUISystem::CFrontEndManager*>(g_frontEndManagerStorage);
static unsigned char g_frontEndDefStorage[0x60];
static unsigned char g_textBankStorage[4];
static unsigned char g_engineStorage[4];

template <class T> static T& At(void* base, unsigned offset)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(base) + offset);
}

static const char* Name(const void* p)
{
    if (p == g_component) return "this";
    if (p == g_textBankStorage) return "textbank";
    if (p == g_engineStorage) return "engine";
    if (p == g_game) return "game";
    if (p == 0) return "null";
    return "?";
}

// ---------------------------------------------------------------- globals Run reads/writes
CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
bool g_FableFrontendFirstRun_01375448;
bool g_FablePlayIntroMovies_0137544A;
CRGBColour g_FableMovieBackgroundColour_013961E0;
bool g_FablePlayingFullScreenMovie_013CA7E9;
CASoundManager* g_FableSoundManager_013B8394;
CFontManager* g_FableFontManager_013B838C =
    reinterpret_cast<CFontManager*>(g_fontManagerStorage);
CInputManager* g_FableInputManager_013B8388 =
    reinterpret_cast<CInputManager*>(g_inputStorage);
CRenderManager2D* g_FableRenderManager2D_013B8384 =
    reinterpret_cast<CRenderManager2D*>(g_renderStorage);
bool GUseRetailBanks;
CGameComponent* g_FableRetiredGameComponent_013B7D58;
CBankFileManager g_FableBankFileManager_013CA79C;

// ---------------------------------------------------------------- strings
static wchar_t* WideDup(const wchar_t* text)
{
    size_t n = wcslen(text);
    wchar_t* copy = static_cast<wchar_t*>(malloc((n + 1) * sizeof(wchar_t)));
    memcpy(copy, text, (n + 1) * sizeof(wchar_t));
    return copy;
}

static CWideStringData* WideData(const wchar_t* text)
{
    CWideStringData* data = static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    memset(data, 0, sizeof(*data));
    data->text = WideDup(text);
    return data;
}

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t* text) : storage_(WideData(text)) {}
CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_ ? WideData(other.storage_->text) : 0) {}
CWideString::~CWideString()
{
    if (storage_) { free(storage_->text); free(storage_); }
    storage_ = 0;
}
CWideString::operator const wchar_t*() const
{
    return storage_ ? storage_->text : L"";
}
CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other)
    {
        CWideStringData* data = other.storage_ ? WideData(other.storage_->text) : 0;
        if (storage_) { free(storage_->text); free(storage_); }
        storage_ = data;
    }
    return *this;
}

CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    wchar_t buffer[512];
    _snwprintf(buffer, 511, L"%s%s", (const wchar_t*)left, (const wchar_t*)right);
    buffer[511] = 0;
    return CWideString(buffer);
}

CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    return left + CWideString(right);
}

CCharString::CCharString(const char* text, long)
{
    storage_ = static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    memset(storage_, 0, sizeof(*storage_));
    storage_->text = _strdup(text);
}
CCharString::CCharString(const CCharString& other)
{
    storage_ = static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    memset(storage_, 0, sizeof(*storage_));
    storage_->text = _strdup(other.GetConstCharPtr());
}
CCharString::~CCharString()
{
    if (storage_) { free(storage_->text); free(storage_); }
    storage_ = 0;
}
const CCharString& CCharString::operator+=(const char* text)
{
    size_t n = strlen(storage_->text) + strlen(text) + 1;
    char* joined = static_cast<char*>(malloc(n));
    strcpy(joined, storage_->text);
    strcat(joined, text);
    free(storage_->text);
    storage_->text = joined;
    return *this;
}
char g_FableEmptyNarrowString_0129AAF4[1];

CWideString FABLE_FASTCALL CWideString::CreateFromCharString(const CCharString& text)
{
    wchar_t buffer[256];
    _snwprintf(buffer, 255, L"%S", text.GetConstCharPtr());
    buffer[255] = 0;
    return CWideString(buffer);
}

// ---------------------------------------------------------------- scripted callees
static int g_timeCalls;
double FABLE_FASTCALL GFGetTime()
{
    Log("time");
    return ++g_timeCalls == 1 ? 1.5 : 2.5;
}

static int g_systemReturns[16];
static int g_systemReturnIndex;
CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    return g_system;
}
ESystemReturn CSystemManager::Update()
{
    Log("sysupdate(%s)", this == g_system ? "sys" : "?");
    return static_cast<ESystemReturn>(g_systemReturns[g_systemReturnIndex++]);
}

static bool g_movieResults[4];
static int g_movieIndex;
static const void* g_lastBuffers;
bool FABLE_FASTCALL InstallationComplete() { return true; }
bool FABLE_FASTCALL GFPlayFullScreenXBoxMovie(
    const CCharString& filename, unsigned long flags, unsigned long width,
    unsigned long height, unsigned long bufferSize, const NGameText::CDataBank* textBank,
    CIEngine* engine, bool fullScreen, float timeOut, bool (FABLE_FASTCALL *continueFunc)(),
    CXMVPlayerBuffers* buffers, CWideString* errorString)
{
    const CRGBColour& c = g_FableMovieBackgroundColour_013961E0;
    Log("movie(%s,%lx,%lu,%lu,%lx,%s,%s,%d,%g,%s,%s,%s,bg=%02x%02x%02x%02x,playing=%d)",
        filename.GetConstCharPtr(), flags, width, height, bufferSize, Name(textBank),
        Name(engine), fullScreen, timeOut,
        continueFunc == InstallationComplete ? "InstallationComplete" : continueFunc ? "?" : "null",
        buffers == 0 ? "null" : buffers == g_lastBuffers ? "buffers" : "?",
        errorString ? "err" : "null", c.R, c.G, c.B, c.A, g_FablePlayingFullScreenMovie_013CA7E9);
    const bool result = g_movieResults[g_movieIndex++];
    if (!result && errorString)
        *errorString = CWideString(L"no player");
    return result;
}

void FABLE_FASTCALL GPlayMusic(float offset) { Log("music(%g)", offset); }

void FABLE_FASTCALL NProfileTimer::StartProfile(const CCharString& label, long id)
{
    Log("prof(%s,%ld)", label.GetConstCharPtr(), id);
}
void FABLE_FASTCALL NProfileTimer::EndProfile(long id) { Log("endprof(%ld)", id); }

void CBankFileManager::OpenRetailBank(const CWideString& pathname, bool startupPriority)
{
    Log("open(%S,%d,%s)", (const wchar_t*)pathname, startupPriority,
        this == &g_FableBankFileManager_013CA79C ? "banks" : "?");
}
void CBankFileManager::CloseRetailBank(const CWideString& pathname)
{
    Log("close(%S,%s)", (const wchar_t*)pathname,
        this == &g_FableBankFileManager_013CA79C ? "banks" : "?");
}

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetGraphicBankDir() { return CWideString(L"G\\"); }
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetShaderBankDir() { return CWideString(L"S\\"); }
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetHDMiscDir() { return CWideString(L"M\\"); }
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDVDDialogueDir() { return CWideString(L"D\\"); }

static const char* Self(const void* p) { return p == g_component ? "" : "BADTHIS"; }
void CNewFrontendGameComponent::Init2() { Log("init2%s", Self(this)); }
void CNewFrontendGameComponent::InitialiseEngine() { Log("initengine%s", Self(this)); }
void CNewFrontendGameComponent::Input() { Log("input%s", Self(this)); }
void CNewFrontendGameComponent::Update() { Log("update%s", Self(this)); }
static const void* g_interpolationInfo;
void CNewFrontendGameComponent::UpdateInterpolation(CEngineInterpolationInfo& info)
{
    g_interpolationInfo = &info;
    Log("interp%s", Self(this));
}
void CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info)
{
    Log("draw%s(%s)", Self(this), &info == g_interpolationInfo ? "same" : "other");
}
void CNewFrontendGameComponent::ReleaseXMVCode() { Log("releasexmv%s", Self(this)); }
void CNewFrontendGameComponent::CleanUpInterfaces() { Log("cleanup%s", Self(this)); }
void CNewFrontendGameComponent::Shutdown() { Log("shutdown%s", Self(this)); }
CEngineInterpolationInfo::CEngineInterpolationInfo() { Log("interpinfo-ctor"); }

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CXMVPlayerBuffers::CXMVPlayerBuffers()
{
    g_lastBuffers = this;
    Log("xmvbuffers");
}
CXMVPlayerBuffers::~CXMVPlayerBuffers()
{
    Log("~xmvbuffers(%s)", this == g_lastBuffers ? "buffers" : "?");
}

CFontBank::~CFontBank() {}
bool CFontBank::Preload(const CWideString&, bool) const { Log("base-preload"); return false; }
class MockFontBank : public CFontBank
{
public:
    virtual C2DExtentsF GetStringExtents(const wchar_t*,bool) const { return C2DExtentsF(0,0); }
    virtual float GetFontMaxHeight() const { return 0; }
    virtual void PrepareToRender(bool) const {}
    virtual bool Preload(const CWideString& text, bool wait) const
    {
        Log("preload(%S,%d,async=%d)", (const wchar_t*)text, wait, g_fontManagerStorage[0x38]);
        return true;
    }
    virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*) const {}
};
static MockFontBank g_menuFont;

void CRenderManager2D::ClearRenderTarget(const CRGBColour& colour, unsigned long flags)
{
    Log("clear(%02x%02x%02x%02x,%lu,%s)", colour.R, colour.G, colour.B, colour.A, flags,
        (void*)this == g_renderStorage ? "render2d" : "?");
}
bool CDisplayManager::SwapScreens()
{
    Log("swap(%s)", (void*)this == g_displayStorage ? "display" : "?");
    return true;
}

static bool g_canAttract;
NUISystem::CFrontEndManager* FABLE_FASTCALL NUISystem::CFrontEndManager::GetInstance()
{
    Log("feminstance");
    return g_frontEndManager;
}
void NUISystem::CFrontEndManager::ChangeStateFirstTime()
{
    Log("firststate(%s)", this == g_frontEndManager ? "fem" : "?");
}
bool NUISystem::CFrontEndManager::CanStartAttractMode()
{
    Log("canattract(%s)", this == g_frontEndManager ? "fem" : "?");
    return g_canAttract;
}
void FABLE_FASTCALL NUISystem::CUIScreenUnitConverter::ResetDimensions() { Log("resetdims"); }
void CInputManager::ClearEvents() { Log("clearevents(%s)", (void*)this == g_inputStorage ? "input" : "?"); }

static const char* Watch(const void* p)
{
    if (p == &At<unsigned char>(g_component, 0x100)) return "attract";
    if (p == &At<unsigned char>(g_component, 0x120)) return "delay";
    return "?";
}
CStopWatch::CStopWatch() {}
void CStopWatch::StartZero() { Log("startzero(%s)", Watch(this)); }
void CStopWatch::Stop() { Log("stop(%s)", Watch(this)); }
void CStopWatch::Reset() { Log("reset(%s)", Watch(this)); }
float CStopWatch::GetElapsedSeconds() const
{
    Log("elapsed(%s)", Watch(this));
    return m_fTimerPeriod;   // the fixture scripts the elapsed time here
}

class MockSound : public CASoundManager
{
public:
    virtual void UnknownSlot18(long value) { Log("sound18(%ld)", value); }
};
static MockSound g_sound;
#define SLOT(n) void CASoundManager::UnknownSlot##n() {}
SLOT(01) SLOT(02) SLOT(03) SLOT(04) SLOT(05) SLOT(06) SLOT(07) SLOT(08) SLOT(09) SLOT(10)
SLOT(11) SLOT(12) SLOT(13) SLOT(14) SLOT(15) SLOT(16) SLOT(17) SLOT(19) SLOT(20)
SLOT(21) SLOT(22) SLOT(23) SLOT(24) SLOT(25) SLOT(26) SLOT(27) SLOT(28) SLOT(29) SLOT(30)
SLOT(31) SLOT(32) SLOT(33) SLOT(34) SLOT(35) SLOT(36) SLOT(37) SLOT(38) SLOT(39) SLOT(40)
SLOT(41) SLOT(42) SLOT(43) SLOT(44) SLOT(45) SLOT(48) SLOT(50)
void CASoundManager::UnknownSlot46(unsigned long, float, float) {}
float CASoundManager::UnknownSlot47(unsigned long) { return 0.0f; }
bool CASoundManager::UnknownSlot49(float&) { return false; }
void CASoundManager::UnknownSlot18(long) { Log("base-sound18"); }
void CASoundManager::GenerateLipSyncData(const CWideString&, LHA::CBinaryDataWriter&) {}

template<> void FABLE_FASTCALL CTBaseSingleton<CFileInstaller>::Release() { Log("installer-release"); }

static unsigned long g_language;
unsigned long FABLE_FASTCALL NLocalisation::GetLanguage() { Log("language"); return g_language; }

// ---------------------------------------------------------------- next components
void* operator new(size_t size)
{
    if (size == 0x161E8 || size == 0x1E60 || size == 0x10)
        Log("new(%x)", (unsigned)size);
    return calloc(1, size ? size : 1);
}
void operator delete(void* block) { free(block); }

void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
CGameComponent::CGameComponent(CGame& game) { Game = &game; Quit = false; Running = false; }
void CGameComponent::Init() {}
bool CGameComponent::Run(CGameComponent**) { return false; }
void CGameComponent::SetQuit() {}

#include "test_support/init_base_class_stubs.h"
CInterpolationInfo::CInterpolationInfo() {}
CGameEventPackageSet::CGameEventPackageSet() {}
CNetworkClient::CNetworkClient() {}
CFrameRateSmoother::CFrameRateSmoother() {}
CRoughFrameCounter::CRoughFrameCounter() {}
CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() {}
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return CFileInformation(); }
CCharString::CCharString() : storage_(0) {}

static CGameComponent* g_mainComponent;
CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit& init)
    : CGameComponent(game)
{
    g_mainComponent = this;
    Log("main(%s,world=%S,save=%S)", Name(&game), (const wchar_t*)init.InitialWorldName,
        (const wchar_t*)init.SaveGameName);
}
CMainGameComponent::~CMainGameComponent() {}
void CMainGameComponent::Init() { Log("main-init(%s)", this == g_mainComponent ? "main" : "?"); }
bool CMainGameComponent::Run(CGameComponent**) { return false; }
void CMainGameComponent::ChangeTextureColourDepth(long) {}
void CMainGameComponent::SetQuit() {}
void CMainGameComponent::OnPreDeviceReset() {}
bool CMainGameComponent::OnPostDeviceReset() { return true; }
void CMainGameComponent::Update() {}
void CMainGameComponent::GetInputs() {}
void CMainGameComponent::Render() {}
void CMainGameComponent::PostInit() {}
bool CMainGameComponent::IsEditorActive() const { return false; }

static CGameComponent* g_devComponent;
CFrontendGameComponent::CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init)
    : CGameComponent(game)
{
    g_devComponent = this;
    Log("dev(%s,nowt=%ld)", Name(&game), init.Nowt);
}
void CFrontendGameComponent::Init() { Log("dev-init(%s)", this == g_devComponent ? "dev" : "?"); }
bool CFrontendGameComponent::Run(CGameComponent**) { return false; }
void CFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CFrontendGameComponent::SetQuit() {}
void CFrontendGameComponent::OnPreDeviceReset() {}
bool CFrontendGameComponent::OnPostDeviceReset() { return true; }

// ---------------------------------------------------------------- scenarios
static CCharString* g_attractMovies;

static void Reset()
{
    g_log[0] = 0;
    g_timeCalls = 0;
    g_systemReturnIndex = 0;
    g_movieIndex = 0;
    g_lastBuffers = 0;
    g_mainComponent = 0;
    g_devComponent = 0;
    memset(g_componentStorage, 0, sizeof(g_componentStorage));
    memset(g_systemStorage, 0, sizeof(g_systemStorage));
    memset(g_fontManagerStorage, 0, sizeof(g_fontManagerStorage));
    memset(g_frontEndManagerStorage, 0, sizeof(g_frontEndManagerStorage));
    memset(g_frontEndDefStorage, 0, sizeof(g_frontEndDefStorage));
    g_fontManagerStorage[0x38] = 1;
    g_system->PDisplayManager.Data = reinterpret_cast<CDisplayManager*>(g_displayStorage);

    At<CGame*>(g_component, 0x0C) = g_game;
    At<const CFontBank*>(g_component, 0x10) = &g_menuFont;
    At<void*>(g_component, 0x3C) = g_frontEndDefStorage;
    At<void*>(g_component, 0x58) = g_engineStorage;
    At<void*>(g_component, 0x60) = g_textBankStorage;
    At<CWideString>(g_component, 0x84) = CWideString(L"Slot1.sav");
    At<void*>(g_component, 0xB4) = g_frontEndManager;
    At<bool>(g_component, 0x144) = true;

    g_attractMovies = static_cast<CCharString*>(calloc(3, sizeof(CCharString)));
    new (&g_attractMovies[0]) CCharString("Attract0_");
    new (&g_attractMovies[1]) CCharString("Attract1_");
    new (&g_attractMovies[2]) CCharString("Attract2_");
    At<CCharString*>(g_frontEndDefStorage, 0x3C) = g_attractMovies;
    At<CCharString*>(g_frontEndDefStorage, 0x40) = g_attractMovies + 3;

    g_FableFrontendGameComponent_013B871C = g_component;
    g_FableRetiredGameComponent_013B7D58 = 0;
    g_FableFrontendFirstRun_01375448 = false;
    g_FablePlayIntroMovies_0137544A = false;
    g_FablePlayingFullScreenMovie_013CA7E9 = false;
    g_FableMovieBackgroundColour_013961E0 = CRGBColour(1, 2, 3, 4);
    g_FableSoundManager_013B8394 = 0;
    GUseRetailBanks = false;
    g_canAttract = false;
    g_language = 9;
}

static bool Expect(const char* scenario, const char* expected)
{
    if (strcmp(g_log, expected) == 0)
        return true;
    printf("%s mismatch\n got: %s\nwant: %s\n", scenario, g_log, expected);
    return false;
}

// Intro logos: the second fails (error text reaches the frontend manager, the
// media-player flag drops); the system asks to quit after it, so Run returns
// false straight from the intro loop.
static bool IntroQuit()
{
    Reset();
    g_FableFrontendFirstRun_01375448 = true;
    g_FablePlayIntroMovies_0137544A = true;
    g_movieResults[0] = true;
    g_movieResults[1] = false;
    g_systemReturns[0] = ESR_OK;
    g_systemReturns[1] = ESR_QUIT;
    CGameComponent* next = reinterpret_cast<CGameComponent*>(0x1234);
    const bool result = g_component->CNewFrontendGameComponent::Run(&next);
    bool ok = Expect("intro",
        "time;time;"
        "movie(Data\\Video\\lionhead_logo.xmv,1a0,640,400,a00000,textbank,engine,1,20,InstallationComplete,null,err,bg=ffffffff,playing=1);"
        "sysupdate(sys);"
        "movie(Data\\Video\\Microsoft_Logo.xmv,1a0,640,480,a00000,textbank,engine,1,20,InstallationComplete,null,err,bg=000000ff,playing=1);"
        "feminstance;sysupdate(sys);");
    const CRGBColour& c = g_FableMovieBackgroundColour_013961E0;
    ok = ok && !result && next == reinterpret_cast<CGameComponent*>(0x1234) &&
        At<float>(g_component, 0xB8) == 1.5f && At<float>(g_component, 0xBC) == 2.5f &&
        !At<bool>(g_component, 0x144) &&
        wcscmp(g_frontEndManager->MediaPlayerErrorString, L"no player") == 0 &&
        c.R == 0 && c.G == 0 && c.B == 0 && c.A == 0xFF &&
        !g_FablePlayingFullScreenMovie_013CA7E9 &&
        !At<bool>(g_component, 9) && !At<bool>(g_component, 8) &&
        g_FableRetiredGameComponent_013B7D58 == 0 &&
        g_FableFrontendGameComponent_013B871C == g_component;
    if (!ok) printf("intro state mismatch result=%d\n", result);
    return ok;
}

// Navigator banks, two frames (forced music + input-delay stop + attract
// movie on the first), then m_bCreateGame hands over to the main game.
static bool CreateGame()
{
    Reset();
    GUseRetailBanks = true;
    g_FableSoundManager_013B8394 = &g_sound;
    g_canAttract = true;
    g_language = 12;
    g_movieResults[0] = true;
    At<bool>(g_component, 0x29) = true;              // m_bCreateGame
    At<float>(g_component, 0x100) = 120.0f;          // attract timer elapsed
    At<float>(g_component, 0x120) = 0.25f;           // input-delay timer elapsed
    At<bool>(g_component, 0x138) = true;             // input-delay timer running
    g_system->ForceMusicPlay = true;
    g_system->ForceMusicPlayOffset = 3.5f;
    g_systemReturns[0] = ESR_OK;
    g_systemReturns[1] = ESR_OK;
    g_systemReturns[2] = ESR_QUIT;

    srand(7);
    const int pick1 = rand() % 3;
    const int pick2 = rand() % 3;
    srand(7);
    char expected[4096];
    CGameComponent* next = 0;
    const bool result = g_component->CNewFrontendGameComponent::Run(&next);
    _snprintf(expected, sizeof(expected) - 1,
        "time;time;"
        "open(G\\pc\\frontend.big,1,banks);open(D\\text.big,0,banks);open(S\\pc\\shaders.big,0,banks);"
        "init2;prof(Init Engine,0);initengine;endprof(0);prof(Init frontend,0);"
        "new(10);xmvbuffers;preload(A,1,async=0);clear(000000ff,0,render2d);swap(display);"
        "music(0);firststate(fem);startzero(attract);reset(attract);endprof(0);resetdims;"
        "sysupdate(sys);music(3.5);elapsed(delay);stop(delay);input;update;interpinfo-ctor;interp;draw(same);"
        "elapsed(attract);canattract(fem);sound18(0);language;"
        "movie(Attract%d_French.wmv,221,0,0,100000,textbank,engine,1,-1,null,buffers,null,bg=01020304,playing=1);"
        "reset(attract);clearevents(input);startzero(delay);music(0);"
        "sysupdate(sys);elapsed(delay);stop(delay);input;update;interpinfo-ctor;interp;draw(same);"
        "elapsed(attract);canattract(fem);sound18(0);language;"
        "movie(Attract%d_French.wmv,221,0,0,100000,textbank,engine,1,-1,null,buffers,null,bg=01020304,playing=1);"
        "reset(attract);clearevents(input);startzero(delay);music(0);"
        "sysupdate(sys);"
        "~xmvbuffers(buffers);releasexmv;cleanup;"
        "prof(Leave frontend,0);sound18(500);"
        "close(G\\pc\\frontend.big,banks);open(G\\graphics.big,1,banks);open(G\\pc\\textures.big,1,banks);"
        "open(D\\dialogue.big,0,banks);open(M\\pc\\effects.big,0,banks);installer-release;"
        "endprof(0);shutdown;prof(Init Game,1);new(161e8);main(game,world=FinalAlbion.wld,save=Slot1.sav);"
        "main-init(main);",
        pick1, pick2);
    bool ok = Expect("create-game", expected);
    ok = ok && result && next == g_mainComponent && g_mainComponent != 0 &&
        g_FableRetiredGameComponent_013B7D58 == g_component &&
        g_FableFrontendGameComponent_013B871C == 0 &&
        At<bool>(g_component, 8) && !At<bool>(g_component, 9) &&
        !g_FableFrontendFirstRun_01375448 && !g_FablePlayingFullScreenMovie_013CA7E9 &&
        !g_system->ForceMusicPlay && g_system->ForceMusicPlayOffset == 0.0f;
    if (!ok) printf("create-game state mismatch result=%d\n", result);
    return ok;
}

// Quit already requested: no frame runs; the dev frontend replaces this one.
static bool DevFrontEnd()
{
    Reset();
    At<bool>(g_component, 8) = true;                 // Quit
    At<bool>(g_component, 0x2A) = true;              // CreateDevFrontEnd
    At<bool>(g_component, 0x29) = true;              // m_bCreateGame (dev wins)
    g_systemReturns[0] = ESR_OK;
    CGameComponent* next = 0;
    const bool result = g_component->CNewFrontendGameComponent::Run(&next);
    bool ok = Expect("dev",
        "time;time;init2;prof(Init Engine,0);initengine;endprof(0);prof(Init frontend,0);"
        "new(10);xmvbuffers;preload(A,1,async=0);clear(000000ff,0,render2d);swap(display);"
        "music(0);firststate(fem);startzero(attract);reset(attract);endprof(0);resetdims;"
        "sysupdate(sys);~xmvbuffers(buffers);releasexmv;cleanup;"
        "shutdown;new(1e60);dev(game,nowt=0);dev-init(dev);");
    ok = ok && result && next == g_devComponent && g_devComponent != 0 &&
        g_FableRetiredGameComponent_013B7D58 == g_component &&
        g_FableFrontendGameComponent_013B871C == 0 &&
        At<bool>(g_component, 8) && !At<bool>(g_component, 9) &&
        g_FableFrontendFirstRun_01375448 == false;
    if (!ok) printf("dev state mismatch result=%d\n", result);
    return ok;
}

// The system quits with neither request pending: shut down, report false.
static bool PlainQuit()
{
    Reset();
    g_FableFrontendFirstRun_01375448 = true;         // intro flag alone: no movies
    g_systemReturns[0] = ESR_QUIT;
    CGameComponent* next = reinterpret_cast<CGameComponent*>(0x5678);
    const bool result = g_component->CNewFrontendGameComponent::Run(&next);
    bool ok = Expect("quit",
        "time;time;init2;prof(Init Engine,0);initengine;endprof(0);prof(Init frontend,0);"
        "new(10);xmvbuffers;preload(A,1,async=0);clear(000000ff,0,render2d);swap(display);"
        "music(0);firststate(fem);startzero(attract);reset(attract);endprof(0);resetdims;"
        "sysupdate(sys);~xmvbuffers(buffers);releasexmv;cleanup;shutdown;");
    ok = ok && !result && next == reinterpret_cast<CGameComponent*>(0x5678) &&
        g_FableRetiredGameComponent_013B7D58 == g_component &&
        g_FableFrontendGameComponent_013B871C == 0 &&
        g_FableFrontendFirstRun_01375448 &&
        At<bool>(g_component, 8) && !At<bool>(g_component, 9);
    if (!ok) printf("quit state mismatch result=%d\n", result);
    return ok;
}

int main()
{
    const bool a = IntroQuit();
    const bool b = CreateGame();
    const bool c = DevFrontEnd();
    const bool d = PlainQuit();
    if (a && b && c && d)
    {
        puts("FABLETLC_NEWFRONTEND_RUN_PASS");
        return 0;
    }
    puts("FABLETLC_NEWFRONTEND_RUN_FAIL");
    return 1;
}
