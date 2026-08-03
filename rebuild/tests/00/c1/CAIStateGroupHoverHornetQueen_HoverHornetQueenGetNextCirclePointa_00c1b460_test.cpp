#include <stdio.h>

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

int main()
{
    CAIStateGroup_HoverHornetQueen obj;
    obj.field_0 = 0;
    obj.m_numCirclePoints = 8;
    obj.field_8 = 0;
    obj.m_curCircleIndex = 5;
    obj.m_nextCircleIndex = 2;
    long r1 = obj.GetNextCirclePoint_a();

    obj.m_curCircleIndex = 2;
    obj.m_nextCircleIndex = 5;
    long r2 = obj.GetNextCirclePoint_a();

    if (r1 == 5 && r2 == 3)
        printf("HHQNEXTPT_OK r1=%ld r2=%ld\n", r1, r2);
    else
        printf("HHQNEXTPT_FAIL r1=%ld r2=%ld\n", r1, r2);
    return 0;
}