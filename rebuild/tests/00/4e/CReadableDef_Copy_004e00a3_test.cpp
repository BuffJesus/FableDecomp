#include <stdio.h>

struct CDefClassBase { char pad[0x40]; };

struct CReadableBase {
    void Copy(CDefClassBase const* src);
};

struct CReadableDef : CReadableBase {
    unsigned char field25pad[0x25];
    unsigned char field25;
    void Copy(CDefClassBase const* src);
};

static int g_basecalled = 0;
void CReadableBase::Copy(CDefClassBase const* src) {
    g_basecalled = 1;
    (void)src;
}

void __fastcall CReadableDef_Copy(CReadableDef* self, int, CDefClassBase const* src);

int main() {
    CReadableDef dst; CReadableDef srcObj;
    for (int i=0;i<(int)sizeof(dst);++i) ((char*)&dst)[i]=0;
    for (int i=0;i<(int)sizeof(srcObj);++i) ((char*)&srcObj)[i]=0;
    srcObj.field25 = 0xAB;
    dst.field25 = 0x00;
    CReadableDef_Copy(&dst, 0, (CDefClassBase const*)&srcObj);
    if (g_basecalled && dst.field25 == 0xAB) {
        printf("004e00a3_TEST PASS\n");
    } else {
        printf("FAIL base=%d f=%02x\n", g_basecalled, (unsigned)dst.field25);
    }
    return 0;
}