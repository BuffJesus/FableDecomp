#pragma optimize("s",on)
extern "C" void __cdecl retail_free(void* p);
struct CActiveFile {
  void* buf;
  void OtherStep();
};
void __fastcall OnReadFinished(CActiveFile* self) {
  self->OtherStep();
  if (self->buf)
    retail_free(self->buf);
}