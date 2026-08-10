struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct NavSub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool IsReset(CTCCreatureNavigation* nav, C3DVector* dest);  // slot 7 = +0x1c
};

struct CTCCreatureNavigation {
    void* f0;
    NavSub* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->f4->IsReset(self, dest);
}