// CGameEvent::AddUBYTE  @ retail 0x0059b310
// Faithful VC7.1 model.
//
// Retail disasm:
//   push ecx            ; reserve 1 local dword (buf)
//   push esi            ; save esi
//   push 0              ; arg n = 0
//   lea  eax,[esp+0xb]  ; &buf (byte inside the reserved dword)
//   push eax            ; arg p = &buf
//   mov  esi,ecx        ; esi = this
//   call 0x59aef9       ; this->Helper(&buf, 0)   (__fastcall, ecx=this)
//   mov  eax,esi        ; return this
//   pop  esi
//   pop  ecx            ; free local
//   ret                 ; no stack args to clean
//
// The trailing `ret` (not `ret 4`) proves the compiled body takes no stack
// parameter; the byte value comes from a local buffer. The result in eax is
// `this`, so the modelled return type is CGameEvent*.

struct CGameEvent
{
    void Helper(void* p, int n);      // retail 0x0059aef9  (__fastcall)
    CGameEvent* AddUBYTE();
};

CGameEvent* CGameEvent::AddUBYTE()
{
    unsigned char buf;
    Helper(&buf, 0);
    return this;
}