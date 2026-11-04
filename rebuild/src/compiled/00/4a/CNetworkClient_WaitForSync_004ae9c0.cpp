// CNetworkClient::WaitForSync, retail 0x004AE9C0 (FableWin ?WaitForSync@CNetworkClient@@QAEXXZ):
// empty in retail (the network sync wait is compiled out; a lone `ret` between
// Uninitialise 0x004AE990 and Update 0x004AE9D0).
#include "fable_game.h"

void CNetworkClient::WaitForSync()
{
}