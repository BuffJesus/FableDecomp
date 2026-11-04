#include "fable_boot.h"
#include "fable_async.h"
#include "fable_file_installer.h"
#include "fable_filesystem.h"
#include "fable_gfmain.h"
#include "fable_ime.h"
#include "fable_language.h"
#include "fable_movie.h"
#include "fable_retail_banks.h"
#include "fable_startup.h"
#include "fable_system.h"
#if defined(FABLETLC_EXECUTE_EXACT_CIME)
#include "fable_surface.h"
#endif
#if defined(FABLETLC_ENABLE_VISUAL_BOOT)
#include "fable_visual_boot.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

class CSystemManager
{
public:
    ESystemCode Initialise(CSystemManagerInit* init);
    void Uninitialise();
    void DisplayCriticalMessage(const char* message);
};

class CThreadedFileManager
{
public:
    void Uninitialise();
};

class CEnginePrimitiveManager
{
public:
    void Uninitialise();
};

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE9)
CSystemManager* FABLE_FASTCALL GFGetSystemManager();
#endif

void FABLE_CDECL GFUninitialise();

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE9) && !defined(FABLETLC_EXECUTE_EXACT_SYSTEM_MANAGER)
// Phase 9/10 fixtures link the real GFGetSystemManager (0x009A4EC0) without the
// exact system-manager boundary, which otherwise owns the singleton's storage.
extern "C" fable_u32 g_FableStage2SystemManagerStorage[0x180 / 4] = {};
#pragma comment(linker, "/alternatename:?GSystemManager@@3VCSystemManager@@A=_g_FableStage2SystemManagerStorage")
#endif

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
fable_u32 g_FableConsoleVariablesBoundaryCalls = 0;
fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868 = 0;
bool g_FableStartupLatch_013964A8 = false;
const char g_FableEmptyText_0122D70E[] = "";

CCountedFileInstaller g_CFileInstallerSingleton_013CA818 = {0, 0};
GFMainPhase2State g_GFMainPhase2State = {};
GFMainPhase3State g_GFMainPhase3State = {};
GFMainPhase4State g_GFMainPhase4State = {};
GFMainPhase5State g_GFMainPhase5State = {};
GFMainPhase6State g_GFMainPhase6State = {};
GFMainPhase7State g_GFMainPhase7State = {};
GFMainPhase8State g_GFMainPhase8State = {true, true};
GFMainPhase9State g_GFMainPhase9State =
    {false, false, false, 0, 1};
GFMainPhase10State g_GFMainPhase10State = {true, false};
fable_i32 g_FablePhase7CurrentLanguage = 0;

fable_u8 g_FableOpenAllRetailBanks_013B8605 = 0;
fable_u8 g_FableUseDvdBankList_013B8615 = 0;
fable_u8 g_FableNavigatorEnabled_013B8616 = 0;
fable_i8 g_FableLeftAlignText_013B861B = 0;
fable_i8 g_FableNoHangulWordWrap_013B861C = 0;
fable_i8 g_FableDisableCapsLock_013B864F = 0;
bool g_FablePrimaryLeftAlignment_013CA7EA = false;
bool g_FableSecondaryLeftAlignment_013CA7EB = false;
bool g_FableMovieLeftAlignText_013C5A7C = false;
NUISystem::CIME g_FableIME_013B7CD8;

extern "C" void* g_FableImeDrawRectCallback_0143CD64 = 0;
extern "C" void* g_FableImeDrawFansCallback_0143E514 = 0;
extern "C" void* g_FableImeOnCharCallback_0143CF08 = 0;
extern "C" void* g_FableImeRenderCallback_0143CDFC = 0;
extern "C" void* g_FableImeGetCaretCallback_0143E630 = 0;
extern "C" fable_u8 g_FableImeFontVTable_0122DCBC = 0;

CBasicRetailBankManager
    g_FableBasicRetailBankManager_013CA79C = {};
CCharString g_FableBasicRetailBankAliasSlots[5];
// This authored checkpoint retains its zero-initialized bank boundary. Native
// CBankFileManager construction/destruction belongs to the separate GFMain
// closure; introducing its new virtual declaration must not construct it here.
extern "C" fable_u32 g_FableBankFileManagerBoundaryStorage[
    (sizeof(CBankFileManager) + 3) / 4] = {};
#pragma comment(linker, "/alternatename:?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A=_g_FableBankFileManagerBoundaryStorage")
CThreadedFileManager g_FableThreadedFileManager_013BC870;

fable_u32 g_FableFileInstallerConstructCalls = 0;
fable_u32 g_FableFileInstallerDestroyCalls = 0;
fable_u8 g_FableLastBasicInstallStartupFlag = 0;

namespace
{
    typedef void (FABLE_FASTCALL *DestroyCountedObject)(void* object);

    struct FileInstallerReference
    {
        fable_i32 owners;
        DestroyCountedObject destroy;
        void* object;
    };

    void FABLE_FASTCALL DestroyFileInstaller(void* object)
    {
        CFileInstaller* installer =
            static_cast<CFileInstaller*>(object);
        g_FableLastBasicInstallStartupFlag =
            installer->basicInstallStartupFlag;
        ++g_FableFileInstallerDestroyCalls;
        free(installer);
    }

    CWideStringData* AllocateWideString(const wchar_t* text)
    {
        const size_t characters = wcslen(text) + 1;
        CWideStringData* data =
            static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
        if (data == 0)
            return 0;

        data->text = static_cast<wchar_t*>(
            malloc(characters * sizeof(wchar_t)));
        if (data->text == 0)
        {
            free(data);
            return 0;
        }

        memcpy(data->text, text, characters * sizeof(wchar_t));
        data->unknown04 = 0;
        data->unknown08 = 0;
        data->owners = 1;
        return data;
    }

