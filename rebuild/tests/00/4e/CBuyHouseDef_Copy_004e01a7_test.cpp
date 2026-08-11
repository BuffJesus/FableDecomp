#include <stdio.h>

struct CDefClassBase { char pad[0x40]; };

static int g_basecalled = 0;

struct CBuyHouseBase {
    void BaseCopy(CDefClassBase const* src);
};
void CBuyHouseBase::BaseCopy(CDefClassBase const* src) { g_basecalled = 1; (void)src; }

struct CBuyHouseDef {
    char pad[0x25];
    unsigned char field25;
};

void __fastcall CBuyHouseDef_Copy(CBuyHouseDef* self, int, CDefClassBase const* src);

int main() {
    CBuyHouseDef dst; CBuyHouseDef srcObj;
    for (int i=0;i<(int)sizeof(dst);++i) ((char*)&dst)[i]=0;
    for (int i=0;i<(int)sizeof(srcObj);++i) ((char*)&srcObj)[i]=0;
    srcObj.field25 = 0xAB;
    dst.field25 = 0x00;
    CBuyHouseDef_Copy(&dst, 0, (CDefClassBase const*)&srcObj);
    if (g_basecalled && dst.field25 == 0xAB) {
        printf("004e01a7_TEST PASS\n");
    } else {
        printf("FAIL base=%d f=%02x\n", g_basecalled, dst.field25);
    }
    return 0;
}