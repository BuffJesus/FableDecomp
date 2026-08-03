#include <cstdio>

struct CWorldMap {
    char pad[0x20];
    char* begin;  // +0x20
    char* end;    // +0x24
};

// function-under-test (thiscall: this in ecx)
static int __fastcall DrawGetNoMaps(CWorldMap* this_)
{
    return (int)((char*)this_->end - (char*)this_->begin) / 0x2c;
}

int main()
{
    char buf[0x2c * 7];
    CWorldMap m;
    m.begin = buf;
    m.end = buf + 0x2c * 5;   // exactly 5 elements

    int n = DrawGetNoMaps(&m);
    if (n != 5) { printf("FAIL n=%d expected 5\n", n); return 1; }

    // partial byte-span (rounds toward zero for signed idiv)
    m.end = buf + (0x2c * 3 + 10);
    int n2 = DrawGetNoMaps(&m);
    if (n2 != 3) { printf("FAIL n2=%d expected 3\n", n2); return 1; }

    // empty
    m.end = buf;
    int n3 = DrawGetNoMaps(&m);
    if (n3 != 0) { printf("FAIL n3=%d expected 0\n", n3); return 1; }

    printf("OK_0x005bce2f n=%d\n", n);
    return 0;
}