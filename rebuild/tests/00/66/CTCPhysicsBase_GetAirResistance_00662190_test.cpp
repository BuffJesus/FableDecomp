#include <cstdio>
struct CTCPhysicsData { char pad_f4[0xf4]; float airResistance; };
struct CTCPhysicsBase { char pad_e0[0xe0]; CTCPhysicsData* data; };
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
    if (r == 3.5f) { std::printf("CTCPhysicsBase_00662190_TEST PASS\n"); return 0; }
    std::printf("FAIL got %f\n", r);
    return 1;
}