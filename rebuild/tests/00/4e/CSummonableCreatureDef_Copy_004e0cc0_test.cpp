#include <cstdio>

struct Def {
    char pad[0x28];
    unsigned int f28;
    unsigned int f2c;
};

static int g_base_called = 0;

void base_copy(Def* self, const Def* src) {
    g_base_called = 1;
    // pretend base copies pad
    for (int i = 0; i < 0x28; ++i) self->pad[i] = src->pad[i];
}

void my_copy(Def* self, const Def* src) {
    base_copy(self, src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
}

int main() {
    Def src; Def dst;
    for (int i = 0; i < 0x28; ++i) src.pad[i] = (char)(i + 1);
    src.f28 = 0xDEADBEEF;
    src.f2c = 0x12345678;
    for (int i = 0; i < 0x28; ++i) dst.pad[i] = 0;
    dst.f28 = 0; dst.f2c = 0;

    my_copy(&dst, &src);

    int ok = g_base_called == 1
        && dst.f28 == 0xDEADBEEF
        && dst.f2c == 0x12345678
        && dst.pad[0] == 1 && dst.pad[0x27] == 0x28;
    if (ok) printf("COPY_OK\n");
    else printf("FAIL\n");
    return 0;
}