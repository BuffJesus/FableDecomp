#include <cstdio>

struct C3DMeshInfo {
    char pad[0x28];
    void* physicsMesh;
    bool HasPhysicsMesh();
};

bool C3DMeshInfo::HasPhysicsMesh()
{
    return this->physicsMesh != 0;
}

int main()
{
    C3DMeshInfo a;
    int dummy = 5;
    a.physicsMesh = 0;
    bool r1 = a.HasPhysicsMesh();
    a.physicsMesh = &dummy;
    bool r2 = a.HasPhysicsMesh();
    if (r1 == false && r2 == true) {
        printf("HASPHYSICSMESH_OK\n");
        return 0;
    }
    printf("HASPHYSICSMESH_FAIL\n");
    return 1;
}