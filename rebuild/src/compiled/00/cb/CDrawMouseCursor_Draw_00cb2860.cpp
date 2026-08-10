struct CDrawMouseCursor { int dummy; };

extern unsigned char g_cursorEnabled;
extern void* g_cursorThis;
extern void* g_cursorArg;

unsigned char g_cursorEnabled;
void* g_cursorThis;
void* g_cursorArg;

void __fastcall Draw(CDrawMouseCursor* self, void* edxArg)
{
    if (g_cursorEnabled) {
        g_cursorThis = self;
        g_cursorArg = edxArg;
    }
}