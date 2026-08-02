#include <stdio.h>

struct C3DMeshInfo
{
    int unused00[15];
    int physicsMeshIndex; // +0x3c

    bool __fastcall HasPhysicsMesh(void* edx_unused);
};

bool __fastcall C3DMeshInfo::HasPhysicsMesh(void* edx_unused)
{
    return physicsMeshIndex != -1;
}

int main()
{
    C3DMeshInfo obj;

    obj.physicsMeshIndex = -1;
    bool r1 = obj.HasPhysicsMesh(0);

    obj.physicsMeshIndex = 0;
    bool r2 = obj.HasPhysicsMesh(0);

    obj.physicsMeshIndex = 42;
    bool r3 = obj.HasPhysicsMesh(0);

    obj.physicsMeshIndex = -2;
    bool r4 = obj.HasPhysicsMesh(0);

    if (r1 == false && r2 == true && r3 == true && r4 == true)
    {
        printf("PHYSMESH_TEST_OK\n");
        return 0;
    }

    printf("PHYSMESH_TEST_FAIL %d %d %d %d\n", r1, r2, r3, r4);
    return 1;
}