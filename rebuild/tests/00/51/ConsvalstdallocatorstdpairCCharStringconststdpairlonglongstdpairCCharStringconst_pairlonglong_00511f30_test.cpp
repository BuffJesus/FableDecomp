#include <stdio.h>

// Behaviour model of _Cons_val for pair<CCharString const, pair<long,long> >.
// It copy-constructs the string subobject (modeled here as a pointer copy via a
// stand-in "copy ctor") and copies the two longs. Null dest => no-op.

struct CCharString {
    int refcopies; // how many times the copy ctor ran, for observability
    void* p;
};

static int g_copyctor_calls = 0;

static void cchar_copy_ctor(CCharString* self, const CCharString* src) {
    // stand-in for engine 0x99ec30: shallow copy of the string body
    self->p = src->p;
    g_copyctor_calls++;
}

struct LongPair { long first; long second; };
struct ValPair { CCharString first; LongPair second; };

static void Cons_val_model(ValPair* dest, ValPair* src) {
    if (dest) {
        cchar_copy_ctor(&dest->first, &src->first);
        dest->second.first  = src->second.first;
        dest->second.second = src->second.second;
    }
}

int main() {
    int markerArr[1];
    markerArr[0] = 0xABCD;
    void* markerPtr = markerArr; // array decays to pointer, no address-of token

    ValPair srcArr[1];
    ValPair* src = srcArr;
    src->first.p = markerPtr;
    src->second.first = 111;
    src->second.second = 222;

    ValPair destArr[1];
    ValPair* dest = destArr;
    dest->first.p = 0;
    dest->second.first = -1;
    dest->second.second = -1;

    g_copyctor_calls = 0;
    Cons_val_model(dest, src);

    int ok = 1;
    ok = ok && (dest->first.p == markerPtr);
    ok = ok && (dest->second.first == 111);
    ok = ok && (dest->second.second == 222);
    ok = ok && (g_copyctor_calls == 1);

    // null-dest must be a no-op and must not crash / not call ctor
    g_copyctor_calls = 0;
    Cons_val_model(0, src);
    ok = ok && (g_copyctor_calls == 0);

    if (ok) printf("CONS_VAL_OK\n");
    else    printf("CONS_VAL_FAIL\n");
    return 0;
}