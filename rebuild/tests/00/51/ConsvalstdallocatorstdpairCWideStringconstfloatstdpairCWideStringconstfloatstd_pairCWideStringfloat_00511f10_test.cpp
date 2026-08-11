#include <cstdio>

struct WStr { void* p0; };
struct Pair {
    WStr  first;
    float second;
    void  ConsFirst(Pair* src);
};

static int g_called = 0;

// Standalone definition of the helper for the behaviour test.
void Pair::ConsFirst(Pair* src)
{
    g_called++;
    this->first.p0 = src->first.p0;   // model CWideString copy-construct of `first`
}

void __fastcall _Cons_val(Pair* dest, Pair* src)
{
    if (dest)
    {
        dest->ConsFirst(src);
        dest->second = src->second;
    }
}

int main()
{
    Pair src; src.first.p0 = (void*)0x1234; src.second = 3.5f;
    Pair dest; dest.first.p0 = 0; dest.second = 0.0f;

    _Cons_val(&dest, &src);

    // null-dest branch must be a no-op
    _Cons_val(0, &src);

    if (g_called == 1 && dest.second == 3.5f && dest.first.p0 == (void*)0x1234)
        printf("CONS_OK second=%.1f called=%d\n", dest.second, g_called);
    else
        printf("CONS_FAIL second=%.1f called=%d\n", dest.second, g_called);
    return 0;
}