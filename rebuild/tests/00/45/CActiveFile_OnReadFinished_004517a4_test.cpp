#include <cstdio>
struct CActiveFile {
  void* buf;
  void notify();
};
static int g_notified = 0;
static void* g_freed = (void*)-1;
void CActiveFile::notify() { g_notified++; }
extern "C" void some_free(void* p) { g_freed = p; }
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self) {
  self->notify();
  void* p = self->buf;
  if (p) some_free(p);
}
int main() {
  CActiveFile a; int dummy = 5; a.buf = &dummy;
  CActiveFile_OnReadFinished(&a);
  if (g_notified != 1) { std::printf("FAIL notify\n"); return 1; }
  if (g_freed != &dummy) { std::printf("FAIL freed\n"); return 1; }
  g_notified = 0; g_freed = (void*)-1;
  CActiveFile b; b.buf = 0;
  CActiveFile_OnReadFinished(&b);
  if (g_notified != 1) { std::printf("FAIL notify2\n"); return 1; }
  if (g_freed != (void*)-1) { std::printf("FAIL freed2\n"); return 1; }
  std::printf("CActiveFile_004517a4_TEST PASS\n");
  return 0;
}