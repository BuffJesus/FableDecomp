// Fixture: CSurface::GetHeight (0x00415BF0): Y of the out-of-line GetDimensions.
#include <stdio.h>
#include <string.h>
#include "fable_surface.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

static int g_calls; static const CSurface* g_this; static long g_x, g_y;
C2DExtentsI CSurface::GetDimensions() const { ++g_calls; g_this = this; return C2DExtentsI(g_x, g_y); }

int main()
{
    static unsigned char obj[sizeof(CSurface)];
    memset(obj, 0xCD, sizeof(obj));
    const CSurface* s = reinterpret_cast<const CSurface*>(obj);
    g_x = 800; g_y = 600;
    EXPECT(s->GetHeight() == 600);
    EXPECT(g_calls == 1 && g_this == s);
    g_x = 3; g_y = 77;
    EXPECT(s->GetHeight() == 77 && g_calls == 2);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("PASS_00415bf0\n");
    return 0;
}
