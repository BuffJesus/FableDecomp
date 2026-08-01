// Behavior test for Destroy_pair_CCharString_0047baf0 (retail 0x0047baf0).
// Self-contained; compiles under cl /Od, links, runs, prints a PASS token.
//
// The retail function walks [_First,_Last) over 12-byte, three-CCharString
// elements and invokes the masked non-virtual CCharString::~CCharString on each
// element's three members in reverse declaration order (+8, +4, +0). We supply
// a real destructor definition so we can observe the per-element call count and
// destruction order.

#include <stdio.h>

// The genuine source.obj emits CCharString's scalar-deleting-destructor COMDAT
// (??_GCCharString), which references operator delete. That deleting dtor is
// never actually invoked by the retail routine (it only calls the plain
// ~CCharString), but the linker still keeps the COMDAT and demands the operator.
// Define the free-store operators locally with trivial bodies so the fallback
// link (source.obj + test.obj) resolves them here without pulling operator
// delete out of the CRT import library -- the extra dependency that made this
// TU (unlike its sibling _Destroy/_Uninit_copy tests) fail to link.
void* operator new(unsigned int n) { (void)n; return 0; }
void  operator delete(void* p) { (void)p; }
void* operator new[](unsigned int n) { (void)n; return 0; }
void  operator delete[](void* p) { (void)p; }

static int g_dtorCalls;
static int g_order[16];
static int g_orderIdx;

class CCharString {
public:
    ~CCharString();
    int tag;
};

CCharString::~CCharString() {
    ++g_dtorCalls;
    if (g_orderIdx < 16) g_order[g_orderIdx++] = tag;
}

struct Elem {
    CCharString _a;   // +0x00
    CCharString _b;   // +0x04
    CCharString _c;   // +0x08
};

extern "C" void __fastcall
Destroy_pair_CCharString_0047baf0(Elem* _First, Elem* _Last, void* _Al);

int main()
{
    const int N = 3;
    // Raw storage so we control lifetime and count destructor calls explicitly.
    static char buf[sizeof(Elem) * N];
    Elem* first = reinterpret_cast<Elem*>(buf);
    for (int i = 0; i < N; ++i) {
        first[i]._a.tag = i * 3 + 0;
        first[i]._b.tag = i * 3 + 1;
        first[i]._c.tag = i * 3 + 2;
    }
    Elem* last = first + N;

    int ok = 1;

    g_dtorCalls = 0;
    g_orderIdx = 0;
    Destroy_pair_CCharString_0047baf0(first, last, (void*)0);

    if (sizeof(Elem) != 0x0c) { printf("size %u want 12\n", (unsigned)sizeof(Elem)); ok = 0; }
    if (g_dtorCalls != N * 3) { printf("dtor-count %d want %d\n", g_dtorCalls, N * 3); ok = 0; }

    // Per element, members must be destroyed in reverse: _c(+2), _b(+1), _a(+0).
    for (int i = 0; i < N && ok; ++i) {
        int base = i * 3;
        if (g_order[i * 3 + 0] != base + 2 ||
            g_order[i * 3 + 1] != base + 1 ||
            g_order[i * 3 + 2] != base + 0) { printf("order elem %d wrong\n", i); ok = 0; }
    }

    // Empty range: no calls.
    g_dtorCalls = 0;
    Destroy_pair_CCharString_0047baf0(first, first, (void*)0);
    if (g_dtorCalls != 0) { printf("empty-range fail\n"); ok = 0; }

    if (ok) printf("DESTROY_PAIR_CCHARSTRING_0047BAF0_TEST PASS\n");
    else    printf("DESTROY_PAIR_CCHARSTRING_0047BAF0_TEST FAIL\n");
    return ok ? 0 : 1;
}