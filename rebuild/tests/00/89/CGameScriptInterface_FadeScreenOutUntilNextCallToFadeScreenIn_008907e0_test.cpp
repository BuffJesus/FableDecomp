#include <cstdio>

struct RGB { unsigned char b0,b1,b2,b3; };
struct Creds { unsigned char pad[0xD8]; unsigned char flag; };
struct Iface;
typedef char(__fastcall* Thunk)(Iface*, void*, float, float, RGB);

struct Iface {
    void** vtbl;
    unsigned char pad4[8];
    Creds* creds;
};

static unsigned int g_color = 0;
static float g_p1 = 0, g_p2 = 0;
static char g_ret = 1;

static char __fastcall myThunk(Iface* self, void* edx, float a, float b, RGB c) {
    g_p1 = a; g_p2 = b;
    g_color = ((unsigned int)c.b0) | ((unsigned int)c.b1<<8) | ((unsigned int)c.b2<<16) | ((unsigned int)c.b3<<24);
    return g_ret;
}

void __fastcall run(Iface* self, void* edx, float p1, float p2) {
    void** vt = self->vtbl;
    RGB colour;
    colour.b2 = 0; colour.b1 = 0; colour.b0 = 0; colour.b3 = 0xFF;
    if (reinterpret_cast<Thunk>(vt[0x5D4/4])(self, 0, p1, p2, colour) != 0) {
        self->creds->flag = 1;
    }
}

int main() {
    static void* vt[0x5D5];
    vt[0x5D4/4] = (void*)myThunk;
    Creds creds; creds.flag = 0;
    Iface iface; iface.vtbl = vt; iface.creds = &creds;

    g_ret = 1;
    run(&iface, 0, 2.5f, 7.5f);
    if (g_color != 0xFF000000u) { printf("BADCOLOR %x\n", g_color); return 1; }
    if (g_p1 != 2.5f || g_p2 != 7.5f) { printf("BADARGS\n"); return 1; }
    if (creds.flag != 1) { printf("BADFLAG_SET\n"); return 1; }

    creds.flag = 0; g_ret = 0;
    run(&iface, 0, 1.0f, 1.0f);
    if (creds.flag != 0) { printf("BADFLAG_UNSET\n"); return 1; }

    printf("FADE_OK\n");
    return 0;
}