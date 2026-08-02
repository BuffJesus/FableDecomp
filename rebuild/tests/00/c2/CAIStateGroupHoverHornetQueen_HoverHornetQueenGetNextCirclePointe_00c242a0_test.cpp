#include <cstdio>

class CAIStateGroup_HoverHornetQueen
{
public:
    long field_0x0;
    long field_0x4;
    long field_0x8;
    long field_0xc;

    long GetNextCirclePoint(void)
    {
        long result = field_0xc - field_0x8;
        if (result < 0)
        {
            result += field_0x4;
        }
        return result;
    }
};

int main()
{
    int pass = 1;

    CAIStateGroup_HoverHornetQueen a;
    a.field_0x4 = 100;
    a.field_0x8 = 10;
    a.field_0xc = 40;
    long r1 = a.GetNextCirclePoint();
    if (r1 != 30) pass = 0;

    CAIStateGroup_HoverHornetQueen b;
    b.field_0x4 = 100;
    b.field_0x8 = 40;
    b.field_0xc = 10;
    long r2 = b.GetNextCirclePoint();
    if (r2 != 70) pass = 0;

    CAIStateGroup_HoverHornetQueen c;
    c.field_0x4 = 8;
    c.field_0x8 = 5;
    c.field_0xc = 5;
    long r3 = c.GetNextCirclePoint();
    if (r3 != 0) pass = 0;

    CAIStateGroup_HoverHornetQueen d;
    d.field_0x4 = 8;
    d.field_0x8 = 0;
    d.field_0xc = 7;
    long r4 = d.GetNextCirclePoint();
    if (r4 != 7) pass = 0;

    if (pass)
        printf("HORNETQUEEN_CIRCLE_OK\n");
    else
        printf("HORNETQUEEN_CIRCLE_FAIL r1=%ld r2=%ld r3=%ld r4=%ld\n", r1, r2, r3, r4);

    return 0;
}