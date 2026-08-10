#include <cstdio>
struct CHornetDrone { char pad[0x1c]; int field_1c; };
static int g_seed=0;
static int myrand(){ g_seed = g_seed*1103515245+12345; return (g_seed>>16)&0x7fff; }
static void Init(CHornetDrone* self){ self->field_1c = (myrand()%200)+1; }
int main(){
    CHornetDrone d; 
    for(int i=0;i<1000;i++){
        Init(&d);
        if(d.field_1c < 1 || d.field_1c > 200){ printf("FAIL %d\n", d.field_1c); return 1; }
    }
    printf("OK_00e11d80\n");
    return 0;
}