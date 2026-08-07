struct Inner { char pad[0xA4]; float air; };
struct Outer { char pad[0xE0]; Inner* inner; };
float __fastcall GetAirResistance(Outer* self)
{
    return self->inner->air;
}