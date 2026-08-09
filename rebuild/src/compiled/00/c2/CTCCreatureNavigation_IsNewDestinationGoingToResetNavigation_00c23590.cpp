struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* pos);
};

struct CTCCreatureNavigation {
    void* pad0;
    Helper* nav;   // +4
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* pos)
{
    return self->nav->slot6(self, pos);
}