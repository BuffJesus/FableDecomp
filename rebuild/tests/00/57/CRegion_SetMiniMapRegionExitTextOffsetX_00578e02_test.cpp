#include <cstdio>

static int g_helper_arg = 0;
int Helper(void* oldp) { g_helper_arg = (int)(long long)oldp; return 0x9999; }

struct CCharString { void* p; };
struct CRegion { void* field0; };

void model(CRegion* self, CCharString* out, float unused) {
    void* old = self->field0;
    self->field0 = (void*)(long long)Helper(old);
    out->p = old;
}

int main() {
    CRegion r; r.field0 = (void*)0x1234;
    CCharString cs; cs.p = 0;
    model(&r, &cs, 1.5f);
    if (g_helper_arg == 0x1234 && r.field0 == (void*)0x9999 && cs.p == (void*)0x1234)
        printf("PARITY_OK\n");
    else
        printf("FAIL a=%x f0=%p csp=%p\n", g_helper_arg, r.field0, cs.p);
    return 0;
}