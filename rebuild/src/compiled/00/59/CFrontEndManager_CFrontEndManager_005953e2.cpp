#pragma optimize("s", on)
#include "fable_nui_manager.h"
namespace NUISystem {
CFrontEndManager::CFrontEndManager()
 : UIActionParams(_STL::allocator<CActionParam*>()),
   m_pVirtualKeyboard(0),m_bStartNewGame(false),
   PScreenToDeactivate(0),PScreenToActivate(0),GoToKeyboard(false),
   PScreenToDeactivateForStartGame(0),PScreenToDeactivateForLoadGame(0),
   m_bQuitFromGame(false),InstallationPaused(false),FinalInstallationNeverPause(false) {}
}
