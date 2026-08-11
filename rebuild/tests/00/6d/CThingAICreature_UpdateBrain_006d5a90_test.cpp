#include <cstdio>

unsigned char g_updateBrainEnabled = 0;

static void* g_seen_self = 0;
static long g_seen_a = 0;
static void* g_seen_p = 0;

extern "C" bool __fastcall sub_88bcb0(void* self, int, long a, void* p) {
    g_seen_self = self;
    g_seen_a = a;
    g_seen_p = p;
    return true;
}

struct CThingAICreature {
    char pad[0x1a8];
    void* f1a8;
    bool __fastcall UpdateBrain(long a, void* p);
};

bool __fastcall CThingAICreature::UpdateBrain(long a, void* p) {
    if (!g_updateBrainEnabled)
        return false;
    return sub_88bcb0(this->f1a8, 0, a, p);
}

int main() {
    CThingAICreature obj;
    obj.f1a8 = (void*)0xdead;

    // flag off -> false, no tail call
    g_updateBrainEnabled = 0;
    bool r = obj.UpdateBrain(7, (void*)0x1234);
    if (r != false) { printf("FAIL off\n"); return 1; }

    // flag on -> tail call with this->f1a8, a, p
    g_updateBrainEnabled = 1;
    g_seen_self = 0; g_seen_a = 0; g_seen_p = 0;
    r = obj.UpdateBrain(42, (void*)0x5678);
    if (r != true) { printf("FAIL on ret\n"); return 1; }
    if (g_seen_self != (void*)0xdead) { printf("FAIL self\n"); return 1; }
    if (g_seen_a != 42) { printf("FAIL a\n"); return 1; }
    if (g_seen_p != (void*)0x5678) { printf("FAIL p\n"); return 1; }

    printf("006d5a90_TEST PASS\n");
    return 0;
}