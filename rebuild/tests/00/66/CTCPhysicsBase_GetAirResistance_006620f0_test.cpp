#include <cstdio>

struct CPhysicsData {
    char pad[0xa8];
    float airResistance;
};

struct CTCPhysicsBase {
    char pad[0xe0];
    CPhysicsData* data;
};

float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}

int main()
{
    CPhysicsData d;
    d.airResistance = 3.5f;
    CTCPhysicsBase b;
    b.data = &d;
    float r = CTCPhysicsBase_GetAirResistance(&b);
    if (r == 3.5f) {
        std::printf("CTCPhysicsBase_006620f0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}