struct C3DVector { float x,y,z; };

struct CTCCreatureNavigation;

struct Helper {
    virtual void  s0();
    virtual void  s1();
    virtual void  s2();
    virtual void  s3();
    virtual void  s4();
    virtual void  s5();
    virtual bool  slot6(CTCCreatureNavigation* outerThis, C3DVector* vec);
};

struct CTCCreatureNavigation {
    void*   pad0;
    Helper* helper;   // +4
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->helper->slot6(self, dest);
}