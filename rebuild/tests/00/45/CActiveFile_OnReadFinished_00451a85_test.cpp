#include <cstdio>
struct CActiveFile { void* payload; };
static int g_prep = 0;
static void* g_freed = 0;
void __fastcall CActiveFile_Prep(CActiveFile* self){ (void)self; g_prep++; }
void CActiveFile_FreePayload(void* p){ g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Prep(self);
    void* p = self->payload;
    if (p)
        CActiveFile_FreePayload(p);
}

int main(){
    int marker = 42;
    void* mp = (void*)(&marker);
    CActiveFile a;
    a.payload = mp;
    CActiveFile_OnReadFinished(&a);
    if (g_prep != 1 || g_freed != mp){ std::printf("FAIL nonnull\n"); return 1; }
    g_prep = 0;
    g_freed = 0;
    CActiveFile b;
    b.payload = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_prep != 1 || g_freed != 0){ std::printf("FAIL null\n"); return 1; }
    std::printf("CActiveFile_00451a85_TEST PASS\n");
    return 0;
}