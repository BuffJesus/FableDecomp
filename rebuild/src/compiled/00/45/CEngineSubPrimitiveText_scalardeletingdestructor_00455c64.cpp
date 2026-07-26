#pragma optimize("s",on)
struct SubObj { int x; };
extern "C" void __fastcall SubObj_dtor(SubObj* p);
struct CEngineSubPrimitiveText {
  char pad[0x3c];
  SubObj member;
  void realdtor();
  void* scalar_deleting_destructor(unsigned int flags);
};
extern "C" void operator_delete(void* p);

void* CEngineSubPrimitiveText::scalar_deleting_destructor(unsigned int flags) {
  SubObj_dtor((SubObj*)((char*)this + 0x3c));
  this->realdtor();
  if (flags & 1) {
    operator_delete((void*)this);
  }
  return this;
}