    void ReleaseWideString(CWideStringData*& data)
    {
        if (data == 0)
            return;

        --data->owners;
        if (data->owners <= 0)
        {
            free(data->text);
            free(data);
        }
        data = 0;
    }

    CWideStringData*& WideStorage(CWideString& value)
    {
        return *reinterpret_cast<CWideStringData**>(&value);
    }

    const CWideStringData* WideStorage(const CWideString& value)
    {
        return *reinterpret_cast<CWideStringData* const*>(&value);
    }

    const CCharStringData* CharStorage(const CCharString& value)
    {
        return *reinterpret_cast<CCharStringData* const*>(&value);
    }

    CWideStringData* JoinWideStrings(
        const wchar_t* left,
        const wchar_t* right)
    {
        const size_t leftCharacters = wcslen(left);
        const size_t rightCharacters = wcslen(right);
        CWideStringData* data = static_cast<CWideStringData*>(
            malloc(sizeof(CWideStringData)));
        if (data == 0)
            return 0;

        data->text = static_cast<wchar_t*>(malloc(
            (leftCharacters + rightCharacters + 1) * sizeof(wchar_t)));
        if (data->text == 0)
        {
            free(data);
            return 0;
        }

        memcpy(
            data->text,
            left,
            leftCharacters * sizeof(wchar_t));
        memcpy(
            data->text + leftCharacters,
            right,
            (rightCharacters + 1) * sizeof(wchar_t));
        data->unknown04 = 0;
        data->unknown08 = 0;
        data->owners = 1;
        return data;
    }

    CEnginePrimitiveManager g_FableEnginePrimitiveManagerBoundary;
    GFMainPhase9State* g_FableActiveExactImeState;
}

void* FABLE_CDECL FableFileInstallerAllocate(fable_u32 size)
{
    return malloc(size);
}

CFileInstaller::CFileInstaller()
{
    // Preserve the compiler-installed virtual pointer in this authored recorder.
    memset(reinterpret_cast<unsigned char*>(this) + sizeof(void*),
        0, sizeof(*this) - sizeof(void*));
    ++g_FableFileInstallerConstructCalls;
}

// No native installer resources are created by this checkpoint recorder.
CFileInstaller::~CFileInstaller() {}

void CCountedFileInstaller::operator=(CFileInstaller* value)
{
    Reset();
    object = value;
    if (value == 0)
        return;

    FileInstallerReference* owner =
        static_cast<FileInstallerReference*>(
            malloc(sizeof(FileInstallerReference)));
    if (owner == 0)
    {
        DestroyFileInstaller(value);
        object = 0;
        return;
    }

    owner->owners = 1;
    owner->destroy = DestroyFileInstaller;
    owner->object = value;
    reference = owner;
}

void CCountedFileInstaller::Reset()
{
    FileInstallerReference* owner =
        static_cast<FileInstallerReference*>(reference);
    if (owner != 0)
    {
        --owner->owners;
        if (owner->owners == 0)
        {
            owner->destroy(owner->object);
            free(owner);
        }
    }

    object = 0;
    reference = 0;
}

void FABLE_FASTCALL FableFileInstallerSingletonRelease()
{
    g_CFileInstallerSingleton_013CA818.Reset();
}

void CBankFileManager::Close()
{
}

void CSystemManager::Uninitialise()
{
}

void CThreadedFileManager::Uninitialise()
{
}

void FABLE_FASTCALL UninitializeStreamingFontTextureCache()
{
}

CEnginePrimitiveManager* FABLE_FASTCALL
FableGetEnginePrimitiveManager_00404A80()
{
    return &g_FableEnginePrimitiveManagerBoundary;
}

void CEnginePrimitiveManager::Uninitialise()
{
}

#if defined(FABLETLC_EXECUTE_EXACT_CIME)
// Stand-ins for the engine members CIME::Initialise (0x00405650) reaches.
// ImeUi stays uninitialised here (ImeUi_Initialize reports failure), so the
// appearance, font and render-target branch is linked but not taken.

// NUISystem::CIME::Reset @ 0x004051C0 (the boundary's CIME is already clear).
void NUISystem::CIME::Reset()
{
}

// CIME::Callback_DrawRect @ 0x00404D50.
void FABLE_FASTCALL NUISystem::CIME::Callback_DrawRect(
    int,
    int,
    int,
    int,
    unsigned long)
{
}

// CIME::Callback_malloc @ 0x00404A60.
void* FABLE_FASTCALL NUISystem::CIME::Callback_malloc(unsigned int bytes)
{
    return malloc(bytes);
}

// CIME::Callback_free @ 0x00404A70.
void FABLE_FASTCALL NUISystem::CIME::Callback_free(void* pointer)
{
    free(pointer);
}

// CIME::Callback_DrawFans @ 0x00404E10.
void FABLE_FASTCALL NUISystem::CIME::Callback_DrawFans(
    const IMEUI_VERTEX*,
    unsigned int)
{
}

// CIME::Callback_OnChar @ 0x00404F00.
void FABLE_FASTCALL NUISystem::CIME::Callback_OnChar(wchar_t)
{
}

// NUISystem::CIMEFont virtuals (vtable emitted by CIME::Initialise's unit).
void NUISystem::CIMEFont::SetHeight(unsigned int)
{
}

void NUISystem::CIMEFont::SetColor(unsigned long)
{
}

void NUISystem::CIMEFont::SetPosition(int, int)
{
}

