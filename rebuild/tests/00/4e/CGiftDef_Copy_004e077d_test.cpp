#include <stdio.h>
#include <string.h>

struct Base { char basepad[0x28]; };
struct Gift : Base {
    unsigned int f28;
    unsigned char f2c;
};

// model: base copy fills the base region, derived copies f28/f2c
static void BaseCopy(Gift* self, const Gift* src) {
    memcpy((Base*)self, (const Base*)src, sizeof(Base));
}

static void Copy(Gift* self, const Gift* src) {
    BaseCopy(self, src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
}

int main() {
    Gift src; memset(&src, 0, sizeof(src));
    for (int i = 0; i < 0x28; i++) src.basepad[i] = (char)(i + 1);
    src.f28 = 0xDEADBEEF;
    src.f2c = 0x5A;

    Gift dst; memset(&dst, 0xFF, sizeof(dst));
    Copy(&dst, &src);

    bool ok = true;
    for (int i = 0; i < 0x28; i++) if (dst.basepad[i] != (char)(i + 1)) ok = false;
    if (dst.f28 != 0xDEADBEEF) ok = false;
    if (dst.f2c != 0x5A) ok = false;

    if (ok) printf("GIFTCOPY_OK\n");
    else printf("FAIL\n");
    return 0;
}