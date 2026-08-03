class CAIStateGroup_HoverHornetQueen
{
public:
    long field_0;
    long m_wrapCount;      // +0x4
    long m_currentIndex;   // +0x8
    long m_nextIndex;      // +0xc

    long GetNextCirclePoint_b();
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint_b()
{
    long eax = m_nextIndex - m_currentIndex;
    if (eax < 0)
        eax += m_wrapCount;
    return eax;
}