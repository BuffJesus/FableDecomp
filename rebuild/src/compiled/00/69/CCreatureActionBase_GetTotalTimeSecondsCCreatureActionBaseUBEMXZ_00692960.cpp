// Byte-exact reconstruction of CCreatureActionBase::GetTotalTimeSeconds @ 0x00692960

struct SubB;                 // object living at self+8

// object returned by call @0xa01b50; Convert is __fastcall member @0x6627f0
struct Helper {
    float Convert(int arg);  // 0x6627f0  (this in ecx, arg pushed)
};

// extern engine callee @0xa01b50
extern Helper* __fastcall GetHelperFromSubB(SubB* subb);

struct CCreatureActionBase {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual int  v7();       // slot at vtable+0x1c
    virtual float GetTotalTimeSeconds() const;   // ?GetTotalTimeSeconds@CCreatureActionBase@@UBEMXZ
};

float CCreatureActionBase::GetTotalTimeSeconds() const
{
    CCreatureActionBase* self = const_cast<CCreatureActionBase*>(this);
    Helper* h = GetHelperFromSubB((SubB*)((char*)self + 8));
    int ticks = self->v7();
    return h->Convert(ticks);
}