#include <cstdio>

struct CActiveFile {
    void* buf;
};

static int g_notify_calls = 0;
static int g_free_calls = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_Notify(CActiveFile* self)
{
    (void)self;
    ++g_notify_calls;
}
void __cdecl CActiveFile_Free(void* p)
{
    ++g_free_calls;
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}

int main()
{
    int dummy = 42;
    CActiveFile a;
    a.buf = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_notify_calls != 1) { std::printf("FAIL notify\n"); return 1; }
    if (g_free_calls != 1)   { std::printf("FAIL free1\n"); return 1; }
    if (g_freed != &dummy)   { std::printf("FAIL freed ptr\n"); return 1; }

    CActiveFile b;
    b.buf = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_notify_calls != 2) { std::printf("FAIL notify2\n"); return 1; }
    if (g_free_calls != 1)   { std::printf("FAIL free_extra\n"); return 1; }

    std::printf("CActiveFile_0045185f_TEST PASS\n");
    return 0;
}