#include <stdio.h>

struct CObjectCacheGroupCollection {
    char pad0[0x28];
    void* field_28;
    char pad2c[0x36 - 0x2c];
    unsigned char flags;
};

struct Mgr {
    void* got_arg;
    int called;
};

static void mgr_release(Mgr* self, void* arg) {
    self->called++;
    self->got_arg = arg;
}

// model of the target
static void clear_load_request(CObjectCacheGroupCollection* self, Mgr* mgr) {
    if (self->flags & 2) {
        mgr_release(mgr, self->field_28);
        self->flags &= (unsigned char)0xfd;
    }
}

int main() {
    // Case 1: bit set -> calls release with field_28, clears bit 2
    CObjectCacheGroupCollection c1; c1.flags = 0x36; c1.field_28 = (void*)0xdead;
    Mgr m1; m1.called = 0; m1.got_arg = 0;
    clear_load_request(&c1, &m1);
    if (m1.called != 1) { printf("FAIL c1 call\n"); return 1; }
    if (m1.got_arg != (void*)0xdead) { printf("FAIL c1 arg\n"); return 1; }
    if (c1.flags != 0x34) { printf("FAIL c1 flags %02x\n", c1.flags); return 1; }

    // Case 2: bit not set -> no call, flags unchanged
    CObjectCacheGroupCollection c2; c2.flags = 0x01; c2.field_28 = (void*)0xbeef;
    Mgr m2; m2.called = 0; m2.got_arg = 0;
    clear_load_request(&c2, &m2);
    if (m2.called != 0) { printf("FAIL c2 call\n"); return 1; }
    if (c2.flags != 0x01) { printf("FAIL c2 flags\n"); return 1; }

    printf("BEHAVIOUR_OK_bdd980\n");
    return 0;
}