// CActionDoCreatureAction::GetActionName() const  @ 0x007276B0
//
// Mangled: ?GetActionName@CActionDoCreatureAction@NTCScriptedControl@@UBE?AVCCharString@@XZ
// const __fastcall member returning CCharString BY VALUE (hidden return ptr).
//
// Retail body:
//   push esi
//   mov  esi,[esp+8]        ; esi = hidden return-value pointer (CCharString*)
//   push -1                 ; 2nd ctor arg (length = -1)
//   push 0x1239f78          ; 1st ctor arg (const char* literal), reloc-masked
//   mov  ecx,esi            ; ecx = this for the CCharString ctor
//   call CCharString::CCharString(const char*,int)
//   mov  eax,esi            ; return the hidden return pointer in eax
//   pop  esi
//   ret  4                  ; clean the hidden return pointer
//
// Modeled as a genuine member method so VC7.1 uses the __fastcall struct-
// return ABI: this in ecx, hidden return pointer on the stack, ret 4.

struct StringRep;

struct CCharString
{
    StringRep* rep;
    CCharString(const char* s, int n);
};

#pragma auto_inline(off)
CCharString::CCharString(const char* s, int n)
{
    rep = (StringRep*)(s ? (void*)(n) : 0);
}
#pragma auto_inline(on)

struct CActionDoCreatureAction
{
    void* vtbl;
    CCharString GetActionName() const;
};

CCharString CActionDoCreatureAction::GetActionName() const
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}