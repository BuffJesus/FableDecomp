extern const double g_clearDouble;

struct CLandscapeBackgroundPatch;

struct TextureOwner {
    void Release(int handle);
};

void __fastcall CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
    unsigned char* p = (unsigned char*)self;
    double dv = g_clearDouble;
    *(char*)(p + 0x10) = 0;
    *(double*)(p + 0x50) = dv;
    *(int*)(p + 0x30) = 0;
    *(int*)(p + 0x1c) = 0;
    *(int*)(p + 0x20) = 0;
    *(int*)(p + 0x24) = 0;
    *(int*)(p + 0x28) = 0;
    *(int*)(p + 0x2c) = 0;
    int h = *(int*)(p + 0x74);
    *(unsigned int*)(p + 0x0c) = 0x3d888889u;
    *(unsigned int*)(p + 0x14) = 0x3f800000u;
    *(unsigned int*)(p + 0x48) = 0x3e800000u;
    if (h != -1) {
        ((TextureOwner*)(*(void**)(p + 0x78)))->Release(h);
        *(int*)(p + 0x74) = -1;
    }
}