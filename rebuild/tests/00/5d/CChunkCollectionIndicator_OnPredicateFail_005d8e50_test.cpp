#include <stdio.h>

typedef void (__fastcall *PredFn)(void* self, void* arg);

struct Target {
    void** vtbl;   // +0x00
};

struct CChunkCollectionIndicator {
    void*   m_pad0;   // +0x00
    void*   m_arg;    // +0x04
    void*   m_pad8;   // +0x08
    void*   m_padC;   // +0x0c
    Target* m_target; // +0x10
};

void __fastcall OnPredicateFail(CChunkCollectionIndicator* this_) {
    Target* t = this_->m_target;
    void*   a = this_->m_arg;
    ((PredFn)(t->vtbl[0x14c/4]))(t, a);
}

// --- test harness ---
static void*  g_seen_self = 0;
static void*  g_seen_arg  = 0;
static int    g_calls     = 0;

static void __fastcall pred_impl(void* self, void* arg) {
    g_seen_self = self;
    g_seen_arg  = arg;
    g_calls++;
}

int main() {
    // Build a vtable with slot 83 (0x14c/4) = pred_impl.
    static void* vt[128];
    for (int i = 0; i < 128; ++i) vt[i] = 0;
    vt[83] = (void*)&pred_impl;

    Target tgt;
    tgt.vtbl = vt;

    int sentinel = 0;
    CChunkCollectionIndicator obj;
    obj.m_pad0 = 0;
    obj.m_arg  = &sentinel;   // the arg that must be forwarded ([this+0x4])
    obj.m_pad8 = 0;
    obj.m_padC = 0;
    obj.m_target = &tgt;      // [this+0x10]

    CChunkCollectionIndicator* pobj = &obj;
    // __fastcall: this in ecx, no stack args, ret 0.
    __asm {
        mov ecx, pobj
        call OnPredicateFail
    }

    if (g_calls == 1 && g_seen_self == (void*)&tgt && g_seen_arg == (void*)&sentinel) {
        printf("OK_0x005d8e50 self=%p arg=%p\n", g_seen_self, g_seen_arg);
        return 0;
    }
    printf("BAD calls=%d self=%p arg=%p\n", g_calls, g_seen_self, g_seen_arg);
    return 1;
}