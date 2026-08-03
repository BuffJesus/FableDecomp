struct CManagerFwd;

struct IErrorMessageSink
{
    virtual void f0();
    virtual void f1();
    virtual void f2();
    virtual void UpdateErrorMessage(CManagerFwd* pOwner);
};

struct CManagerFwd
{
    void* vtbl;
    IErrorMessageSink* sink;
};

typedef CManagerFwd CManager;

void __fastcall CManager_UpdateErrorMessage_a(CManager* self, void* /*edx*/)
{
    self->sink->UpdateErrorMessage(self);
}