struct CIEngine {
  char pad[0x8];
  bool active;
};

bool __fastcall CIEngine_IsActive(const CIEngine* self) {
  return self->active;
}