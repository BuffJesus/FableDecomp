#include <cstdio>

struct CDefClassBase { int tag; };

struct SubDef {
    int copied_from;
    void Copy(const CDefClassBase* src) { copied_from = src->tag; }
};
struct BaseDef {
    int copied_from;
    void Copy(const CDefClassBase* src) { copied_from = src->tag; }
};
struct Def : public BaseDef {
    char gap[0x28 - sizeof(BaseDef)];
    SubDef sub;
    void Copy(const CDefClassBase* src) {
        BaseDef::Copy(src);
        this->sub.Copy((const CDefClassBase*)((const char*)src + 0x28));
    }
};

int main() {
    char srcbuf[0x40];
    for (int i = 0; i < 0x40; ++i) srcbuf[i] = 0;
    ((CDefClassBase*)(srcbuf + 0))->tag = 111;
    ((CDefClassBase*)(srcbuf + 0x28))->tag = 222;

    Def d;
    d.BaseDef::copied_from = -1;
    d.sub.copied_from = -1;
    d.Copy((const CDefClassBase*)srcbuf);

    if (d.BaseDef::copied_from == 111 && d.sub.copied_from == 222)
        printf("PARITY_OK_461b98\n");
    else
        printf("FAIL b=%d s=%d\n", d.BaseDef::copied_from, d.sub.copied_from);
    return 0;
}