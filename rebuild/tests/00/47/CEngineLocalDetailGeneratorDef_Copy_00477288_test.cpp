#include <cstdio>
#include <cstring>

struct Sub { int a; int b; };
struct Def { char base[0x3c]; Sub sub; };

static int g_baseCopied = 0;
static Sub g_subDst;

void baseCopy(void* self, void* src){ g_baseCopied = 1; }
void subCopy(Sub* self, Sub const* src){ *self = *src; }

void doCopy(Def* self, char const* src)
{
    baseCopy(self, (void*)src);
    subCopy(&self->sub, (Sub const*)(src + 0x3c));
}

int main()
{
    Def d; memset(&d, 0, sizeof(d));
    char srcbuf[0x3c + sizeof(Sub)];
    memset(srcbuf, 0, sizeof(srcbuf));
    Sub s; s.a = 111; s.b = 222;
    memcpy(srcbuf + 0x3c, &s, sizeof(Sub));

    doCopy(&d, srcbuf);

    if (g_baseCopied == 1 && d.sub.a == 111 && d.sub.b == 222)
        printf("PARITY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}