// ??_GCSystemFont@@UAEPAXI@Z @ 0x00a0a920 -- slot 0 of the retail CSystemFont
// vtable 0x0129C998 (RTTI COL 0x01356B00 .?AVCSystemFont@@). The implicit
// ~CSystemFont inlines to ~CInitBaseClass (0x0099A410).
#include "fable_system_font.h"

// Constructing and deleting through the virtual destructor makes the
// compiler emit the CSystemFont vtable and its scalar deleting destructor.
void CSystemFont_EmitScalarDeletingDestructor()
{
    CSystemFont* font = new CSystemFont;
    delete font;
}
