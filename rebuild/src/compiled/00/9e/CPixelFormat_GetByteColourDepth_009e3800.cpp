struct PixFmtEntry { int bits; int pad[8]; };
extern PixFmtEntry g_pixFmtTable[];
struct CPixelFormat { int fmt; };
unsigned long __fastcall GetByteColourDepth(CPixelFormat* self)
{
    int bits = g_pixFmtTable[self->fmt].bits;
    return (unsigned long)(bits / 8);
}