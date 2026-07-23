#include <cstdio>
struct MapNode { int key; struct MapNode* aux; };
struct MapKind { int _pad0; struct MapNode* head; struct MapNode* lb(int* key); };
struct EngineObj { char _pad[0x94]; };
struct SubA { void* GetB(); };
struct GSI { char _pad[0x14]; SubA* member14; };

static int g_finalized = -1;
static MapNode g_nodeA = { 3, 0 };
static MapNode g_nodeEnd = { 0, 0 };
static MapNode* g_lb_ret = 0;
static EngineObj g_obj;

void* SubA::GetB(){ return this; }
static SubA g_subA;
EngineObj* ResolveObj_impl(void* p){ return &g_obj; }
MapNode* MapKind::lb(int* key){ return g_lb_ret; }
void FinalizeNode_impl(void* n){ g_finalized = n ? ((MapNode*)n)->key : -99; }

void CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(GSI* self)
{
    int key;
    void* a = self->member14->GetB();
    EngineObj* obj = ResolveObj_impl(a);
    if (!obj) return;
    if (*(unsigned char*)((char*)obj + 0x91) & 1) return;
    if (!(*(unsigned char*)((char*)obj + 0x20) & 0x10)) return;

    MapKind* m = (MapKind*)((char*)obj + 0x44);
    key = 4;
    MapNode* found = m->lb(&key);
    MapNode* endn = m->head;
    if (found == endn || *(int*)found > 4) {
        found = endn;
    }
    FinalizeNode_impl(*(void**)((char*)found + 4));
}

int main(){
    g_subA = SubA();
    GSI gsi;
    gsi.member14 = &g_subA;

    g_obj._pad[0x91] = 0;
    g_obj._pad[0x20] = 0x10;
    MapKind* m = (MapKind*)((char*)&g_obj + 0x44);
    m->head = &g_nodeEnd;

    // Case 1: found key <=4 -> keep found; finalize found->aux
    g_nodeA.key = 3; g_nodeA.aux = &g_nodeA;
    g_lb_ret = &g_nodeA; g_finalized = -1;
    CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(&gsi);
    if (g_finalized != 3){ std::printf("FAIL case1 got %d\n", g_finalized); return 1; }

    // Case 2: found key >4 -> use endn->aux
    g_nodeA.key = 10;
    g_nodeEnd.aux = &g_nodeEnd; g_nodeEnd.key = 77;
    g_lb_ret = &g_nodeA; g_finalized = -1;
    CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(&gsi);
    if (g_finalized != 77){ std::printf("FAIL case2 got %d\n", g_finalized); return 1; }

    // Case 3: gate blocks
    g_obj._pad[0x91] = 1; g_finalized = -1;
    CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(&gsi);
    if (g_finalized != -1){ std::printf("FAIL case3\n"); return 1; }

    std::printf("CGameScriptInterface_00896fd0_TEST PASS\n");
    return 0;
}