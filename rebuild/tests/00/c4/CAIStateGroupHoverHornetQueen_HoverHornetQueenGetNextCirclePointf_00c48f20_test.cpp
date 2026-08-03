struct CAIStateGroup_HoverHornetQueen
{
    void* vfptr;
    long m_wrapAmount;
    long field_0x8;
    long m_current;
    long m_total;

    long __fastcall GetNextCirclePoint(void*);
};

long __fastcall CAIStateGroup_HoverHornetQueen::GetNextCirclePoint(void*)
{
    long result = m_total - m_current;
    if (result < 0)
        result += m_wrapAmount;
    return result;
}

#include <cstdio>

int main()
{
    CAIStateGroup_HoverHornetQueen a;
    a.vfptr = 0;
    a.m_wrapAmount = 100;
    a.field_0x8 = 0;
    a.m_current = 30;
    a.m_total = 50;
    long r1 = a.GetNextCirclePoint(0);
    if (r1 != 20) { printf("FAIL1 %ld\n", r1); return 1; }

    CAIStateGroup_HoverHornetQueen b;
    b.vfptr = 0;
    b.m_wrapAmount = 100;
    b.field_0x8 = 0;
    b.m_current = 90;
    b.m_total = 50;
    long r2 = b.GetNextCirclePoint(0);
    if (r2 != 60) { printf("FAIL2 %ld\n", r2); return 1; }

    printf("HOVERHORNET_OK\n");
    return 0;
}