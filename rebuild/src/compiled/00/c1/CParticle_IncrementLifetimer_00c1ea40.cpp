
struct CParticle {
    char pad[0x1e];
    unsigned short lifetimer;
};
void __fastcall IncrementLifetimer(CParticle* self)
{
    --self->lifetimer;
}