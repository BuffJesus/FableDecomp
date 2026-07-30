#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

class CWideString;
class CSystemManagerInit;

// Phase 1 is an authored integration unit spanning retail GFMain
// 0x00402510-0x004025A6. It is not claimed as a standalone retail function.
long FABLE_FASTCALL FableRunGFMainPhase1(char* commandLine);

struct GFMainPhase2State
{
    bool sharedSystemSetting;
    bool mirroredSystemSettingA;
    bool mirroredSystemSettingB;
    bool mirroredSystemSettingC;

    bool fileSystemSetting;
    bool mirroredFileSystemSetting;

    bool applyBasicInstallFlag;
    bool basicInstallAlreadyConfigured;
    bool basicInstallFlag;

    bool clearStartupLatch;
    bool enableAsyncFailureHandling;
};

// Phase 2 is an authored integration unit spanning retail GFMain
// 0x004025A6-0x00402668. Its direct callees are retail-matched; this wrapper
// gives the sequence a readable state object while the enclosing GFMain body
// and its inline ownership cleanup remain under reconstruction.
long FABLE_FASTCALL FableRunGFMainPhase2(GFMainPhase2State& state);

struct GFMainPhase3State
{
    // Injectable filesystem/parser results for the authored integration
    // boundary. Retail obtains these from PathExists, CDiskFileWin32,
    // CStringParser, and CPersistContext.
    bool settingsFileExists;
    bool settingsFileOpened;

    bool hasLeftAlignText;
    fable_i8 persistedLeftAlignText;
    bool hasNoHangulWordWrap;
    fable_i8 persistedNoHangulWordWrap;
    bool hasDisableCapsLock;
    fable_i8 persistedDisableCapsLock;

    // Observable boundary activity. These counters keep the optional-file
    // branch and its ownership sequence behavior-testable while the complete
    // disk/parser object graph remains outside the startup executable.
    fable_u32 pathExistsCalls;
    fable_u32 diskFileConstructCalls;
    fable_u32 loadFromFileCalls;
    fable_u32 stringParserConstructCalls;
    fable_u32 persistContextConstructCalls;
    fable_u32 transferCalls;
    fable_u32 loadImeSettingsCalls;
    fable_u32 sortTreeCalls;
    fable_u32 diskFileCloseCalls;

    wchar_t resolvedSettingsPath[64];
};

// Phase 3 is an authored integration unit spanning retail GFMain
// 0x00402668-0x0040284E. The recovered narrow/wide string and profile leaves
// execute directly. Disk/parser ownership and the three still-unrecovered
// helpers remain behind explicit boundaries represented by the state above.
long FABLE_FASTCALL FableRunGFMainPhase3(GFMainPhase3State& state);

struct GFMainPhase4Alias
{
    char platformName[32];
    char genericName[32];
};

struct GFMainPhase4State
{
    fable_u32 setNavigatorCalls;
    fable_u32 addAliasCalls;
    bool navigatorEnabled;
    GFMainPhase4Alias aliases[5];
};

// Phase 4 is an authored integration unit spanning retail GFMain
// 0x0040284E-0x004029DC. Its navigator setter, alias wrapper, string
// assignment, profile, and string lifetime leaves are retail-matched.
long FABLE_FASTCALL FableRunGFMainPhase4(GFMainPhase4State& state);

struct GFMainPhase5BankOpen
{
    wchar_t pathname[96];
    bool startupPriority;
};

struct GFMainPhase5State
{
    fable_u32 openRetailBankCalls;
    GFMainPhase5BankOpen retailBanks[9];

    fable_u32 openIniFileCalls;
    wchar_t iniPathname[32];

    fable_u32 setHeaderDirectoryCalls;
    wchar_t headerDirectory[64];
    wchar_t compiledDefinitionsDirectory[64];
};

// Phase 5 is an authored integration unit spanning retail GFMain
// 0x004029DC-0x00402CE6. Exact directory factories and recovered string
// leaves construct the retail paths; the large bank manager bodies remain
// explicit observable boundaries.
long FABLE_FASTCALL FableRunGFMainPhase5(GFMainPhase5State& state);

enum GFMainPhase6PathKind
{
    GFMainPhase6Path_Checkpoint,
    GFMainPhase6Path_Save,
    GFMainPhase6Path_MyDocuments
};

enum GFMainPhase6TextRole
{
    GFMainPhase6Text_PermissionError,
    GFMainPhase6Text_ErrorCaption
};

