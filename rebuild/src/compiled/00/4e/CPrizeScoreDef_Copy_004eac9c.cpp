#pragma optimize("s",on)
// CPrizeScoreDef::Copy @ 0x004eac9c
struct CDefClassBase;
struct CPrizeScoreDef;

// base class Copy at 0x431f10 : void __fastcall CDefClassBase::Copy(CDefClassBase* self, CDefClassBase const* src)
void __fastcall base_dup(CPrizeScoreDef* self, void* edx, CDefClassBase const* src);

struct CPrizeScoreDef {
    char pad[0x28];
    int f28;
    int f2c;
};

void __fastcall CPrizeScoreDef_Copy(CPrizeScoreDef* self, void* edx, CDefClassBase const* src)
{
    base_dup(self, edx, src);
    const CPrizeScoreDef* s = (const CPrizeScoreDef*)src;
    self->f28 = s->f28;
    self->f2c = s->f2c;
}