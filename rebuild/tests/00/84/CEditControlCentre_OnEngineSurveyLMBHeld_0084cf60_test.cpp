#include <cstdio>

struct C3DVector { float x; float y; float z; };

struct CEditControlCentre {
    char pad[0x98];
    unsigned int f98;
    unsigned int f9c;
};

void __fastcall OnEngineSurveyLMBHeld(CEditControlCentre* self, int, C3DVector* v);

int main()
{
    CEditControlCentre obj;
    obj.f98 = 0; obj.f9c = 0;
    C3DVector v;
    unsigned int* p = (unsigned int*)&v;
    p[0] = 0x11223344u;
    p[1] = 0x55667788u;
    OnEngineSurveyLMBHeld(&obj, 0, &v);
    if (obj.f98 == 0x11223344u && obj.f9c == 0x55667788u)
        printf("0084cf60_TEST PASS\n");
    else
        printf("FAIL %08x %08x\n", obj.f98, obj.f9c);
    return 0;
}