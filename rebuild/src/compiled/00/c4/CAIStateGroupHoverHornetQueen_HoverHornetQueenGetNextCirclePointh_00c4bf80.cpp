struct CAIStateGroup_HoverHornetQueen
{
    int unk00;
    int numCirclePoints;   // +0x04
    int unk08;
    int currentCircleIndex; // +0x0c
    int circlePointCount;   // +0x10
};

long __fastcall HoverHornetQueen_GetNextCirclePoint_h(CAIStateGroup_HoverHornetQueen* self, int)
{
    long next = self->circlePointCount - self->currentCircleIndex;
    if (next < 0)
        next += self->numCirclePoints;
    return next;
}