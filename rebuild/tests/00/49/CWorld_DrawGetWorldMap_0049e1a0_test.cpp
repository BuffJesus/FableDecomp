#include <stdio.h>
struct CWorld {
  char pad[0x10];
  char* m_map; // +0x10
  void* DrawGetWorldMap();
};
void* CWorld::DrawGetWorldMap() {
  char* p = m_map;
  if (p == 0) return 0;
  return p + 4;
}
int main() {
  // non-null case: m_map at +0x10 -> returns m_map+4
  char buf[64];
  CWorld w1;
  w1.m_map = buf;
  void* r1 = w1.DrawGetWorldMap();
  if (r1 != (void*)(buf + 4)) { printf("FAIL nonnull got %p exp %p\n", r1, (void*)(buf+4)); return 1; }
  // null case: returns 0
  CWorld w2;
  w2.m_map = 0;
  void* r2 = w2.DrawGetWorldMap();
  if (r2 != 0) { printf("FAIL null got %p\n", r2); return 1; }
  printf("OK_0x0049e1a0\n");
  return 0;
}