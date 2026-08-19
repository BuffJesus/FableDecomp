struct TCEntry {
    int key;        // +0x00
    void* iface;    // +0x04
};

struct VMap {
    TCEntry* begin; // +0x00 (at Target+0x44)
    TCEntry* end;   // +0x04 (at Target+0x48)
    TCEntry* LowerBound(const int* key);
};

struct TargetT {
    unsigned char _pad0[0x2c];
    unsigned int flags2c;   // +0x2c
    unsigned char _pad30[0x14];
    VMap vmap;              // +0x44 begin, +0x48 end
};

struct GSI;
typedef TargetT* (__fastcall *GetTargetFn)(GSI*);
struct GSIVtbl {
    unsigned char _pad[0x2c];
    GetTargetFn slot2c; // +0x2c
};
struct GSI {
    GSIVtbl* vt;
};

extern "C" void __fastcall Iface_Do(void* self);

bool __stdcall CGameScriptInterface_ManuallyTriggerTrap(GSI* self)
{
    TargetT* t = self->vt->slot2c(self);
    if (t && (t->flags2c & 0x20000000)) {
        VMap* vm = &t->vmap;
        int key = 0x7d;
        TCEntry* it = vm->LowerBound(&key);
        TCEntry* end = vm->end;
        if (it == end || it->key > 0x7d)
            it = end;
        Iface_Do(it->iface);
        return true;
    }
    return false;
}