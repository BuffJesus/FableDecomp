#include <stdio.h>
#include <string.h>

// Behaviour model of CParticleAttacherDef::Copy:
// - copies base-class portion [0,0x28) from src
// - copies sub-object at +0x28 from src+0x28
// We model the two Copy callees as memcpy of their respective regions.

struct Def {
    unsigned char base[0x28];
    unsigned char sub[0x28];
};

static void BaseCopy(Def* self, const unsigned char* src) {
    memcpy(self->base, src, 0x28);
}
static void SubCopy(unsigned char* selfsub, const unsigned char* src) {
    memcpy(selfsub, src, 0x28);
}

static void AttacherCopy(Def* self, const unsigned char* src) {
    BaseCopy(self, src);
    SubCopy(self->sub, src + 0x28);
}

int main() {
    Def dst; memset(&dst, 0, sizeof(dst));
    unsigned char src[0x50];
    for (int i = 0; i < 0x50; ++i) src[i] = (unsigned char)(i + 1);

    AttacherCopy(&dst, src);

    int ok = 1;
    for (int i = 0; i < 0x28; ++i) if (dst.base[i] != src[i]) ok = 0;
    for (int i = 0; i < 0x28; ++i) if (dst.sub[i] != src[0x28 + i]) ok = 0;

    if (ok) printf("COPY_OK\n");
    else printf("COPY_BAD\n");
    return 0;
}