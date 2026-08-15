#include <cstdio>

struct Sub {
    char present;
    char pad[0xb];
    char f_c;
    char f_d;
};

struct CDrawMiniMap {
    char pad0[0xe9];
    char active;
    char pad1[0x16];
    Sub sub;
};

void __fastcall SetAsActive(CDrawMiniMap* self, bool active)
{
    self->active = active;
    if (!active) {
        if (self->sub.present != 0) {
            self->sub.f_d = 0;
            self->sub.f_c = 1;
        }
    }
}

int main()
{
    CDrawMiniMap m;

    // active = true -> sets active, leaves sub untouched
    m.active = 0; m.sub.present = 1; m.sub.f_c = 9; m.sub.f_d = 9;
    SetAsActive(&m, true);
    if (m.active != 1) { printf("BAD1\n"); return 1; }
    if (m.sub.f_c != 9 || m.sub.f_d != 9) { printf("BAD2\n"); return 1; }

    // active = false, present != 0 -> f_d=0, f_c=1
    m.active = 1; m.sub.present = 1; m.sub.f_c = 9; m.sub.f_d = 9;
    SetAsActive(&m, false);
    if (m.active != 0) { printf("BAD3\n"); return 1; }
    if (m.sub.f_c != 1 || m.sub.f_d != 0) { printf("BAD4\n"); return 1; }

    // active = false, present == 0 -> sub untouched
    m.active = 1; m.sub.present = 0; m.sub.f_c = 9; m.sub.f_d = 9;
    SetAsActive(&m, false);
    if (m.active != 0) { printf("BAD5\n"); return 1; }
    if (m.sub.f_c != 9 || m.sub.f_d != 9) { printf("BAD6\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}