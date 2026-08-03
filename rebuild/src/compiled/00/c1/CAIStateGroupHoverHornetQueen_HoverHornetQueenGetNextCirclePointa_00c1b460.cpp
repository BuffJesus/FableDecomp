class CAIStateGroup_HoverHornetQueen
{
public:
    int field_0;
    int m_numCirclePoints;
    int field_8;
    int m_curCircleIndex;
    int m_nextCircleIndex;

    long GetNextCirclePoint_a();
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint_a()
{
    long next = m_nextCircleIndex - m_curCircleIndex;
    if (next < 0)
        next += m_numCirclePoints;
    return next;
}