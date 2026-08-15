// Byte-exact reconstruction of CMiniMapDisplay::InitialiseOnRegionLoad @ 0x0082a180

struct CCharString;

// Global singleton object with a vtable.
struct GSlot0x24Ret; // bool at vtable slot +0x24
struct GObj {
    // vtable slot layout (each slot 4 bytes)
    // +0x24 => bool IsSomething()
    unsigned char pad_to_24[0x24];
    void *member_0x24; // object pointer at +0x24
};

struct GVtbl {
    void *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8;
    unsigned char (__fastcall *slot24)(void *self); // +0x24
};

struct GObjV {
    GVtbl *vtbl;
    unsigned char pad[0x20];
    void *member_0x24; // +0x24
};

struct SubVtbl {
    void *s0, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *sa, *sb, *sc;
    void * (__fastcall *slot34)(void *self); // +0x34 = slot 13
};

struct SubObj {
    SubVtbl *vtbl;
};

// Obj returned by slot34 — has thiscall methods 0x4fb150 / 0x4fc180.
struct Obj {
    void *m4fb150();            // this-call, no stack args
    void *m4fc180(void *a);     // this-call, one stack arg
};

// Self type: has the thiscall method at 0x829d90.
struct Self {
    void m829d90(void *a, CCharString *cs, int flag); // this-call, 3 stack args
};

extern GObjV *g_singleton; // at 0x13b86a0

void __fastcall InitialiseOnRegionLoad(Self *self, void *edx, CCharString *cs)
{
    if (g_singleton->vtbl->slot24(g_singleton))
        return;
    SubObj *sub = (SubObj *)((void **)g_singleton)[9]; // g_singleton->member_0x24
    Obj *obj = (Obj *)sub->vtbl->slot34(sub);
    void *a = obj->m4fb150();
    void *b = obj->m4fc180(a);
    self->m829d90(b, cs, 1);
}