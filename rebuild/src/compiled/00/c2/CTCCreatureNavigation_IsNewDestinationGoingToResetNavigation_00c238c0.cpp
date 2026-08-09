struct C3DVector { float x, y, z; };

struct Callee {
    virtual bool s0();
    virtual bool s1();
    virtual bool s2();
    virtual bool s3();
    virtual bool s4();
    virtual bool s5();
    virtual bool Query(void* nav, C3DVector* v);
};

struct CTCCreatureNavigation {
    void* f0;
    Callee* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return f4->Query(this, dest);
}