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