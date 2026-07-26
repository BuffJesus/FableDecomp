struct CDecalThing {
    int __fastcall GetDecalCount();
};

struct Pair {
    CDecalThing* obj;
    void* other;
};

struct CParticleEmitter {
    char pad[8];
    Pair* begin;
    Pair* end;
};

bool __fastcall CParticleEmitter_HasAllocatedDecals(CParticleEmitter* self)
{
    Pair* it = self->begin;
    if (it != self->end) {
        do {
            if (it->obj->GetDecalCount() > 0)
                return true;
            it++;
        } while (it != self->end);
    }
    return false;
}