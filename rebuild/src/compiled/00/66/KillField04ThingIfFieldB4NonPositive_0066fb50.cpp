struct CThing_0066fb50
{
    unsigned char prefix[0xb4];
    float field_b4;

    void Kill(bool immediate);
};

struct Object_0066fb50
{
    unsigned char prefix[4];
    CThing_0066fb50* thing;
};

extern "C" void __fastcall
KillField04ThingIfFieldB4NonPositive_0066fb50(
    Object_0066fb50* self)
{
    CThing_0066fb50* thing = self->thing;

    if (thing != 0 && thing->field_b4 <= 0.0f)
        thing->Kill(true);
}
