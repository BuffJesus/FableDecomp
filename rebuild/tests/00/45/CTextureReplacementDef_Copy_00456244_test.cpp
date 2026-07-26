#include <cstdio>

struct CSub {
    int v;
    void Copy(const CSub* o) { v = o->v + 100; }
};
struct CTextureReplacementDef {
    int marker;
    char pad[0x24];
    CSub sub;
    void BaseCopy(const CTextureReplacementDef* o) { marker = o->marker + 7; }
    void CTextureReplacementDef_Copy(const CTextureReplacementDef* o);
};

void CTextureReplacementDef::CTextureReplacementDef_Copy(const CTextureReplacementDef* o)
{
    this->BaseCopy(o);
    this->sub.Copy(&o->sub);
}

int main()
{
    CTextureReplacementDef src, dst;
    src.marker = 10;
    src.sub.v = 5;
    dst.marker = 0;
    dst.sub.v = 0;
    dst.CTextureReplacementDef_Copy(&src);
    if (dst.marker == 17 && dst.sub.v == 105) {
        std::printf("CTextureReplacementDef_00456244_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL marker=%d sub=%d\n", dst.marker, dst.sub.v);
    return 1;
}