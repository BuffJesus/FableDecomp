struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool v6(CTCCreatureNavigation* owner, C3DVector* dest);  // slot +0x18
};

struct CTCCreatureNavigation {
    void* pad0;
    Sub* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->f4->v6(self, dest);
}