struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Helper {
    virtual void s0(); virtual void s1(); virtual void s2();
    virtual void s3(); virtual void s4(); virtual void s5();
    virtual bool s6(CTCCreatureNavigation* nav, C3DVector* v);
};
struct CTCCreatureNavigation {
    void* field0;
    Helper* field4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return field4->s6(this, dest);
}