#include <cstdio>

struct CActiveFile {
    void* p;
    void* p4;
};

static char* g_seenBuf = 0;
static void* g_consumeEcx = (void*)0;
static void* g_consumeEdx = (void*)0;
static void* g_freed = (void*)0;

void __fastcall Consume(void* ecxArg, void* edxArg, char* buf)
{
    g_consumeEcx = ecxArg;
    g_consumeEdx = edxArg;
    g_seenBuf = buf;
}
void FreeThing(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Consume(self->p, self->p4, &local);
    void* q = self->p;
    if (q) {
        FreeThing(q);
    }
}

int main()
{
    CActiveFile a;
    a.p = (void*)0x1234;
    a.p4 = (void*)0x5678;
    CActiveFile_OnReadFinished(&a);
    if (g_consumeEcx != (void*)0x1234) { std::printf("FAIL ecx\n"); return 1; }
    if (g_consumeEdx != (void*)0x5678) { std::printf("FAIL edx\n"); return 1; }
    if (g_freed != (void*)0x1234) { std::printf("FAIL freed\n"); return 1; }

    CActiveFile b;
    b.p = (void*)0;
    b.p4 = (void*)0xAAAA;
    g_freed = (void*)0xDEAD;
    CActiveFile_OnReadFinished(&b);
    if (g_freed != (void*)0xDEAD) { std::printf("FAIL should-not-free\n"); return 1; }

    std::printf("CActiveFile_004533e3_TEST PASS\n");
    return 0;
}