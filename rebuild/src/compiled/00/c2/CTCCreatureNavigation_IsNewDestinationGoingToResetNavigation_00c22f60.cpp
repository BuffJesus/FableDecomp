struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Sub {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual int v6();
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest); // slot 0x1c
};

struct CTCCreatureNavigation {
    void* f0;
    Sub* f4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
    return f4->v7(this, dest);
}