void NUISystem::CIMEFont::GetTextExtent(
    const wchar_t*,
    unsigned long* width,
    unsigned long* height)
{
    if (width != 0)
        *width = 0;
    if (height != 0)
        *height = 0;
}

void NUISystem::CIMEFont::DrawTextW(const wchar_t*)
{
}

// DXUT ImeUi callback slots (0x0143CD64, 0x0143E514, 0x0143CF08,
// 0x0143CDFC, 0x0143E630).
void (FABLE_FASTCALL* ImeUiCallback_DrawRect)(
    int,
    int,
    int,
    int,
    unsigned long) = 0;
void* (FABLE_FASTCALL* ImeUiCallback_Malloc)(unsigned int) = 0;
void (FABLE_FASTCALL* ImeUiCallback_Free)(void*) = 0;
void (FABLE_FASTCALL* ImeUiCallback_DrawFans)(
    const IMEUI_VERTEX*,
    unsigned int) = 0;
void (FABLE_FASTCALL* ImeUiCallback_OnChar)(wchar_t) = 0;

// ImeUi_Initialize @ 0x00CB5180: records the call and reports failure.
bool FABLE_FASTCALL ImeUi_Initialize(HWND__* window, bool disable)
{
    if (g_FableActiveExactImeState != 0)
    {
        ++g_FableActiveExactImeState->exactImeUiInitialiseCalls;
        g_FableActiveExactImeState->observedImeInitialiseMode =
            disable ? 1 : 0;
        g_FableActiveExactImeState->observedImeWindowHandleToken =
            static_cast<fable_u32>(
                reinterpret_cast<size_t>(window));
    }
    return false;
}

// ImeUi_SetAppearance @ 0x00CB2E20.
void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE*)
{
}

// ImeUi_SetScreenDimension @ 0x00CB2EE0.
void FABLE_FASTCALL ImeUi_SetScreenDimension(unsigned int, unsigned int)
{
}

// ImeUi_FinalizeString @ 0x00CB2C60.
void FABLE_FASTCALL ImeUi_FinalizeString(bool)
{
}

// ImeUi_EnableIme @ 0x00CB5130.
void FABLE_FASTCALL ImeUi_EnableIme(bool)
{
}

// CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0: no render target
// exists in this runtime.
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    return C2DExtentsI(0, 0);
}
#endif

void* operator new(size_t, void*);
extern "C" void* FablePlacementNewBoundary(size_t, void* address)
{
    return address;
}

#pragma comment(linker,"/alternatename:??2@YAPAXIPAX@Z=_FablePlacementNewBoundary")

CSystemManagerInit* FABLE_FASTCALL
FableGetSystemManagerInitBoundary()
{
    static CSystemManagerInit systemManagerInit;
    return &systemManagerInit;
}

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(
        fable_u8* /* stackTop */,
        unsigned long /* stackSize */)
    {
    }
}

// From GFMain phase 3 on, the reconstructed operator+ units (0x0099BF30,
// 0x0099BFF0) are linked and bring the real CWideString(const wchar_t*),
// UnassignString and operator+= members; these stand-ins cover the earlier
// phases only. The real members reach the text through the range assign/append
// helpers below, which keep the stand-ins' malloc/free ownership.
#if !defined(FABLETLC_ENABLE_GFMAIN_PHASE3)
CWideString::CWideString(const wchar_t* text)
    : storage_(AllocateWideString(text))
{
    ++g_CWideStringInstanceCount_013BCA20;
}
#endif

CWideString::CWideString(const CCharString& text)
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;

    const CCharStringData* source = CharStorage(text);
    if (source == 0 || source->text == 0)
        return;

    const size_t characters = strlen(source->text);
    wchar_t* converted = static_cast<wchar_t*>(
        malloc((characters + 1) * sizeof(wchar_t)));
    if (converted == 0)
        return;

    size_t index;
    for (index = 0; index < characters; ++index)
        converted[index] = static_cast<unsigned char>(source->text[index]);
    converted[characters] = L'\0';

    storage_ = static_cast<CWideStringData*>(
        malloc(sizeof(CWideStringData)));
    if (storage_ == 0)
    {
        free(converted);
        return;
    }

    storage_->text = converted;
    storage_->unknown04 = 0;
    storage_->unknown08 = 0;
    storage_->owners = 1;
}

CWideString::operator const wchar_t*() const
{
    static const wchar_t kEmpty[] = L"";
    return storage_ != 0 ? storage_->text : kEmpty;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other)
    {
        ReleaseWideString(storage_);
        storage_ = other.storage_;
        if (storage_ != 0)
            ++storage_->owners;
    }
    return *this;
}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    ReleaseWideString(storage_);
    storage_ = AllocateWideString(text);
    return *this;
}

#if !defined(FABLETLC_ENABLE_GFMAIN_PHASE3)
void CWideString::UnassignString()
{
    ReleaseWideString(storage_);
}

const CWideString& CWideString::operator+=(const CWideString& other)
{
    const wchar_t* left =
        storage_ != 0 && storage_->text != 0 ? storage_->text : L"";
    const CWideStringData* otherStorage = WideStorage(other);
    const wchar_t* right =
        otherStorage != 0 && otherStorage->text != 0
            ? otherStorage->text
            : L"";
    CWideStringData* joined = JoinWideStrings(left, right);
    ReleaseWideString(storage_);
    storage_ = joined;
    return *this;
}

