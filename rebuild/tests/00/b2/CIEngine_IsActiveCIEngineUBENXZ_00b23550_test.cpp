#include <cstdio>

struct CIEngine {
  char pad[0x8];
  bool active;
};

bool __fastcall CIEngine_IsActive(const CIEngine* self) {
  return self->active;
}

int main() {
  CIEngine e;
  e.active = true;
  if (CIEngine_IsActive(&e) != true) { std::printf("FAIL true\n"); return 1; }
  e.active = false;
  if (CIEngine_IsActive(&e) != false) { std::printf("FAIL false\n"); return 1; }
  std::printf("CIEngine_00b23550_TEST PASS\n");
  return 0;
}