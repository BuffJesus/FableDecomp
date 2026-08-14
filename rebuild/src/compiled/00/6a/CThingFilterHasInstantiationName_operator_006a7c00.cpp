// Byte-exact reconstruction of CThingFilter_HasInstantiationName::operator() @ 0x006a7c00

struct CThing;

// The object at thing->+0x70 exposes a virtual method at vtable slot 0x28 (index 10):
//   virtual ??? Method(CThing** ppThing, void* nameKey);  // __fastcall
struct NameObj {
    virtual void* v00();
    virtual void* v04();
    virtual void* v08();
    virtual void* v0c();
    virtual void* v10();
    virtual void* v14();
    virtual void* v18();
    virtual void* v1c();
    virtual void* v20();
    virtual void* v24();
    virtual void* Method(CThing** ppThing, void* nameKey); // slot 0x28
};

struct CThing {
    char pad[0x70];
    NameObj* nameObj;   // +0x70
};

struct CThingFilter_HasInstantiationName {
    void* nameKey;   // +0x00
};

// helper at 0x4b7980, __fastcall(ecx = result of vcall), returns bool
extern "C" bool __fastcall helper_4b7980(void* ecx);

bool __fastcall op(CThingFilter_HasInstantiationName* self, void* /*edx*/, CThing* thing)
{
    void* key = self->nameKey;             // mov ecx,[ecx]
    NameObj* obj = thing->nameObj;         // [esp+4] -> [+0x70]
    void* r = obj->Method(&thing, key);    // virtual thiscall, slot 0x28
    return helper_4b7980(r);
}