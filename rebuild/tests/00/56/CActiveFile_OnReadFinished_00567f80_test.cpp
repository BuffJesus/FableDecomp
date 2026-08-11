#include <cstdio>

struct CActiveFile { void* m0; };

static int g_frame_called = 0;
static void* g_freed = (void*)0;

extern void __fastcall CActiveFile_FrameUpdate_566dc0(CActiveFile* self) { g_frame_called++; (void)self; }
extern void __cdecl re_free(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    int dummy = 0;
    CActiveFile a; a.m0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_frame_called == 1) && (g_freed == &dummy);

    g_frame_called = 0; g_freed = (void*)0;
    CActiveFile b; b.m0 = (void*)0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_frame_called == 1) && (g_freed == (void*)0);

    if (ok1 && ok2) printf("00567f80_TEST PASS\n");
    else printf("FAIL f=%d freed=%p\n", g_frame_called, g_freed);
    return 0;
}