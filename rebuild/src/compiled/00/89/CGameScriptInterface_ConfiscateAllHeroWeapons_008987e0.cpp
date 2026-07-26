struct Node { int key; void* val; };
struct Container { char pad[4]; Node* endp; Node* LowerBound(int* key); };
struct Obj { char pad20[0x20]; unsigned int flags20; char pad24[0x44-0x24]; Container c44; char padmid[0x91-0x4c]; unsigned char flag91; };
struct Iface { char pad[0x14]; void* field14; };

extern void* __fastcall GetSub(void* self);
extern Obj* __fastcall GetObj(void* self);
extern void __fastcall DoIt(void* self);

void __fastcall CGameScriptInterface_ConfiscateAllHeroWeapons(Iface* self)
{
    void* a = GetSub(self->field14);
    Obj* o = GetObj(a);
    if (!o) return;
    if (o->flag91 & 1) return;
    if (!(o->flags20 & 0x80000)) return;
    Container* c = &o->c44;
    int k = 0x13;
    Node* n = c->LowerBound(&k);
    Node* end = c->endp;
    if (n != end && n->key <= 0x13) {
    } else {
        n = end;
    }
    DoIt(n->val);
}