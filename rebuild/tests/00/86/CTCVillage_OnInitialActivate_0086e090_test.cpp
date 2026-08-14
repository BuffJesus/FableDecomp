#include <stdio.h>
struct V {
    void* m0; void* m4;
    unsigned char m8,m9,ma,mb,mc,md;
};
static int g_called=0; static unsigned char g_arg=0xff;
int f(void* ecx, void* edx, unsigned char b){ g_called++; g_arg=b; return 0; }
void act(V* self){ if(self->md){ f(self->m0,self->m4,self->mc); self->md=0; } }
int main(){
    V a; a.m0=&a; a.m4=&a; a.mc=0x5a; a.md=1;
    act(&a);
    int ok1 = (g_called==1 && g_arg==0x5a && a.md==0);
    g_called=0; a.md=0; a.mc=0x11;
    act(&a);
    int ok2 = (g_called==0 && a.md==0);
    if(ok1 && ok2) printf("PARITY_OK\n");
    else printf("FAIL c=%d arg=%x md=%d\n", g_called, g_arg, a.md);
    return 0;
}