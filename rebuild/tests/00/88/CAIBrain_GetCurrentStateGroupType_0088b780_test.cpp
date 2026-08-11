#include <cstdio>

typedef int EAIStateGroupType;
struct CAIStateGroup;
struct CAIStateGroupVtbl {
    void* slot0; void* slot1; void* slot2; void* slot3;
    void* slot4; void* slot5; void* slot6; void* slot7;
    EAIStateGroupType (__fastcall *GetType)(CAIStateGroup* self);
};
struct CAIStateGroup {
    CAIStateGroupVtbl* vtbl;
    char pad[0x10 - 4];
    unsigned char f10;
};
struct CAIBrain {
    char pad[0x38];
    CAIStateGroup* f38;
};

EAIStateGroupType __fastcall GetCurrentStateGroupType(CAIBrain* self);

static EAIStateGroupType __fastcall fakeGetType(CAIStateGroup*) { return 42; }

int main() {
    CAIStateGroupVtbl vt;
    vt.GetType = fakeGetType;
    CAIStateGroup g;
    g.vtbl = &vt;
    g.f10 = 0;

    CAIBrain b1;
    b1.f38 = &g;
    int r1 = GetCurrentStateGroupType(&b1);

    CAIBrain b2;
    b2.f38 = 0;
    int r2 = GetCurrentStateGroupType(&b2);

    CAIStateGroup g3;
    g3.vtbl = &vt;
    g3.f10 = 1;
    CAIBrain b3;
    b3.f38 = &g3;
    int r3 = GetCurrentStateGroupType(&b3);

    if (r1 == 42 && r2 == 0 && r3 == 0) {
        printf("0088b780_TEST PASS\n");
    } else {
        printf("FAIL r1=%d r2=%d r3=%d\n", r1, r2, r3);
    }
    return 0;
}