struct CEngine {
  char pad[0x18];
  unsigned char active;   // +0x18
  unsigned long sceneId;  // +0x1c
  float sceneParam;       // +0x20
};

void __fastcall CEngine_StartNewScene(CEngine* self, int /*edx*/, unsigned long id, float param) {
  self->active = 1;
  self->sceneId = id;
  self->sceneParam = param;
}