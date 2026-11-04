#include <stdio.h>
#include <string.h>

#include "fable_game.h"
#include "fable_movie.h"

// Behaviour fixture for CNewFrontendGameComponent::ReleaseXMVCode @ 0x0042DBD8.
// Retail offsets: XMVCodeLoaded +0xB1 (AVIPlaying +0xB0 must stay untouched).

static int g_releaseCalls;
void FABLE_FASTCALL CXMVPlayer::StaticReleaseXMVCode()
{
    ++g_releaseCalls;
}

static unsigned char g_component[0x148];

static bool RunCase(const char* name, unsigned char loaded, int expectCalls)
{
    g_releaseCalls = 0;
    memset(g_component, 0x5A, sizeof(g_component));
    g_component[0xB1] = loaded;
    reinterpret_cast<CNewFrontendGameComponent*>(g_component)->ReleaseXMVCode();
    bool ok = g_releaseCalls == expectCalls;
    ok = ok && g_component[0xB1] == (expectCalls ? 0 : loaded);
    for (int i = 0; i < 0x148; ++i)
        if (i != 0xB1 && g_component[i] != 0x5A) { ok = false; break; }
    if (!ok) printf("FAIL %s calls=%d flag=%d\n", name, g_releaseCalls, g_component[0xB1]);
    return ok;
}

int main()
{
    bool ok = true;
    ok &= RunCase("loaded", 1, 1);
    ok &= RunCase("not-loaded", 0, 0);
    if (ok) printf("PASS_0042dbd8\n");
    return ok ? 0 : 1;
}
