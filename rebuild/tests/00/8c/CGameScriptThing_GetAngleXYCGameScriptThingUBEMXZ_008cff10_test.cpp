#include "engine/CGameScriptThing.h"
#include <stdio.h>


float __fastcall CGameScriptThing__GetAngleXY(const CGameScriptThing* this_ptr)
{
    return this_ptr->AngleXY;
}

int main()
{
    CGameScriptThing obj;
    obj.AngleXY = 1.5f;
    float r = CGameScriptThing__GetAngleXY(&obj);
    if (r == 1.5f) {
        printf("CGAMESCRIPTTHING_008cff10_TEST PASS\n");
    } else {
        printf("FAIL got %f\n", r);
    }
    return 0;
}