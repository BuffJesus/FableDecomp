// InitialiseConsoleVariables (retail 0x413c50): registers the start-up console commands and
// variables with the console singleton, then runs default_userst.ini (when present) and,
// if GRunUserScript is set, userst.ini. FableWin does the same registrations inside
// GFRunInitScripts (fablelib/command_line.cpp), naming the globals G<ConsoleName>.
//
// Every name is passed as a literal: the CCharString/CWideString temporaries come from implicit
// conversions, which is what makes VC7.1 re-take the temporary's address instead of reusing the
// constructor's return value. CTBaseSingleton<CConsole>::Get is inlined until the compiler's
// per-function inline budget runs out, so the two RunScript calls reach the out-of-line copy
// (retail 0x414c90).

#include "fable_console.h"
#include "fable_filesystem.h"
#include "fable_string.h"

void FABLE_FASTCALL GConsoleSetLevel(const CGenericVarVector& params);                 // 0x413800
void FABLE_FASTCALL GConsoleSetStartingHolySite(const CGenericVarVector& params);      // 0x413840
void FABLE_FASTCALL GConsoleEnableCodeSectionLoading(const CGenericVarVector& params); // 0x413880

extern bool g_FableUsePhysicalDVD_013B85F2;
extern bool g_FableUseRetailSaveGameSystem_013B8646;
extern bool g_FableRunFromDVD_013B8615;
extern bool GUseRetailBanks;
extern bool g_FableUseCompiledAnimationEvents_013B860A;
extern bool g_FableUseCompiledGlobalThings_013B8609;
extern bool g_FableUseCompiledDefs_013B8617;
extern bool g_FableUseCompiledWorldFiles_013B8618;
extern bool g_FableUseCompiledSoundSymbols_013B8619;
extern bool g_FableAllowDataGeneration_01375459;
extern long g_FableInstallerBufferSize_01375494;
extern long g_FableNoInstallBuffers_01375498;
extern CEngineMemoryInit g_FableEngineMemoryInit_0137549C;
// FableWin's GRunUserScript (the retail layout of this bool block differs slightly).
extern bool g_FableRunUserScript_01375444;

void FABLE_FASTCALL InitialiseConsoleVariables()
{
    CTBaseSingleton<CConsole>::Get().Initialise('a', KB_A, 0);

    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleClasslessCommand("SetLevel", &GConsoleSetLevel, GENERIC_VAR_STRING));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleClasslessCommand("SetStartingHolySite", &GConsoleSetStartingHolySite, GENERIC_VAR_STRING));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleClasslessCommand("EnableCodeSectionLoading", &GConsoleEnableCodeSectionLoading, GENERIC_VAR_BOOL));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UsePhysicalDVD", g_FableUsePhysicalDVD_013B85F2));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseRetailSaveGameSystem", g_FableUseRetailSaveGameSystem_013B8646));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("RunFromDVD", g_FableRunFromDVD_013B8615));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseRetailBanks", GUseRetailBanks));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseCompiledAnimationEvents", g_FableUseCompiledAnimationEvents_013B860A));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseCompiledGlobalThings", g_FableUseCompiledGlobalThings_013B8609));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseCompiledDefs", g_FableUseCompiledDefs_013B8617));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseCompiledWorldFiles", g_FableUseCompiledWorldFiles_013B8618));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("UseCompiledSoundSymbols", g_FableUseCompiledSoundSymbols_013B8619));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("AllowDataGeneration", g_FableAllowDataGeneration_01375459));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("InstallerBufferSize", g_FableInstallerBufferSize_01375494));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("NoInstallBuffers", g_FableNoInstallBuffers_01375498));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("LandscapePoolSize", g_FableEngineMemoryInit_0137549C.LandscapePoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("LandscapePhysicalMemoryRatio", g_FableEngineMemoryInit_0137549C.LandscapePhysicalMemoryRatio));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("LandscapeVirtualMemoryMinSize", g_FableEngineMemoryInit_0137549C.LandscapeVirtualMemoryMinSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("MeshPoolSize", g_FableEngineMemoryInit_0137549C.MeshPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("MeshStatsPoolSize", g_FableEngineMemoryInit_0137549C.MeshStatsPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("MeshPhysicalMemoryRatio", g_FableEngineMemoryInit_0137549C.MeshPhysicalMemoryRatio));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("HiresTextureMemory", g_FableEngineMemoryInit_0137549C.HiresTextureMemory));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("PhysicsMeshPoolSize", g_FableEngineMemoryInit_0137549C.PhysicsMeshPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("AnimationPoolSize", g_FableEngineMemoryInit_0137549C.AnimationPoolSize));
    CTBaseSingleton<CConsole>::Get().AddInput(
        new CConsoleVariable("ClothPoolSize", g_FableEngineMemoryInit_0137549C.ClothPoolSize));

    if (CAFile::PathExists(L"default_userst.ini"))
        CTBaseSingleton<CConsole>::Get().RunScript(L"default_userst.ini");
    if (g_FableRunUserScript_01375444)
        CTBaseSingleton<CConsole>::Get().RunScript(L"userst.ini");
}
