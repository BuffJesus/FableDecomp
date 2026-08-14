typedef unsigned long ulong;

struct CEngineInternalPrimitiveParticleGroup {
    char pad[0xa4];
    unsigned long type;
};

ulong __fastcall GetRenderLayerMask(CEngineInternalPrimitiveParticleGroup* self)
{
    switch (self->type) {
    case 0:
        return 0x8000;
    case 1:
        return 0x100000;
    default:
        return 0x80000;
    }
}