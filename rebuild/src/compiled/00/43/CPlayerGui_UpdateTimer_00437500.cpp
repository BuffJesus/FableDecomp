// CPlayerGui::UpdateTimer  (retail 0x00437500)
//
// Genuine C++ decompilation.  VC7.1 `cl /c /O2 /Oy /W3` compiles this to the
// exact retail bytes; the only masked bytes are the two `fcomp disp32` operands
// (the upper/lower threshold float globals), which are relocation-masked in
// parity.
//
// ABI: the retail body reads every argument from the stack ([esp+4]..[esp+14h])
// with no register this/edx and closes with `ret 14h` (5 dword args popped),
// i.e. a __stdcall free helper.  The `sub eax,2 / je / dec eax / jne` entry
// guard and the shared `fstp [pTimer]` tail are the classic MSVC codegen for a
// switch(*pMode) over the two adjacent cases 2 and 3.  The trailing clamp is
// two float compares against the extern threshold globals.
//
// Params (byte layout inferred from the disasm):
//   [esp+04] const float* pRate     (fld [pRate])
//   [esp+08] float        stepUp    (fdiv divisor, case 2 add branch)
//   [esp+0C] float        stepDown  (fdiv divisor, case 3 subr branch)
//   [esp+10] int*         pMode     (switch selector; also written on clamp)
//   [esp+14] float*       pTimer    (accumulator; fld/fstp [pTimer])
//
// The two masked absolute float constants are the comparison thresholds
// (g_Upper == 1.0f trigger, g_Lower == 0.0f floor).

extern const float CPlayerGui_UpdateTimer_00437500_Upper;
extern const float CPlayerGui_UpdateTimer_00437500_Lower;

void __stdcall CPlayerGui_UpdateTimer_00437500(
    const float* pRate,
    float stepUp,
    float stepDown,
    int* pMode,
    float* pTimer)
{
    switch (*pMode) {
    case 2:
        *pTimer = *pTimer + *pRate / stepUp;
        break;
    case 3:
        *pTimer = *pTimer - *pRate / stepDown;
        break;
    }

    if (*pTimer <= CPlayerGui_UpdateTimer_00437500_Upper) {
        *pMode = 1;
        *pTimer = 0.0f;
    } else if (*pTimer >= CPlayerGui_UpdateTimer_00437500_Lower) {
        *pMode = 0;
        *pTimer = 1.0f;
    }
}