#include <stdio.h>

struct WStr {
    void* p;
    void Cons(void* srcStr);
};
struct PairWF {
    WStr  first;
    float second;
};

static int g_consCalls = 0;
static void* g_lastSrc = 0;

void WStr::Cons(void* srcStr)
{
    g_consCalls++;
    g_lastSrc = srcStr;
    this->p = *(void**)srcStr;   // mimic a string copy
}

void __fastcall _Cons_val(PairWF* _P, void* _Vraw);

int main()
{
    PairWF src;
    src.first.p = (void*)0xABCD1234;
    src.second = 3.5f;

    PairWF dst;
    dst.first.p = 0;
    dst.second = 0.0f;

    _Cons_val(&dst, &src);

    int ok = (g_consCalls == 1)
          && (g_lastSrc == (void*)&src.first)
          && (dst.first.p == (void*)0xABCD1234)
          && (dst.second == 3.5f);

    // null dest path must be a no-op
    _Cons_val((PairWF*)0, &src);
    ok = ok && (g_consCalls == 1);

    if (ok) printf("PARITY_OK cons=%d f=%.1f\n", g_consCalls, dst.second);
    else    printf("FAIL cons=%d\n", g_consCalls);
    return 0;
}