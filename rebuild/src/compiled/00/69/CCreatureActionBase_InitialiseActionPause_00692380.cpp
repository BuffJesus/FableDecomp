// Byte-exact reconstruction of CCreatureActionBase::InitialiseActionPause @ 0x00692380

struct Registrar;

// 0x6f90b0 : returns a Registrar* (used as this for next call)
extern "C" Registrar* __cdecl GetActionRegistrar(void);

// 0x6f9720 : __fastcall member on Registrar taking (int id, void* desc)
struct Registrar {
    void RegisterAction(int id, void* desc);
};

struct CCreatureActionBase {
    // vtable slot +0x1c returns an int (action id/count)
    virtual void v0();  // +0x00
    virtual void v4();  // +0x04
    virtual void v8();  // +0x08
    virtual void vC();  // +0x0c
    virtual void v10(); // +0x10
    virtual void v14(); // +0x14
    virtual void v18(); // +0x18
    virtual int  GetActionId(); // +0x1c
};

void __fastcall InitialiseActionPause(CCreatureActionBase* self)
{
    int esi = self->GetActionId();
    if (esi > 0) {
        Registrar* r = GetActionRegistrar();
        r->RegisterAction(esi, (void*)0x13bac90);
    }
}