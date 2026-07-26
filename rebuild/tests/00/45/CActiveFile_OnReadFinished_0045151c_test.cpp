#include <cstdio>
struct CActiveFile {
    void* p;
    void* q;
};
static int g_freed = 0;
static void* g_last = 0;
extern "C" void __cdecl ExternFree(void* p) { g_freed++; g_last = p; }
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    void* h = self->p;
    if (h)
        ExternFree(h);
}
int main()
{
    CActiveFile a;
    int dummy = 42;
    a.p = &dummy; a.q = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_freed != 1 || g_last != &dummy) { std::printf("FAIL nonnull\n"); return 1; }
    CActiveFile b;
    b.p = 0; b.q = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_freed != 1) { std::printf("FAIL null\n"); return 1; }
    std::printf("CActiveFile_0045151c_TEST PASS\n");
    return 0;
}