#include <stdio.h>

typedef void (*pfn_t)(void* self);

struct Base { void** vtbl; };

static int g_called = 0;
static void* g_this = 0;

static void slotfn(void* self) { g_called++; g_this = self; }

static void model_ProcessEvent(void* self, int ev)
{
    if (ev == 0x19) {
        Base* b = (Base*)((char*)self - 4);
        ((pfn_t)b->vtbl[0x244/4])(b);
    }
}

int main(){
    static void* vt[0x100];
    vt[0x244/4] = (void*)&slotfn;
    Base b; b.vtbl = vt;
    void* self = (char*)&b + 4;

    g_called = 0;
    model_ProcessEvent(self, 0x10);
    if (g_called != 0) { printf("FAIL nocall\n"); return 1; }

    g_called = 0; g_this = 0;
    model_ProcessEvent(self, 0x19);
    if (g_called != 1) { printf("FAIL call\n"); return 1; }
    if (g_this != (void*)&b) { printf("FAIL this\n"); return 1; }

    printf("OK_0055b9d0\n");
    return 0;
}