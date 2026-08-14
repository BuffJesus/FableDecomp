#include <cstdio>

struct CCharString { char* p; };
struct CArrayUL { void* vt; unsigned long* a; int n; int c; };
struct PairT { CCharString first; CArrayUL second; };

static int g_first_calls = 0;
static int g_array_calls = 0;
static PairT* g_first_self = 0;
static const PairT* g_first_src = 0;
static CArrayUL* g_arr_self = 0;
static const CArrayUL* g_arr_src = 0;

void CCharString_copy(CCharString* self, const CCharString* src) {
    g_first_calls++;
    self->p = src->p;
}
void CArrayUL_copy(CArrayUL* self, const CArrayUL* src) {
    g_array_calls++;
    self->vt = src->vt; self->a = src->a; self->n = src->n; self->c = src->c;
}

// mirror of the reconstructed logic
void Cons_val_pair(PairT* self, PairT* src) {
    if (!self) return;
    CCharString_copy(&self->first, &src->first);
    CArrayUL_copy(&self->second, &src->second);
}

int main() {
    char buf[] = "hello";
    unsigned long arr[3] = {1,2,3};
    PairT src;
    src.first.p = buf;
    src.second.vt = (void*)0xABCD;
    src.second.a = arr; src.second.n = 3; src.second.c = 4;

    PairT dst;
    dst.first.p = 0;
    dst.second.vt = 0; dst.second.a = 0; dst.second.n = 0; dst.second.c = 0;

    Cons_val_pair(&dst, &src);

    bool ok = true;
    if (g_first_calls != 1) ok = false;
    if (g_array_calls != 1) ok = false;
    if (dst.first.p != buf) ok = false;
    if (dst.second.vt != (void*)0xABCD) ok = false;
    if (dst.second.a != arr) ok = false;
    if (dst.second.n != 3) ok = false;

    // null self must no-op safely
    Cons_val_pair(0, &src);
    if (g_first_calls != 1) ok = false;

    if (ok) printf("CONSVAL_PAIR_OK\n");
    else printf("FAIL c1=%d c2=%d\n", g_first_calls, g_array_calls);
    return 0;
}