#include <cstdio>
struct RefTarget {
    char pad0[0x74];
    unsigned long refs;
};
struct CPixelShader {
    char pad0[0x40];
    RefTarget* inner;
};
unsigned long __fastcall CountReferences(CPixelShader* self);
int main() {
    RefTarget rt;
    rt.refs = 0x1234abcdUL;
    CPixelShader ps;
    ps.inner = &rt;
    unsigned long r = CountReferences(&ps);
    if (r == 0x1234abcdUL) printf("OK_0084d050\n");
    else printf("FAIL %lx\n", r);
    return 0;
}