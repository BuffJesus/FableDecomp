
#include <cstdio>
struct CDrawMouseCursor { int dummy; };
unsigned char g_cursorEnabled;
void* g_cursorThis;
void* g_cursorArg;
static void Draw(CDrawMouseCursor* self, void* edxArg)
{
    if (g_cursorEnabled) {
        g_cursorThis = self;
        g_cursorArg = edxArg;
    }
}
int main()
{
    CDrawMouseCursor c;
    // disabled branch: nothing stored
    g_cursorEnabled = 0;
    g_cursorThis = 0; g_cursorArg = 0;
    Draw(&c, (void*)0x1234);
    if (g_cursorThis != 0 || g_cursorArg != 0) { printf("FAIL disabled\n"); return 1; }
    // enabled branch: both stored
    g_cursorEnabled = 1;
    Draw(&c, (void*)0x1234);
    if (g_cursorThis != &c || g_cursorArg != (void*)0x1234) { printf("FAIL enabled\n"); return 2; }
    printf("OK_00cb2860\n");
    return 0;
}