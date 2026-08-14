#include <cstdio>

struct Coll {
    unsigned char pad0[0x34];
    void* field_34;
    unsigned char pad38[0x48-0x38];
    unsigned char flags;
};

static void* g_released = (void*)1;
static int g_called = 0;

static void MgrRelease(void* block) { g_called++; g_released = block; }

static void ClearLoadRequest(Coll* self, void* mgr) {
    (void)mgr;
    if (self->flags & 8) {
        MgrRelease(self->field_34);
        self->flags &= 0xf7;
    }
}

int main() {
    Coll a; a.flags = 0x08 | 0x01; a.field_34 = (void*)0xABCD;
    g_called = 0; g_released = 0;
    ClearLoadRequest(&a, (void*)0x1234);
    bool ok1 = (g_called == 1) && (g_released == (void*)0xABCD) &&
               (a.flags == 0x01);

    Coll b; b.flags = 0x01; b.field_34 = (void*)0x9999;
    g_called = 0;
    ClearLoadRequest(&b, (void*)0x1234);
    bool ok2 = (g_called == 0) && (b.flags == 0x01);

    if (ok1 && ok2) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL c=%d r=%p f1=%02x f2=%02x\n",
               g_called, g_released, a.flags, b.flags);
    return 0;
}