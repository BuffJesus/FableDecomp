#include <cstdio>

struct CTavernTableDef {
    unsigned char pad[0x25];
    unsigned char b25;
    unsigned char b26;
    int base_copied;
};

static void Base_Copy(CTavernTableDef* self, CTavernTableDef* src) {
    self->base_copied = 1;
    (void)src;
}

static void Copy(CTavernTableDef* self, CTavernTableDef* src) {
    Base_Copy(self, src);
    self->b25 = src->b25;
    self->b26 = src->b26;
}

int main() {
    CTavernTableDef src = {};
    CTavernTableDef dst = {};
    src.b25 = 0xAB;
    src.b26 = 0xCD;
    Copy(&dst, &src);
    if (dst.b25 == 0xAB && dst.b26 == 0xCD && dst.base_copied == 1) {
        printf("COPY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}