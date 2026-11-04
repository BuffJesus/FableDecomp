// Behaviour: Update writes LocalFrame/Checksum1/Checksum2 (+0x266C/+0x2670/+0x2674)
// only when LocalGame (+0x2662) is set; nothing else in the object changes.
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned char g_obj[sizeof(CNetworkClient)];
static unsigned char g_before[sizeof(CNetworkClient)];

int main()
{
    CNetworkClient* client = reinterpret_cast<CNetworkClient*>(g_obj);
    memset(g_obj, 0x11, sizeof(g_obj));
    g_obj[0x2662] = 0;
    memcpy(g_before, g_obj, sizeof(g_obj));
    client->Update(1, 2, 3);
    if (memcmp(g_before, g_obj, sizeof(g_obj)) != 0) { printf("FAIL remote game wrote\n"); return 1; }

    g_obj[0x2662] = 1;
    memcpy(g_before, g_obj, sizeof(g_obj));
    client->Update(static_cast<long>(0xAAAA0001), 0xBBBB0002, 0xCCCC0003);
    if (*reinterpret_cast<unsigned long*>(g_obj + 0x266C) != 0xAAAA0001 ||
        *reinterpret_cast<unsigned long*>(g_obj + 0x2670) != 0xBBBB0002 ||
        *reinterpret_cast<unsigned long*>(g_obj + 0x2674) != 0xCCCC0003) { printf("FAIL local game values\n"); return 2; }
    memcpy(g_before + 0x266C, g_obj + 0x266C, 12);
    if (memcmp(g_before, g_obj, sizeof(g_obj)) != 0) { printf("FAIL neighbours\n"); return 3; }
    printf("OK_0x004ae9d0\n");
    return 0;
}