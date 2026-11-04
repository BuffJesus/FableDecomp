#include <stdio.h>
#include "fable_game.h"

static int g_calls;
static const void* g_receiver;

void CInitBaseClass::Uninitialise()
{
    ++g_calls;
    g_receiver = this;
}

int main()
{
    static unsigned char storage[sizeof(CGame)];
    CGame* game = reinterpret_cast<CGame*>(storage);
    game->Uninitialise();
    // CInitBaseClass is CGame's first base: the forward keeps the same this.
    if (g_calls != 1 || g_receiver != storage) return 1;
    game->Uninitialise();
    if (g_calls != 2) return 2;
    puts("PASS_00412f60");
    return 0;
}
