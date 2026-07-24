struct CGameScriptInterface {
    void* vt;        // +0x0
    void* f4;        // +0x4
    void* f8;        // +0x8
    void* fc;        // +0xc
    unsigned char _pad0[0x4];
    void* member14;  // +0x14
};

struct Obj;
struct ObjVtbl {
    void* slots0[0x12];        // 0..0x44
    void (__fastcall *slot48)(Obj*); // +0x48
};
struct Obj {
    ObjVtbl* vt;
    unsigned char _pad[0x91 - 4];
    unsigned char flags;       // +0x91
};

struct TableEntry {
    Obj* obj;                  // +0x0
    void* other;               // +0x4  (8-byte stride)
};
struct Manager {
    unsigned char _pad[0x2c];
    TableEntry* table;         // +0x2c
};

extern Manager* g_manager;

extern void* __fastcall GetA(void* p);           // 0xffbb6b80
extern Obj*  __fastcall GetObj(void* a);         // 0xffbf4fd0
extern int   __fastcall GetIndex(Obj* o);        // 0xffc34eb0

void __fastcall CGameScriptInterface_ClearAllRumbles(CGameScriptInterface* self)
{
    void* a = GetA(self->member14);
    Obj* o = GetObj(a);
    if (o == 0)
        return;
    if (o->flags & 1)
        return;
    int idx = GetIndex(o);
    Obj* target = g_manager->table[idx].obj;
    target->vt->slot48(target);
}