#include <cstdio>

struct Node { int key; void* p4; };
struct Container {
    Node* begin; Node* end_;
    Node* LowerBound(int* key);
};

struct Obj {
    unsigned char _pad0[0x24];
    unsigned int field24;
    unsigned char _pad28[0x44 - 0x28];
    Container cont;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char flags;
};

struct CGameScriptInterface {
    unsigned char _pad0[0x14];
    void* member14;
};

struct M14 { char d; };

static Obj g_obj;
static Node g_nodes[2];
static int g_final_called = 0;
static void* g_final_arg = 0;
static int g_lb_mode = 0;

Obj* __fastcall Resolve1(M14* self) { return &g_obj; }
Obj* __fastcall Resolve2(Obj* p) { return p; }
Node* Container::LowerBound(int* key) {
    if (g_lb_mode == 1) return this->end_;
    return &g_nodes[0];
}
void __fastcall FinalCall(void* p) { g_final_called = 1; g_final_arg = p; }

void __fastcall CGameScriptInterface_StartSneaking(CGameScriptInterface* self)
{
    Obj* a = Resolve1((M14*)self->member14);
    Obj* o = Resolve2(a);
    if (!o) return;
    if (o->flags & 1) return;
    if (!(o->field24 & 0x400000)) return;

    Container* c = &o->cont;
    int local = 0x36;
    Node* r = c->LowerBound(&local);
    Node* e = c->end_;
    if (r != e && r->key <= 0x36) {
    } else {
        r = e;
    }
    FinalCall(r->p4);
}

int main() {
    CGameScriptInterface self;
    static char dummy;
    self.member14 = &dummy;

    g_nodes[0].key = 0x30; g_nodes[0].p4 = (void*)0xAAAA;
    g_nodes[1].key = 0x99; g_nodes[1].p4 = (void*)0xBBBB;
    g_obj.cont.begin = &g_nodes[0];
    g_obj.cont.end_ = &g_nodes[1];

    g_obj.flags = 1; g_obj.field24 = 0x400000; g_final_called = 0;
    CGameScriptInterface_StartSneaking(&self);
    if (g_final_called != 0) { std::printf("FAIL flags-guard\n"); return 1; }

    g_obj.flags = 0; g_obj.field24 = 0; g_final_called = 0;
    CGameScriptInterface_StartSneaking(&self);
    if (g_final_called != 0) { std::printf("FAIL field24-guard\n"); return 1; }

    g_obj.flags = 0; g_obj.field24 = 0x400000; g_final_called = 0; g_lb_mode = 0;
    CGameScriptInterface_StartSneaking(&self);
    if (g_final_called != 1 || g_final_arg != (void*)0xAAAA) { std::printf("FAIL normal-r\n"); return 1; }

    g_final_called = 0; g_lb_mode = 1;
    CGameScriptInterface_StartSneaking(&self);
    if (g_final_called != 1 || g_final_arg != (void*)0xBBBB) { std::printf("FAIL end-case\n"); return 1; }

    std::printf("CGameScriptInterface_008a1300_TEST PASS\n");
    return 0;
}