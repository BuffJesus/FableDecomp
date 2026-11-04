#pragma once

#include "fable_string.h"

CWideString FABLE_FASTCALL
FableGetMiscDirectoryA_0041A0A0();
CWideString FABLE_FASTCALL
FableGetMiscDirectoryB_0041A0C0();
CWideString FABLE_FASTCALL
FableGetGraphicsDirectory_0041A100();
CWideString FABLE_FASTCALL
FableGetLanguageDirectoryA_0041A120();
CWideString FABLE_FASTCALL
FableGetShadersDirectory_0041A1E0();
CWideString FABLE_FASTCALL
FableGetCacheDirectory_0041A180();
CWideString FABLE_FASTCALL
FableGetLanguageDirectoryB_0041A330();
CWideString FABLE_FASTCALL
FableGetMyDocumentsDirectory_0041A430();
CWideString FABLE_FASTCALL
FableGetAutoSaveCheckpointDirectory_0041A4C0();

namespace NResourceDirectoryNames
{
    CWideString FABLE_FASTCALL GetMyDocumentsDir();
    CWideString FABLE_FASTCALL GetUserDataDir();
    CWideString FABLE_FASTCALL GetSaveDirWithoutTrailingBlackslash();
    CWideString FABLE_FASTCALL GetSaveDir();
    // Ego_r directory_names.cpp, retail 0x0041A5B0 / 0x0041A600 / 0x0041A650 (each
    // GetUserDataDir() + a subdirectory, GetUserDataDir expanded).
    CWideString FABLE_FASTCALL GetScoreboardImageCacheDir();
    CWideString FABLE_FASTCALL GetPhotojournalDir();
    CWideString FABLE_FASTCALL GetTattooUserDir();
    // Ego_r ?GetBrushDir@...@@YI?AVCWideString@@XZ, retail 0x0041A0E0 (Data\Brushes\);
    // ?GetTattooDataDir@...@@YI?AVCWideString@@XZ, retail 0x0041A6A0 (Data\Tattoos\).
    CWideString FABLE_FASTCALL GetBrushDir();
    // Ego_r ?GetDebugSaveDir@...@@YI?AVCWideString@@XZ, retail 0x0041A1A0 (through the profile manager).
    CWideString FABLE_FASTCALL GetDebugSaveDir();
    CWideString FABLE_FASTCALL GetTattooDataDir();
    CWideString FABLE_FASTCALL GetSoundDir();
    // FableWin names matched by body (each returns its literal directory):
    CWideString FABLE_FASTCALL GetGraphicBankDir();   // 0x0041A100, Data/Graphics/
    CWideString FABLE_FASTCALL GetShaderBankDir();    // 0x0041A1E0, Data/Shaders/
    // FableWin names in CNewFrontendGameComponent::InitialiseEngine's call
    // order (retail 0x0042E204 calls 0x0041A100, 0x0041A1C0, 0x0041A1E0,
    // 0x0041A200, 0x0041A220 into the matching CEngineInit paths).
    CWideString FABLE_FASTCALL GetMeshBankDir();              // 0x0041A1C0
    CWideString FABLE_FASTCALL GetEngineCacheDir();           // 0x0041A200
    CWideString FABLE_FASTCALL GetWriteableEngineCacheDir();  // 0x0041A220
    // Ego_r's address order (Level, Def, Misc, HDMisc, ...) places GetMiscDir at
    // 0x0041A0A0 and GetHDMiscDir at 0x0041A0C0; both return Data/Misc/.
    CWideString FABLE_FASTCALL GetMiscDir();
    CWideString FABLE_FASTCALL GetHDMiscDir();
    // Ego_r directory_names.cpp, the rest in retail order: GetFontBankDir 0x0041A120,
    // GetCacheDir 0x0041A180, GetBoneConfigDir 0x0041A290, GetHDDialogueDir 0x0041A330,
    // GetDVDSoundDir 0x0041A390, GetHDSoundDir 0x0041A3B0, GetHDMusicDir 0x0041A3D0,
    // GetDVDMusicDir 0x0041A3F0 (GetSoundDir 0x0041A2B0 and GetStaticMapDir
    // 0x0041A410 are declared elsewhere in this namespace).
    CWideString FABLE_FASTCALL GetFontBankDir();
    CWideString FABLE_FASTCALL GetCacheDir();
    CWideString FABLE_FASTCALL GetBoneConfigDir();
    CWideString FABLE_FASTCALL GetHDDialogueDir();
    CWideString FABLE_FASTCALL GetDVDSoundDir();
    CWideString FABLE_FASTCALL GetHDSoundDir();
    CWideString FABLE_FASTCALL GetHDMusicDir();
    CWideString FABLE_FASTCALL GetDVDMusicDir();
    CWideString FABLE_FASTCALL GetDVDDialogueDir();   // 0x0041A2D0 (Data\lang\<language>\; 0x0041A330 is GetHDDialogueDir, same body)
    // 0x0041A060, Data\Levels\ (FableWin's CMainGameComponent::PostInit calls
    // GetLevelDir where retail's 0x00416953 calls 0x0041A060).
    CWideString FABLE_FASTCALL GetLevelDir();
    // FableWin ?GetDefDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ,
    // retail 0x0041A080 (Data\Defs\; CStartupDefinitionManager::
    // InitAndCompilePlatformSpecific 0x004336BC builds its def paths on it).
    CWideString FABLE_FASTCALL GetDefDir();
    // FableWin ?GetStaticMapDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ,
    // retail 0x0041A410 (returns "Data\Levels\" too; it follows the three
    // "Data\Sound\" getters 0x0041A3B0..0x0041A3F0, as in FableWin's order).
    // Callers: the CWorld ConvertWorldTo*Path helpers 0x0049D550 / 0x0049D6B0.
    CWideString FABLE_FASTCALL GetStaticMapDir();
}
