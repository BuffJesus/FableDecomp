#include <cstdio>

class CAIStateGroup_HoverHornetQueen {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;

    long GetNextCirclePoint();
};

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint()
{
    long delta = field_0xc - field_0x8;
    if (delta < 0)
        delta += field_0x4;
    return delta;
}

int main()
{
    CAIStateGroup_HoverHornetQueen obj;

    // Case 1: positive delta, no wrap
    obj.field_0x4 = 100;
    obj.field_0x8 = 10;
    obj.field_0xc = 30;
    long r1 = obj.GetNextCirclePoint();

    // Case 2: negative delta, wraps around using field_0x4
    obj.field_0x4 = 100;
    obj.field_0x8 = 90;
    obj.field_0xc = 10;
    long r2 = obj.GetNextCirclePoint();

    // Case 3: exact zero delta
    obj.field_0x4 = 50;
    obj.field_0x8 = 20;
    obj.field_0xc = 20;
    long r3 = obj.GetNextCirclePoint();

    if (r1 == 20 && r2 == 20 && r3 == 0) {
        printf("CIRCLEPOINT_OK\n");
    } else {
        printf("CIRCLEPOINT_FAIL %ld %ld %ld\n", r1, r2, r3);
    }

    return 0;
}