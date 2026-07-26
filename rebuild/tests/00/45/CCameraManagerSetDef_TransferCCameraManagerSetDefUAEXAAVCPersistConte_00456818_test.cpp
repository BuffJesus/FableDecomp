#include <cstdio>
struct CPersistContext {
  int count;
  int* seen[8];
  void Transfer(int* p) { if (count < 8) seen[count] = p; count++; }
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
int main() {
  CCameraManagerSetDef d;
  CPersistContext ctx; ctx.count = 0;
  CCameraManagerSetDef_Transfer(&d, 0, &ctx);
  if (ctx.count == 4 &&
      ctx.seen[0] == &d.f3c && ctx.seen[1] == &d.f40 &&
      ctx.seen[2] == &d.f44 && ctx.seen[3] == &d.f48) {
    std::printf("CCameraManagerSetDef_00456818_TEST PASS\n");
    return 0;
  }
  std::printf("FAIL count=%d\n", ctx.count);
  return 1;
}