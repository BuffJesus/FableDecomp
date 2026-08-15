#include <stdio.h>

struct C3DClothQuad { unsigned int a, b, c, d; };

static C3DClothQuad* fill_impl(C3DClothQuad* dest, unsigned int count, const C3DClothQuad* val)
{
    for (; count != 0; --count) {
        if (dest) { *dest = *val; }
        ++dest;
    }
    return dest;
}

int main()
{
    C3DClothQuad buf[4];
    C3DClothQuad v = { 1, 2, 3, 4 };
    C3DClothQuad* end = fill_impl(buf, 4, &v);
    int ok = (end == buf + 4);
    for (int i = 0; i < 4; ++i)
        ok &= (buf[i].a==1 && buf[i].b==2 && buf[i].c==3 && buf[i].d==4);
    ok &= (fill_impl(buf, 0, &v) == buf);
    if (ok) printf("FILLQUAD_OK\n");
    else printf("FILLQUAD_FAIL\n");
    return 0;
}