#include <cstdio>
struct CActiveFile { void* field0; };
static int g_helper=0; static int g_free=0; static void* g_freed=0;
void __fastcall Helper_4db748(CActiveFile* self){ (void)self; g_helper++; }
void __cdecl Free_bfea14(void* p){ g_free++; g_freed=p; }
void __fastcall OnReadFinished(CActiveFile* self){ Helper_4db748(self); void* p = self->field0; if(p) Free_bfea14(p); }
int main(){
    CActiveFile a; int dummy=5; a.field0=&dummy;
    OnReadFinished(&a);
    if(g_helper!=1||g_free!=1||g_freed!=&dummy){ printf("FAIL1\n"); return 1; }
    CActiveFile b; b.field0=0; g_helper=0; g_free=0;
    OnReadFinished(&b);
    if(g_helper!=1||g_free!=0){ printf("FAIL2\n"); return 1; }
    printf("OK_004dd118\n");
    return 0;
}