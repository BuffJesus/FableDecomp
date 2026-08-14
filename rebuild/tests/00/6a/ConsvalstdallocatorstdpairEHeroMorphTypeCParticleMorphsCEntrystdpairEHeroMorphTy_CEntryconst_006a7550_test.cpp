#include <cstdio>
struct Pair { int v; };
struct AllocInner { int payload; };
struct Alloc { AllocInner* m_first; };

static AllocInner* g_seen_arg_matches = 0;
static Pair* g_last_arg = 0;
static AllocInner* g_last_payload = 0;

AllocInner* callee(Pair* arg, AllocInner* payload) {
    g_last_arg = arg;
    g_last_payload = payload;
    return payload;
}

AllocInner* cons_val(Alloc* self, Pair* arg) {
    return callee(arg, self->m_first);
}

int main() {
    AllocInner inner; inner.payload = 77;
    Alloc a; a.m_first = &inner;
    Pair p; p.v = 5;
    AllocInner* r = cons_val(&a, &p);
    if (r == &inner && g_last_arg == &p && g_last_payload == &inner) {
        printf("LV_OK_006a7550\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}