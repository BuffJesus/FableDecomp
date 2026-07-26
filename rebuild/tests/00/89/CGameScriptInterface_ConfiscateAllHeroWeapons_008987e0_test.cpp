#include <cstdio>
struct Node { int key; void* val; };
struct Container { char pad[4]; Node* endp; Node* LowerBound(int* key); };
struct Obj { char pad20[0x20]; unsigned int flags20; char pad24[0x44-0x24]; Container c44; char padmid[0x91-0x4c]; unsigned char flag91; };
struct Iface { char pad[0x14]; void* field14; };

static Node g_node = { 0x10, (void*)0xABCD };
static Node g_end  = { 0x99, 0 };
static Obj g_obj;
static int g_called = 0;
static void* g_arg = 0;

Node* Container::LowerBound(int* key){ return &g_node; }
void* __fastcall GetSub(void* self){ return self; }
Obj* __fastcall GetObj(void* self){ return &g_obj; }
void __fastcall DoIt(void* self){ g_called++; g_arg = self; }

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

int main(){
    g_obj.flags20 = 0x80000;
    g_obj.flag91 = 0;
    g_obj.c44.endp = &g_end;
    Iface self;
    self.field14 = 0;
    CGameScriptInterface_ConfiscateAllHeroWeapons(&self);
    if (g_called == 1 && g_arg == (void*)0xABCD) {
        std::printf("CGameScriptInterface_008987e0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d arg=%p\n", g_called, g_arg);
    return 1;
}