const CWideString& CWideString::operator+=(const wchar_t* text)
{
    const wchar_t* left =
        storage_ != 0 && storage_->text != 0 ? storage_->text : L"";
    CWideStringData* joined =
        JoinWideStrings(left, text != 0 ? text : L"");
    ReleaseWideString(storage_);
    storage_ = joined;
    return *this;
}
#else
// The shared empty text operator+=(const CWideString&) appends for a null side.
wchar_t g_FableEmptyWideString_0129A8E0[1] = {L'\0'};

// CBasicString<wchar_t> range assign/append (retail 0x0099C670 / 0x0099C7E0).
void FABLE_CDECL FableWideTextAssign_0099C670(
    CWideStringData* data,
    const wchar_t* first,
    const wchar_t* last)
{
    const size_t length = static_cast<size_t>(last - first);
    wchar_t* text =
        static_cast<wchar_t*>(malloc((length + 1) * sizeof(wchar_t)));
    if (text == 0)
        return;

    memcpy(text, first, length * sizeof(wchar_t));
    text[length] = L'\0';
    if (data->text != 0)
        free(data->text);
    data->text = text;
}

void FABLE_CDECL FableWideTextAppend_0099C7E0(
    CWideStringData* data,
    const wchar_t* first,
    const wchar_t* last)
{
    const size_t length = data->text != 0 ? wcslen(data->text) : 0;
    const size_t extra = static_cast<size_t>(last - first);
    wchar_t* text = static_cast<wchar_t*>(
        realloc(data->text, (length + extra + 1) * sizeof(wchar_t)));
    if (text == 0)
        return;

    memcpy(text + length, first, extra * sizeof(wchar_t));
    text[length + extra] = L'\0';
    data->text = text;
}
#endif

extern "C" void __fastcall
FableWideStringConstructLiteral_0099B6B0(
    CWideString* self,
    void*,
    const wchar_t* text)
{
    new (self) CWideString(text);
}

extern "C" const CWideString& __fastcall
FableWideStringAppendWide_0099B8D0(
    CWideString* self,
    void*,
    const CWideString& text)
{
    return self->operator+=(text);
}

extern "C" const CWideString& __fastcall
FableWideStringAppendLiteral_0099B940(
    CWideString* self,
    void*,
    const wchar_t* text)
{
    return self->operator+=(text);
}

extern "C" void __fastcall
FableWideStringUnassign_0099B4D0(CWideString* self)
{
    CWideStringData*& storage = WideStorage(*self);
    ReleaseWideString(storage);
}

extern "C" void __cdecl FableWideStringFree_00BFEA14(void* memory)
{
    free(memory);
}

extern "C" void __cdecl FableWideStringDelete_00BFE9BC(void* memory)
{
    free(memory);
}

CCharStringData* CCharString::AllocStringData(
    const char* text,
    long length)
{
    if (length < 0)
        length = static_cast<long>(strlen(text));

    CCharStringData* data =
        static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    if (data == 0)
        return 0;

    data->text = static_cast<char*>(malloc(length + 1));
    if (data->text == 0)
    {
        free(data);
        return 0;
    }

    memcpy(data->text, text, length);
    data->text[length] = '\0';
    data->unknown04 = 0;
    data->unknown08 = 0;
    data->flags0C = 0;
    data->owners = 1;
    return data;
}

void CCharString::UnassignString()
{
    if (storage_ == 0)
        return;

    --storage_->owners;
    if (storage_->owners <= 0)
    {
        free(storage_->text);
        free(storage_);
    }
    storage_ = 0;
}

const CCharString& CCharString::operator=(const char* text)
{
    UnassignString();
    storage_ = AllocStringData(text, -1);
    return *this;
}

// Authored link boundary until the recovered 1,052-byte text-tag parser is
// ready to join the visual runtime. The connected false path never calls it.
void CProgressDisplay::CalculateNextTextTag()
{
}

void FABLE_FASTCALL FableConstructWideString(CWideString* value)
{
    new (value) CWideString;
}

void FABLE_FASTCALL FableConstructCharString(CCharString* value)
{
    new (value) CCharString;
}

const CWideString& FABLE_FASTCALL FableAssignWideString(
    CWideString* value,
    void*,
    const wchar_t* text)
{
    return *value = text;
}

// Retail 0x004016F0; defaults only, SkipConfigDetection is left to the caller.
CDisplayManagerInit::CDisplayManagerInit()
{
    Width = 0;
    Height = 0;
    Depth = 0;
    RefreshRate = 0;
    WindowHandle = 0;
    MultisampleType = 0;
    RenderManagerInitFlags = 0;
    UseRenderManager = true;
    UseShaderRenderManager = true;
    PresentationIntevalVSyncCount = 1;
    PresentImmediatelyIfMissedVSync = true;
    PresentImmediate = true;
    ZBufferDepth = 16;
    NoBackBuffers = 1;
    PushBufferSize = 0x80000;
    KickOffSize = 0x8000;
}

CWideString g_CAFileCurrentPath_013BCA10;

void FABLE_FASTCALL FableInitialiseConsoleVariablesBoundary()
{
    ++g_FableConsoleVariablesBoundaryCalls;
}

CCharString& CBasicRetailBankAliasMap::operator[](
    const CCharString& key)
{
    static const char* const kPlatformNames[] =
    {
        "GBANK_MAIN_PC",
        "GBANK_GUI_PC",
        "GBANK_FRONT_END_PC",
        "PARTICLE_MAIN_PC",
        "PARTICLE_FRONTEND_PC"
    };

    const CCharStringData* storage = CharStorage(key);
    const char* text =
        storage != 0 && storage->text != 0 ? storage->text : "";
    for (fable_u32 index = 0; index < 5; ++index)
    {
        if (strcmp(text, kPlatformNames[index]) == 0)
            return g_FableBasicRetailBankAliasSlots[index];
    }
    return g_FableBasicRetailBankAliasSlots[0];
}

