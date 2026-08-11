#include <stdio.h>
#include <string.h>

struct CCharString {
    char pad[13];
    CCharString() {}
    CCharString(const CCharString& o);
};

CCharString::CCharString(const CCharString& o) { memcpy(pad, o.pad, 13); }

struct MyPair {
    CCharString first;
    bool second;
    MyPair() : second(false) {}
};

void __fastcall _Cons_val(MyPair* _Ptr, MyPair* _Val);

int main()
{
    MyPair src;
    memset(&src, 0, sizeof(src));
    strcpy(src.first.pad, "hi");
    src.second = true;

    MyPair dst;
    memset(&dst, 0x55, sizeof(dst));
    _Cons_val(&dst, &src);

    bool ok = (strcmp(dst.first.pad, "hi") == 0) && (dst.second == true);

    _Cons_val(0, &src);

    printf("CONSVAL_%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}