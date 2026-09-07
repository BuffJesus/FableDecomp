#include "engine/CIEngine.h"
#include <cstdio>


bool __fastcall CIEngine_IsActive(const CIEngine* self) {
  return self->Active;
}

int main() {
  CIEngine e;
  e.Active = true;
  if (CIEngine_IsActive(&e) != true) { std::printf("FAIL true\n"); return 1; }
  e.Active = false;
  if (CIEngine_IsActive(&e) != false) { std::printf("FAIL false\n"); return 1; }
  std::printf("CIEngine_00b23550_TEST PASS\n");
  return 0;
}