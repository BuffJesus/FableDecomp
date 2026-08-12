#include <stdio.h>

struct C3DVector {
    float x;
    float y;
    float z;
};

struct CCombatAction_GenericStrikeResponseBase {
    char pad[0x11c];
    C3DVector hitPos;

    void SetHitPosition(C3DVector pos);
};

int main()
{
    CCombatAction_GenericStrikeResponseBase obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;

    C3DVector v;
    v.x = 1.5f; v.y = -2.25f; v.z = 42.0f;

    obj.SetHitPosition(v);

    if (obj.hitPos.x == 1.5f && obj.hitPos.y == -2.25f && obj.hitPos.z == 42.0f) {
        printf("SETHITPOS_OK\n");
        return 0;
    }
    printf("SETHITPOS_FAIL\n");
    return 1;
}