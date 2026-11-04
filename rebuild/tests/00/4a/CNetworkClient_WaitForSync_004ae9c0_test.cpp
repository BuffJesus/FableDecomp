// Behaviour: WaitForSync is a no-op for a local client (retail body is a bare ret).
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned char g_obj[sizeof(CNetworkClient)];
static unsigned char g_before[sizeof(CNetworkClient)];

int main()
{
    for (unsigned i = 0; i < sizeof(g_obj); ++i) g_obj[i] = static_cast<unsigned char>(i * 7);
    memcpy(g_before, g_obj, sizeof(g_obj));
    reinterpret_cast<CNetworkClient*>(g_obj)->WaitForSync();
    if (memcmp(g_before, g_obj, sizeof(g_obj)) != 0) { printf("FAIL wrote\n"); return 1; }
    printf("OK_004ae9c0\n");
    return 0;
}