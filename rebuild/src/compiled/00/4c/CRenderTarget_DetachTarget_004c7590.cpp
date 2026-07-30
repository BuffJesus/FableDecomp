// CRenderTarget::DetachTarget @ 0x004c7590
// Thin forwarder: load this->m_child (offset +0x64); if non-null, tail-call
// the child's DetachTarget with the same 1 stack arg. Otherwise return.
// __fastcall: this in ECX, one 4-byte stack arg (ret 4).
// External tail-call target 0x4bf9e0 is reloc-masked at verify time.
extern "C" void __fastcall CRenderTarget_ChildDetach(void* self, void* edx, void* arg);

__declspec(naked) void __fastcall DetachTarget(void* thisptr, void* edx, void* arg)
{
    (void)thisptr; (void)edx; (void)arg;
    __asm {
        mov ecx, dword ptr [ecx+0x64]
        test ecx, ecx
        je short done
        jmp CRenderTarget_ChildDetach
    done:
        ret 4
    }
}