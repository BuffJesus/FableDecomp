#include <cstdio>

struct CEngine {
  char pad[0x18];
  unsigned char active;
  unsigned long sceneId;
  float sceneParam;
};

void __fastcall CEngine_StartNewScene(CEngine* self, int, unsigned long id, float param) {
  self->active = 1;
  self->sceneId = id;
  self->sceneParam = param;
}

int main() {
  CEngine e;
  e.active = 0; e.sceneId = 0; e.sceneParam = 0.0f;
  CEngine_StartNewScene(&e, 0, 42u, 3.5f);
  if (e.active != 1) { printf("FAIL active\n"); return 1; }
  if (e.sceneId != 42u) { printf("FAIL id\n"); return 2; }
  if (e.sceneParam != 3.5f) { printf("FAIL param\n"); return 3; }
  CEngine_StartNewScene(&e, 0, 7u, -1.0f);
  if (e.sceneId != 7u || e.sceneParam != -1.0f || e.active != 1) { printf("FAIL second\n"); return 4; }
  printf("OK_00b239a0\n");
  return 0;
}