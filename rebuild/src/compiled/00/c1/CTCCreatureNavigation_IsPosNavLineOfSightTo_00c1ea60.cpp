// Byte-exact reconstruction of CTCCreatureNavigation::IsPosNavLineOfSightTo @ 0x00c1ea60

struct C3DVector { float x, y, z; };

// arg1 type: a real object with a __fastcall member (0xc26260) taking a C3DVector*
struct Thing {
    void* Compute(C3DVector* pos);   // __fastcall member, returns pointer
};

struct CTCCreatureNavigation {
    // virtual slot 2 (vtable + 8): bool(void*)
    virtual void v0();
    virtual void v1();
    virtual bool CanReach(void* r);

    bool IsPosNavLineOfSightTo(C3DVector* pos, Thing* arg1);
};

bool CTCCreatureNavigation::IsPosNavLineOfSightTo(C3DVector* pos, Thing* arg1)
{
    return this->CanReach(arg1->Compute(pos));
}