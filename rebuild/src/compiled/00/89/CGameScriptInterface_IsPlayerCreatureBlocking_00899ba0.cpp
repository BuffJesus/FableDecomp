// CGameScriptInterface::IsPlayerCreatureBlocking @ 00899ba0
struct Node { int key; void* val; };

extern void* __fastcall GetManagerA(void* self);
extern void* __fastcall GetPlayerEnt(void* mgr);
struct KeyArg { int* p; };
extern Node* __fastcall LowerBound(void* tree, KeyArg k);
extern char  __fastcall CheckBlocking(void* obj);

bool __fastcall CGameScriptInterface_IsPlayerCreatureBlocking(void* self)
{
    void* mgr = GetManagerA(*(void**)((char*)self + 0x14));
    void* e = GetPlayerEnt(mgr);
    if (e && !(*(unsigned char*)((char*)e + 0x91) & 1)
          && (*(unsigned int*)((char*)e + 0x24) & 0x10000))
    {
        char* tree = (char*)e + 0x44;
        int key = 0x30;
        KeyArg ka; ka.p = &key;
        Node* found = LowerBound(tree, ka);
        Node* end = *(Node**)(tree + 4);
        if (found == end || found->key > 0x30)
            found = end;
        if (CheckBlocking(*(void**)((char*)found + 4)))
            return true;
    }
    return false;
}