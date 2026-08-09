struct C3DVector { float x, y, z; };

struct Inner {
    // vtable auto at offset 0
    virtual bool method0();      // +0x00
    virtual bool method1();      // +0x04
    virtual bool method2();      // +0x08
    virtual bool method3();      // +0x0c
    virtual bool method4();      // +0x10
    virtual bool method5();      // +0x14
    virtual bool method6();      // +0x18
    virtual bool Check(void* nav, C3DVector* dest);  // +0x1c
};

struct CTCCreatureNavigation {
    void*  field_0;
    Inner* field_4;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(
        CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->field_4->Check(self, dest);
}