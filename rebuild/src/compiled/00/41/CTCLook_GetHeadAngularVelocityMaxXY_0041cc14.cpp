#pragma optimize("s",on)
// CTCLook::GetHeadAngularVelocityMaxXY @ retail 0x0041cc14 (23 bytes)
// Retail disasm:
//   cmp  BYTE PTR ds:0x13b8768, 0
//   je   else
//   fld  DWORD PTR ds:0x13b876c
//   ret
//  else:
//   fld  DWORD PTR ds:0x1375cd4
//   ret
//
// Pure global accessor: the this-pointer (ecx) is never touched.
// return  g_HeadAngularVelocityMaxXY_Override_Flag
//            ? g_HeadAngularVelocityMaxXY_Override_Value
//            : g_HeadAngularVelocityMaxXY_Default;
//
// VC7.1 (MSVC 2003): no C++11. Member accessor modeled as __fastcall
// (object ptr in ecx) so we never place __fastcall on a free function (C4234).

// --- engine globals (relocation-masked; addresses are illustrative) ---
// 0x13b8768 : override flag  (byte-sized bool)
extern unsigned char g_HeadAngVelMaxXY_OverrideFlag;
// 0x13b876c : override value (4 bytes after the flag -> same struct)
extern float         g_HeadAngVelMaxXY_OverrideValue;
// 0x1375cd4 : default value  (separate global)
extern float         g_HeadAngVelMaxXY_Default;

float __fastcall CTCLook__GetHeadAngularVelocityMaxXY(void *ecx_this)
{
    (void)ecx_this; // ecx unused; every operand is an absolute global
    if (g_HeadAngVelMaxXY_OverrideFlag != 0)
        return g_HeadAngVelMaxXY_OverrideValue;
    return g_HeadAngVelMaxXY_Default;
}