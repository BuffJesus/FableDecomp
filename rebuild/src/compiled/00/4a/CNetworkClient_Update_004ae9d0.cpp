// CNetworkClient::Update, retail 0x004AE9D0 (FableWin ?Update@CNetworkClient@@QAEXJKK@Z):
// a local game records the current server frame and the two world checksums.
#include "fable_game.h"

void CNetworkClient::Update(long serverFrame, unsigned long worldChecksum, unsigned long worldSeed)
{
    if (LocalGame)
    {
        LocalFrame = serverFrame;
        Checksum1 = worldChecksum;
        Checksum2 = worldSeed;
    }
}