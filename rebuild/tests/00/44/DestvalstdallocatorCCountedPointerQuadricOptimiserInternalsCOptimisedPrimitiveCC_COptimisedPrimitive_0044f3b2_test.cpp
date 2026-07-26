#include <cstdio>

struct Ctrl { int refcount; void (__fastcall *destroy)(void*); void* arg; };
struct CCP { void* val; Ctrl* ctrl; };

static int g_deleted = 0;
static int g_destroyed = 0;
static void op_delete(void* p) { g_deleted++; }
static void __fastcall destroyer(void* p) { g_destroyed++; }

void __fastcall Dest_val(CCP* self)
{
    Ctrl* c = self->ctrl;
    if (c) {
        c->refcount--;
        c = self->ctrl;
        if (c->refcount == 0) {
            c->destroy(c->arg);
            op_delete(self->ctrl);
        }
    }
    self->val = 0;
    self->ctrl = 0;
}

int main() {
    int dummyarg = 5;
    Ctrl c;
    c.refcount = 1;
    c.destroy = destroyer;
    c.arg = &dummyarg;
    CCP obj;
    obj.val = (void*)0x1234;
    obj.ctrl = &c;
    Dest_val(&obj);
    if (g_destroyed != 1) { std::printf("FAIL destroyed=%d\n", g_destroyed); return 1; }
    if (g_deleted != 1) { std::printf("FAIL deleted=%d\n", g_deleted); return 1; }
    if (obj.val != 0 || obj.ctrl != 0) { std::printf("FAIL not zeroed\n"); return 1; }

    /* refcount stays >0: no destroy/delete */
    g_destroyed = 0; g_deleted = 0;
    Ctrl c2; c2.refcount = 2; c2.destroy = destroyer; c2.arg = &dummyarg;
    CCP obj2; obj2.val = (void*)0x1; obj2.ctrl = &c2;
    Dest_val(&obj2);
    if (g_destroyed != 0 || g_deleted != 0) { std::printf("FAIL early free\n"); return 1; }
    if (c2.refcount != 1) { std::printf("FAIL refcount=%d\n", c2.refcount); return 1; }
    if (obj2.val != 0 || obj2.ctrl != 0) { std::printf("FAIL not zeroed2\n"); return 1; }

    /* null ctrl: nothing happens */
    CCP obj3; obj3.val = (void*)0xAB; obj3.ctrl = 0;
    Dest_val(&obj3);
    if (obj3.val != 0) { std::printf("FAIL null path\n"); return 1; }

    std::printf("DestVal_0044f3b2_TEST PASS\n");
    return 0;
}