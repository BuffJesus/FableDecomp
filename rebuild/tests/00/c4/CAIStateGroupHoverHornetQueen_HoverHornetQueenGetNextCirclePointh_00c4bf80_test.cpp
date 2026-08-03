#include <cstdio>

struct CAIStateGroup_HoverHornetQueen
{
    int unk00;
    int numCirclePoints;
    int unk08;
    int currentCircleIndex;
    int circlePointCount;
};

long __fastcall HoverHornetQueen_GetNextCirclePoint_h(CAIStateGroup_HoverHornetQueen* self, int)
{
    long next = self->circlePointCount - self->currentCircleIndex;
    if (next < 0)
        next += self->numCirclePoints;
    return next;
}

int main()
{
    CAIStateGroup_HoverHornetQueen obj;
    obj.unk00 = 0;
    obj.numCirclePoints = 8;
    obj.unk08 = 0;
    obj.currentCircleIndex = 5;
    obj.circlePointCount = 3;

    long r1 = HoverHornetQueen_GetNextCirclePoint_h(&obj, 0);
    if (r1 != 6) { printf("FAIL1 %ld\n", r1); return 1; }

    obj.currentCircleIndex = 2;
    obj.circlePointCount = 5;
    long r2 = HoverHornetQueen_GetNextCirclePoint_h(&obj, 0);
    if (r2 != 3) { printf("FAIL2 %ld\n", r2); return 1; }

    printf("OK_HHQ_CIRCLE_TEST\n");
    return 0;
}