struct GFMainPhase6State
{
    // Injectable results owned by the unrecovered filesystem, definition,
    // compressed-stream, and text-bank boundaries.
    bool definitionTableEnabled;
    bool definitionTableLoadDisabled;
    bool checkpointOpenSucceeds;
    bool saveOpenSucceeds;
    fable_u32 checkpointPermissionResult;
    fable_u32 savePermissionResult;
    fable_u32 myDocumentsPermissionResult;
    fable_u32 permissionTextLookupResult;
    fable_u32 captionTextLookupResult;
    bool permissionGroupAvailable;
    bool captionGroupAvailable;
    wchar_t myDocumentsDirectory[64];

    // Observable retail-order activity.
    fable_u32 setDefinitionTablePathCalls;
    fable_u32 loadDefinitionTableCalls;
    bool definitionTableLoadAllowed;
    wchar_t definitionTablePath[96];
    fable_u32 exactDefinitionTableSetPathCalls;
    fable_u32 exactDefinitionTableLoadCalls;
    fable_u32 exactDefinitionTablePathExistsCalls;
    fable_u32 exactDefinitionTableOpenCalls;
    fable_u32 exactDefinitionTableStreamConstructCalls;
    fable_u32 exactDefinitionTableReadCalls;
    fable_u32 exactDefinitionTableStreamDestroyCalls;
    fable_u32 exactDefinitionTableCloseCalls;
    fable_u32 exactDefinitionTableStringDestroyCalls;
    fable_u32 exactDefinitionTableFileDestroyCalls;
    bool exactDefinitionTablePathAssigned;
    bool exactDefinitionTableArgumentsValid;
    bool exactDefinitionTableLoadedBeforeCleanup;
    bool exactDefinitionTableLoaded;
    bool exactDefinitionTableReadOnly;

    fable_u32 openDirectoryCalls[3];
    fable_u32 closeDirectoryCalls[3];
    fable_u32 permissionCheckCalls[3];
    wchar_t checkpointDirectory[96];
    wchar_t saveDirectory[96];
    wchar_t permissionPath[96];
    fable_u32 exactPermissionProbeCalls;
    fable_u32 exactPermissionCreateCalls;
    fable_u32 exactPermissionCloseHandleCalls;
    fable_u32 exactPermissionOpenCalls;
    fable_u32 exactPermissionCloseFileCalls;
    fable_u32 exactPermissionDeleteCalls;
    fable_u32 exactPermissionWideDestroyCalls;
    fable_u32 exactPermissionCharDestroyCalls;
    fable_u32 exactPermissionResult;
    bool exactPermissionArgumentsValid;

    fable_u32 compressedStreamConstructCalls;
    fable_u32 countedPointerResetCalls;
    fable_u32 openFontBankCalls;
    fable_u32 textLookupCalls[2];
    fable_u32 groupEntryCalls[2];
    fable_u32 bankHandleCalls[2];
    fable_u32 displayPermissionErrorCalls;
    fable_u32 displayedPermissionError;
};

// Phase 6 is an authored integration unit spanning retail
// 0x00402CE6-0x00403082. Exact string, CRC, bank-handle, and map leaves are
// proven separately; the eleven ownership/OS calls remain explicit below.
long FABLE_FASTCALL FableRunGFMainPhase6(
    const GFMainPhase5State& phase5,
    GFMainPhase6State& state);

struct GFMainPhase7State
{
    // Injectable command-line and startup-option globals consumed by retail's
    // CSystemManagerInit population block.
    fable_u32 instanceHandleToken;
    char commandLine[96];
    fable_i32 windowsShow;
    bool preferPrimaryDisplayDevice;
    bool useAlternateDisplayDefaults;
    fable_u32 regionWords[4];
    bool regionFlag18;
    fable_u32 regionWord20;
    fable_u32 regionWord24;
    bool regionFlag28;
    bool overrideRegionMode;
    bool alternateRegionMode;
    bool exclusiveMode;
    bool disableInactiveWait;
    bool skipConfigDetection;
    bool staticMapMode;
    bool staticMapWorkEnabled;
    fable_i32 language;
    wchar_t windowTitleSource[64];

