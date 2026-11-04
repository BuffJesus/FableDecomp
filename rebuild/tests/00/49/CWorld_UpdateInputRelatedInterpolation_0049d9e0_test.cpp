// Behaviour: UpdateInputRelatedInterpolation leaves the world untouched.
#include "fable_world.h"
#include <stdio.h>
#include <string.h>

static unsigned char g_obj[sizeof(CWorld)];
static unsigned char g_before[sizeof(CWorld)];

int main()
{
    for (unsigned i = 0; i < sizeof(g_obj); ++i) g_obj[i] = static_cast<unsigned char>(i * 13 + 1);
    memcpy(g_before, g_obj, sizeof(g_obj));
    reinterpret_cast<CWorld*>(g_obj)->UpdateInputRelatedInterpolation();
    if (memcmp(g_before, g_obj, sizeof(g_obj)) != 0) { printf("FAIL wrote\n"); return 1; }
    printf("OK_0049d9e0\n");
    return 0;
}