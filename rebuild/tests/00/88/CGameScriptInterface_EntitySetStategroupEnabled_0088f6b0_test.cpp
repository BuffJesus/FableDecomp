#include <cstdio>

struct Entity;
struct Self;

struct SelfVtbl {
    void* slot[0x2c / 4];
    Entity* (__fastcall* getEntity)(Self*);
};

struct Entity {
    char pad[0x10];
    char enabled;   // +0x10
    void SetStategroup(int a2, int a3);
};

struct Self {
    SelfVtbl* vt;
};

static int g_calls = 0;
static int g_seen_a2 = 0, g_seen_a3 = 0;

void Entity::SetStategroup(int a2, int a3) {
    g_calls++; g_seen_a2 = a2; g_seen_a3 = a3;
}

static Entity g_entity;
static Entity* __fastcall provide(Self* self) {
    (void)self;
    return &g_entity;
}
static SelfVtbl g_vt;

void __stdcall CGameScriptInterface_EntitySetStategroupEnabled(Self* self, int a2, int a3)
{
    Entity* e = self->vt->getEntity(self);
    if (e && e->enabled == 1) {
        e->SetStategroup(a2, a3);
    }
}

int main() {
    g_vt.getEntity = provide;
    Self self;
    self.vt = &g_vt;

    // Case A: entity enabled==1 -> method called with args
    g_entity.enabled = 1;
    g_calls = 0;
    CGameScriptInterface_EntitySetStategroupEnabled(&self, 7, 42);
    if (g_calls != 1 || g_seen_a2 != 7 || g_seen_a3 != 42) {
        std::printf("FAIL enabled-path calls=%d a2=%d a3=%d\n", g_calls, g_seen_a2, g_seen_a3);
        return 1;
    }

    // Case B: entity enabled!=1 -> not called
    g_entity.enabled = 0;
    g_calls = 0;
    CGameScriptInterface_EntitySetStategroupEnabled(&self, 1, 2);
    if (g_calls != 0) {
        std::printf("FAIL disabled-path calls=%d\n", g_calls);
        return 1;
    }

    std::printf("CGameScriptInterface_0088f6b0_TEST PASS\n");
    return 0;
}