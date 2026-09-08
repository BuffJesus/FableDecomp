#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld_Methods : CWorld {
  void* DrawGetWorldMap();
};
void* CWorld_Methods::DrawGetWorldMap() {
  char* p = (char*)this->DefinitionManager;  // mov eax,[ecx+0x10]
  if (p != 0)                                // test eax,eax / je
    return p + 4;                            // add eax,4 / ret
  return 0;                                  // xor eax,eax / ret
}
