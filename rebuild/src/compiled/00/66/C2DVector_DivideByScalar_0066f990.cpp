extern "C" float C2DVector_DivideByScalar_0066f990_Unit;

extern "C" __declspec(naked) void* __fastcall
C2DVector_DivideByScalar_0066f990(
    void* vector, void*, float divisor)
{
    __asm
    {
        fld dword ptr [C2DVector_DivideByScalar_0066f990_Unit]
        mov eax, ecx
        fdiv dword ptr [esp + 4]
        fld st(0)
        fmul dword ptr [eax]
        fstp dword ptr [eax]
        fmul dword ptr [eax + 4]
        fstp dword ptr [eax + 4]
        ret 4
    }
}
