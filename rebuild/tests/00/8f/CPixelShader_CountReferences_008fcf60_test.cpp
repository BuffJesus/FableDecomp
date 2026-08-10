#include <cstdio>

struct CPixelShader {
    void* f0;
    struct Inner { char pad[0x1c]; unsigned long refs; } * f4;
};

unsigned long __fastcall CPixelShader_CountReferences(CPixelShader* self);

int main() {
    CPixelShader::Inner inner;
    inner.refs = 4242u;
    CPixelShader obj;
    obj.f0 = 0;
    obj.f4 = &inner;
    unsigned long r = CPixelShader_CountReferences(&obj);
    if (r == 4242u) printf("OK_008fcf60\n");
    else printf("FAIL %lu\n", r);
    return 0;
}