// CGameScriptInterface::GetHeroWillEnergyPercentage @ 0089a210
// __fastcall, returns long, 0 params.

struct MapNode {
    int key;        // +0x0
    void* value;    // +0x4
};

struct IFaceMap {
    void* pad0;         // +0x0
    MapNode* end;       // +0x4
    MapNode* LowerBound(int* key); // __fastcall: this in ecx, key on stack
};

struct Thing {
    unsigned char pad0[0x20];
    unsigned int field20;      // +0x20
    unsigned char pad24[0x44 - 0x24];
    IFaceMap ifaces;           // +0x44
    unsigned char pad4c[0x91 - 0x4c];
    unsigned char flags91;     // +0x91
};

struct GSI {
    unsigned char pad0[0x14];
    void* member14;            // +0x14
};

// externs (reloc-masked); all __fastcall to match retail this-in-ecx idiom
extern void* __fastcall GetMgr(void* self);            // call A: this=member14
extern Thing* __fastcall GetThing(void* mgr);          // call B: this=mgr
extern long __fastcall CalcPct(void* value);           // call C: this=value
extern long __cdecl Finalize(void);                    // call D

long __fastcall CGameScriptInterface_GetHeroWillEnergyPercentage(GSI* self)
{
    Thing* thing = GetThing(GetMgr(self->member14));
    if (thing != 0 && !(thing->flags91 & 1) && (thing->field20 & 0x10)) {
        int key = 4;
        IFaceMap* m = &thing->ifaces;
        MapNode* it = m->LowerBound(&key);
        MapNode* end = m->end;
        if (it != end && it->key <= 4) {
            // use it
        } else {
            it = end;
        }
        CalcPct(it->value);
        return Finalize();
    }
    return 0;
}