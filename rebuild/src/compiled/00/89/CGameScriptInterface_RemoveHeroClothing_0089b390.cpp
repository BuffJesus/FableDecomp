// CGameScriptInterface::RemoveHeroClothing @ 0089b390
struct Node { int key; void* val; };

struct Vec {
    Node* begin;   // +0x00
    Node* end;     // +0x04
    Node* lower_bound(int* key); // __fastcall, key pushed as stack arg
};

extern void* __fastcall GetHeroObj(void* mgr);   // 0xffbae5e0
extern void* __fastcall ResolveHero(void* obj);  // 0xffbeca30
extern void  __fastcall DoRemove(void* self);    // 0xffd18d56

struct HeroData {
    char pad[0x20];
    unsigned int flags20; // +0x20
    char pad2[0x91 - 0x24];
    unsigned char flag91;  // +0x91
    // Vec at +0x44
};

struct GSI {
    char pad0[0x14];
    void* mgr; // +0x14
};

void __fastcall CGameScriptInterface_RemoveHeroClothing(GSI* self)
{
    int key;
    void* h = GetHeroObj(self->mgr);
    HeroData* d = (HeroData*)ResolveHero(h);
    if (!d) return;
    if (d->flag91 & 1) return;
    if (!(d->flags20 & 0x40000)) return;
    Vec* vec = (Vec*)((char*)d + 0x44);
    key = 0x12;
    Node* it = vec->lower_bound(&key);
    Node* end = vec->end;
    if (it != end && it->key <= 0x12) {
        // keep it
    } else {
        it = end;
    }
    DoRemove(it->val);
}