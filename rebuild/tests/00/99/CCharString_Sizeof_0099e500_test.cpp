#include <cstdio>

struct StringHeader {
    unsigned long refcount;
    unsigned long field4;
    unsigned long length;
};

struct CCharString {
    StringHeader* header;
};

unsigned long __fastcall Sizeof(CCharString* self);

int main() {
    CCharString a;
    a.header = 0;
    bool ok = (Sizeof(&a) == 4);

    StringHeader h;
    h.refcount = 1; h.field4 = 0; h.length = 10;
    CCharString b;
    b.header = &h;
    ok = ok && (Sizeof(&b) == (unsigned long)(10 + 0x15));

    h.length = 0x80000005; // high bit set -> masked to 5
    ok = ok && (Sizeof(&b) == (unsigned long)(5 + 0x15));

    if (ok) printf("0099e500_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}