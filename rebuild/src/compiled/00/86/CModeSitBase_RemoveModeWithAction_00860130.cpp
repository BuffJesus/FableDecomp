// Byte-exact reconstruction of CModeSitBase::RemoveModeWithAction @ 0x00860130

struct CCreatureActionBase {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual int  GetSomething();   // slot 0x2c
};

// callee at 0x668d90 modeled as a natural thiscall member (this in ecx, arg on stack)
struct CSubObject {
    int dummy;
    void Sub_668d90(int arg);   // 0x668d90
};

struct CModeSitBase {
    unsigned int flags;   // +0x00
    char pad[0x1c];       // +0x04 .. +0x1f
    CSubObject sub;       // +0x20
};

void __fastcall RemoveModeWithAction(CModeSitBase* self, void* /*edx*/, CCreatureActionBase* action)
{
    self->flags |= 8;
    int r = action->GetSomething();
    self->sub.Sub_668d90(r);
}