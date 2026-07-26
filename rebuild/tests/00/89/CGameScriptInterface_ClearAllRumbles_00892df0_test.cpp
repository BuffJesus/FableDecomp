#include <cstdio>

struct CGameScriptInterface {
    void* vt;
    void* f4;
    void* f8;
    void* fc;
    unsigned char _pad0[0x4];
    void* member14;
};

struct Obj;
struct ObjVtbl {
    void* slots0[0x12];
    void (__fastcall *slot48)(Obj*);
};
struct Obj {
    ObjVtbl* vt;
    unsigned char _pad[0x91 - 4];
    unsigned char flags;
};

struct TableEntry {
    Obj* obj;
    void* other;
};
struct Manager {
    unsigned char _pad[0x2c];
    TableEntry* table;
};

Manager* g_manager;

static Obj* g_gotObj;
static int g_index;
static int g_slotCalls;

void* __fastcall GetA(void* p) { return p; }
Obj* __fastcall GetObj(void* a) { return g_gotObj; }
int __fastcall GetIndex(Obj* o) { return g_index; }

void __fastcall Slot48(Obj* o) { g_slotCalls++; }

void __fastcall CGameScriptInterface_ClearAllRumbles(CGameScriptInterface* self)
{
    void* a = GetA(self->member14);
    Obj* o = GetObj(a);
    if (o == 0)
        return;
    if (o->flags & 1)
        return;
    int idx = GetIndex(o);
    Obj* tgt = g_manager->table[idx].obj;
    tgt->vt->slot48(tgt);
}

int main() {
    // Case 1: null obj -> no crash, no slot call
    g_gotObj = 0;
    CGameScriptInterface self;
    self.member14 = (void*)0x1234;
    CGameScriptInterface_ClearAllRumbles(&self);
    if (g_slotCalls != 0) { std::printf("FAIL null\n"); return 1; }

    // Case 2: flag bit set -> return early
    ObjVtbl vt; vt.slot48 = &Slot48;
    Obj obj; obj.vt = &vt; obj.flags = 1;
    g_gotObj = &obj;
    CGameScriptInterface_ClearAllRumbles(&self);
    if (g_slotCalls != 0) { std::printf("FAIL flagset\n"); return 1; }

    // Case 3: valid -> tail-call slot48 on table[idx].obj
    obj.flags = 0;
    Obj tgt; tgt.vt = &vt; tgt.flags = 0;
    Obj* ptgt = &tgt + 0;
    TableEntry entries[4];
    entries[2].obj = ptgt;
    Manager mgr; mgr.table = entries;
    g_manager = &mgr;
    g_index = 2;
    CGameScriptInterface_ClearAllRumbles(&self);
    if (g_slotCalls != 1) { std::printf("FAIL noslot\n"); return 1; }

    std::printf("CGameScriptInterface_00892df0_TEST PASS\n");
    return 0;
}