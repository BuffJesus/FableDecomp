#include <cstdio>

static int g_inner_called = 0;
static void* g_inner_a = 0;
static unsigned int g_inner_b = 0;
static int g_free_called = 0;
static void* g_freed = 0;

struct CBankFileAsyncData {
  char pad0[0xc];
  void* p0c;
  char pad10[0x1c-0x10];
  unsigned int v1c;
  void* p20;
  void Inner(void* a, unsigned int b);
};

void CBankFileAsyncData::Inner(void* a, unsigned int b) {
  g_inner_called++; g_inner_a = a; g_inner_b = b;
}
void __cdecl EngineFree(void* p) { g_free_called++; g_freed = p; }

void __fastcall CBankFileAsyncData_FreeBuffer(CBankFileAsyncData* self)
{
  if (self->p20 != 0) {
    self->Inner(self->p0c, self->v1c + 4);
    if (self->p20 != 0) {
      EngineFree(self->p20);
    }
  }
}

int main() {
  CBankFileAsyncData o;
  char buf[8];
  o.p0c = (void*)0x1234;
  o.v1c = 100;
  o.p20 = (void*)buf;
  CBankFileAsyncData_FreeBuffer(&o);
  if (g_inner_called != 1) { std::printf("FAIL inner\n"); return 1; }
  if (g_inner_a != (void*)0x1234) { std::printf("FAIL a\n"); return 1; }
  if (g_inner_b != 104) { std::printf("FAIL b\n"); return 1; }
  if (g_free_called != 1) { std::printf("FAIL free\n"); return 1; }
  if (g_freed != (void*)buf) { std::printf("FAIL freed\n"); return 1; }

  g_inner_called = 0; g_free_called = 0;
  o.p20 = 0;
  CBankFileAsyncData_FreeBuffer(&o);
  if (g_inner_called != 0 || g_free_called != 0) { std::printf("FAIL null path\n"); return 1; }

  std::printf("CBankFileAsyncData_0040f470_TEST PASS\n");
  return 0;
}