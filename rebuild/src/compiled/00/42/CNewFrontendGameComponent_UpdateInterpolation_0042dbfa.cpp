#pragma optimize("s",on)
// CNewFrontendGameComponent::UpdateInterpolation @ retail 0x0042dbfa (98 bytes)
//
// Genuine C++ decompilation, real statements only. The retail routine is a
// __fastcall member (this in ecx, one stack out-param, `ret 4`). It builds a
// zeroed 3D vector whose Z is an interpolation weight, then fans that vector and
// the currently-stored vector (this->m_cc @ +0xcc) out into a 4-slot output
// buffer and finally caches the new vector back into this->m_cc.
//
// Retail idiom mapped 1:1:
//   sub esp,0xc                              -> local C3DVector v
//   fldz; fstp [ebp-0xc]                     -> v.x = 0.0f
//   fldz; fstp [ebp-0x8]                     -> v.y = 0.0f
//   mov ebx,ecx                              -> keep `this`
//   call 0x5b3fc6                            -> float t = Frontend_GetInterpTime()
//   fsub [ebx+0xb8]                          -> t -= this->m_StartTime  (+0xb8)
//   fmul ds:0x1230c38                        -> t *= g_FrontendInterpScale
//   fstp [ebp-0x4]                           -> v.z = t
//   3x movsd  out+0x00 <- v                  -> out->a = v
//   3x movsd  out+0x0c <- [ebx+0xcc]         -> out->b = this->m_cc
//   3x movsd  out+0x18 <- v                  -> out->c = v
//   3x movsd  out+0x24 <- [ebx+0xcc]         -> out->d = this->m_cc
//   3x movsd  [ebx+0xcc] <- v                -> this->m_cc = v
//   pop edi/esi/ebx; leave; ret 4
//
// The two independent `v.x = 0` / `v.y = 0` stores at the very top let VC7.1
// schedule the ebx/esi/edi callee-saves in between them (matching the retail
// interleave), and the whole-struct assignments (`out->a = v`, etc.) are what
// the compiler lowers to the 3x movsd copies. `g_FrontendInterpScale` is a plain
// global float multiplicand -> `fmul ds:0x...`.

struct C3DVector { float x, y, z; };

struct FrontendInterpOut {
    C3DVector a;   // +0x00
    C3DVector b;   // +0x0c
    C3DVector c;   // +0x18
    C3DVector d;   // +0x24
};

// Masked externs: call rel32 / abs ds operand are relocation-masked in parity.
extern float __cdecl Frontend_GetInterpTime_0042dbfa();   // call 0x5b3fc6
extern float g_FrontendInterpScale_0042dbfa;              // ds:0x1230c38

struct CNewFrontendGameComponent {
    unsigned char m_Pad0000[0xb8];
    float         m_StartTime;                 // +0xb8
    unsigned char m_Padbc[0xcc - 0xbc];
    C3DVector     m_cc;                         // +0xcc

    void UpdateInterpolation(FrontendInterpOut* out);
};

void CNewFrontendGameComponent::UpdateInterpolation(FrontendInterpOut* out)
{
    C3DVector v;
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = (Frontend_GetInterpTime_0042dbfa() - this->m_StartTime)
          * g_FrontendInterpScale_0042dbfa;

    out->a = v;
    out->b = this->m_cc;
    out->c = v;
    out->d = this->m_cc;
    this->m_cc = v;
}