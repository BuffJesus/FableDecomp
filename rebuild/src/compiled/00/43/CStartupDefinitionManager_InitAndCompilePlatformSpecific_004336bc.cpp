// CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool) @ 0x004336BC
// (FableWin ?InitAndCompilePlatformSpecific@CStartupDefinitionManager@@QAEX_N@Z;
// CNewFrontendGameComponent::CompileDefsPlatformSpecific 0x0042F5A9 calls it):
// the front end's symbol sources (bank headers, then def files and headers)
// and its def sources, the five startup def classes, then Compile. The PC
// build adds the front-end, controller and keyboard files; the other one
// only frontend_test.def. Size-optimised unit (/Os: the ebp frame).
#pragma optimize("s", on)
#include "fable_definition_manager.h"
#include "fable_bank_file.h"
#include "fable_resource_directories.h"

void CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool xbox)
{
    _STL::vector<CWideString> symbolPaths;
    _STL::vector<CWideString> compilePaths;
    CWideString platform(L"pc\\");

    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + platform + L"textures.h");
    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + L"fonts.h");
    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + platform + L"front_end_bank.h");
    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + platform + L"particles_frontend.h");
    symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\engine.def");
    symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\engine_video_options.def");
    symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\front_end.def");
    symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"ui_types.h");
    symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"ui_dialogs.def");
    if (xbox)
    {
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\frontend_test.def");
    }
    else
    {
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\pc_frontend.def");
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"config_options_defaults.def");
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"controller_def.h");
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"keyboard_keys.h");
        symbolPaths.push_back(NResourceDirectoryNames::GetDefDir() + L"pc_controls.def");
    }

    compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\engine.def");
    compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\engine_video_options.def");
    compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\front_end.def");
    compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"ui_dialogs.def");
    if (xbox)
    {
        compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\frontend_test.def");
    }
    else
    {
        compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"FrontEndDefs\\pc_frontend.def");
        compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"pc_controls.def");
        compilePaths.push_back(NResourceDirectoryNames::GetDefDir() + L"config_options_defaults.def");
    }

    {
        CCharString name("ENGINE_VIDEO_OPTIONS", -1);
        AddDefClass(CDefClassInit(name, CEngineVideoOptionsDef::Alloc));
    }
    {
        CCharString name("ENGINE", -1);
        AddDefClass(CDefClassInit(name, CEngineDef::Alloc));
    }
    {
        CCharString name("UI", -1);
        AddDefClass(CDefClassInit(name, NUISystem::CUIDef::Alloc));
    }
    {
        CCharString name("CONTROL_SCHEME", -1);
        AddDefClass(CDefClassInit(name, CControlsDef::Alloc));
    }
    {
        CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF", -1);
        AddDefClass(CDefClassInit(name, CConfigOptionsDefaultsDef::Alloc));
    }

    SetSymbolPathList(symbolPaths);
    SetCompilePathList(compilePaths);
    Compile();
}