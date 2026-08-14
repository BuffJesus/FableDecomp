#include <stdio.h>

// Behaviour model of FinishAction @ 0x008c9e40:
//   sub = self + 8
//   inner = lookup(sub)
//   h = *(inner + 0x60)
//   h->vtbl[0x90/4](h, 0)     // virtual call, arg 0
//   base_FinishAction(self)

static int g_vslot_called = 0;
static int g_vslot_arg = -1;
static int g_base_called = 0;
static void* g_lookup_ecx = 0;

struct Inner { char pad[0x60]; void* pField60; };

struct Fake {
    void** vtbl;
};

static void __fastcall vslot(void* self, int arg) {
    (void)self; g_vslot_called++; g_vslot_arg = arg;
}
static void __fastcall base_fn(void* self) { (void)self; g_base_called++; }

static Inner* lookup(void* ecxSub) {
    g_lookup_ecx = ecxSub;
    static Inner inner;
    static void* vtbl[64];
    static Fake fake;
    vtbl[0x90/4] = (void*)&vslot;
    fake.vtbl = vtbl;
    inner.pField60 = &fake;
    return &inner;
}

static void finish(void* self) {
    Inner* p = lookup((char*)self + 8);
    void* h = *(void**)((char*)p + 0x60);
    void** vt = *(void***)h;
    ((void (__fastcall*)(void*, int))vt[0x90/4])(h, 0);
    base_fn(self);
}

int main() {
    char obj[64];
    finish(obj);
    if (g_lookup_ecx == (void*)(obj + 8) &&
        g_vslot_called == 1 && g_vslot_arg == 0 &&
        g_base_called == 1) {
        printf("NYMPH_FINISH_OK\n");
    } else {
        printf("FAIL c=%d a=%d b=%d\n", g_vslot_called, g_vslot_arg, g_base_called);
    }
    return 0;
}