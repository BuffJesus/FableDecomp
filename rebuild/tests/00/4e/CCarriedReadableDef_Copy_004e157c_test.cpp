#include <cstdio>
struct Base { int dummy; };
struct Def { char pad[0x28]; int f28; int f2c; };
static int g_called = 0;
void basecopy(Base const* o){ g_called = 1; (void)o; }
void copyfn(Def* self, Base const* other){
    basecopy(other);
    const Def* o = (const Def*)other;
    self->f28 = o->f28;
    self->f2c = o->f2c;
}
int main(){
    Def src; Def dst;
    for(int i=0;i<0x28;i++){src.pad[i]=(char)i;dst.pad[i]=0;}
    src.f28 = 0x11223344; src.f2c = 0x55667788;
    dst.f28 = 0; dst.f2c = 0;
    copyfn(&dst, (Base const*)&src);
    if(g_called==1 && dst.f28==0x11223344 && dst.f2c==0x55667788)
        printf("COPY_OK\n");
    else printf("FAIL\n");
    return 0;
}