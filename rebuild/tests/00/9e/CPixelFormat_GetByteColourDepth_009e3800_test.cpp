#include <cstdio>
struct PixFmtEntry { int bits; int pad[8]; };
PixFmtEntry g_pixFmtTable[4] = { {8}, {16}, {24}, {32} };
struct CPixelFormat { int fmt; };
unsigned long model(CPixelFormat* self)
{
    int bits = g_pixFmtTable[self->fmt].bits;
    return (unsigned long)(bits / 8);
}
int main(){
    CPixelFormat a; a.fmt=0; if(model(&a)!=1) return 1;
    CPixelFormat b; b.fmt=1; if(model(&b)!=2) return 2;
    CPixelFormat c; c.fmt=2; if(model(&c)!=3) return 3;
    CPixelFormat d; d.fmt=3; if(model(&d)!=4) return 4;
    printf("OK_009e3800\n");
    return 0;
}