bool FABLE_FASTCALL FableGFMainPhase3PathExistsBoundary(
    const wchar_t* pathname,
    GFMainPhase3State& state)
{
    ++state.pathExistsCalls;
    wcsncpy(
        state.resolvedSettingsPath,
        pathname != 0 ? pathname : L"",
        sizeof(state.resolvedSettingsPath) /
            sizeof(state.resolvedSettingsPath[0]) - 1);
    state.resolvedSettingsPath[
        sizeof(state.resolvedSettingsPath) /
            sizeof(state.resolvedSettingsPath[0]) - 1] = L'\0';
    return state.settingsFileExists;
}

void FABLE_FASTCALL FableGFMainPhase3LoadSettingsBoundary(
    const wchar_t* /* pathname */,
    GFMainPhase3State& state)
{
    ++state.diskFileConstructCalls;
    if (state.settingsFileOpened)
    {
        ++state.loadFromFileCalls;
        ++state.stringParserConstructCalls;
        ++state.persistContextConstructCalls;

        g_FableLeftAlignText_013B861B =
            state.hasLeftAlignText
                ? state.persistedLeftAlignText
                : 0;
        g_FableNoHangulWordWrap_013B861C =
            state.hasNoHangulWordWrap
                ? state.persistedNoHangulWordWrap
                : 0;
        g_FableDisableCapsLock_013B864F =
            state.hasDisableCapsLock
                ? state.persistedDisableCapsLock
                : 0;
        state.transferCalls += 3;
        ++state.loadImeSettingsCalls;
        ++state.sortTreeCalls;
    }
    ++state.diskFileCloseCalls;
}

void FABLE_FASTCALL FableGFMainPhase5OpenRetailBankBoundary(
    const wchar_t* pathname,
    bool startupPriority,
    GFMainPhase5State& state)
{
    if (state.openRetailBankCalls >= 9)
        return;

    GFMainPhase5BankOpen& record =
        state.retailBanks[state.openRetailBankCalls++];
    wcsncpy(
        record.pathname,
        pathname != 0 ? pathname : L"",
        sizeof(record.pathname) / sizeof(record.pathname[0]) - 1);
    record.pathname[
        sizeof(record.pathname) / sizeof(record.pathname[0]) - 1] = L'\0';
    record.startupPriority = startupPriority;
}

void FABLE_FASTCALL FableGFMainPhase5OpenIniFileBoundary(
    const wchar_t* pathname,
    GFMainPhase5State& state)
{
    ++state.openIniFileCalls;
    wcsncpy(
        state.iniPathname,
        pathname != 0 ? pathname : L"",
        sizeof(state.iniPathname) / sizeof(state.iniPathname[0]) - 1);
    state.iniPathname[
        sizeof(state.iniPathname) / sizeof(state.iniPathname[0]) - 1] = L'\0';
}

void FABLE_FASTCALL FableGFMainPhase5SetHeaderDirectoryBoundary(
    const wchar_t* pathname,
    GFMainPhase5State& state)
{
    ++state.setHeaderDirectoryCalls;
    wcsncpy(
        state.headerDirectory,
        pathname != 0 ? pathname : L"",
        sizeof(state.headerDirectory) /
            sizeof(state.headerDirectory[0]) - 1);
    state.headerDirectory[
        sizeof(state.headerDirectory) /
            sizeof(state.headerDirectory[0]) - 1] = L'\0';
}

namespace
{
    void CopyPhase6Path(
        wchar_t* destination,
        size_t capacity,
        const wchar_t* pathname)
    {
        wcsncpy(
            destination,
            pathname != 0 ? pathname : L"",
            capacity - 1);
        destination[capacity - 1] = L'\0';
    }
}

void FABLE_FASTCALL FableGFMainPhase6SetDefinitionTablePathBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.setDefinitionTablePathCalls;
    CopyPhase6Path(
        state.definitionTablePath,
        sizeof(state.definitionTablePath) /
            sizeof(state.definitionTablePath[0]),
        pathname);
#if defined(FABLETLC_EXECUTE_EXACT_DEFINITION_TABLE)
    FableRunExactDefinitionTableSetPathBoundary(pathname, state);
#endif
}

void FABLE_FASTCALL FableGFMainPhase6LoadDefinitionTableBoundary(
    bool loadAllowed,
    GFMainPhase6State& state)
{
    ++state.loadDefinitionTableCalls;
    state.definitionTableLoadAllowed = loadAllowed;
#if defined(FABLETLC_EXECUTE_EXACT_DEFINITION_TABLE)
    FableRunExactDefinitionTableLoadBoundary(loadAllowed, state);
#endif
}

bool FABLE_FASTCALL FableGFMainPhase6OpenDirectoryBoundary(
    GFMainPhase6PathKind kind,
    const wchar_t*,
    GFMainPhase6State& state)
{
    ++state.openDirectoryCalls[kind];
    return kind == GFMainPhase6Path_Checkpoint
        ? state.checkpointOpenSucceeds
        : state.saveOpenSucceeds;
}

void FABLE_FASTCALL FableGFMainPhase6CloseDirectoryBoundary(
    GFMainPhase6PathKind kind,
    GFMainPhase6State& state)
{
    ++state.closeDirectoryCalls[kind];
}

