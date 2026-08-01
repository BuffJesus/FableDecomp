// Self-contained behavior test for the leading _Fill_n (routine 1) of the
// over-captured slice 0x0047a6e7. Compiles under cl /Od, links, runs, prints
// an exact uppercase pass token. The out-of-line element operator= is provided
// here as a link-time probe that records its calls.
#include <stdio.h>

static unsigned long g_lastThis  = 0;
static unsigned long g_lastValue = 0;
static int           g_calls     = 0;

struct Elem {
    Elem& operator=(unsigned long _v);
    char _storage[0x50];
};

// Out-of-line definition (the masked __fastcall element-assign helper).
Elem& Elem::operator=(unsigned long _v)
{
    g_lastThis  = (unsigned long)this;
    g_lastValue = _v;
    ++g_calls;
    return *this;
}

// Behavioral mirror of the reconstructed source (identical control flow).
static Elem* Fill_n_mirror(Elem* _dest, unsigned int _count, unsigned long _value)
{
    for (; _count > 0; --_count) {
        if (_dest)
            *_dest = _value;
        ++_dest;
    }
    return _dest;
}

int main()
{
    const unsigned int N = 4;
    static Elem buf[N];
    Elem* ret = Fill_n_mirror(buf, N, 0xABCD1234u);

    int ok = 1;
    if (g_calls != (int)N) ok = 0;
    if (g_lastValue != 0xABCD1234u) ok = 0;
    if (g_lastThis != (unsigned long)&buf[N - 1]) ok = 0;
    if (ret != buf + N) ok = 0;
    if (sizeof(Elem) != 0x50) ok = 0;   // stride must be 0x50

    // count==0 must make zero calls and return _dest unchanged.
    g_calls = 0;
    Elem* r0 = Fill_n_mirror(buf, 0, 1u);
    if (g_calls != 0) ok = 0;
    if (r0 != buf) ok = 0;

    if (ok)
        printf("FILL_N_STRIDE80_THISCALL_TEST PASS\n");
    else
        printf("FILL_N_STRIDE80_THISCALL_TEST FAIL calls=%d val=%08lx\n", g_calls, g_lastValue);
    return 0;
}