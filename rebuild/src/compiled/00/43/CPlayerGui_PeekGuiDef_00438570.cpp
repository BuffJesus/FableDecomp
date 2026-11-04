#include "fable_player_gui.h"
#include "fable_definition_manager.h"
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef()
{
    if (PGuiDef.Get() == 0)
    {
        CCharString name("PLAYER_GUI_PC");
        CGameDefinitionManager::Get().GetDef<CPlayerGuiDef>(name,&PGuiDef);
    }
    return *PGuiDef.Get();
}