fable_u32 FABLE_FASTCALL FableGFMainPhase6CheckPermissionsBoundary(
    GFMainPhase6PathKind kind,
    const wchar_t* pathname,
    GFMainPhase6State& state)
{
    ++state.permissionCheckCalls[kind];
    CopyPhase6Path(
        state.permissionPath,
        sizeof(state.permissionPath) /
            sizeof(state.permissionPath[0]),
        pathname);
#if defined(FABLETLC_EXECUTE_EXACT_WRITE_PERMISSIONS)
    return FableRunExactWritePermissionsBoundary(pathname, state);
#else
    if (kind == GFMainPhase6Path_Checkpoint)
        return state.checkpointPermissionResult;
    if (kind == GFMainPhase6Path_Save)
        return state.savePermissionResult;
    return state.myDocumentsPermissionResult;
#endif
}

void FABLE_FASTCALL FableGFMainPhase6ConstructCompressedStreamBoundary(
    GFMainPhase6State& state)
{
    ++state.compressedStreamConstructCalls;
}

void FABLE_FASTCALL FableGFMainPhase6ResetCountedPointerBoundary(
    GFMainPhase6State& state)
{
    ++state.countedPointerResetCalls;
}

void FABLE_FASTCALL FableGFMainPhase6OpenFontBankBoundary(
    GFMainPhase6State& state)
{
    ++state.openFontBankCalls;
}

fable_u32 FABLE_FASTCALL FableGFMainPhase6LookupTextBoundary(
    GFMainPhase6TextRole role,
    GFMainPhase6State& state)
{
    ++state.textLookupCalls[role];
    return role == GFMainPhase6Text_PermissionError
        ? state.permissionTextLookupResult
        : state.captionTextLookupResult;
}

bool FABLE_FASTCALL FableGFMainPhase6GetTextEntryBoundary(
    GFMainPhase6TextRole role,
    fable_u32,
    GFMainPhase6State& state)
{
    ++state.textEntryCalls[role];
    return role == GFMainPhase6Text_PermissionError
        ? state.permissionGroupAvailable
        : state.captionGroupAvailable;
}

void FABLE_FASTCALL FableGFMainPhase6GetTextBoundary(
    GFMainPhase6TextRole role,
    GFMainPhase6State& state)
{
    ++state.getTextCalls[role];
}

void FABLE_FASTCALL FableGFMainPhase6DisplayPermissionErrorBoundary(
    fable_u32 errorCode,
    GFMainPhase6State& state)
{
    ++state.displayPermissionErrorCalls;
    state.displayedPermissionError = errorCode;
}

unsigned long FABLE_FASTCALL NLocalisation::GetLanguage()
{
    return g_FablePhase7CurrentLanguage;
}

CWideString FABLE_FASTCALL
FableGFMainPhase7GetWindowTitleBoundary(
    GFMainPhase7State& state)
{
    ++state.windowTitleCalls;
#if defined(FABLETLC_EXECUTE_EXACT_WINDOW_TITLE)
    return FableRunExactWindowTitleBoundary(state);
#else
    const wchar_t* title =
        state.windowTitleSource[0] != L'\0'
            ? state.windowTitleSource
            : L"Fable - The Lost Chapters";
    return CWideString(title);
#endif
}

bool FABLE_FASTCALL FableGFMainPhase8DoEULABoundary(
    GFMainPhase8State& state)
{
    ++state.eulaCalls;
#if defined(FABLETLC_EXECUTE_EXACT_PHASE8)
    return FableRunExactPhase8EulaBoundary(state);
#else
    return state.eulaSucceeds;
#endif
}

bool FABLE_FASTCALL FableGFMainPhase8DetectConfigurationBoundary(
    GFMainPhase8State& state)
{
    ++state.configurationCalls;
#if defined(FABLETLC_EXECUTE_EXACT_PHASE8)
    return FableRunExactPhase8ConfigurationBoundary(state);
#else
    return state.configurationSucceeds;
#endif
}

void FABLE_FASTCALL FableGFMainPhase8FreeConfigurationBoundary(
    GFMainPhase8State& state)
{
    ++state.freeConfigurationCalls;
#if defined(FABLETLC_EXECUTE_EXACT_PHASE8)
    FableRunExactPhase8FreeConfigurationBoundary(state);
#endif
}

void FABLE_FASTCALL FableGFMainPhase8ProcessExitBoundary(
    fable_i32 exitCode,
    GFMainPhase8State& state)
{
    ++state.processExitCalls;
    state.requestedExitCode = exitCode;
}

fable_i32 FABLE_FASTCALL
FableGFMainPhase9InitialiseSystemManagerBoundary(
    void*,
    fable_u32,
    GFMainPhase9State& state)
{
    ++state.systemInitialiseCalls;
#if defined(FABLETLC_EXECUTE_EXACT_SYSTEM_MANAGER)
    return FableRunExactSystemManagerInitialiseBoundary(
        FableGetSystemManagerInitBoundary(),
        state);
#else
    return state.systemInitialiseResult;
#endif
}

void* FABLE_FASTCALL
FableGFMainPhase9GetSystemManagerBoundary(
    GFMainPhase9State& state)
{
    ++state.systemManagerLookupCalls;
#if defined(FABLETLC_ENABLE_GFMAIN_PHASE9)
    return GFGetSystemManager();
#else
    return 0;
#endif
}

void FABLE_FASTCALL FableGFMainPhase9InitialiseImeBoundary(
    void* imePrimitive,
    void* windowHandle,
    GFMainPhase9State& state)
{
    ++state.imeInitialiseCalls;
    state.observedImeWindowHandleToken =
        static_cast<fable_u32>(
            reinterpret_cast<size_t>(windowHandle));
#if defined(FABLETLC_EXECUTE_EXACT_CIME)
    g_FableActiveExactImeState = &state;
    static_cast<NUISystem::CIME*>(imePrimitive)->Initialise(
        static_cast<HWND__*>(windowHandle));
    g_FableActiveExactImeState = 0;
#endif
}

