// Behaviour: CNetworkClient::Uninitialise forwards to CInitBaseClass::Uninitialise
// on the same object exactly once and touches nothing itself.
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static int g_calls;
static const void* g_this;
void CInitBaseClass::Uninitialise() { ++g_calls; g_this = this; }

static unsigned char g_obj[sizeof(CNetworkClient)];
static unsigned char g_before[sizeof(CNetworkClient)];

int main()
{
    memset(g_obj, 0x3C, sizeof(g_obj));
    memcpy(g_before, g_obj, sizeof(g_obj));
    CNetworkClient* client = reinterpret_cast<CNetworkClient*>(g_obj);
    client->Uninitialise();
    if (g_calls != 1 || g_this != static_cast<CInitBaseClass*>(client)) { printf("FAIL forward\n"); return 1; }
    if (memcmp(g_before, g_obj, sizeof(g_obj)) != 0) { printf("FAIL wrote\n"); return 2; }
    printf("PASS_004ae990\n");
    return 0;
}