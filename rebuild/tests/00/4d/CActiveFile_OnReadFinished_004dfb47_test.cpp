
#include <cstdio>
struct CActiveFile;
static int g_helper=0; static void* g_freed=(void*)0;
struct CActiveFile { void* payload; };
extern "C" void __fastcall helper_4dd641(CActiveFile* self){ g_helper++; }
void free_thing_bfea14(void* p){ g_freed=p; }
void __fastcall OnReadFinished_15(CActiveFile* self){ helper_4dd641(self); void* p=self->payload; if(p){ free_thing_bfea14(p);} }
int main(){
    // non-null payload
    int x=5; CActiveFile a; a.payload=&x;
    g_helper=0; g_freed=(void*)0;
    OnReadFinished_15(&a);
    if(g_helper!=1) { printf("FAIL helper\n"); return 1; }
    if(g_freed!=&x) { printf("FAIL freed\n"); return 1; }
    // null payload
    CActiveFile b; b.payload=(void*)0;
    g_helper=0; g_freed=(void*)0x1234;
    OnReadFinished_15(&b);
    if(g_helper!=1){ printf("FAIL helper2\n"); return 1; }
    if(g_freed!=(void*)0x1234){ printf("FAIL freed2\n"); return 1; }
    printf("OK_004dfb47\n");
    return 0;
}