#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"
int main() { fable_u8 s[0x4850]; CMouseDX* m=reinterpret_cast<CMouseDX*>(s); m->MMBDown=false; if(m->IsMMBDown()) return 1; m->MMBDown=true; if(!m->IsMMBDown()) return 2; printf("MOUSE_DX_ISMMBDOWN_TEST PASS\n"); return 0; }
