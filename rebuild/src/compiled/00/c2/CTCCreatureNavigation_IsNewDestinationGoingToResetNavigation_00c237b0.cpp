struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
    void* pad0;
    Helper* helper;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest) {
    Helper* h = self->helper;
    return h->slot6(self, dest);
}