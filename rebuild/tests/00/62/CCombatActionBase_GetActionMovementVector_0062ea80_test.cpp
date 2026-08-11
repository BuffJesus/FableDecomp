#include <cstdio>

struct C3DVector {
    float x;
    float y;
    float z;
    C3DVector(float ix, float iy, float iz) : x(ix), y(iy), z(iz) {}
};

struct CCombatActionBase {
    C3DVector GetActionMovementVector();
};

C3DVector CCombatActionBase::GetActionMovementVector()
{
    return C3DVector(0.0f, 1.0f, 0.0f);
}

int main()
{
    CCombatActionBase obj;
    C3DVector r = obj.GetActionMovementVector();
    if (r.x == 0.0f && r.y == 1.0f && r.z == 0.0f) {
        printf("MOVEVEC_OK\n");
        return 0;
    }
    printf("MOVEVEC_FAIL x=%f y=%f z=%f\n", r.x, r.y, r.z);
    return 1;
}