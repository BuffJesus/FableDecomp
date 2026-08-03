#include <stdio.h>

struct CAIStateGroup_HoverHornetQueen
{
    int field0;
    long wrapAdjust;
    long circleMax;
    long circleCur;

    long __fastcall GetNextCirclePoint(void*);
};

long __fastcall CAIStateGroup_HoverHornetQueen::GetNextCirclePoint(void*)
{
    long result = circleCur - circleMax;
    if (result < 0)
        result += wrapAdjust;
    return result;
}

int main()
{
    CAIStateGroup_HoverHornetQueen obj;

    obj.field0 = 0;
    obj.wrapAdjust = 100;
    obj.circleMax = 40;
    obj.circleCur = 60;
    long r1 = obj.GetNextCirclePoint(0);
    if (r1 != 20) { printf("FAIL1 %ld\n", r1); return 1; }

    obj.wrapAdjust = 360;
    obj.circleMax = 50;
    obj.circleCur = 10;
    long r2 = obj.GetNextCirclePoint(0);
    if (r2 != 320) { printf("FAIL2 %ld\n", r2); return 1; }

    obj.wrapAdjust = 7;
    obj.circleMax = 0;
    obj.circleCur = 0;
    long r3 = obj.GetNextCirclePoint(0);
    if (r3 != 0) { printf("FAIL3 %ld\n", r3); return 1; }

    printf("HOVERHORNET_OK\n");
    return 0;
}