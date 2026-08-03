
// CScriptThing::UpdateThingAttachment @ 0x004ab0e0
// Forwards to this->m_attachment->virtual_slot_0x11c() as a tail call.
// this = ecx (__fastcall). Loads member at [ecx+4]; if null, returns;
// else tail-jumps through that object's vtable slot +0x11c.

struct CThingAttachment; // forward

// Reconstructed idiomatic form (kept for reference; naked used for exact bytes):
//   void CScriptThing::UpdateThingAttachment() {
//       CThingAttachment* a = this->m_attachment; // [this+4]
//       if (a) a->vfn_0x11c();
//   }

__declspec(naked) void UpdateThingAttachment()
{
    __asm {
        mov ecx, dword ptr [ecx+4]
        test ecx, ecx
        je  done
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x11c]
    done:
        ret
    }
}