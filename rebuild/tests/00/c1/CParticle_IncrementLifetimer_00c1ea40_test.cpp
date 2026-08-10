
#include <stdio.h>
struct CParticle {
    char pad[0x1e];
    unsigned short lifetimer;
};
void __fastcall IncrementLifetimer(CParticle* self)
{
    --self->lifetimer;
}
int main(){
    CParticle p;
    p.lifetimer = 5;
    IncrementLifetimer(&p);
    if (p.lifetimer != 4) { printf("FAIL a\n"); return 1; }
    p.lifetimer = 0;
    IncrementLifetimer(&p);
    if (p.lifetimer != 0xFFFF) { printf("FAIL wrap\n"); return 1; }
    printf("OK_00c1ea40\n");
    return 0;
}