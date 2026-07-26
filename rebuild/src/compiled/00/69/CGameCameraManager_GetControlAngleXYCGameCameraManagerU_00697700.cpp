// ?GetControlAngleXY@CGameCameraManager@@UBEMXZ @ 0x00697700
// Retail: d9 81 b8 00 00 00 c3  =>  fld DWORD PTR [ecx+0xb8] ; ret
//
// Virtual const float accessor. Under VC7.1 (MSVC 2003) a this-only
// float getter compiled with /O2 emits `fld [ecx+disp]; ret` where the
// float return value is left in ST0. We model the __fastcall member as
// a __fastcall free function so `this` arrives in ECX (byte-identical),
// and place the returned float member at exactly offset 0xB8.

// Layout: single float member sits at +0xB8. Pad the leading region.
struct CGameCameraManager
{
    char _pad0[0xB8];      // 0x00 .. 0xB7
    float m_controlAngleXY; // 0xB8  <-- fld DWORD PTR [ecx+0xB8]
};

// __fastcall models __fastcall: object pointer (this) in ECX.
// (const accessor => object pointer is read-only; treat as CGameCameraManager*.)
float __fastcall GetControlAngleXY_CGameCameraManager(CGameCameraManager *thisptr)
{
    return thisptr->m_controlAngleXY;
}