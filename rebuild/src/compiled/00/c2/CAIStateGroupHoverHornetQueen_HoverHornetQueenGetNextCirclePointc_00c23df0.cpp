class CAIStateGroup_HoverHornetQueen {
public:
    int field_0x0;   // +0  unused by this method
    int field_0x4;   // +4  wrap length / count
    int field_0x8;   // +8  base index
    int field_0xc;   // +c  current index

    long GetNextCirclePoint();
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint()
{
    long delta = field_0xc - field_0x8;
    if (delta < 0)
        delta += field_0x4;
    return delta;
}