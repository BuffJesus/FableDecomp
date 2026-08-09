#include <cstdio>

struct C3DVector { float x,y,z; };
struct Sub { void** vtbl; };
struct CTCCreatureNavigation { void* pad0; Sub* sub; };

typedef bool (__stdcall *PFN)(Sub* self, C3DVector* v);

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest)
{
    Sub* s = self->sub;
    PFN fn = (PFN)s->vtbl[7];
    return fn(s, dest);
}

static Sub* g_expect_self;
static C3DVector* g_expect_vec;
static bool g_ret;

static bool __stdcall vslot(Sub* self, C3DVector* v)
{
    if (self != g_expect_self) { printf("bad self\n"); }
    if (v != g_expect_vec) { printf("bad vec\n"); }
    return g_ret;
}

int main()
{
    void* vt[8] = {0,0,0,0,0,0,0,(void*)&vslot};
    Sub s; s.vtbl = vt;
    CTCCreatureNavigation nav; nav.pad0=0; nav.sub=&s;
    C3DVector dest = {1,2,3};

    g_expect_self=&s; g_expect_vec=&dest;

    g_ret=true;
    bool r1 = IsNewDestinationGoingToResetNavigation(&nav,0,&dest);
    g_ret=false;
    bool r2 = IsNewDestinationGoingToResetNavigation(&nav,0,&dest);

    if (r1==true && r2==false) { printf("OK_00c152d0\n"); return 0; }
    printf("FAIL\n"); return 1;
}