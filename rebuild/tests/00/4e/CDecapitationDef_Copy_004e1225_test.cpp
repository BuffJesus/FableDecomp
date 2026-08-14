#include <stdio.h>

// Behaviour model of CDecapitationDef::Copy
struct Base { unsigned int baseCopied; };

struct Def {
    char pad[0x28];
    unsigned int f28;
    unsigned int f2c;
    unsigned int baseFlag;
};

static void base_copy(Def* self, const Def* src) {
    self->baseFlag = src->baseFlag; // stand-in for base class field copy
}

static void Copy(Def* self, const Def* src) {
    base_copy(self, src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
}

int main() {
    Def src; Def dst;
    for (int i = 0; i < 0x28; ++i) { src.pad[i] = (char)0; dst.pad[i] = (char)0; }
    src.f28 = 0x11112222;
    src.f2c = 0x33334444;
    src.baseFlag = 0xABCD;
    dst.f28 = 0; dst.f2c = 0; dst.baseFlag = 0;
    Copy(&dst, &src);
    if (dst.f28 == 0x11112222 && dst.f2c == 0x33334444 && dst.baseFlag == 0xABCD) {
        printf("PARITY_OK_004e1225\n");
    } else {
        printf("PARITY_FAIL\n");
    }
    return 0;
}