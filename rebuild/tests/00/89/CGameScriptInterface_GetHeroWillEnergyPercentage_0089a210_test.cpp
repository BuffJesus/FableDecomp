#include <cstdio>

struct MapNode { int key; void* value; };
struct IFaceMap {
    void* pad0;
    MapNode* end;
    MapNode* LowerBound(int* key);
};
struct Thing {
    unsigned char pad0[0x20];
    unsigned int field20;
    unsigned char pad24[0x44 - 0x24];
    IFaceMap ifaces;
    unsigned char pad4c[0x91 - 0x4c];
    unsigned char flags91;
};
struct GSI { unsigned char pad0[0x14]; void* member14; };

static Thing g_thing;
static MapNode g_node;
static MapNode g_end;
static int g_calc_called = 0;

MapNode* IFaceMap::LowerBound(int* key) { (void)key; return &g_node; }
extern void* __fastcall GetMgr(void* self) { return self; }
extern Thing* __fastcall GetThing(void* mgr) { return (Thing*)mgr; }
extern long __fastcall CalcPct(void* value) { (void)value; g_calc_called = 1; return 0; }
extern long __cdecl Finalize(void) { return 77; }

long __fastcall CGameScriptInterface_GetHeroWillEnergyPercentage(GSI* self)
{
    Thing* thing = GetThing(GetMgr(self->member14));
    if (thing != 0 && !(thing->flags91 & 1) && (thing->field20 & 0x10)) {
        int key = 4;
        IFaceMap* m = &thing->ifaces;
        MapNode* it = m->LowerBound(&key);
        MapNode* end = m->end;
        if (it != end && it->key <= 4) {
        } else {
            it = end;
        }
        CalcPct(it->value);
        return Finalize();
    }
    return 0;
}

int main() {
    GSI s;
    for (int i=0;i<0x14;i++) s.pad0[i]=0;
    s.member14 = &g_thing;
    g_thing.field20 = 0x10;
    g_thing.flags91 = 1;
    g_thing.ifaces.end = &g_end;
    if (CGameScriptInterface_GetHeroWillEnergyPercentage(&s) != 0) { std::printf("FAIL blocked\n"); return 1; }

    g_thing.flags91 = 0;
    g_node.key = 0; g_node.value = (void*)0x1234;
    g_calc_called = 0;
    long r = CGameScriptInterface_GetHeroWillEnergyPercentage(&s);
    if (r != 77 || !g_calc_called) { std::printf("FAIL allowed r=%ld c=%d\n", r, g_calc_called); return 1; }

    g_thing.field20 = 0;
    if (CGameScriptInterface_GetHeroWillEnergyPercentage(&s) != 0) { std::printf("FAIL bitclear\n"); return 1; }

    std::printf("CGameScriptInterface_0089a210_TEST PASS\n");
    return 0;
}