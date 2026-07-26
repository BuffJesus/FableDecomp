#include <stdio.h>
#include "candidates/mouse_dx_accessors_compiled.h"
int main() { fable_u8 s[0x4850]; CMouseDX* m=reinterpret_cast<CMouseDX*>(s); m->LMBDown=false; if(m->IsLMBDown()) return 1; m->LMBDown=true; if(!m->IsLMBDown()) return 2; printf("MOUSE_DX_ISLMBDOWN_TEST PASS\n"); return 0; }
