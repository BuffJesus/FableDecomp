
#include <cstdio>
static void* g_a; static void* g_b; static int g_called;
extern "C" void __stdcall Helper_9cbd10(void* a, void* b){ g_a=a; g_b=b; g_called++; }
void* GetEnumerationType_Font(void* a, void* b)
{
    Helper_9cbd10(a, b);
    return a;
}
int main(){
    int x, y;
    void* pa=&x; void* pb=&y;
    void* r = GetEnumerationType_Font(pa, pb);
    int ok=1;
    if (r!=pa) ok=0;
    if (g_a!=pa) ok=0;
    if (g_b!=pb) ok=0;
    if (g_called!=1) ok=0;
    void* r2 = GetEnumerationType_Font(pb, pa);
    if (r2!=pb) ok=0;
    if (g_a!=pb) ok=0;
    if (g_b!=pa) ok=0;
    if (g_called!=2) ok=0;
    if(ok) printf("OK_00a63380\n"); else printf("FAIL\n");
    return ok?0:1;
}