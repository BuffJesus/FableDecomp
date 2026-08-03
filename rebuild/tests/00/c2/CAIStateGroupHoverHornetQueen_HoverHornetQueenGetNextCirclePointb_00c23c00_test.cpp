#include <stdio.h>

class CAIStateGroup_HoverHornetQueen
{
public:
    long field_0;
    long m_wrapCount;
    long m_currentIndex;
    long m_nextIndex;

    long GetNextCirclePoint_b();
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint_b()
{
    long eax = m_nextIndex - m_currentIndex;
    if (eax < 0)
        eax += m_wrapCount;
    return eax;
}

int main()
{
    CAIStateGroup_HoverHornetQueen obj;

    obj.field_0 = 0;
    obj.m_wrapCount = 8;
    obj.m_currentIndex = 2;
    obj.m_nextIndex = 5;
    long r1 = obj.GetNextCirclePoint_b();
    if (r1 != 3) { printf("FAIL1 %ld\n", r1); return 1; }

    obj.m_wrapCount = 8;
    obj.m_currentIndex = 6;
    obj.m_nextIndex = 1;
    long r2 = obj.GetNextCirclePoint_b();
    if (r2 != 3) { printf("FAIL2 %ld\n", r2); return 1; }

    obj.m_wrapCount = 10;
    obj.m_currentIndex = 0;
    obj.m_nextIndex = 0;
    long r3 = obj.GetNextCirclePoint_b();
    if (r3 != 0) { printf("FAIL3 %ld\n", r3); return 1; }

    printf("TESTOK_HHQ_CIRCLE\n");
    return 0;
}