#include "fable_player_gui.h"
NHeroInformationScreens::CManager* CPlayerGui::GetPHeroInfoScreensManager() const
{
    return PHeroInfoScreensManager.get();
}
