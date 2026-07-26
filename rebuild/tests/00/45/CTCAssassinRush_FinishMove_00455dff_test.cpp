#include <cstdio>

struct SubA { void FinishA(); };
struct SubB { void FinishB(); };
struct CTCMoveBase { void FinishMove(); };

struct CTCAssassinRush {
    char pad0[0x3c];
    void* p3c;
    char pad2[0x50 - 0x40];
    SubB b50;
    char pad3[0x5c - 0x54];
    void* p5c;
    char pad4[0x84 - 0x60];
    SubA a84;
};

static int g_a=0, g_b=0, g_base=0;
static int g_freed=0;

void SubA::FinishA(){ g_a++; }
void SubB::FinishB(){ g_b++; }
void CTCMoveBase::FinishMove(){ g_base++; }
void __cdecl engine_free(void* p){ if(p) g_freed++; }

void __fastcall CTCAssassinRush_FinishMove(CTCAssassinRush* self)
{
    self->a84.FinishA();
    if (self->p5c)
        engine_free(self->p5c);
    self->b50.FinishB();
    if (self->p3c)
        engine_free(self->p3c);
    ((CTCMoveBase*)self)->FinishMove();
}

int main(){
    char buf[0x100];
    for(int i=0;i<0x100;i++) buf[i]=0;
    CTCAssassinRush* self=(CTCAssassinRush*)buf;
    int dummy;
    self->p3c=&dummy;
    self->p5c=&dummy;
    CTCAssassinRush_FinishMove(self);
    if(g_a==1 && g_b==1 && g_base==1 && g_freed==2){
        // now null pointers -> no free
        g_a=g_b=g_base=g_freed=0;
        self->p3c=0; self->p5c=0;
        CTCAssassinRush_FinishMove(self);
        if(g_a==1 && g_b==1 && g_base==1 && g_freed==0){
            std::printf("CTCAssassinRush_00455dff_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL a=%d b=%d base=%d freed=%d\n", g_a,g_b,g_base,g_freed);
    return 1;
}