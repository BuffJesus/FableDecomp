// Retail SetDebugBoxColour1 @ 0x00c8cdc0 (__fastcall, self in ecx)
// self -> data ptr at +0; read 4 packed color bytes and assemble a 32-bit value.
struct ColourBytes {
    unsigned char pad[0xe];
    unsigned char b0;   // +0xe
    unsigned char b1;   // +0xf
    unsigned char b2;   // +0x10
    unsigned char b3;   // +0x11
};

struct CEngineWeatherRenderer {
    ColourBytes* data;  // +0
    unsigned int SetDebugBoxColour1();
};

unsigned int CEngineWeatherRenderer::SetDebugBoxColour1()
{
    ColourBytes* p = this->data;
    return ((((((unsigned int)p->b3 << 8) | (unsigned int)p->b2) << 8)
            | (unsigned int)p->b1) << 8) | (unsigned int)p->b0;
}