#include <cstdio>

struct Callee { int calls; void* lastEdx; int lastArg; };

static void CalleeDo(Callee* ecx, void* edx, int arg) {
    ecx->calls++;
    ecx->lastEdx = edx;
    ecx->lastArg = arg;
}

struct CTCVillage {
    Callee* field0;
    void*   field4;
    char    pad8[4];
    unsigned char flagC;
    unsigned char flagD;
};

static void OnInitialActivate(CTCVillage* self) {
    if (self->flagD) {
        CalleeDo(self->field0, self->field4, self->flagC);
        self->flagD = 0;
    }
}

int main() {
    Callee c = {0,0,0};
    void* edxVal = (void*)0x1234;
    CTCVillage v;
    v.field0 = &c;
    v.field4 = edxVal;
    v.flagC = 42;
    v.flagD = 1;

    OnInitialActivate(&v);
    if (c.calls != 1) { printf("BAD calls\n"); return 1; }
    if (c.lastEdx != edxVal) { printf("BAD edx\n"); return 1; }
    if (c.lastArg != 42) { printf("BAD arg\n"); return 1; }
    if (v.flagD != 0) { printf("BAD flag not cleared\n"); return 1; }

    OnInitialActivate(&v);
    if (c.calls != 1) { printf("BAD extra call\n"); return 1; }

    Callee c2 = {0,0,0};
    CTCVillage v2;
    v2.field0 = &c2; v2.field4 = 0; v2.flagC = 7; v2.flagD = 0;
    OnInitialActivate(&v2);
    if (c2.calls != 0) { printf("BAD inactive called\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}