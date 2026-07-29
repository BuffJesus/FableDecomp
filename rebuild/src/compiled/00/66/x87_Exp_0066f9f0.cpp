extern "C" __declspec(naked) double __stdcall
x87_Exp_0066f9f0(double value)
{
    __asm
    {
        fldl2e
        fmul qword ptr [esp + 4]
        fld st(0)
        frndint
        fxch st(1)
        fsub st(0), st(1)
        f2xm1
        fld1
        faddp st(1), st(0)
        fscale
        fstp st(1)
        ret 8
    }
}
