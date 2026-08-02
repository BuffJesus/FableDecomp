#include <cstdio>

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

int main()
{
    CAIStateGroup_HoverHornetQueen obj;
    obj.vfptr_unused_00 = 0;
    obj.m_pad_08 = 0;

    // Case 1: no wraparound (cur >= max)
    obj.m_circleTotal_04 = 100;
    obj.m_circleMax_0c = 5;
    obj.m_circleCur_10 = 20;
    long r1 = obj.GetNextCirclePoint_g();
    if (r1 != 15) { printf("FAIL1 %ld\n", r1); return 1; }

    // Case 2: wraparound (cur < max)
    obj.m_circleTotal_04 = 360;
    obj.m_circleMax_0c = 30;
    obj.m_circleCur_10 = 10;
    long r2 = obj.GetNextCirclePoint_g();
    if (r2 != 340) { printf("FAIL2 %ld\n", r2); return 1; }

    // Case 3: exact zero boundary
    obj.m_circleTotal_04 = 50;
    obj.m_circleMax_0c = 7;
    obj.m_circleCur_10 = 7;
    long r3 = obj.GetNextCirclePoint_g();
    if (r3 != 0) { printf("FAIL3 %ld\n", r3); return 1; }

    printf("HOVERHORNET_QUEEN_CIRCLE_OK\n");
    return 0;
}