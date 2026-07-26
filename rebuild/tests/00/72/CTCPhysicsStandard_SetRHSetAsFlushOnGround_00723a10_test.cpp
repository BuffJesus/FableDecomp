#include <cstdio>

struct Vec { int x, y, z; };
struct Helper;

struct CTCPhysicsStandard {
    void** vtbl;      // +0x00
    int pad4;         // +0x04
    int pad8;         // +0x08
    Vec ground;       // +0x0c
    Helper* compute(Vec* out, Vec* ground);
    void handle(Vec* p);   // stand-in for the vtbl+0x134 slot target
};

static int g_stage = 0;
static Vec g_seen_ground;
static Vec* g_handle_arg = 0;

Helper* CTCPhysicsStandard::compute(Vec* out, Vec* ground) {
    g_stage |= 1;
    g_seen_ground = *ground;
    out->x = 7;
    return (Helper*)0x1234;
}
void CTCPhysicsStandard::handle(Vec* p) {
    g_stage |= 4;
    g_handle_arg = p;
}

void __fastcall Helper_use(Helper* h) {
    if (h == (Helper*)0x1234) g_stage |= 2;
}

void __fastcall CTCPhysicsStandard_SetRHSetAsFlushOnGround(CTCPhysicsStandard* self)
{
    Vec local;
    Helper* h = self->compute(&local, &self->ground);
    Helper_use(h);
    typedef void (CTCPhysicsStandard::*Fn)(Vec*);
    Fn fn = *(Fn*)((char*)self->vtbl + 0x134);
    (self->*fn)(&local);
}

int main() {
    // fake vtable: place a pointer to CTCPhysicsStandard::handle at +0x134
    static char vtbl[0x200];
    typedef void (CTCPhysicsStandard::*Fn)(Vec*);
    Fn hp = &CTCPhysicsStandard::handle;
    *(Fn*)(vtbl + 0x134) = hp;

    CTCPhysicsStandard obj;
    obj.vtbl = (void**)vtbl;
    obj.ground.x = 11; obj.ground.y = 22; obj.ground.z = 33;

    CTCPhysicsStandard_SetRHSetAsFlushOnGround(&obj);

    if (g_stage == 7 && g_seen_ground.x == 11 && g_handle_arg != 0) {
        std::printf("CTCPhysicsStandard_00723a10_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL stage=%d ground=%d arg=%p\n", g_stage, g_seen_ground.x, (void*)g_handle_arg);
    return 1;
}