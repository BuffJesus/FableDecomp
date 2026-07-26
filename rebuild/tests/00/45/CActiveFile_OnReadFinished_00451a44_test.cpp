#include <cstdio>
struct CActiveFile { void* p; };
static int g_notify = 0;
static int g_freed = 0;
static void* g_freed_ptr = 0;
void __fastcall CActiveFile_Notify(CActiveFile* self) { g_notify++; }
void __cdecl FreeBuffer(void* p) { g_freed++; g_freed_ptr = p; }
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self);
    void* q = self->p;
    if (q)
        FreeBuffer(q);
}
int main() {
    int dummy = 0;
    CActiveFile a; a.p = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_notify != 1) { std::printf("FAIL notify\n"); return 1; }
    if (g_freed != 1 || g_freed_ptr != &dummy) { std::printf("FAIL free path\n"); return 1; }
    CActiveFile b; b.p = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_notify != 2) { std::printf("FAIL notify2\n"); return 1; }
    if (g_freed != 1) { std::printf("FAIL null skip\n"); return 1; }
    std::printf("CActiveFile_00451a44_TEST PASS\n");
    return 0;
}