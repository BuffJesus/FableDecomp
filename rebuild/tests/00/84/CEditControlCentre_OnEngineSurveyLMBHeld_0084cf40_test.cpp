#include <cstdio>

struct C3DVector { unsigned long x, y, z; };

struct CEditControlCentre {
    char pad[0x90];
    unsigned long f90;
    unsigned long f94;
};

void __fastcall OnEngineSurveyLMBHeld(CEditControlCentre* self, int edx, C3DVector* v)
{
    self->f90 = v->x;
    self->f94 = v->y;
}

int main()
{
    CEditControlCentre o;
    o.f90 = 0; o.f94 = 0;
    C3DVector v; v.x = 0x40600000; v.y = 0x40E80000; v.z = 0x41100000;
    OnEngineSurveyLMBHeld(&o, 0, &v);
    if (o.f90 == 0x40600000 && o.f94 == 0x40E80000)
        printf("0084cf40_TEST PASS\n");
    else
        printf("FAIL %lu %lu\n", o.f90, o.f94);
    return 0;
}