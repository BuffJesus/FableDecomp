// CNetworkClient::Uninitialise, retail 0x004AE990 (FableWin ?Uninitialise@CNetworkClient@@QAEXXZ):
// nothing of its own to release; tail-calls the CInitBaseClass bookkeeping (0x0099A360).
#include "fable_game.h"

void CNetworkClient::Uninitialise()
{
    CInitBaseClass::Uninitialise();
}