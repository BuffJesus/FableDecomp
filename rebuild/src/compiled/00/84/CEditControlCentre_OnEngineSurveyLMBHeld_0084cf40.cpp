struct C3DVector { unsigned long x, y, z; };

struct CEditControlCentre {
    char pad[0x90];
    unsigned long f90; // +0x90
    unsigned long f94; // +0x94
};

void __fastcall OnEngineSurveyLMBHeld(CEditControlCentre* self, int /*edx*/, C3DVector* v)
{
    self->f90 = v->x;
    self->f94 = v->y;
}