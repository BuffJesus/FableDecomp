struct CDecalGroup {
    int GetCount() const;  // extern method, returns int
};

struct Pair {
    CDecalGroup* group;
    int other;
};

struct CParticleEmitter {
    int a;
    int b;
    Pair* begin;   // +0x8
    Pair* end;     // +0xc
};

bool __fastcall CParticleEmitter_HasAllocatedDecals(const CParticleEmitter* self)
{
    Pair* it = self->begin;
    if (it != self->end) {
        do {
            if (it->group->GetCount() > 0)
                return true;
            ++it;
        } while (it != self->end);
    }
    return false;
}