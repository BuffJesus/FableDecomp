#pragma optimize("s",on)
struct CPersistContext {
  void Transfer(int* p);
};
struct CCameraManagerSetDef {
  char pad0[0x3c];
  int f3c;
  int f40;
  int f44;
  int f48;
};
void __fastcall CCameraManagerSetDef_Transfer(CCameraManagerSetDef* self, void* unused, CPersistContext* ctx) {
  ctx->Transfer(&self->f3c);
  ctx->Transfer(&self->f40);
  ctx->Transfer(&self->f44);
  ctx->Transfer(&self->f48);
}