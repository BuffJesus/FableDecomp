struct C3DVector { float x; float y; float z; };

struct CEditControlCentre {
    char pad[0x98];
    unsigned int f98;
    unsigned int f9c;
};

void __fastcall OnEngineSurveyLMBHeld(CEditControlCentre* self, int /*edx*/, C3DVector* v)
{
    unsigned int* p = (unsigned int*)v;
    self->f98 = p[0];
    self->f9c = p[1];
}