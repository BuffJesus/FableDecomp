struct CDefClassBase { int base[10]; int f28; };

struct CExpressionSubDef {
    int base[10];
    int f28;
    // base class copy: real __fastcall member (this in ECX, src pushed on stack)
    void BaseClone(const CDefClassBase* src);
};

void __fastcall CExpressionSubDef_Copy(CExpressionSubDef* self, int /*edx*/, const CDefClassBase* src)
{
    self->BaseClone(src);
    self->f28 = ((const CExpressionSubDef*)src)->f28;
}