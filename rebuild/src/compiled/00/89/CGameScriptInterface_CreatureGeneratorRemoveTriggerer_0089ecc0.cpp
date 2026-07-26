struct Node { int key; Node* left; };
struct BObj {
    unsigned char pad[0x6c];
    unsigned char flag6c;
    void consume(BObj* b);   // real member => thiscall, arg on stack
};
struct MapT {
    Node* dummy0;
    Node* end;
    Node* find(int* key);    // real member => thiscall, arg on stack
};
struct AObj {
    unsigned char pad0[0x24];
    unsigned int field24; // read as [esi+0x24]
    unsigned char pad2[0x1c];
    MapT map44;           // at +0x44
};
struct Host { void** vt; };

typedef AObj* (__fastcall *GetA_t)(void*);
typedef BObj* (__fastcall *GetB_t)(void*);

void __stdcall CGameScriptInterface_CreatureGeneratorRemoveTriggerer(Host* p0, Host* p1)
{
    AObj* a = ((GetA_t)(p0->vt[0x0b]))(p0);
    BObj* b = ((GetB_t)(p1->vt[0x0b]))(p1);

    if (a->field24 & 0x20000000)
    {
        int local = 0x3d;
        MapT* m = &a->map44;
        Node* it = m->find(&local);
        Node* end = m->end;
        Node* node = (it == end || it->key > 0x3d) ? end : it;
        BObj* volatile* pv = (BObj* volatile*)((char*)node + 4);
        BObj* val = *pv;
        if (b->flag6c & 8)
            val->consume(b);
    }
}