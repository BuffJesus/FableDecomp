#include <stdio.h>

struct CTCCameraCollisionField
{
    char pad0[0x28];
    void* field0x28;
};

class CTCCameraCollision
{
public:
    bool HasCameraPhysicsMeshObject();

    char pad0[0xc];
    CTCCameraCollisionField* field0xc;
};

bool CTCCameraCollision::HasCameraPhysicsMeshObject()
{
    return field0xc->field0x28 != 0 ? true : false;
}

int main()
{
    CTCCameraCollisionField target_nonzero;
    target_nonzero.field0x28 = (void*)0x1234;

    CTCCameraCollisionField target_zero;
    target_zero.field0x28 = 0;

    CTCCameraCollision obj_true;
    obj_true.field0xc = &target_nonzero;

    CTCCameraCollision obj_false;
    obj_false.field0xc = &target_zero;

    bool r1 = obj_true.HasCameraPhysicsMeshObject();
    bool r2 = obj_false.HasCameraPhysicsMeshObject();

    if (r1 == true && r2 == false)
    {
        printf("TESTOK_HASCAMPHYSMESH\n");
        return 0;
    }
    printf("TESTFAIL r1=%d r2=%d\n", (int)r1, (int)r2);
    return 1;
}