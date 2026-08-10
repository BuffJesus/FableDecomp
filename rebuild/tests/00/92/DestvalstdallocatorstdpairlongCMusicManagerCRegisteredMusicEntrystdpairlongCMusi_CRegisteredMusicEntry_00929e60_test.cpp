#include <cstdio>
struct Sub { int x; };
struct Obj { char head[0xa8]; Sub member; int finalized; };

static int g_sub_called = 0;
static int g_final_called = 0;
static Obj* g_last = 0;

void SubCleanup(Sub* s){ g_sub_called++; s->x = 0; }
void FinalDtor(Obj* o){ g_final_called++; g_last = o; o->finalized = 1; }

void DestVal_A8_E(Obj* self){
    SubCleanup(&self->member);
    FinalDtor(self);
}

int main(){
    Obj o;
    o.member.x = 7;
    o.finalized = 0;
    DestVal_A8_E(&o);
    if (g_sub_called != 1) { printf("FAIL sub\n"); return 1; }
    if (g_final_called != 1) { printf("FAIL final\n"); return 2; }
    if (g_last != &o) { printf("FAIL ptr\n"); return 3; }
    if (o.member.x != 0) { printf("FAIL cleanup\n"); return 4; }
    if (o.finalized != 1) { printf("FAIL flag\n"); return 5; }
    printf("OK_00929e60\n");
    return 0;
}