void FABLE_FASTCALL
FableGFMainPhase9ProcessSaveMetadataBoundary(
    GFMainPhase9State& state)
{
    ++state.processSaveMetadataCalls;
#if defined(FABLETLC_EXECUTE_EXACT_SAVE_METADATA)
    FableRunExactSaveMetadataBoundary(state);
#endif
}

bool FABLE_FASTCALL FableGFMainPhase10GFInitialiseBoundary(
    GFMainPhase10State& state)
{
    ++state.gfInitialiseCalls;
#if defined(FABLETLC_ENABLE_VISUAL_BOOT) || \
    defined(FABLETLC_EXECUTE_EXACT_GFINITIALISE)
    FablePrepareGFInitialiseBoundary();
    state.observedGFInitialiseResult = GFInitialise();
#else
    state.observedGFInitialiseResult = state.gfInitialiseSucceeds;
#endif
    return state.observedGFInitialiseResult;
}

bool FABLE_FASTCALL
FableGFMainPhase10HasStartupMessageBoundary(
    GFMainPhase10State& state)
{
    ++state.startupMessageCheckCalls;
    return state.startupMessagePresent;
}

void FABLE_FASTCALL
FableGFMainPhase10GetRenderManagerBoundary(
    GFMainPhase10State& state)
{
    ++state.getRenderManagerCalls;
}

void FABLE_FASTCALL
FableGFMainPhase10AddStartupLineBoundary(
    GFMainPhase10State& state)
{
    ++state.addStartupLineCalls;
}

void FABLE_FASTCALL FableGFMainPhase10PlayBoundary(
    GFMainPhase10State& state)
{
    ++state.playCalls;
}

void FABLE_FASTCALL
FableGFMainPhase10GFUninitialiseBoundary(
    GFMainPhase10State& state)
{
    ++state.gfUninitialiseCalls;
#if defined(FABLETLC_EXECUTE_EXACT_GFUNINITIALISE)
    GFUninitialise();
#endif
}

void FABLE_FASTCALL
FableGFMainPhase10HandleSystemInitErrorBoundary(
    fable_i32 errorCode,
    GFMainPhase10State& state)
{
    ++state.systemInitErrorCalls;
    state.observedSystemInitError = errorCode;
}

void FABLE_FASTCALL
FableGFMainPhase10ConstructRegistryBoundary(
    GFMainPhase10State& state)
{
    ++state.registryConstructCalls;
}

void FABLE_FASTCALL
FableGFMainPhase10WriteRegistryBoolBoundary(
    bool value,
    GFMainPhase10State& state)
{
    ++state.registryWriteCalls;
    state.registryWrittenValue = value;
}

void FABLE_FASTCALL
FableGFMainPhase10DestroyRegistryBoundary(
    GFMainPhase10State& state)
{
    ++state.registryDestroyCalls;
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
#if defined(FABLETLC_ENABLE_GFMAIN_PHASE10)
#if defined(FABLETLC_EXECUTE_EXACT_DEFINITION_TABLE)
    g_GFMainPhase6State.definitionTableEnabled = true;
#endif
#if defined(FABLETLC_EXECUTE_EXACT_SAVE_METADATA)
    g_GFMainPhase9State.processSaveFileMetadata = true;
#endif
    const long completeResult = FableRunGFMainComplete(
        instance,
        commandLine,
        showCommand);
    if (completeResult != 0)
        return completeResult;
#if defined(FABLETLC_EXECUTE_EXACT_DEFINITION_TABLE)
    if (
        g_GFMainPhase6State.setDefinitionTablePathCalls != 1 ||
        g_GFMainPhase6State.loadDefinitionTableCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableSetPathCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableLoadCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTablePathExistsCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableOpenCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableStreamConstructCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableReadCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableStreamDestroyCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableCloseCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableStringDestroyCalls != 1 ||
        g_GFMainPhase6State.exactDefinitionTableFileDestroyCalls != 1 ||
        !g_GFMainPhase6State.exactDefinitionTablePathAssigned ||
        !g_GFMainPhase6State.exactDefinitionTableArgumentsValid ||
        !g_GFMainPhase6State.exactDefinitionTableLoadedBeforeCleanup ||
        !g_GFMainPhase6State.exactDefinitionTableLoaded ||
        !g_GFMainPhase6State.exactDefinitionTableReadOnly)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_WRITE_PERMISSIONS)
    if (
        g_GFMainPhase6State.permissionCheckCalls[
            GFMainPhase6Path_MyDocuments] != 1 ||
        g_GFMainPhase6State.exactPermissionProbeCalls != 1 ||
        g_GFMainPhase6State.exactPermissionCreateCalls != 1 ||
        g_GFMainPhase6State.exactPermissionCloseHandleCalls != 1 ||
        g_GFMainPhase6State.exactPermissionDeleteCalls != 1 ||
        !g_GFMainPhase6State.exactPermissionProbeFileRemoved ||
        g_GFMainPhase6State.exactPermissionWideStringDelta != 0 ||
        g_GFMainPhase6State.exactPermissionCharStringDelta != 0 ||
        g_GFMainPhase6State.exactPermissionResult != 0 ||
        !g_GFMainPhase6State.exactPermissionArgumentsValid)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_CIME)
    if (
        g_GFMainPhase9State.exactImeUiInitialiseCalls != 1 ||
        g_GFMainPhase9State.observedImeInitialiseMode != 1)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_SYSTEM_MANAGER)
    if (
        g_GFMainPhase9State.exactSystemPreInitialiseCalls != 1 ||
        g_GFMainPhase9State.exactSystemColoursConstructCalls != 1 ||
        g_GFMainPhase9State.exactSystemWindowInitialiseCalls != 0 ||
        g_GFMainPhase9State.exactSystemDisplayConstructCalls != 1 ||
        g_GFMainPhase9State.exactSystemInputConstructCalls != 1 ||
        g_GFMainPhase9State.exactSystemDriveInitialiseCalls != 1 ||
        g_GFMainPhase9State.exactSystemTextConstructCalls != 1 ||
        g_GFMainPhase9State.exactSystemMeshConstructCalls != 1 ||
        g_GFMainPhase9State.exactSystemComponentAddCalls != 1 ||
        g_GFMainPhase9State.observedExactSystemInitialiseCount != 1)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_SAVE_METADATA)
    // 0x00418C3B's /GS cookie check is the CRT's own __security_check_cookie
    // now, so exactSaveMetadataSecurityChecks is no longer observable.
    if (
        g_GFMainPhase9State.processSaveMetadataCalls != 1 ||
        g_GFMainPhase9State.exactSaveMetadataCoordinatorCalls != 1 ||
        g_GFMainPhase9State.exactSaveMetadataFindFirstCalls != 1)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_PHASE8)
    if (
        g_GFMainPhase8State.eulaCalls != 1 ||
        g_GFMainPhase8State.exactEulaExportCalls != 1 ||
        g_GFMainPhase8State.exactEulaFreeCalls != 1 ||
        g_GFMainPhase8State.configurationCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationCreateCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationVideoCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationRequirementsCalls != 1 ||
        g_GFMainPhase8State.freeConfigurationCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationReleaseCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationFreeCalls != 1 ||
        g_GFMainPhase8State.exactConfigurationSecurityChecks != 0)
    {
        return 1;
    }
