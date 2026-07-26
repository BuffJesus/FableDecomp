#include <cstdio>

struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

static int g_read = 0;
static int g_freed = 0;
static void* g_freed_arg = 0;

void __fastcall Reader_Read(void* ecx, void* edx, unsigned char* buf)
{
    (void)ecx; (void)edx;
    *buf = 0x42;
    g_read++;
}
void __cdecl FreeThing(void* p)
{
    g_freed++;
    g_freed_arg = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    unsigned char b;
    Reader_Read(self->p, self->q, &b);
    if (self->p) {
        FreeThing(self->p);
    }
}

int main()
{
    int dummy = 5;
    CActiveFile a;
    a.p = &dummy;
    a.q = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_read != 1 || g_freed != 1 || g_freed_arg != &dummy) {
        std::printf("FAIL non-null path\n");
        return 1;
    }

    CActiveFile b;
    b.p = 0;
    b.q = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_read != 2 || g_freed != 1) {
        std::printf("FAIL null path\n");
        return 1;
    }

    std::printf("CActiveFile_00453406_TEST PASS\n");
    return 0;
}