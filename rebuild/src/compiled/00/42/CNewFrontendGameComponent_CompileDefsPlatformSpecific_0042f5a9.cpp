#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_definition_manager.h"

// Build the startup definition manager over (x)frontend.bin, register the
// front-end def classes (the control scheme only on PC) and compile them.
void FABLE_FASTCALL CNewFrontendGameComponent::CompileDefsPlatformSpecific(bool xbox)
{
    CWideString filename;
    if (xbox)
        filename = L"xfrontend.bin";
    else
        filename = L"frontend.bin";
    CStartupDefinitionManager::Set(new CStartupDefinitionManager(filename));
    CStartupDefinitionManager& manager = CStartupDefinitionManager::Get();
    {
        CCharString name("FRONT_END", -1);
        manager.AddDefClass(CDefClassInit(name, CFrontEndDef::Alloc));
    }
    {
        CCharString name("UI_MISC_THINGS_DEF", -1);
        manager.AddDefClass(CDefClassInit(name, NUISystem::CUIMiscThingsDef::Alloc));
    }
    {
        CCharString name("UI_ICONS_DEF", -1);
        manager.AddDefClass(CDefClassInit(name, NUISystem::CUIIconsDef::Alloc));
    }
    if (!xbox)
    {
        CCharString name("CONTROL_SCHEME", -1);
        manager.AddDefClass(CDefClassInit(name, CControlsDef::Alloc));
    }
    manager.InitAndCompilePlatformSpecific(xbox);
}