struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct ISubNav {
    virtual bool v0();
    virtual bool v1();
    virtual bool v2();
    virtual bool v3();
    virtual bool v4();
    virtual bool v5();
    virtual bool Check(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
    void* f0;
    ISubNav* sub;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
    return sub->Check(this, dest);
}