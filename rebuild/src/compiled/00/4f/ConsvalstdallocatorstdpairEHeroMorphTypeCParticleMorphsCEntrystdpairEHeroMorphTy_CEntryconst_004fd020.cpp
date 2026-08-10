struct Entry { char pad0[0x10]; void* p; char pad1[0x48-0x14]; };
struct Obj { char pad0[0x20]; Entry* table; };

void __fastcall sub_821850(void* self);

void __fastcall ConsVal_ParticleMorphs(Obj* self, int /*edx*/, int index)
{
    sub_821850(self->table[index].p);
}