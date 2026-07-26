#pragma optimize("s",on)
struct CBinaryStream {
    void PutInt(int v);
    void PutFloat(float v);
};

struct CCombatAbilityData {
    int   field_0;
    int   int4;
    float f8;
    float fc;
    int   int10;
};

void __fastcall CCombatAbilityData_TransferBinaryOut(CCombatAbilityData* self, void* edx, CBinaryStream* stream, int extra)
{
    (void)edx; (void)extra;
    stream->PutInt(self->int4);
    stream->PutFloat(*(volatile float*)&self->f8);
    stream->PutFloat(*(volatile float*)&self->fc);
    stream->PutInt(self->int10);
}