    // Observable CSystemManagerInit values written by 0x0040309B-0x0040329A.
    fable_u32 configuredInstanceHandleToken;
    char configuredCommandLine[96];
    fable_i32 configuredWindowsShow;
    bool configuredPreferPrimaryDisplayDevice;
    fable_u32 startupFlags;
    float displayDepth;
    float nearPlane;
    float displayScale;
    fable_u32 displayUnknown4C;
    fable_u32 memoryBudget;
    fable_u32 configuredRegionWords[4];
    bool configuredRegionFlag10;
    bool configuredRegionFlag18;
    fable_u32 configuredRegionWord20;
    fable_u32 configuredRegionWord24;
    bool configuredRegionFlag28;
    fable_i32 configuredRegionMode;
    bool configuredRegionFlag19;
    bool configuredUnknown58;
    bool configuredUnknown59;
    bool configuredUnknown5A;
    bool errorCallbackInstalled;
    bool configuredExclusiveMode;
    bool configuredWaitWhileInactive;
    bool configuredSkipConfigDetection;
    bool skipPhase8;
    fable_u32 windowTitleCalls;
    fable_u32 exactWindowTitleCoordinatorCalls;
    fable_u32 exactWindowTitleBankOpenCalls;
    fable_u32 exactWindowTitleLookupCalls;
    fable_u32 exactWindowTitleBankDeleteCalls;
    wchar_t shaderResourcePath[64];
    wchar_t windowTitle[64];
    wchar_t cacheDirectory[64];
    char streamingFontBank[48];
};

// Phase 7 is an authored integration unit spanning retail
// 0x00403082-0x0040329C. The recovered CSystemManagerInit offsets and exact
// cache/shader/string/font leaves execute directly. Complete runtimes also
// execute the exact GetWindowTitle text-bank ownership coordinator.
long FABLE_FASTCALL FableRunGFMainPhase7(
    GFMainPhase7State& state);

struct GFMainPhase8State
{
    bool eulaSucceeds;
    bool configurationSucceeds;
    fable_u32 eulaCalls;
    fable_u32 configurationCalls;
    fable_u32 freeConfigurationCalls;
    fable_u32 processExitCalls;
    fable_i32 requestedExitCode;
    fable_u32 exactEulaExportCalls;
    fable_u32 exactEulaFreeCalls;
    fable_u32 exactConfigurationCreateCalls;
    fable_u32 exactConfigurationVideoCalls;
    fable_u32 exactConfigurationRequirementsCalls;
    fable_u32 exactConfigurationReleaseCalls;
    fable_u32 exactConfigurationFreeCalls;
    fable_u32 exactConfigurationSecurityChecks;
};

// Phase 8 is an authored integration unit spanning retail
// 0x0040329C-0x004032D5. Complete runtimes execute exact EULA and hardware
// detection/release coordinators. Process termination remains injectable so
// both retail exit branches are safe to test.
long FABLE_FASTCALL FableRunGFMainPhase8(
    const GFMainPhase7State& phase7,
    GFMainPhase8State& state);

struct GFMainPhase9State
{
    // Retail globals read at 0x004032D5-0x00403307. Source B is inverted by
    // the original branch: its clear state enables bit 1.
    bool engineFlagSourceA;
    bool engineFlagSourceB;
    bool forceMovieLeftAlign;
    fable_u32 initialEngineFlags;
    fable_i32 systemInitialiseResult;
    fable_u32 systemWindowHandleToken;
    bool processSaveFileMetadata;

    // Observable engine-root assembly and initialization sequence.
    fable_u32 configuredEngineFlags;
    bool movieLeftAlignApplied;
    fable_u32 systemManagerToken;
    fable_u32 secondSystemManagerToken;
    fable_u32 systemManagerLookupCalls;
    fable_u32 systemInitialiseCalls;
    fable_i32 observedSystemInitialiseResult;
    fable_u32 exactSystemPreInitialiseCalls;
    fable_u32 exactSystemColoursConstructCalls;
    fable_u32 exactSystemAllocationCalls;
    fable_u32 exactSystemWindowInitialiseCalls;
    fable_u32 exactSystemDisplayConstructCalls;
    fable_u32 exactSystemInputConstructCalls;
    fable_u32 exactSystemDriveInitialiseCalls;
    fable_u32 exactSystemTextConstructCalls;
    fable_u32 exactSystemMeshConstructCalls;
    fable_u32 exactSystemComponentAddCalls;
    fable_u8 observedExactSystemInitialiseCount;
    fable_u32 imePrimitiveToken;
    fable_u32 imeInitialiseCalls;
    fable_u32 observedImeWindowHandleToken;
    fable_u32 exactImeUiInitialiseCalls;
    fable_u8 observedImeInitialiseMode;
    fable_u32 processSaveMetadataCalls;
    fable_u32 exactSaveMetadataCoordinatorCalls;
    fable_u32 exactSaveMetadataFindFirstCalls;
    fable_u32 exactSaveMetadataSecurityChecks;
};

