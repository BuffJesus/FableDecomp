
#include <cstdio>
struct CTexture {
    virtual long slot0(int v){ return 0; }
};
static int g_arg=-1;
struct Impl : CTexture {
    long slot0(int v){ g_arg=v; return 77; }
};
long __fastcall GetNoLevels(CTexture* self, int, void* p){
    int* ip=(int*)p;
    long r=self->slot0(*ip);
    *ip=0;
    return r;
}
int main(){
    Impl impl;
    int cell=42;
    long r=GetNoLevels(&impl,0,&cell);
    if(r==77 && g_arg==42 && cell==0) printf("OK_00c1b1e0\n");
    else { printf("FAIL r=%ld arg=%d cell=%d\n",r,g_arg,cell); return 1; }
    return 0;
}