#endif
#if defined(FABLETLC_EXECUTE_EXACT_WINDOW_TITLE)
    if (
        g_GFMainPhase7State.windowTitleCalls != 1 ||
        g_GFMainPhase7State.exactWindowTitleCoordinatorCalls != 1 ||
        g_GFMainPhase7State.exactWindowTitleBankOpenCalls != 1 ||
        g_GFMainPhase7State.exactWindowTitleLookupCalls != 1 ||
        g_GFMainPhase7State.exactWindowTitleBankDeleteCalls != 1)
    {
        return 1;
    }
#endif
#else
    const long phase1Result = FableRunGFMainPhase1(commandLine);
    if (phase1Result != 0)
        return phase1Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE2)
    const long phase2Result = FableRunGFMainPhase2(g_GFMainPhase2State);
    if (phase2Result != 0)
        return phase2Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE3)
    const long phase3Result = FableRunGFMainPhase3(g_GFMainPhase3State);
    if (phase3Result != 0)
        return phase3Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE4)
    const long phase4Result = FableRunGFMainPhase4(g_GFMainPhase4State);
    if (phase4Result != 0)
        return phase4Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE5)
    const long phase5Result = FableRunGFMainPhase5(g_GFMainPhase5State);
    if (phase5Result != 0)
        return phase5Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE6)
    const long phase6Result = FableRunGFMainPhase6(
        g_GFMainPhase5State,
        g_GFMainPhase6State);
    if (phase6Result != 0)
        return phase6Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE7)
    g_GFMainPhase7State.instanceHandleToken =
        static_cast<fable_u32>(
            reinterpret_cast<size_t>(instance));
    if (commandLine != 0)
    {
        strncpy(
            g_GFMainPhase7State.commandLine,
            commandLine,
            sizeof(g_GFMainPhase7State.commandLine) - 1);
        g_GFMainPhase7State.commandLine[
            sizeof(g_GFMainPhase7State.commandLine) - 1] = '\0';
    }
    g_GFMainPhase7State.windowsShow = showCommand;
    const long phase7Result =
        FableRunGFMainPhase7(g_GFMainPhase7State);
    if (phase7Result != 0)
        return phase7Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE8)
    const long phase8Result = FableRunGFMainPhase8(
        g_GFMainPhase7State,
        g_GFMainPhase8State);
    if (phase8Result != 0)
        return phase8Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE9)
    const long phase9Result =
        FableRunGFMainPhase9(g_GFMainPhase9State);
    if (phase9Result != 0)
        return phase9Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE10)
    const long phase10Result = FableRunGFMainPhase10(
        g_GFMainPhase9State,
        g_GFMainPhase10State);
    if (phase10Result != 0)
        return phase10Result;
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#if defined(FABLETLC_ENABLE_VISUAL_BOOT)
#if !defined(FABLETLC_ENABLE_GFMAIN_PHASE10)
    FablePrepareGFInitialiseBoundary();
    if (!GFInitialise())
    {
        FableReleaseProgressDisplayBoundary();
        return 1;
    }
#else
    if (!g_GFMainPhase10State.observedGFInitialiseResult)
    {
        FableReleaseProgressDisplayBoundary();
        return 1;
    }
#endif

    CCountedProgressDisplay progressDisplay = GetProgressDisplay();
    if (progressDisplay.object != 0)
        progressDisplay.object->SetToDisplayText(false);
    FableSetVisualProgressDisplayState(
        progressDisplay.object != 0,
        progressDisplay.object != 0 &&
            progressDisplay.object->IsActive());

    const long visualResult = FableRunVisualBootCheckpoint(
        instance,
        commandLine,
        showCommand);
    FableReleaseProgressDisplayBoundary();
    return visualResult;
#else
    return 0;
#endif
}