// Phase 9 is an authored integration unit spanning retail
// 0x004032D5-0x00403389. Exact singleton, movie-layout, profile, and string
// leaves execute directly. Complete runtimes also execute the exact system
// manager, CIME, and optional save-metadata coordinators.
long FABLE_FASTCALL FableRunGFMainPhase9(
    GFMainPhase9State& state);

struct GFMainPhase10State
{
    bool gfInitialiseSucceeds;
    bool startupMessagePresent;

    fable_u32 gfInitialiseCalls;
    bool observedGFInitialiseResult;
    fable_u32 startupMessageCheckCalls;
    fable_u32 getRenderManagerCalls;
    fable_u32 addStartupLineCalls;
    fable_u32 endProfileCalls;
    fable_u32 playCalls;
    fable_u32 gfUninitialiseCalls;
    fable_u32 systemInitErrorCalls;
    fable_i32 observedSystemInitError;
    fable_u32 registryConstructCalls;
    fable_u32 registryWriteCalls;
    bool registryWrittenValue;
    fable_u32 registryDestroyCalls;
    fable_u32 wideStringCleanupCalls;
    fable_u32 charStringCleanupCalls;
};

// Phase 10 is the final authored integration unit for retail GFMain,
// spanning 0x00403389-0x00403480. The exact GFInitialise/profile leaves are
// linked independently; launch, error, registry, and cross-phase ownership
// operations remain observable boundaries.
long FABLE_FASTCALL FableRunGFMainPhase10(
    const GFMainPhase9State& phase9,
    GFMainPhase10State& state);

// Authored parent seam that sequences all ten recovered phase units, forwards
// WinMain inputs into Phase 7, and preserves the retail early-return gates.
long FABLE_FASTCALL FableRunGFMainComplete(
    void* instance,
    char* commandLine,
    int showCommand);

// Exact relocatable retail registrar plus the authored Stage 2 execution
// boundary used while its console object model remains isolated.
void FABLE_FASTCALL InitialiseConsoleVariables();
void FABLE_FASTCALL FableInitialiseConsoleVariablesBoundary();

extern fable_u32 g_FableConsoleVariablesBoundaryCalls;
extern GFMainPhase2State g_GFMainPhase2State;
extern GFMainPhase3State g_GFMainPhase3State;
extern GFMainPhase4State g_GFMainPhase4State;
extern GFMainPhase5State g_GFMainPhase5State;
extern GFMainPhase6State g_GFMainPhase6State;
extern GFMainPhase7State g_GFMainPhase7State;
extern GFMainPhase8State g_GFMainPhase8State;
extern GFMainPhase9State g_GFMainPhase9State;
extern GFMainPhase10State g_GFMainPhase10State;
extern fable_i32 g_FablePhase7CurrentLanguage;

// Phase 3 persistence values retain their retail static addresses until the
// owning settings type is recovered.
extern fable_i8 g_FableLeftAlignText_013B861B;
extern fable_i8 g_FableNoHangulWordWrap_013B861C;
extern fable_i8 g_FableDisableCapsLock_013B864F;
extern fable_u8 g_FableNavigatorEnabled_013B8616;
extern fable_u8 g_FableOpenAllRetailBanks_013B8605;
extern fable_u8 g_FableUseDvdBankList_013B8615;

bool FABLE_FASTCALL FableGFMainPhase3PathExistsBoundary(
    const wchar_t* pathname,
    GFMainPhase3State& state);
void FABLE_FASTCALL FableGFMainPhase3LoadSettingsBoundary(
    const wchar_t* pathname,
    GFMainPhase3State& state);
void FABLE_FASTCALL FableGFMainPhase5OpenRetailBankBoundary(
    const wchar_t* pathname,
    bool startupPriority,
    GFMainPhase5State& state);
void FABLE_FASTCALL FableGFMainPhase5OpenIniFileBoundary(
    const wchar_t* pathname,
    GFMainPhase5State& state);
void FABLE_FASTCALL FableGFMainPhase5SetHeaderDirectoryBoundary(
    const wchar_t* pathname,
    GFMainPhase5State& state);
