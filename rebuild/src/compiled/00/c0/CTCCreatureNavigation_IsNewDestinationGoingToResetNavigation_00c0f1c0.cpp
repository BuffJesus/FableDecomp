struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void m00();
    virtual void m01();
    virtual void m02();
    virtual void m03();
    virtual void m04();
    virtual void m05();
    virtual void m06();
    virtual bool m07(CTCCreatureNavigation* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    Helper* h = this->field_4;
    return h->m07(this, dest);
}