#include <cstdio>

struct CTCVillage {
    void* field0;
    void* field4;
    char  pad8[4];
    unsigned char activateArg;
    unsigned char pendingFlag;
};

static int g_calls = 0;
static void* g_ecx = 0;
static void* g_edx = 0;
static unsigned char g_arg = 0;

void __fastcall Callee_9b8ef0(void* self, void* edxarg, unsigned char arg) {
    g_calls++; g_ecx = self; g_edx = edxarg; g_arg = arg;
}

void __fastcall OnInitialActivate(CTCVillage* self) {
    if (self->pendingFlag) {
        Callee_9b8ef0(self->field0, self->field4, self->activateArg);
        self->pendingFlag = 0;
    }
}

int main() {
    CTCVillage v;
    v.field0 = (void*)0x1111;
    v.field4 = (void*)0x2222;
    v.activateArg = 0x7F;
    v.pendingFlag = 0;

    OnInitialActivate(&v);
    if (g_calls != 0) { printf("FAIL nocall\n"); return 1; }
    if (v.pendingFlag != 0) { printf("FAIL flag1\n"); return 1; }

    v.pendingFlag = 1;
    OnInitialActivate(&v);
    if (g_calls != 1) { printf("FAIL call\n"); return 1; }
    if (g_ecx != (void*)0x1111) { printf("FAIL ecx\n"); return 1; }
    if (g_edx != (void*)0x2222) { printf("FAIL edx\n"); return 1; }
    if (g_arg != 0x7F) { printf("FAIL arg\n"); return 1; }
    if (v.pendingFlag != 0) { printf("FAIL flag2\n"); return 1; }

    printf("BEHAVIOR_OK_009b9300\n");
    return 0;
}