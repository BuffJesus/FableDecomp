#include <stdio.h>

struct Obj { int tag; };

static Obj* g_lastSelf;
static void* g_lastArg2;
static int  g_lastFlag;
static int  g_callCount;

void __fastcall SubCall(Obj* self, void* arg2, unsigned char flag)
{
    g_lastSelf = self;
    g_lastArg2 = arg2;
    g_lastFlag = flag;
    ++g_callCount;
}

struct CTCVillage {
    Obj*          field0;
    void*         field4;
    unsigned char field8;
    unsigned char field9;
    unsigned char fieldA;
    unsigned char fieldB;
    unsigned char fieldC;
    unsigned char fieldD;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->fieldD) {
        SubCall(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}

int main()
{
    Obj o; o.tag = 7;
    int dummy = 0;

    CTCVillage v;
    v.field0 = &o;
    v.field4 = &dummy;
    v.fieldC = 0xC5;
    v.fieldD = 1;
    g_callCount = 0;
    OnInitialActivate(&v);
    if (g_callCount != 1) { printf("BAD count1\n"); return 1; }
    if (g_lastSelf != &o) { printf("BAD self\n"); return 1; }
    if (g_lastArg2 != &dummy) { printf("BAD arg2\n"); return 1; }
    if (g_lastFlag != 0xC5) { printf("BAD flag %d\n", g_lastFlag); return 1; }
    if (v.fieldD != 0) { printf("BAD flag not cleared\n"); return 1; }

    CTCVillage v2;
    v2.field0 = &o;
    v2.field4 = &dummy;
    v2.fieldC = 3;
    v2.fieldD = 0;
    g_callCount = 0;
    OnInitialActivate(&v2);
    if (g_callCount != 0) { printf("BAD count2\n"); return 1; }
    if (v2.fieldD != 0) { printf("BAD flag2\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}