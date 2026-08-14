#include <cstdio>

struct SubMember {
    int v;
    void Copy(const SubMember* src) { v = src->v; }
};

struct CDefClassBase {
    char pad[0x28];
    SubMember member;
    int baseval;
};

struct CAnimatingObjectDef : public CDefClassBase {
    void CopyBase(const CDefClassBase* src) { baseval = src->baseval; }
    void Copy(const CDefClassBase* src) {
        CopyBase(src);
        this->member.Copy(&src->member);
    }
};

int main() {
    CDefClassBase src;
    src.baseval = 77;
    src.member.v = 42;
    CAnimatingObjectDef dst;
    dst.baseval = 0;
    dst.member.v = 0;
    dst.Copy(&src);
    if (dst.baseval == 77 && dst.member.v == 42)
        printf("COPY_OK\n");
    else
        printf("COPY_BAD\n");
    return 0;
}