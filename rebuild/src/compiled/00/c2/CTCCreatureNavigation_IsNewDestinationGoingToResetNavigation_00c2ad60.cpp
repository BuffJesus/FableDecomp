
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

struct INavHelper {
    virtual bool m00();
    virtual bool m04();
    virtual bool m08();
    virtual bool m0c();
    virtual bool m10();
    virtual bool m14();
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* dest);
};

struct CTCCreatureNavigation {
    void* pad0;
    INavHelper* helper; // +4
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->helper->slot6(this, dest);
}