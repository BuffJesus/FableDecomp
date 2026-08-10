#include <cstdio>
static int g_ret=0;
static bool helper_model(void* self, void* a){ return g_ret!=0; }
static bool IsAbleToBeUsedToBlock_model(void* self, void* a){
    bool r = helper_model(self,a);
    if(!r) return false;
    return true;
}
int main(){
    int self=1,a=2;
    g_ret=0; bool r0=IsAbleToBeUsedToBlock_model(&self,&a);
    g_ret=1; bool r1=IsAbleToBeUsedToBlock_model(&self,&a);
    if(r0==false && r1==true){ printf("OK_005dacf0\n"); return 0; }
    printf("FAIL\n"); return 1;
}