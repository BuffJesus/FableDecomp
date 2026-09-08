#include <stdio.h>
#include "engine/CWorld.h"
struct CWorld_Methods : CWorld {
  void* DrawGetWorldMap();
};
void* CWorld_Methods::DrawGetWorldMap() {
  char* p = (char*)this->DefinitionManager;
  if (p == 0) return 0;
  return p + 4;
}
int main() {
  // non-null case: DefinitionManager at +0x10 -> returns ptr+4
  char buf[64];
  CWorld_Methods w1;
  w1.DefinitionManager = (CGameDefinitionManager*)buf;
  void* r1 = w1.DrawGetWorldMap();
  if (r1 != (void*)(buf + 4)) { printf("FAIL nonnull got %p exp %p\n", r1, (void*)(buf+4)); return 1; }
  // null case: returns 0
  CWorld_Methods w2;
  w2.DefinitionManager = 0;
  void* r2 = w2.DrawGetWorldMap();
  if (r2 != 0) { printf("FAIL null got %p\n", r2); return 1; }
  printf("OK_0x0049e1a0\n");
  return 0;
}
