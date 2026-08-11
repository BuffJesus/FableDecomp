#include <cstdio>

struct CCharString {
    char* p;
    void CopyFrom(const CCharString* other);
};
struct PairKV { CCharString first; float second; };

static int g_copyctor_calls = 0;

void CCharString::CopyFrom(const CCharString* other)
{
    this->p = other->p;   // shallow copy is enough for the behaviour probe
    g_copyctor_calls++;
}

void __fastcall _Cons_val(PairKV* dest, PairKV* src);

int main()
{
    char buf[] = "hello";
    PairKV src;
    src.first.p = buf;
    src.second = 3.5f;

    PairKV dest;
    dest.first.p = 0;
    dest.second = 0.0f;

    _Cons_val(&dest, &src);

    bool ok = (dest.first.p == buf) && (dest.second == 3.5f) && (g_copyctor_calls == 1);

    // null dest: must be a no-op, no copyctor call
    _Cons_val(0, &src);
    ok = ok && (g_copyctor_calls == 1);

    if (ok) printf("CONS_VAL_OK\n");
    else    printf("CONS_VAL_FAIL c=%d p=%p s=%f\n", g_copyctor_calls, (void*)dest.first.p, dest.second);
    return ok ? 0 : 1;
}