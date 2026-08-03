class CAIStateGroup_HoverHornetQueen
{
public:
    long field_0x0;
    long field_0x4;
    long field_0x8;
    long field_0xc;

    long GetNextCirclePoint(void);
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint(void)
{
    long result = field_0xc - field_0x8;
    if (result < 0)
    {
        result += field_0x4;
    }
    return result;
}