// Fixture: CGameJoystickManager::HasExplicitMainPlayerJoystickDeviceNumber.
#include <stdio.h>
#include <string.h>
#include "fable_input_event.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

static unsigned char g_obj[0x0C + 4];
template <class T> static void Poke(unsigned off, T v) { memcpy(g_obj + off, &v, sizeof(v)); }

int main()
{
    memset(g_obj, 0xCD, sizeof(g_obj));
    const CGameJoystickManager* j = reinterpret_cast<const CGameJoystickManager*>(g_obj);
    Poke(8, (unsigned char)1); Poke(4, 0L);
    EXPECT(j->HasExplicitMainPlayerJoystickDeviceNumber() == true);
    Poke(8, (unsigned char)0); Poke(4, 5L);
    EXPECT(j->HasExplicitMainPlayerJoystickDeviceNumber() == false);
    EXPECT(g_obj[0] == 0xCD && g_obj[0x0C] == 0xCD);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("CTCInventoryBase_0049bb50_TEST PASS\n");
    return 0;
}
