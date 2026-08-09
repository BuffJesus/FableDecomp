struct C3DVector { float x, y, z; };

struct INavTarget {
    virtual bool slot0();
    virtual bool slot1();
    virtual bool slot2();
    virtual bool slot3();
    virtual bool slot4();
    virtual bool slot5();
    virtual bool Query(void* self, C3DVector* v); // slot6 -> [edx+0x18]
};

struct CTCCreatureNavigation {
    void* field_0;
    INavTarget* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return field_4->Query(this, dest);
}