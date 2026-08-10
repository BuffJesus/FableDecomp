struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Sub {
    // vtable slot 0x1c/4 = 7
    virtual void pad0();
    virtual void pad1();
    virtual void pad2();
    virtual void pad3();
    virtual void pad4();
    virtual void pad5();
    virtual void pad6();
    virtual bool Query(CTCCreatureNavigation* outer, C3DVector* v);
};

struct CTCCreatureNavigation {
    void* m_x0;      // +0
    Sub* m_sub;      // +4
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return m_sub->Query(this, dest);
}