#include <cstdio>
#include <cstring>

// Behaviour model of _Cons_val: copy-construct both pair members at dest from src.

struct CCharString { const char* p; };
struct CArrayUL { void* a; void* b; unsigned long n; };
struct Pair { CCharString first; CArrayUL second; };

static int g_str_calls = 0;
static int g_arr_calls = 0;

static void CCharString_copyctor(CCharString* self, CCharString* src) {
    self->p = src->p;   // shallow model of ref-copy
    ++g_str_calls;
}
static void CArrayUL_copyctor(CArrayUL* self, CArrayUL* src) {
    self->a = src->a; self->b = src->b; self->n = src->n;
    ++g_arr_calls;
}

static void Cons_val(Pair* self, Pair* src) {
    if (self) {
        CCharString_copyctor(&self->first, &src->first);
        CArrayUL_copyctor(&self->second, &src->second);
    }
}

int main() {
    CCharString sstr; sstr.p = "hello";
    CArrayUL sarr; sarr.a = (void*)0x1111; sarr.b = (void*)0x2222; sarr.n = 7;
    Pair src; src.first = sstr; src.second = sarr;

    Pair dst; memset(&dst, 0, sizeof(dst));
    Cons_val(&dst, &src);

    bool ok = true;
    ok = ok && (dst.first.p == src.first.p);
    ok = ok && (dst.second.a == src.second.a);
    ok = ok && (dst.second.b == src.second.b);
    ok = ok && (dst.second.n == 7);
    ok = ok && (g_str_calls == 1) && (g_arr_calls == 1);

    // null self: no-op, no calls
    Cons_val(0, &src);
    ok = ok && (g_str_calls == 1) && (g_arr_calls == 1);

    if (ok) printf("CONSVAL_OK\n");
    else printf("FAIL\n");
    return 0;
}