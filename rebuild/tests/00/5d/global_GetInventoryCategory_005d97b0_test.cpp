#include <cstdio>
struct GIC_Obj { char pad[0x3c]; long cat; };
static int g_flag = 1;
static GIC_Obj g_obj;
static char check(void){ return (char)g_flag; }
static long model(){ if(check()) return g_obj.cat; return -1; }
int main(){
    g_obj.cat = 7;
    g_flag = 1;
    long a = model();
    g_flag = 0;
    long b = model();
    if(a==7 && b==-1) printf("OK_005d97b0\n");
    else { printf("FAIL %ld %ld\n", a, b); return 1; }
    return 0;
}