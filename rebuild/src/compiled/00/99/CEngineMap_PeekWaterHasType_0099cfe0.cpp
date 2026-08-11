// CEngineMap::PeekWaterHasType @ 0099cfe0
//   bool __fastcall CEngineMap::PeekWaterHasType(long a, long b, EWaterType)
//
// Retail body (26 bytes):
//   mov  eax,[esp+8]      ; b
//   mov  edx,[esp+4]      ; a
//   push eax
//   push edx
//   call 0x99cf80         ; this->PeekWaterGetType(a,b)  (thiscall, ecx=this
//                         ;   already in ecx -> no reload)
//   test eax,eax
//   setne al
//   movzx eax,al
//   ret  8
//
// Observations that pin the shape:
//  * ret 8 -> only TWO dword stack params are consumed, so the manifest's 4th
//    EWaterType is not passed on the stack for this variant; the reconstructed
//    method takes (long a, long b).
//  * The helper is a __fastcall MEMBER on the same object: 'this' stays pinned
//    in ecx across the call, which is what forces the a-arg into edx and the
//    boolean result into al (a free-function helper frees ecx and mis-allocates
//    to ecx/dl, diverging by 2 bytes).
//  * The trailing 'movzx eax,al' (zero-extend to full register) only appears
//    when the return is widened to int; modelled here as an int return of a
//    bool test, which is value-identical to the bool the manifest lists.

struct CEngineMap
{
    void* PeekWaterGetType(long a, long b);   // helper @ 0x99cf80 (thiscall)
    int   PeekWaterHasType(long a, long b);
};

int CEngineMap::PeekWaterHasType(long a, long b)
{
    return static_cast<bool>(PeekWaterGetType(a, b));
}