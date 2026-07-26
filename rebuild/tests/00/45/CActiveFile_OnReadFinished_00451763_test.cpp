#include <cstdio>
struct CActiveFile { void* p; };
static int g_notify = 0;
static void* g_freed = 0;
extern "C" void __fastcall CActiveFile_BaseNotify(CActiveFile* self) { g_notify++; }
extern "C" void FreeBuffer(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseNotify(self);
    void* buf = self->p;
    if (buf)
        FreeBuffer(buf);
}

int main() {
    int dummy = 5;
    CActiveFile a; a.p = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_notify != 1) { std::printf("FAIL notify\n"); return 1; }
    if (g_freed != &dummy) { std::printf("FAIL freed\n"); return 1; }
    CActiveFile b; b.p = 0;
    g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_notify != 2) { std::printf("FAIL notify2\n"); return 1; }
    if (g_freed != 0) { std::printf("FAIL freed2\n"); return 1; }
    std::printf("CActiveFile_00451763_TEST PASS\n");
    return 0;
}