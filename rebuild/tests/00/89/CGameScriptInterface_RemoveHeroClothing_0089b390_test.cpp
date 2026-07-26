#include <cstdio>

struct Node { int key; void* val; };
struct Vec {
    Node* begin;
    Node* end;
    Node* lower_bound(int* key);
};

Node* Vec::lower_bound(int* key){
    for (Node* p = begin; p != end; ++p)
        if (p->key >= *key) return p;
    return end;
}

static int g_removed = 0;

void* GetHeroObj(void* mgr){ return mgr; }
static void* g_data = 0;
void* ResolveHero(void* obj){ (void)obj; return g_data; }
void DoRemove(void* self){ g_removed = (int)(long long)self; }

struct HeroData {
    char pad[0x20];
    unsigned int flags20;
    char pad2[0x91 - 0x24];
    unsigned char flag91;
};

struct GSI { char pad0[0x14]; void* mgr; };

void CGameScriptInterface_RemoveHeroClothing(GSI* self)
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
    } else {
        it = end;
    }
    DoRemove(it->val);
}

int main(){
    static char blob[0x100];
    HeroData* d = (HeroData*)blob;
    d->flags20 = 0x40000;
    d->flag91 = 0;
    static Node nodes[3];
    nodes[0].key = 0x10; nodes[0].val = (void*)0x1111;
    nodes[1].key = 0x12; nodes[1].val = (void*)0x2222;
    nodes[2].key = 0x20; nodes[2].val = (void*)0x3333;
    Vec* v = (Vec*)(blob + 0x44);
    v->begin = &nodes[0];
    v->end = &nodes[3];
    g_data = blob;

    GSI gsi; gsi.mgr = (void*)0xABCD;
    CGameScriptInterface_RemoveHeroClothing(&gsi);
    if (g_removed == 0x2222){
        std::printf("CGameScriptInterface_0089b390_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL removed=%d\n", g_removed);
    return 1;
}