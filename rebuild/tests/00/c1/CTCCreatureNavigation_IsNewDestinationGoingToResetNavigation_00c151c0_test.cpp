
#include <cstdio>
struct C3DVector { float x, y, z; };
struct Nav;
// model: delegate object with vtable; slot7 called with (self, vec)
static int g_called = 0;
static void* g_self = 0;
static void* g_vec = 0;
struct Delegate { void* vtbl[8]; };
struct Nav { void* field0; Delegate* field4; };

typedef bool (__stdcall *slot_t)(Nav*, C3DVector*);

bool __stdcall slot7_true(Nav* s, C3DVector* v){ g_called++; g_self=s; g_vec=v; return true; }
bool __stdcall slot7_false(Nav* s, C3DVector* v){ g_called++; g_self=s; g_vec=v; return false; }

// behavioural model of the target
bool model(Nav* self, C3DVector* vec, float f){
    Delegate* d = self->field4;
    slot_t fn = (slot_t)d->vtbl[7];
    return fn(self, vec);
}

int main(){
    Delegate dT; for(int i=0;i<8;i++) dT.vtbl[i]=0;
    dT.vtbl[7] = (void*)&slot7_true;
    Nav navT; navT.field0=0; navT.field4=&dT;
    C3DVector v = {1,2,3};

    g_called=0;
    bool r1 = model(&navT, &v, 5.0f);
    if(!r1){ printf("FAIL r1\n"); return 1; }
    if(g_called!=1){ printf("FAIL called\n"); return 1; }
    if(g_self!=&navT){ printf("FAIL self\n"); return 1; }
    if(g_vec!=&v){ printf("FAIL vec\n"); return 1; }

    Delegate dF; for(int i=0;i<8;i++) dF.vtbl[i]=0;
    dF.vtbl[7] = (void*)&slot7_false;
    Nav navF; navF.field0=0; navF.field4=&dF;
    bool r2 = model(&navF, &v, 0.0f);
    if(r2){ printf("FAIL r2\n"); return 1; }
    if(g_called!=2){ printf("FAIL called2\n"); return 1; }

    printf("OK_00c151c0\n");
    return 0;
}