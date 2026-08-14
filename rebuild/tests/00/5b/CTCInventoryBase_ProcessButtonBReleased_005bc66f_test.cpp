#include <cstdio>

struct Inv;
static int g_called_80 = 0;
static int g_called_b8 = 0;
static int g_b8_arg = -1;

typedef void (*Fn)(Inv*, int);

struct VT { Fn slots[64]; };
struct Inv { VT* vt; char pad[0x1c-4]; int state_1c; };

static void slot80(Inv*, int) { g_called_80++; }
static void slotb8(Inv*, int a) { g_called_b8++; g_b8_arg = a; }

static void model(Inv* self) {
    switch (self->state_1c) {
    case 0: ((void(*)(Inv*))self->vt->slots[0x80/4])(self); break;
    case 1: self->vt->slots[0xb8/4](self, 0); break;
    default: break;
    }
}

int main() {
    VT vt; for (int i=0;i<64;i++) vt.slots[i]=0;
    vt.slots[0x80/4] = (Fn)slot80;
    vt.slots[0xb8/4] = slotb8;
    Inv inv; inv.vt = &vt;
    g_called_80=g_called_b8=0; inv.state_1c=0; model(&inv);
    if (g_called_80!=1 || g_called_b8!=0) { printf("FAIL case0\n"); return 1; }
    g_called_80=g_called_b8=0; g_b8_arg=-1; inv.state_1c=1; model(&inv);
    if (g_called_80!=0 || g_called_b8!=1 || g_b8_arg!=0) { printf("FAIL case1\n"); return 1; }
    g_called_80=g_called_b8=0; inv.state_1c=5; model(&inv);
    if (g_called_80!=0 || g_called_b8!=0) { printf("FAIL default\n"); return 1; }
    printf("BEHAVIOR_OK\n");
    return 0;
}