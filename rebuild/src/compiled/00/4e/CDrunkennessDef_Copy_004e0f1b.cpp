struct CDefClassBase {
    char pad[0x28];
    unsigned long field28;
};

struct CDrunkennessDef : public CDefClassBase {
};

// Base-class copy — real target of the rel32 call. this in ECX, src on stack.
void __fastcall BaseClassCopyImpl(CDefClassBase* self, int edx, CDefClassBase const* src);

void __fastcall CDrunkennessDef_Copy(CDrunkennessDef* self, int edx, CDefClassBase const* src)
{
    BaseClassCopyImpl(self, edx, src);
    self->field28 = src->field28;
}