#pragma optimize("s",on)
struct CActiveFile {
  void* buf;
  void notify();
};
extern "C" void some_free(void*);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self) {
  self->notify();
  void* p = self->buf;
  if (p) some_free(p);
}