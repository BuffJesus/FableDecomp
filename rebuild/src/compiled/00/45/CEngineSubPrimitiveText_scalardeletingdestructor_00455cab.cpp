#pragma optimize("s",on)
#include <new>

struct CEngineSubPrimitiveText;

struct CMember { void __fastcall dtor(); };
extern void __fastcall CMember_dtor(CMember* self);
extern void __fastcall CEngineSubPrimitiveText_dtor(CEngineSubPrimitiveText* self);

struct CEngineSubPrimitiveText {
  char pad[0x3c];
};

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int /*edx*/, unsigned int flags)
{
  CMember_dtor((CMember*)((char*)self + 0x3c));
  CEngineSubPrimitiveText_dtor(self);
  if (flags & 1) {
    ::operator delete(self);
  }
  return self;
}