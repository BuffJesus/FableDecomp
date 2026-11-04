#pragma optimize("s", on)
#include "fable_game.h"
#include "fable_definition_manager.h"

void CNewFrontendGameComponent::InitialiseDefs()
{
    CompileDefs();
    CCharString name("FRONT_END", -1);
    CStartupDefinitionManager::Get().GetDef<CFrontEndDef>(name, 1, &m_pFrontEndDef);
}
