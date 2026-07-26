// NDisplayView::CViewBase::GetDisplayEngine @ retail 0x004c79d0
// Retail bytes: a1 1c8a3b01 8b4028 c3
//   mov eax, ds:[0x13b8a1c]   ; load global singleton pointer
//   mov eax, [eax+0x28]       ; return CDisplayEngine* at +0x28
//   ret
//
// The accessor ignores its 'this' pointer entirely -- it reads a fixed
// process-global instance pointer, not ecx. We model it as a free
// __fastcall function (byte-identical to __fastcall for a this-only
// accessor; the this arg simply arrives in ecx and is never used).
//
// VC7.1 (MSVC 2003) constraints honored: no C++11, extern global for the
// relocation-masked absolute address.

struct CDisplayEngine;

// Forward decl of the object type the global points at. Only the +0x28
// member layout matters for byte parity.
struct CViewGlobal
{
    char _pad0x28[0x28];        // 0x00 .. 0x27
    CDisplayEngine *pEngine;    // 0x28 -> returned value
};

// The absolute-address global read (ds:[0x13b8a1c]). Relocation-masked in
// parity, so the actual link address is irrelevant to .text bytes.
extern CViewGlobal *g_viewBaseGlobal;

// __fastcall so the (unused) this arrives in ecx exactly like __fastcall.
CDisplayEngine * __fastcall NDisplayView__CViewBase__GetDisplayEngine(void * /*thisptr*/)
{
    return g_viewBaseGlobal->pEngine;
}