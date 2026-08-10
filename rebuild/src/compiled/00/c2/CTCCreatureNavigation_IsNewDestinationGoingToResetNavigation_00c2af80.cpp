struct C3DVector { float x, y, z; };

struct Sub {
    virtual bool m00();
    virtual bool m04();
    virtual bool m08();
    virtual bool m0c();
    virtual bool m10();
    virtual bool m14();
    virtual bool Check(void* origThis, C3DVector* p);   // slot 6 -> +0x18
};

struct CTCCreatureNavigation {
    void* f0;
    Sub*  f4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* p)
{
    return self->f4->Check(self, p);
}