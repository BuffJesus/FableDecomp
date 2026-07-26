extern "C" void free(void*);
extern "C" void engine_op_delete(void*);

struct CInner {
};

extern void __fastcall CInner_dtor(CInner* p);

struct CLandscapeBackgroundPatch {
  int f0;
  int f4;
  void* f8;
  CInner inner;
  void* CLandscapeBackgroundPatch_ClearTextures(int flag);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_ClearTextures(int flag)
{
  if (this->f8) {
    free(this->f8);
    this->f8 = 0;
  }
  this->f4 = 0;
  CInner_dtor(&this->inner);
  if (flag & 1) {
    engine_op_delete(this);
  }
  return this;
}