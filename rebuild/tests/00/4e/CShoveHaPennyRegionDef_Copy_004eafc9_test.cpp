#include <cstdio>

struct Def {
    unsigned int pad[10]; // 0x00..0x27
    unsigned int f28;
    unsigned int f2c;
    unsigned int base_marker;
};

static bool g_base_called = false;

static void BaseCopy(Def* self, const Def* src) {
    g_base_called = true;
    self->base_marker = src->base_marker;
}

static void Copy(Def* self, const Def* src) {
    BaseCopy(self, src);
    self->f28 = src->f28;
    self->f2c = src->f2c;
}

int main() {
    Def dst; Def s;
    for (int i = 0; i < 10; i++) { dst.pad[i] = 0; }
    dst.f28 = 0; dst.f2c = 0; dst.base_marker = 0;
    s.f28 = 0x11112222; s.f2c = 0x33334444; s.base_marker = 0x55556666;

    Copy(&dst, &s);

    bool ok = g_base_called
        && dst.f28 == 0x11112222
        && dst.f2c == 0x33334444
        && dst.base_marker == 0x55556666;
    if (ok) printf("COPYOK_4eafc9\n");
    else printf("FAIL\n");
    return 0;
}