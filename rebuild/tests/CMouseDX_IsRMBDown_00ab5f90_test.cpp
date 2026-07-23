#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"
int main() { fable_u8 s[0x4850]; CMouseDX* m=reinterpret_cast<CMouseDX*>(s); m->RMBDown=false; if(m->IsRMBDown()) return 1; m->RMBDown=true; if(!m->IsRMBDown()) return 2; printf("MOUSE_DX_ISRMBDown_TEST PASS\n"); return 0; }
