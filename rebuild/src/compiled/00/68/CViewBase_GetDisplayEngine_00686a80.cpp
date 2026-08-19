// GetDisplayEngine @ 0x004c7aa0 (CViewBase)
// Retail:
//   mov eax, ds:[0x013b8a1c]  ; load global singleton pointer
//   mov eax, [eax+0x24]       ; return field at +0x24
//   ret
// Idiomatic: a global CViewBase* singleton; return m_pDisplayEngine at +0x24.

struct CDisplayEngine;

struct CViewBase {
    char pad[0x24];
    CDisplayEngine* m_pDisplayEngine; // +0x24
};

// Global singleton pointer (reloc-masked extern -> ds:[imm32]).
extern CViewBase* g_pViewBase;

CDisplayEngine* GetDisplayEngine()
{
    return g_pViewBase->m_pDisplayEngine;
}