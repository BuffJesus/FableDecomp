// CActionDoCreatureAction::GetActionName  @ 0x004d2a74
//
// Retail body:
//   push esi
//   push -1
//   push <string literal 0x1239fb0>
//   mov  esi, ecx        ; ecx = hidden struct-return buffer (sret in ecx)
//   call CCharString::CCharString(char const*, int)   ; 0x99ebf0
//   mov  eax, esi        ; return the buffer pointer
//   pop  esi
//   ret
//
// The CCharString is returned by value. VC7.1 passes the hidden return-buffer
// pointer in ECX (sret-in-ecx) and emits a plain `ret`. A no-explicit-parameter
// __fastcall returning a UDT by value reproduces this exact codegen: the return
// buffer arrives in ecx ("mov esi,ecx"), the CCharString ctor is called on it,
// and the buffer pointer is returned in eax. len argument is -1.

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, int n);
};

CCharString __fastcall GetActionName()
{
    return CCharString("DoCreatureActionAttack", -1);
}