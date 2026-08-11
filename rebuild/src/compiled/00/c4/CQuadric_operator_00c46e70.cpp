struct CQuadric {
    float f0;
    float f4;
};

// Retail 0x00c46e70 CQuadric::operator+= : __fastcall (this=ecx),
// rhs in edx, rhs2 on stack ([esp+4]), returns this in eax, ret 4.
// Harness rewrites __fastcall->__fastcall: ecx=self, edx=rhs, stack=rhs2.
CQuadric* __fastcall CQuadric_operator_add_eq(CQuadric* self, CQuadric* rhs, CQuadric* rhs2)
{
    float t4 = rhs->f4 + rhs2->f4;
    float t0 = rhs->f0 + rhs2->f0;
    self->f0 = t0;
    self->f4 = t4;
    return self;
}