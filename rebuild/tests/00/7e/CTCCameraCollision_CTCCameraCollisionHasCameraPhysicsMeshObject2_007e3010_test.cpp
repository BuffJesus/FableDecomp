#include <cstdio>

struct CTCCameraCollision {
    void* field_0;
    void* field_4;
    void* field_8;
    struct Inner { char pad[0x28]; void* field_28; } *field_c;

    unsigned char __fastcall HasCameraPhysicsMeshObject_2(void* /*edx*/);
};

unsigned char __fastcall CTCCameraCollision::HasCameraPhysicsMeshObject_2(void* /*edx*/)
{
    return field_c->field_28 != 0;
}

int main()
{
    CTCCameraCollision::Inner innerNonNull;
    innerNonNull.field_28 = (void*)0x1234;
    CTCCameraCollision obj1;
    obj1.field_c = &innerNonNull;
    unsigned char r1 = obj1.HasCameraPhysicsMeshObject_2(0);

    CTCCameraCollision::Inner innerNull;
    innerNull.field_28 = 0;
    CTCCameraCollision obj2;
    obj2.field_c = &innerNull;
    unsigned char r2 = obj2.HasCameraPhysicsMeshObject_2(0);

    if (r1 == 1 && r2 == 0) {
        printf("CAMPHYSMESH_OK\n");
    } else {
        printf("CAMPHYSMESH_FAIL r1=%d r2=%d\n", (int)r1, (int)r2);
    }
    return 0;
}