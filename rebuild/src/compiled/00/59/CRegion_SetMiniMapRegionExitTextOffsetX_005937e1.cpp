#pragma optimize("s",on)
// Byte-exact reconstruction of 0x005937e1 CRegion::SetMiniMapRegionExitTextOffsetX
// Disasm:
//   push [esp+8]      ; forward stack arg2 (the float)
//   call 0x592f92     ; helper cleans its 4-byte arg, returns eax
//   mov ecx,[esp+4]   ; reload stack arg1 (dst ptr)
//   mov [ecx],eax     ; *dst = eax
//   mov eax,ecx       ; return dst
//   ret 8             ; stdcall-clean the 2 stack args
//
// Incoming ecx is never used -> not a real thiscall body. Two stack args
// cleaned via ret 8 -> __stdcall. Value-returning forwarder keeps call;ret.

struct CCharString;

// Helper at 0x592f92: takes the forwarded raw dword and cleans it (stdcall).
extern int __stdcall Helper592f92(int f);

extern "C" CCharString* __stdcall SetMiniMapRegionExitTextOffsetX(CCharString* dst, int f)
{
    *(int*)dst = Helper592f92(f);
    return dst;
}