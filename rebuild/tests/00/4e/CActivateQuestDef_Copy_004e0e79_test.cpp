#include <cstdio>

struct Base {
    int calls;
    Base(){calls=0;}
};

struct Def {
    char pad[0x28];
    int  field28;
    char field2c;
};

static int g_basecopy_calls = 0;

void BaseCopy(Def* self, Def const* src) {
    g_basecopy_calls++;
    (void)self; (void)src;
}

void Copy(Def* self, Def const* src) {
    BaseCopy(self, src);
    self->field28 = src->field28;
    self->field2c = src->field2c;
}

int main() {
    Def a; Def b;
    b.field28 = 0x12345678;
    b.field2c = (char)0xAB;
    a.field28 = 0; a.field2c = 0;
    Copy(&a, &b);
    if (a.field28 == 0x12345678 && a.field2c == (char)0xAB && g_basecopy_calls == 1) {
        printf("COPY_OK\n");
    } else {
        printf("COPY_FAIL\n");
    }
    return 0;
}