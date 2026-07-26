#include <cstdio>

struct KeyPair { int key; void* value; };
struct VMap { KeyPair* begin; KeyPair* end; };

struct Thing {
    char pad0[0x20];
    unsigned int flags20;
    char pad24[0x20];
    VMap map44;
};

struct GSI {
    char pad0[0x14];
    void* member14;
};

struct ArgVT;
struct Arg { ArgVT* vt; };
struct ArgVT {
    char pad[0x2c];
    int (__fastcall* slot2c)(Arg* self);
};

struct Iface;
struct IntBox { int v; };
struct PtrBox { const int* p; };

static Thing g_obj;
static KeyPair g_entry;
static int g_iface_val = -1;
static int g_slot_called = 0;

void* __fastcall Sub1(void* self) { return self; }
Thing* __fastcall Sub2(void* self) { (void)self; return &g_obj; }
KeyPair* __fastcall VMap_LowerBound(VMap* self, PtrBox key) { (void)self; (void)key; return &g_entry; }
void __fastcall Iface_Do(Iface* self, IntBox v) { g_iface_val = (int)(long)self + v.v; }

int g_slot_ret = 7;
int __fastcall slot2c_impl(Arg* self) { (void)self; g_slot_called = 1; return g_slot_ret; }

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

int main() {
    GSI gsi; gsi.member14 = (void*)&gsi;
    g_obj.flags20 = 0x100000;
    g_obj.map44.begin = &g_entry;
    g_obj.map44.end = (KeyPair*)0;
    g_entry.key = 0x10;
    g_entry.value = (void*)0x1000;

    ArgVT vt; vt.slot2c = (int(__fastcall*)(Arg*))slot2c_impl;
    Arg arg; arg.vt = &vt;

    CGameScriptInterface_SetThingAsKilled(&gsi, 0, &arg);

    if (g_slot_called == 1 && g_iface_val == 0x1000 + 7) {
        std::printf("CGameScriptInterface_0089a270_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL slot=%d ifaceval=%d\n", g_slot_called, g_iface_val);
    return 1;
}