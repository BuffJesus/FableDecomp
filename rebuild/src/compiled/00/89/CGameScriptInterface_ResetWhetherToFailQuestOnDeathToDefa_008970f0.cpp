// CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault
// __fastcall, void, 1 param (this)

struct MapNode {
    int key;      // +0x00
    void* value;  // +0x04
};

struct Map {
    void* root;      // +0x00
    MapNode* header; // +0x04  (end sentinel)
    MapNode* LowerBound(int* keyPtr); // thiscall: this in ecx, keyPtr on stack
};

extern void* __fastcall GetSubObject(void* self);
extern void* __fastcall Resolve(void* self);
extern void __fastcall Consume(void* value);

struct Iface {
    char pad[0x14];
    void* sub; // +0x14
};

void __fastcall CGameScriptInterface_ResetWhetherToFailQuestOnDeathToDefault(Iface* self)
{
    void* a = GetSubObject(self->sub);
    unsigned char* eax = (unsigned char*)Resolve(a);
    if (!eax) return;
    if (eax[0x91] & 0x1) return;
    if (!(eax[0x20] & 0x10)) return;

    Map* map = (Map*)((char*)eax + 0x44);
    int key = 4;
    MapNode* node = map->LowerBound(&key);
    MapNode* endNode = map->header;
    MapNode* sel;
    if (node == endNode) sel = endNode;
    else if (node->key <= 4) sel = node;
    else sel = endNode;
    Consume(sel->value);
}