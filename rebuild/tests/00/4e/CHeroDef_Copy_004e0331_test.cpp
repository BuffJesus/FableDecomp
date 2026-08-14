#include <stdio.h>

struct Base {
    int pad[10];
    int m28;
    int m2c;
    int baseMarker;
};

struct HeroDef {
    int pad[10];
    int m28;
    int m2c;
    int baseMarker;
};

static void BaseCopy(HeroDef* self, HeroDef const* src)
{
    self->baseMarker = src->baseMarker;
}

static void HeroCopy(HeroDef* self, HeroDef const* src)
{
    BaseCopy(self, src);
    self->m28 = src->m28;
    self->m2c = src->m2c;
}

int main()
{
    HeroDef s, d;
    for (int i = 0; i < 10; i++) { s.pad[i] = 0; d.pad[i] = -1; }
    s.m28 = 0x11112222;
    s.m2c = 0x33334444;
    s.baseMarker = 0x55556666;
    d.m28 = d.m2c = d.baseMarker = 0;

    HeroCopy(&d, &s);

    if (d.m28 == 0x11112222 && d.m2c == 0x33334444 && d.baseMarker == 0x55556666)
        printf("COPY_OK\n");
    else
        printf("COPY_FAIL\n");
    return 0;
}