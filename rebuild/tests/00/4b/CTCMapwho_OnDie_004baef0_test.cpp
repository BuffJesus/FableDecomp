#include <cstdio>
struct CTCMapwho { int x; };
static int g_sub=0; static int g_free=0; static CTCMapwho* g_freed=0;
void sub(CTCMapwho* self){ g_sub++; }
void freefn(CTCMapwho* self){ g_free++; g_freed=self; }
void model(CTCMapwho* self){ if(self){ sub(self); freefn(self); } }
int main(){
    CTCMapwho a; a.x=7;
    model(&a);
    if(g_sub!=1||g_free!=1||g_freed!=&a){ printf("FAIL\n"); return 1; }
    g_sub=0; g_free=0; g_freed=0;
    model(0);
    if(g_sub!=0||g_free!=0){ printf("FAIL\n"); return 1; }
    printf("OK_004baef0\n");
    return 0;
}