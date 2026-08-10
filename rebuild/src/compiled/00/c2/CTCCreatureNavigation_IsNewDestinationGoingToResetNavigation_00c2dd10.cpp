struct C3DVector { float x, y, z; };

struct IHelper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    // slot 7 at vtbl+0x1c
    virtual bool CheckReset(void* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* field0;
    IHelper* field4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->field4->CheckReset(this, dest);
}