struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* v); // vtable [+0x1c]
};

struct CTCCreatureNavigation {
    void* f0;
    Helper* f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* v) {
    return self->f4->slot7(self, v);
}