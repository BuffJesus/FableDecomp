#include <stdio.h>

struct CNavigationLayer_t {
    void* a;
    void* b;
    void* c;
    char pad;
    char flag;
};

CNavigationLayer_t* __fastcall CNavigationLayer(CNavigationLayer_t* self) {
    self->a = 0;
    self->b = 0;
    self->c = 0;
    self->flag = 0;
    return self;
}

int main() {
    CNavigationLayer_t obj;
    unsigned char* raw = (unsigned char*)&obj;
    int i;
    for (i = 0; i < (int)sizeof(obj); ++i) raw[i] = 0xAB;
    CNavigationLayer_t* r;
    CNavigationLayer_t* p = &obj;
    __asm { mov ecx, p }
    r = CNavigationLayer(p);
    if (r != &obj) { printf("BAD ret\n"); return 1; }
    if (obj.a != 0 || obj.b != 0 || obj.c != 0) { printf("BAD ptr\n"); return 1; }
    if (obj.flag != 0) { printf("BAD flag\n"); return 1; }
    if (raw[12] != 0xAB) { printf("BAD pad clobbered\n"); return 1; }
    printf("OK_0x004e786c\n");
    return 0;
}