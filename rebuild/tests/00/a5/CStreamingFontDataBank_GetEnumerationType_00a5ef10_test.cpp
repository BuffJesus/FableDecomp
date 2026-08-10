#include <stdio.h>

static void* g_lastA; static void* g_lastB;
static void* helper(void* a, void* b){ g_lastA=a; g_lastB=b; return (void*)0; }

static void* model(void* a, void* b){
    void* r = helper(b, a);
    (void)r;
    return b;
}

int main(){
    int x, y;
    void* pa=&x; void* pb=&y;
    void* ret = model(pa, pb);
    if(ret!=pb){ printf("FAIL ret\n"); return 1; }
    if(g_lastA!=pb){ printf("FAIL argA\n"); return 1; }
    if(g_lastB!=pa){ printf("FAIL argB\n"); return 1; }
    void* ret2 = model(0,0);
    if(ret2!=0){ printf("FAIL null\n"); return 1; }
    printf("OK_00a5ef10\n");
    return 0;
}