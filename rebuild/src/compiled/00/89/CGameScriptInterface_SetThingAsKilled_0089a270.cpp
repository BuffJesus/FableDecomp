struct KeyPair { int key; void* value; };
struct VMap { KeyPair* begin; KeyPair* end; };

struct Thing {
    char pad0[0x20];
    unsigned int flags20;   // +0x20
    char pad24[0x20];       // to 0x44
    VMap map44;             // +0x44
};

struct GSI {
    char pad0[0x14];
    void* member14;         // +0x14
};

struct ArgVT;
struct Arg { ArgVT* vt; };
struct ArgVT {
    char pad[0x2c];
    int (__fastcall* slot2c)(Arg* self);   // +0x2c
};

struct Iface;
struct IntBox { int v; };
struct PtrBox { const int* p; };

extern void* __fastcall Sub1(void* self);
extern Thing* __fastcall Sub2(void* self);
extern KeyPair* __fastcall VMap_LowerBound(VMap* self, PtrBox key);
extern void __fastcall Iface_Do(Iface* self, IntBox v);

void __fastcall CGameScriptInterface_SetThingAsKilled(const GSI* self, int edx_unused, Arg* thing)
{
    int key;
    (void)edx_unused;
    void* a = Sub1(((GSI*)self)->member14);
    Thing* obj = Sub2(a);
    if (obj != 0 && (obj->flags20 & 0x100000)) {
        VMap* m = &obj->map44;
        PtrBox kb;
        key = 0x14;
        kb.p = &key;
        KeyPair* e = VMap_LowerBound(m, kb);
        KeyPair* end = m->end;
        if (e == end || e->key > 0x14)
            e = end;
        Iface* iv = (Iface*)e->value;
        IntBox box;
        box.v = thing->vt->slot2c(thing);
        Iface_Do(iv, box);
    }
}