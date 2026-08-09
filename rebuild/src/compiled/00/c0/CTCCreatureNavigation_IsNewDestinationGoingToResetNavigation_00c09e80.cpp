struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Delegate {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool Check(CTCCreatureNavigation* orig, C3DVector* v);
};

struct CTCCreatureNavigation {
    void* f0;
    Delegate* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* v)
{
    return self->f4->Check(self, v);
}