#include <cstdio>
struct CActiveFile { void* buf; int closed; void Close(){ closed = 1; } };
static int g_freed = 0;
static void* g_last = 0;
void FreeBuffer(void* p){ g_freed = g_freed + 1; g_last = p; }
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    self->Close();
    void* p = self->buf;
    if (p != 0)
        FreeBuffer(p);
}
int main(){
    char storage[8];
    void* marker = storage;
    CActiveFile a;
    a.buf = marker;
    a.closed = 0;
    CActiveFile_OnReadFinished(&a);
    if (a.closed != 1){ std::printf("FAIL close\n"); return 1; }
    if (g_freed != 1 || g_last != marker){ std::printf("FAIL free\n"); return 1; }
    CActiveFile b;
    b.buf = 0;
    b.closed = 0;
    g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    if (b.closed != 1){ std::printf("FAIL close2\n"); return 1; }
    if (g_freed != 0){ std::printf("FAIL free2\n"); return 1; }
    std::printf("CActiveFile_004518a0_TEST PASS\n");
    return 0;
}