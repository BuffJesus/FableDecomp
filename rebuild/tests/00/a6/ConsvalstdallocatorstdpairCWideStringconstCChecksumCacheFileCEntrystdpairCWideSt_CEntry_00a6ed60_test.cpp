// Behaviour model for _Cons_val range-construct
#include <stdio.h>
#include <string.h>

struct WStr { wchar_t* p; };
struct Entry { int raw[13]; };
struct Pair { WStr first; Entry second; };

static int g_ctor_calls = 0;

// model the copy ctor as an out-of-line call
static void wstr_copy(WStr* dst, const WStr* src) {
    dst->p = src->p;   // shallow, but counts
    g_ctor_calls++;
}

static void cons_val(Pair* dest, Pair* src) {
    if (dest) {
        wstr_copy(&dest->first, &src->first);
        dest->second = src->second;   // 13 dwords raw copy
    }
}

int main() {
    Pair src;
    src.first.p = (wchar_t*)0xDEADBEEF;
    for (int i = 0; i < 13; i++) src.second.raw[i] = i * 7 + 1;

    Pair dst;
    memset(&dst, 0, sizeof(dst));
    cons_val(&dst, &src);

    int ok = 1;
    if (dst.first.p != src.first.p) ok = 0;
    if (g_ctor_calls != 1) ok = 0;
    for (int i = 0; i < 13; i++) if (dst.second.raw[i] != src.second.raw[i]) ok = 0;

    // null dest = no-op, no crash
    cons_val(0, &src);
    if (g_ctor_calls != 1) ok = 0;

    if (ok) printf("CONS_VAL_OK\n");
    else printf("CONS_VAL_FAIL\n");
    return 0;
}