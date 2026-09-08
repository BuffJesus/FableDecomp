#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CScriptAttachmentTargetView {
    unsigned char _pad_0x00[0x91];
    unsigned char StateFlags;
    unsigned char _pad_0x92[0x100];
};

struct CGameScriptInterfaceAttachmentVTable {
    void* Slots[11];
    CScriptAttachmentTargetView* (__fastcall* GetTarget)(CGameScriptInterface*);
};

struct CScriptAttachmentComponent {
    void AttachToScript(int scriptId);
};

void __stdcall CGameScriptInterface_EntityAttachToScript(
    CGameScriptInterface* self, int scriptId)
{
    CGameScriptInterfaceAttachmentVTable* vtable =
        (CGameScriptInterfaceAttachmentVTable*)self->__vftable;
    CScriptAttachmentTargetView* target = vtable->GetTarget(self);
    if (target == 0)
        return;
    if (target->StateFlags & 1)
        return;
    ((CScriptAttachmentComponent*)((char*)target + 0x58))->AttachToScript(scriptId);
}
