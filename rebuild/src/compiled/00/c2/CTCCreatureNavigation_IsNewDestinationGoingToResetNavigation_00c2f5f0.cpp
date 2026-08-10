struct C3DVector { float x, y, z; };

struct CTCCreatureNavigation;

struct NavDelegate {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* pad0;
    NavDelegate* field4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return field4->v7(this, dest);
}