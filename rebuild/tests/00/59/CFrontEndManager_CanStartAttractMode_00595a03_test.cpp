#include <stdio.h>
#include <string.h>

#include "fable_nui_manager.h"

// Behaviour fixture for NUISystem::CFrontEndManager::CanStartAttractMode
// @ 0x00595A03: the PC build never starts attract mode, whatever the
// manager holds, and the manager is left untouched.

static unsigned char g_manager[0xE0];

int main()
{
    bool ok = true;
    for (int fill = 0; fill < 256; fill += 0x55)
    {
        memset(g_manager, fill, sizeof(g_manager));
        ok = ok && !reinterpret_cast<NUISystem::CFrontEndManager*>(g_manager)->CanStartAttractMode();
        for (int i = 0; i < 0xE0; ++i)
            if (g_manager[i] != (unsigned char)fill) { ok = false; break; }
    }
    if (ok) printf("AUTO_TINY_00595a03_TEST PASS\n");
    return ok ? 0 : 1;
}
