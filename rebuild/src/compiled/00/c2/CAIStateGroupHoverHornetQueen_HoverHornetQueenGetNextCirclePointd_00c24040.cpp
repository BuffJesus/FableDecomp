struct CAIStateGroup_HoverHornetQueen
{
    int field0;      // +0x0
    long wrapAdjust; // +0x4
    long circleMax;  // +0x8
    long circleCur;  // +0xc

    long __fastcall GetNextCirclePoint(void* /*edx unused*/);
};

long __fastcall CAIStateGroup_HoverHornetQueen::GetNextCirclePoint(void*)
{
    long result = circleCur - circleMax;
    if (result < 0)
        result += wrapAdjust;
    return result;
}