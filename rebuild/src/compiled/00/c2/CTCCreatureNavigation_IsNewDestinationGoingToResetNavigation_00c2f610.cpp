struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool check(CTCCreatureNavigation* nav, C3DVector* v);
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* helper;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* v) {
    return this->helper->check(this, v);
}