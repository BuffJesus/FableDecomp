// NDisplayView::CViewBase::GetDisplayEngine @ 0x004c7990
// Retail (9 bytes):
//   a1 1c 8a 3b 01   mov eax, ds:0x13b8a1c   ; load global singleton pointer
//   8b 40 20         mov eax, [eax+0x20]     ; return singleton->field_0x20
//   c3               ret
//
// The accessor ignores 'this' entirely: it reads a fixed global pointer and
// returns its member at offset 0x20. Modeled as __fastcall so the this pointer
// arrives in ecx (byte-identical to __fastcall for a this-only accessor); the
// pointer is unused, matching the disasm.
//
// Build: cl /c /O2 /Oy /W3

struct CDisplayEngine;

// The struct holding the display engine pointer at +0x20. Only offset 0x20
// matters for byte parity; earlier fields are padding to force the layout.
struct DisplaySingleton {
    char _pad[0x20];               // 0x00..0x1F
    CDisplayEngine *displayEngine; // 0x20
};

// Global singleton pointer at absolute address 0x13b8a1c (relocation-masked).
extern DisplaySingleton *g_displaySingleton;

struct CViewBase {
    CDisplayEngine *GetDisplayEngine();
};

CDisplayEngine *CViewBase::GetDisplayEngine()
{
    // mov eax, ds:g_displaySingleton ; mov eax, [eax+0x20] ; ret
    return g_displaySingleton->displayEngine;
}