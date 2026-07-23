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