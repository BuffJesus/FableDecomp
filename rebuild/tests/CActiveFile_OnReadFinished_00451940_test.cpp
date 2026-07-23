
#include <cstdio>

struct CActiveFile {
    void* buf;
};

static int g_finish_called = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFile_DoFinish(CActiveFile* self) { (void)self; g_finish_called++; }
extern "C" void FreeThing(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_DoFinish(self);
    void* p = self->buf;
    if (p)
        FreeThing(p);
}

int main()
{
    CActiveFile a;
    int dummy = 5;
    a.buf = &dummy;
    g_finish_called = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    if (g_finish_called != 1 || g_freed != &dummy) { std::printf("FAIL nonnull\n"); return 1; }

    CActiveFile b;
    b.buf = 0;
    g_finish_called = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    if (g_finish_called != 1 || g_freed != (void*)-1) { std::printf("FAIL null\n"); return 1; }

    std::printf("CActiveFile_00451940_TEST PASS\n");
    return 0;
}