// CMainGameComponent::PostInit @ 0x00416953 (vtable slot 8)
// FableWin.pdb: ?PostInit@CMainGameComponent@@MAEXXZ.
// Hand the world its display engine, then either load the requested save or
// load the start-up world (the component's world, else the console-forced
// level, else updatedscenic.wld). A static-map build run copies the level's
// static map and ISM files to their retail paths and stops there; otherwise
// the world is activated, the initial quest started and the LUA script run.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_profile.h"
#include "fable_progress_display.h"
#include "fable_quest_manager.h"
#include "fable_resource_directories.h"
#include "fable_world.h"

// FableWin GForceLevel / GForceStartingHolySite (set by the SetLevel and
// SetStartingHolySite console commands 0x00413800 / 0x00413840).
extern CWideString g_FableForceLevel_013B8668;
extern CWideString g_FableForceStartingHolySite_013B866C;
// FableWin GSaveGameLaunchFailed.
extern bool g_FableSaveGameLaunchFailed_013B89C0;
// FableWin GUseRetailBanks (console variable UseRetailBanks).
extern bool GUseRetailBanks;
// FableWin GRunLUAScript.
extern CCharString g_FableRunLUAScript_013B8674;

void CMainGameComponent::PostInit()
{
    PWorld->PostInit(*PDisplayEngine.get());

    if (InitStructure.SaveGameName.GetLength() > 0)
    {
        NProgressDisplay::DisplayProgress("Loading save", -1.0f, false, false);
        if (!PWorld->LoadGameState(InitStructure.SaveGameName, true))
            g_FableSaveGameLaunchFailed_013B89C0 = true;
    }
    else
    {
        NProgressDisplay::DisplayProgress("Loading world", -1.0f, false, false);

        CWideString worldName;
        if (InitStructure.InitialWorldName.GetLength() > 0)
        {
            worldName = NResourceDirectoryNames::GetLevelDir() + InitStructure.InitialWorldName;
            g_FableForceLevel_013B8668 = InitStructure.InitialWorldName;
            if (InitStructure.InitialWorldHolySiteName.GetLength() > 0)
                g_FableForceStartingHolySite_013B866C = InitStructure.InitialWorldHolySiteName;
        }
        else if (g_FableForceLevel_013B8668.GetLength() > 0)
        {
            worldName = NResourceDirectoryNames::GetLevelDir() + g_FableForceLevel_013B8668;
        }
        else
        {
            worldName = NResourceDirectoryNames::GetLevelDir() + L"updatedscenic.wld";
        }

        PWorld->Load(worldName);

        if (g_FableCompileFrontendDefinitions_013B8648)
        {
            const CWideString staticMapSourcePath = PWorld->ConvertWorldToStaticMapPath(worldName);
            CDiskFileWin32 staticMapSource(staticMapSourcePath, FILE_ACCESS_READ, 2);
            const CWideString staticMapTargetPath = PWorld->ConvertWorldToStaticMapPath(
                worldName, true, GUseRetailBanks);
            CDiskFileWin32 staticMapTarget(staticMapTargetPath, FILE_ACCESS_CREATE, 2);
            CAFile::CopyFileW(staticMapSource, staticMapTarget);

            const CWideString ismSourcePath = PWorld->ConvertWorldToISMThingyPath(worldName);
            CDiskFileWin32 ismSource(ismSourcePath, FILE_ACCESS_READ, 2);
            const CWideString ismTargetPath = PWorld->ConvertWorldToISMThingyPath(
                worldName, true, GUseRetailBanks);
            CDiskFileWin32 ismTarget(ismTargetPath, FILE_ACCESS_CREATE, 2);
            CAFile::CopyFileW(ismSource, ismTarget);
            return;
        }

        PWorld->ActivateWorld(false);

        NProfileTimer::StartProfile("Activate Initial Quests", 0);
        if (InitStructure.InitialQuestName != "")
            CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateQuest(
                InitStructure.InitialQuestName, false, true);
        NProfileTimer::EndProfile(0);
    }

    InitLUA(g_FableRunLUAScript_013B8674);
}
