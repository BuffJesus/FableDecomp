#include <cstdio>
// Behavioural model: Ucopy_B forwards two args plus &local(ebp+0xf) to a copy helper.
static void* g_a; static void* g_b; static void* g_local_seen;
static void* copy_helper(void* dst, void* src, void* extra){ g_a=dst; g_b=src; g_local_seen=extra; return dst; }
static void* model(void* dst, void* src){ char local[8]; void* p = &local[0]; return copy_helper(dst, src, p); }
int main(){
    int x=1, y=2;
    void* r = model(&x, &y);
    int ok = (g_a==&x) && (g_b==&y) && (g_local_seen!=0) && (r==&x);
    if(ok){ printf("OK_005c13e3\n"); return 0; }
    printf("FAIL\n"); return 1;
}