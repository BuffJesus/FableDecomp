#include <cstdio>

struct Inner { int tag; };

struct CTCVillage {
    Inner *field0;
    int    field4;
    char   pad8[4];
    char   fieldC;
    char   fieldD;
};

static Inner   *g_calledSelf = 0;
static int      g_calledEdx  = -1;
static int      g_calledArg  = -1;
static int      g_callCount  = 0;

void __fastcall Helper(Inner *self, int edxVal, unsigned char arg)
{
    g_calledSelf = self;
    g_calledEdx  = edxVal;
    g_calledArg  = arg;
    ++g_callCount;
}

void __fastcall OnInitialActivate(CTCVillage *self)
{
    if (self->fieldD) {
        Helper(self->field0, self->field4, (unsigned char)self->fieldC);
        self->fieldD = 0;
    }
}

int main()
{
    Inner inner; inner.tag = 77;

    // Case 1: guard set -> Helper called, arg = fieldC (as unsigned char), flag cleared.
    CTCVillage v;
    v.field0 = &inner;
    v.field4 = 0x1234;
    v.fieldC = (char)0xAB;   // 171
    v.fieldD = 1;
    OnInitialActivate(&v);
    bool ok1 = (g_callCount == 1) && (g_calledSelf == &inner) &&
               (g_calledEdx == 0x1234) && (g_calledArg == 0xAB) &&
               (v.fieldD == 0);

    // Case 2: guard clear -> Helper NOT called, flag stays 0.
    g_callCount = 0;
    CTCVillage v2;
    v2.field0 = &inner;
    v2.field4 = 5;
    v2.fieldC = 3;
    v2.fieldD = 0;
    OnInitialActivate(&v2);
    bool ok2 = (g_callCount == 0) && (v2.fieldD == 0);

    if (ok1 && ok2) {
        printf("BEHAVIOUR_OK_a84b60\n");
    } else {
        printf("BEHAVIOUR_FAIL ok1=%d ok2=%d\n", (int)ok1, (int)ok2);
    }
    return 0;
}