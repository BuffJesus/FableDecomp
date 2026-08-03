struct CAIStateGroup_HoverHornetQueen
{
    void* vfptr_unused_00;
    long m_circleTotal_04;
    long m_pad_08;
    long m_circleMax_0c;
    long m_circleCur_10;

    long __fastcall GetNextCirclePoint_g();
};

long __fastcall CAIStateGroup_HoverHornetQueen::GetNextCirclePoint_g()
{
    long result = m_circleCur_10 - m_circleMax_0c;
    if (result < 0)
        result += m_circleTotal_04;
    return result;
}