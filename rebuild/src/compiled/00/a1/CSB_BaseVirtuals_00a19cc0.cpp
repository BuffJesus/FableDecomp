// CSB (retail vtable 0x0129CC1C): destructor 0x00A19CC0 (vtable store only; no base),
// CreateLineList 0x00A19CE0 and Reset 0x00A19CF0 (slots 3/4, shared by every CSB* vtable),
// and the scalar deleting destructor 0x00A19D00 (expands the destructor in place).
// Names: FableWin.pdb 031b3c40 / 031b3c50 / 031b3ca0.
#include "fable_steering_behaviour.h"

CSB::~CSB() {}
void CSB::CreateLineList(_STL::vector<C3DVector>&) {}
void CSB::Reset() {}
