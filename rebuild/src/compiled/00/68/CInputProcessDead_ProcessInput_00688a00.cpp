// CInputProcessDead::ProcessInput  @ 0x00688a00
// Empty override: dead hero consumes no input. Body is a no-op; the two
// reference args (CInputEvent const&, CProcessedInput&) occupy 8 bytes of
// stack, so the __fastcall epilogue is `ret 8`.
//
// The harness rewrites __fastcall -> __fastcall (this in ecx, next in edx).
// A real __fastcall puts `this` in ecx and BOTH refs on the stack. To model
// that under __fastcall we give a dummy edx param, then the two refs land on
// the stack -> callee cleans 8 bytes -> `ret 8`.

#include "engine/CInputProcessDead.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputEvent { int _pad; };
struct CProcessedInput { int _pad; };

void __fastcall ProcessInput(CInputProcessDead* self,
                             int edx_dummy,
                             const CInputEvent& ev,
                             CProcessedInput& out)
{
    (void)self;
    (void)edx_dummy;
    (void)ev;
    (void)out;
}