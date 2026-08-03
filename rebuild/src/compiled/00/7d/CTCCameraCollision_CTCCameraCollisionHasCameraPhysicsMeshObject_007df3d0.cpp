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