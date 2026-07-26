#include <cstdio>

static int g_flags = 0;
static int g_slot1 = 0, g_slot2 = 0;
static void* g_seen = 0;

struct IVisitor {
    virtual void f00() {}
    virtual void f01(void* a, void* b) { g_slot1++; g_seen = a; }
    virtual void f02(void* a, void* b) { g_slot2++; g_seen = a; }
};

struct CScrollingComponent {
    char pad[0x18];
    int  state;    /* +0x18 */
    char pad2[0x08];
    void* field24; /* +0x24 */
    void* field28; /* +0x28 */
};

struct ScopeGuard { char buf[0x18]; void init(int flag); };
void ScopeGuard::init(int flag) { g_flags |= 2; }

void __stdcall SomeGlobalFunc(const void* arg) { g_flags |= 1; }

void __fastcall CScrollingComponent_ChangeState(CScrollingComponent* self, void* edx, IVisitor* p)
{
    SomeGlobalFunc((const void*)0x122d70e);
    ScopeGuard g;
    g.init(0);
    int s = self->state;
    switch (s) {
    case 2:
        p->f01(self->field24, p);
        break;
    case 3:
        p->f02(self->field28, p);
        break;
    }
}

int main() {
    IVisitor v;
    CScrollingComponent c;
    c.field24 = (void*)0x1111;
    c.field28 = (void*)0x2222;

    c.state = 2;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot1 != 1 || g_slot2 != 0 || g_seen != (void*)0x1111) {
        std::printf("FAIL state2\n"); return 1;
    }

    c.state = 3;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot2 != 1 || g_seen != (void*)0x2222) {
        std::printf("FAIL state3\n"); return 1;
    }

    c.state = 7;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot1 != 1 || g_slot2 != 1) {
        std::printf("FAIL other\n"); return 1;
    }

    if ((g_flags & 3) != 3) { std::printf("FAIL flags\n"); return 1; }

    std::printf("CScrollingComponent_00452b4d_TEST PASS\n");
    return 0;
}