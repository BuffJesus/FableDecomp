#include <cstdio>

struct CTCPhysicsData { char pad[0x38]; float airResistance; };
struct CTCPhysicsBase { char pad[0x28]; CTCPhysicsData* data; };

float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}

int main()
{
    CTCPhysicsData d;
    d.airResistance = 3.5f;
    CTCPhysicsBase b;
    b.data = &d;
    float r = CTCPhysicsBase_GetAirResistance(&b);
    if (r != 3.5f) { std::printf("FAIL got %f\n", r); return 1; }
    std::printf("CTCPhysicsBase_00670000_TEST PASS\n");
    return 0;
}