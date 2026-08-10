struct CEngineLandscapeMemoryManager {
    char pad0[0x34];
    int frame;
    char pad1[0x40 - 0x34 - 4];
    unsigned char flag;
};

void __fastcall FrameStart(CEngineLandscapeMemoryManager* self)
{
    self->frame = (self->frame + 1) % 3;
    self->flag = 1;
}