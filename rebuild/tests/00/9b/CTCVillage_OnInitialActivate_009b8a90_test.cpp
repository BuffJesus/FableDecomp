#include <cstdio>

struct CTCVillage {
    void*         field_0;
    void*         field_4;
    char          pad8[4];
    unsigned char field_C;
    unsigned char field_D;
};

static int g_called = 0;
static int g_arg = -1;
static void* g_ecx = 0;
static void* g_edx = 0;

void Sub_model(void* ecx, void* edx, int arg) {
    g_called++;
    g_ecx = ecx;
    g_edx = edx;
    g_arg = arg;
}

void OnInitialActivate_model(CTCVillage* self) {
    if (self->field_D) {
        Sub_model(self->field_0, self->field_4, self->field_C);
        self->field_D = 0;
    }
}

int main() {
    int e0 = 0, e4 = 0;
    CTCVillage v;
    v.field_0 = &e0;
    v.field_4 = &e4;
    v.field_C = 0x7A;
    v.field_D = 1;

    // active: should call and clear flag
    OnInitialActivate_model(&v);
    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_arg != 0x7A) { printf("FAIL arg\n"); return 1; }
    if (g_ecx != &e0 || g_edx != &e4) { printf("FAIL ptrs\n"); return 1; }
    if (v.field_D != 0) { printf("FAIL flag not cleared\n"); return 1; }

    // second call: flag now 0, should NOT call again
    OnInitialActivate_model(&v);
    if (g_called != 1) { printf("FAIL called twice\n"); return 1; }

    // fresh with flag 0 from start
    CTCVillage v2;
    v2.field_0 = 0; v2.field_4 = 0; v2.field_C = 5; v2.field_D = 0;
    OnInitialActivate_model(&v2);
    if (g_called != 1) { printf("FAIL inactive called\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}