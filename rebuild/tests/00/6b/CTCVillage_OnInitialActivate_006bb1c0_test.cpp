#include <cstdio>

struct Inner { int calls; int lastArg; unsigned char lastV; };

static int g_calls = 0;
static Inner* g_lastSelf = 0;
static int g_lastEdx = 0;
static unsigned char g_lastV = 0;

void __fastcall Inner_Do(Inner* self, int edx, unsigned char v){
    g_calls++; g_lastSelf=self; g_lastEdx=edx; g_lastV=v;
}

struct CTCVillage {
    Inner* p0; int v4;
    char pad8,pad9,pada,padb;
    unsigned char bytec; char flagd;
};

void __fastcall OnInitialActivate(CTCVillage* self){
    if (self->flagd) {
        Inner_Do(self->p0, self->v4, self->bytec);
        self->flagd = 0;
    }
}

int main(){
    Inner innerObj = {0,0,0};
    Inner* pin = &innerObj;
    CTCVillage v;
    CTCVillage* pv = &v;
    v.p0 = pin; v.v4 = 0x1234; v.bytec = 0x7f; v.flagd = 1;
    v.pad8=v.pad9=v.pada=v.padb=0;

    OnInitialActivate(pv);
    // flag was set: should have called once, cleared flag
    if (g_calls!=1) { printf("FAIL calls\n"); return 1; }
    if (g_lastSelf!=pin) { printf("FAIL self\n"); return 1; }
    if (g_lastEdx!=0x1234) { printf("FAIL edx\n"); return 1; }
    if (g_lastV!=0x7f) { printf("FAIL v\n"); return 1; }
    if (v.flagd!=0) { printf("FAIL flag not cleared\n"); return 1; }

    // second call: flag now 0, no call
    OnInitialActivate(pv);
    if (g_calls!=1) { printf("FAIL second call\n"); return 1; }

    // flag 0 from start: no call
    g_calls=0; v.flagd=0;
    OnInitialActivate(pv);
    if (g_calls!=0) { printf("FAIL noflag\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}