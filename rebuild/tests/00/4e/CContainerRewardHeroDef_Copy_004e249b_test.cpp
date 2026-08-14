#include <cstdio>

// Behaviour model: Copy forwards to base Copy then copies member at +0x28.

struct CDefClassBase { int base; int m28[4]; };

struct Member28 {
    int v[4];
    void Copy(const Member28& s) { for (int i=0;i<4;i++) v[i]=s.v[i]; }
};

struct Hero {
    char pad[0x28];
    Member28 m28;
    int baseCopied;
    void BaseCopy(const CDefClassBase& s) { baseCopied = s.base; }
    void Copy(const CDefClassBase& src) {
        this->BaseCopy(src);
        const Member28* srcm = (const Member28*)((const char*)&src + 0x28);
        this->m28.Copy(*srcm);
    }
};

int main() {
    // Build a src blob: base at +0, member vals at +0x28
    static char buf[0x40];
    CDefClassBase* src = (CDefClassBase*)buf;
    src->base = 1234;
    // member at +0x28
    int* mem = (int*)(buf + 0x28);
    mem[0]=11; mem[1]=22; mem[2]=33; mem[3]=44;

    Hero h;
    h.baseCopied = 0;
    for (int i=0;i<4;i++) h.m28.v[i]=0;
    h.Copy(*src);

    bool ok = (h.baseCopied==1234) && h.m28.v[0]==11 && h.m28.v[1]==22 &&
              h.m28.v[2]==33 && h.m28.v[3]==44;
    printf(ok ? "PARITY_OK\n" : "PARITY_FAIL\n");
    return 0;
}