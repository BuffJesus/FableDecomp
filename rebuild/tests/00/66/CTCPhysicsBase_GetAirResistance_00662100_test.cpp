#include <cstdio>

struct CPhysData { char pad[0xac]; float airResistance; };
struct CTCPhysicsBase { char pad[0xe0]; CPhysData* data; };

float __fastcall CTCPhysicsBase_GetAirResistance(const CTCPhysicsBase* self)
{
    return self->data->airResistance;
}

int main()
{
    CPhysData d;
    d.airResistance = 3.5f;
    CTCPhysicsBase b;
    b.data = &d;
    float r = CTCPhysicsBase_GetAirResistance(&b);
    if (r == 3.5f) {
        std::printf("CTCPhysicsBase_00662100_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %f\n", r);
    return 1;
}