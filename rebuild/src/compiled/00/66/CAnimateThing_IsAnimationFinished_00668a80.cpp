struct CAnimateThing {
    char pad0[4];
    int curFrame;
    int endFrame;
    char pad1[0x51 - 0xC];
    unsigned char flags;
};

int __fastcall CAnimateThing_IsAnimationFinished(const CAnimateThing* self)
{
    if (self->curFrame != self->endFrame && (self->flags & 2) == 0)
        return 0;
    return 1;
}