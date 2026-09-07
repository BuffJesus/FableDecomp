#include "engine/CTCWife.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTCWife; extern int __fastcall imp_helper(CTCWife*);
bool __fastcall IsMarriedToThePlayer(CTCWife* self){ return self->FrameGotMarriedToThePlayer <= imp_helper(self) ? true : false; }