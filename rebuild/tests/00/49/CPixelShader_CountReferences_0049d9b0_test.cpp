#include <cstdio>
struct RefTarget {
    char pad0[0x10];
    unsigned long refs;
};
struct CPixelShader {
    char pad0[8];
    RefTarget* inner;
};
unsigned long __fastcall CountReferences(CPixelShader* self) {
    return self->inner->refs;
}
int main() {
    RefTarget t; t.refs = 42u;
    CPixelShader s; s.inner = &t;
    if (CountReferences(&s) != 42u) { printf("FAIL\n"); return 1; }
    t.refs = 0u;
    if (CountReferences(&s) != 0u) { printf("FAIL\n"); return 1; }
    printf("OK_0049d9b0\n");
    return 0;
}