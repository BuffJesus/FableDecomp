#include <cstdio>

// Behavioural model: PostAddScriptedEntities loads a global singleton,
// walks *(g+0x88) then +8, and forwards that pointer to Callee_4b26b0.

struct Inner { char pad[8]; void* f8; };
struct Mid { char pad[0x88]; Inner* f88; };

static void* g_captured = 0;
static void Callee_model(void* p){ g_captured = p; }

static Mid* g_singleton;

static void PostAddScriptedEntities_model()
{
    Mid* g = g_singleton;
    Inner* i = g->f88;
    Callee_model(i->f8);
}

int main(){
    // case 1: normal chain
    Inner innerA; innerA.f8 = (void*)0xDEADBEEF;
    Mid midA; midA.f88 = &innerA;
    g_singleton = &midA;
    PostAddScriptedEntities_model();
    if(g_captured != (void*)0xDEADBEEF){ printf("FAIL1\n"); return 1; }

    // case 2: different payload propagates
    Inner innerB; innerB.f8 = (void*)0x12345678;
    Mid midB; midB.f88 = &innerB;
    g_singleton = &midB;
    PostAddScriptedEntities_model();
    if(g_captured != (void*)0x12345678){ printf("FAIL2\n"); return 1; }

    printf("OK_006e7460\n");
    return 0;
}