#include <cstdio>

struct CInner {
    char pad[0x124];
    unsigned long refCount;
};

struct CPixelShader {
    char pad[0xe0];
    CInner* inner;
};

unsigned long __fastcall CPixelShader_CountReferences(CPixelShader* self);

int main()
{
    CInner innerObj;
    innerObj.refCount = 0xABCD1234u;
    CInner* innerPtr = &innerObj;
    CPixelShader ps;
    ps.inner = innerPtr;

    unsigned long r = CPixelShader_CountReferences(&ps);
    if (r == 0xABCD1234u) {
        printf("COUNTREF_OK\n");
        return 0;
    }
    printf("COUNTREF_FAIL %lu\n", r);
    return 1;
}