void FABLE_FASTCALL FableGFMainPhase6SetDefinitionTablePathBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6LoadDefinitionTableBoundary(
    bool loadAllowed,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableRunExactDefinitionTableSetPathBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableRunExactDefinitionTableLoadBoundary(
    bool readOnly,
    GFMainPhase6State& state);
fable_u32 FABLE_FASTCALL FableRunExactWritePermissionsBoundary(
    const wchar_t* pathname,
    GFMainPhase6State& state);
bool FABLE_FASTCALL FableGFMainPhase6OpenDirectoryBoundary(
    GFMainPhase6PathKind kind,
    const wchar_t* pathname,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6CloseDirectoryBoundary(
    GFMainPhase6PathKind kind,
    GFMainPhase6State& state);
fable_u32 FABLE_FASTCALL FableGFMainPhase6CheckPermissionsBoundary(
    GFMainPhase6PathKind kind,
    const wchar_t* pathname,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6ConstructCompressedStreamBoundary(
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6ResetCountedPointerBoundary(
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6OpenFontBankBoundary(
    GFMainPhase6State& state);
fable_u32 FABLE_FASTCALL FableGFMainPhase6LookupTextBoundary(
    GFMainPhase6TextRole role,
    GFMainPhase6State& state);
bool FABLE_FASTCALL FableGFMainPhase6GetGroupEntryBoundary(
    GFMainPhase6TextRole role,
    fable_u32 identifier,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6GetBankHandleBoundary(
    GFMainPhase6TextRole role,
    GFMainPhase6State& state);
void FABLE_FASTCALL FableGFMainPhase6DisplayPermissionErrorBoundary(
    fable_u32 errorCode,
    GFMainPhase6State& state);
CWideString FABLE_FASTCALL FableGFMainPhase7GetWindowTitleBoundary(
    GFMainPhase7State& state);
CWideString FABLE_FASTCALL FableRunExactWindowTitleBoundary(
    GFMainPhase7State& state);
bool FABLE_FASTCALL FableGFMainPhase8DoEULABoundary(
    GFMainPhase8State& state);
bool FABLE_FASTCALL FableGFMainPhase8DetectConfigurationBoundary(
    GFMainPhase8State& state);
void FABLE_FASTCALL FableGFMainPhase8FreeConfigurationBoundary(
    GFMainPhase8State& state);
void FABLE_FASTCALL FableGFMainPhase8ProcessExitBoundary(
    fable_i32 exitCode,
    GFMainPhase8State& state);
bool FABLE_FASTCALL FableRunExactPhase8EulaBoundary(
    GFMainPhase8State& state);
bool FABLE_FASTCALL FableRunExactPhase8ConfigurationBoundary(
    GFMainPhase8State& state);
void FABLE_FASTCALL FableRunExactPhase8FreeConfigurationBoundary(
    GFMainPhase8State& state);
fable_i32 FABLE_FASTCALL FableGFMainPhase9InitialiseSystemManagerBoundary(
    void* systemManager,
    fable_u32 engineFlags,
    GFMainPhase9State& state);
void* FABLE_FASTCALL FableGFMainPhase9GetSystemManagerBoundary(
    GFMainPhase9State& state);
void FABLE_FASTCALL FableGFMainPhase9InitialiseImeBoundary(
    void* imePrimitive,
    void* windowHandle,
    GFMainPhase9State& state);
void FABLE_FASTCALL FableGFMainPhase9ProcessSaveMetadataBoundary(
    GFMainPhase9State& state);
fable_i32 FABLE_FASTCALL FableRunExactSystemManagerInitialiseBoundary(
    CSystemManagerInit* init,
    GFMainPhase9State& state);
void FABLE_FASTCALL FableRunExactSaveMetadataBoundary(
    GFMainPhase9State& state);
bool FABLE_FASTCALL FableGFMainPhase10GFInitialiseBoundary(
    GFMainPhase10State& state);
bool FABLE_FASTCALL FableGFMainPhase10HasStartupMessageBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10GetRenderManagerBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10AddStartupLineBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10PlayBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10GFUninitialiseBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10HandleSystemInitErrorBoundary(
    fable_i32 errorCode,
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10ConstructRegistryBoundary(
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10WriteRegistryBoolBoundary(
    bool value,
    GFMainPhase10State& state);
void FABLE_FASTCALL FableGFMainPhase10DestroyRegistryBoundary(
    GFMainPhase10State& state);

// Observable counters belong to the Stage 3 engine boundary, not retail.
// They let the integration fixture prove the counted singleton's lifetime.
extern fable_u32 g_FableFileInstallerConstructCalls;
extern fable_u32 g_FableFileInstallerDestroyCalls;
extern fable_u8 g_FableLastBasicInstallStartupFlag;
