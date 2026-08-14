#include <stdio.h>
#include <string.h>

// Behaviour model of CObjectFamilyDef::Copy: base copy of whole src, then
// copy of the sub-object at +0x3c. Verify the composite effect.

struct Fam {
    char base[0x3c];
    char sub[0x10];
};

static int g_baseCalled;
static int g_subCalled;

static void BaseCopy(Fam* dst, const Fam* src) {
    g_baseCalled = 1;
    memcpy(dst->base, src->base, 0x3c);
}
static void SubCopy(char* dst, const char* src) {
    g_subCalled = 1;
    memcpy(dst, src, 0x10);
}

static void FamCopy(Fam* self, const Fam* src) {
    BaseCopy(self, src);
    SubCopy((char*)self + 0x3c, (const char*)src + 0x3c);
}

int main() {
    Fam a, b;
    for (int i = 0; i < (int)sizeof(Fam); ++i) ((char*)&a)[i] = (char)(i + 1);
    memset(&b, 0, sizeof(b));
    FamCopy(&b, &a);
    if (g_baseCalled && g_subCalled && memcmp(&a, &b, sizeof(Fam)) == 0) {
        printf("COPY_OK\n");
    } else {
        printf("COPY_FAIL\n");
    }
    return 0;
}