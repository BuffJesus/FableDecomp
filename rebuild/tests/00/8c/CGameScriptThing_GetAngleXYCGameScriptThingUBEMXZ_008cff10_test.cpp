#include <stdio.h>

struct CGameScriptThing {
    char pad[0x40];   // 0x00 .. 0x3F
    float mAngleXY;   // 0x40
};

float __fastcall CGameScriptThing__GetAngleXY(const CGameScriptThing* this_ptr)
{
    return this_ptr->mAngleXY;
}

int main()
{
    CGameScriptThing obj;
    obj.mAngleXY = 1.5f;
    float r = CGameScriptThing__GetAngleXY(&obj);
    if (r == 1.5f) {
        printf("CGAMESCRIPTTHING_008cff10_TEST PASS\n");
    } else {
        printf("FAIL got %f\n", r);
    }
    return 0;
}