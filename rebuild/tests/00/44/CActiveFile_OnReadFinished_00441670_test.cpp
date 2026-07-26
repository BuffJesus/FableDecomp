#include <cstdio>

struct CActiveFile {
    void* p;
    void* q;
};

static int g_base_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_Base_OnReadFinished(CActiveFile* self)
{
    (void)self;
    g_base_called++;
}
void __cdecl FreeBuffer(void* p)
{
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base_OnReadFinished(self);
    void* buf = self->p;
    if (buf)
        FreeBuffer(buf);
}

int main()
{
    CActiveFile a;
    a.p = (void*)0x1234;
    a.q = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (g_base_called != 1) { std::printf("FAIL base\n"); return 1; }
    if (g_freed != (void*)0x1234) { std::printf("FAIL free\n"); return 1; }

    CActiveFile b;
    b.p = (void*)0;
    g_freed = (void*)0xdead;
    CActiveFile_OnReadFinished(&b);
    if (g_base_called != 2) { std::printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0xdead) { std::printf("FAIL nofree\n"); return 1; }

    std::printf("CActiveFile_00441670_TEST PASS\n");
    return 0;
}