#include "engine/CJoystickDX.h"  // retyped onto the PDB layout; byte parity re-verified
struct CJoystickDX_Methods : CJoystickDX {
    float M();
};
float CJoystickDX_Methods::M(){ return this->XAxis; }