struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->field_4->v6(this, dest);
}