#include <cstdio>

struct Sub { int ran; };
struct VT { void (__fastcall *m[64])(void*); };
struct CCreatureActionBase { VT* vtbl; char pad[0x5D]; char flag; };

static Sub g_sub;
static int g_virt_called = 0;
static int g_run_called = 0;

Sub* __fastcall getSub(void* subself) { return &g_sub; }
void __fastcall runSub(Sub* p) { g_run_called++; p->ran = 1; }

void __fastcall virt0xe0(void* self) { g_virt_called++; }

void __fastcall Update(CCreatureActionBase* self)
{
    if (self->flag) {
        char* base = (char*)self + 8;
        Sub* p = getSub((void*)base);
        runSub(p);
    } else {
        self->vtbl->m[0xe0/4](self);
    }
}

int main()
{
    VT vt; for (int i=0;i<64;i++) vt.m[i]=0; vt.m[0xe0/4]=virt0xe0;
    CCreatureActionBase obj; obj.vtbl=&vt;

    // flag set -> forwarder path
    obj.flag = 1; g_run_called=0; g_virt_called=0;
    Update(&obj);
    if (g_run_called!=1 || g_virt_called!=0) { printf("BAD1\n"); return 1; }

    // flag clear -> virtual path
    obj.flag = 0; g_run_called=0; g_virt_called=0;
    Update(&obj);
    if (g_run_called!=0 || g_virt_called!=1) { printf("BAD2\n"); return 1; }

    printf("UPDATE_OK\n");
    return 0;
}