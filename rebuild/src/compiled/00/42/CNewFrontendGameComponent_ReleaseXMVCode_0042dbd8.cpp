#pragma optimize("s", on)
// CNewFrontendGameComponent::ReleaseXMVCode @ 0x0042DBD8 (22 bytes).
// FableWin ?ReleaseXMVCode@CNewFrontendGameComponent@@AAEXXZ: if the XMV
// movie code was loaded, release it (CXMVPlayer::StaticReleaseXMVCode
// 0x009BCFC0) and clear the flag (+0xB1).
#include "fable_game.h"
#include "fable_movie.h"

void CNewFrontendGameComponent::ReleaseXMVCode()
{
    if (XMVCodeLoaded)
    {
        CXMVPlayer::StaticReleaseXMVCode();
        XMVCodeLoaded = false;
    }
}
