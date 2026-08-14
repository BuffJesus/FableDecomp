struct CEngine {
    char pad0[0x14];
    unsigned int flags;   // +0x14
    char pad1[0x08];
    float value;          // +0x20
};

void __fastcall Enable2DSceneRendering(CEngine *self, float v)
{
    if (self->value != v) {
        self->value = v;
        self->flags |= 4;
    }
}