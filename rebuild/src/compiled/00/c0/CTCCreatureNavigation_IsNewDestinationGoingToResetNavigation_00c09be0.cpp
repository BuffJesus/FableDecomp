struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct IInner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* dest); // +0x1c
};

struct CTCCreatureNavigation {
    void* pad0;      // +0x00
    IInner* inner;   // +0x04
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->inner->slot7(this, dest);
}