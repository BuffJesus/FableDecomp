#include "engine/CHornetDrone.h"
#include <cstdio>
static int g_seed=0;
static int myrand(){ g_seed = g_seed*1103515245+12345; return (g_seed>>16)&0x7fff; }
static void Init(CHornetDrone* self){ self->EatNow = (myrand()%200)+1; }
int main(){
    CHornetDrone d; 
    for(int i=0;i<1000;i++){
        Init(&d);
        if(d.EatNow < 1 || d.EatNow > 200){ printf("FAIL %d\n", d.EatNow); return 1; }
    }
    printf("OK_00e11d80\n");
    return 0;
}