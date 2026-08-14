#include <cstdio>
typedef unsigned long ulong;

struct Group { char pad[0xa4]; unsigned long type; };

ulong GetRenderLayerMask(Group* self)
{
    switch (self->type) {
    case 0:  return 0x8000;
    case 1:  return 0x100000;
    default: return 0x80000;
    }
}

int main()
{
    Group g;
    bool ok = true;
    g.type = 0; ok = ok && (GetRenderLayerMask(&g) == 0x8000);
    g.type = 1; ok = ok && (GetRenderLayerMask(&g) == 0x100000);
    g.type = 2; ok = ok && (GetRenderLayerMask(&g) == 0x80000);
    g.type = 5; ok = ok && (GetRenderLayerMask(&g) == 0x80000);
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}