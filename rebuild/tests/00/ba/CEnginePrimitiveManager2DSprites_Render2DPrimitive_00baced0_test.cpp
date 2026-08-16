
#include <stdio.h>
static int g_a=0,g_b=0;
struct Prim { char pad[8]; int f8; };
struct Self {
    unsigned char active; int lastT;
    void a(Prim*,void*,int t){ ++g_a; lastT=t; }
    void b(Prim*,void*,int t){ ++g_b; lastT=t; }
    void Render(Prim* p, void* c, int t){
        if (active){ if (p->f8==0x23) a(p,c,t); else b(p,c,t); }
    }
};
int main(){
    Self s; s.active=1; s.lastT=0;
    Prim p1; p1.f8=0x23; s.Render(&p1,0,7);
    Prim p2; p2.f8=0x10; s.Render(&p2,0,9);
    Self off; off.active=0; Prim p3; p3.f8=0x23; g_a=g_a; off.Render(&p3,0,1);
    int ok=(g_a==1)&&(g_b==1);
    printf(ok?"SPRITE_R2D_OK\n":"FAIL\n");
    return 0;
}