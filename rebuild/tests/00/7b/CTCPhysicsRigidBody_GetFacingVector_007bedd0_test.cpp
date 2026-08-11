#include <stdio.h>
#include <string.h>

struct C3DVector { unsigned long x, y, z; };

#pragma pack(push, 1)
struct CTCPhysicsRigidBody {
    char pad[0x51];
    unsigned long facing_x;   // +0x51
    unsigned long facing_y;   // +0x55
    unsigned long facing_z;   // +0x59
};
#pragma pack(pop)

void __fastcall GetFacingVector(CTCPhysicsRigidBody* self, int, C3DVector* out)
{
    out->x = self->facing_x;
    out->y = self->facing_y;
    out->z = self->facing_z;
}

int main()
{
    CTCPhysicsRigidBody obj;
    memset(&obj, 0, sizeof(obj));
    // store three float bit patterns to prove raw dword copy
    float fx = 1.5f, fy = -2.25f, fz = 3.75f;
    memcpy(&obj.facing_x, &fx, 4);
    memcpy(&obj.facing_y, &fy, 4);
    memcpy(&obj.facing_z, &fz, 4);

    C3DVector v;
    memset(&v, 0xAB, sizeof(v));
    GetFacingVector(&obj, 0, &v);

    float rx, ry, rz;
    memcpy(&rx, &v.x, 4);
    memcpy(&ry, &v.y, 4);
    memcpy(&rz, &v.z, 4);

    int ok = (v.x == obj.facing_x && v.y == obj.facing_y && v.z == obj.facing_z);
    printf("OK x=%.2f y=%.2f z=%.2f\n", rx, ry, rz);
    if (!ok) { printf("FAIL\n"); return 1; }
    return 0;
}