// CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault
struct MapNode;
struct SubA {
    void* GetB();          // __fastcall, returns object ptr in eax
};
struct SubB;
struct EngineObj;
struct MapKind {
    int _pad0;             // +0x00
    struct MapNode* head;  // +0x04
    struct MapNode* lb(int* key);   // __fastcall lower_bound-like
};
struct Node2 { void* p; };
struct GSI {
    char _pad[0x14];
    SubA* member14;        // +0x14
};

extern "C" EngineObj* __fastcall ResolveObj(void* p);  // this in ecx
extern "C" void __fastcall FinalizeNode(void* n);      // this in ecx

void __fastcall CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(GSI* self)
{
    int key;
    void* a = self->member14->GetB();
    EngineObj* obj = ResolveObj(a);
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
    FinalizeNode(*(void**)((char*)found + 4));
}