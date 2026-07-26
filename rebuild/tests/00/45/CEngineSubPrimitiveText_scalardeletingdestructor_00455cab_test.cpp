#include <cstdio>
#include <new>

struct CEngineSubPrimitiveText { char pad[0x3c]; };
struct CMember { char c; };

static int g_member = 0;
static int g_dtor = 0;

void __fastcall CMember_dtor(CMember* self) { (void)self; g_member++; }
void __fastcall CEngineSubPrimitiveText_dtor(CEngineSubPrimitiveText* self) { (void)self; g_dtor++; }

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int, unsigned int flags)
{
  CMember_dtor((CMember*)((char*)self + 0x3c));
  CEngineSubPrimitiveText_dtor(self);
  if (flags & 1) {
    ::operator delete(self);
  }
  return self;
}

int main() {
  CEngineSubPrimitiveText* obj = (CEngineSubPrimitiveText*)::operator new(sizeof(CEngineSubPrimitiveText));
  void* r = CEngineSubPrimitiveText_scalar_deleting_destructor(obj, 0, 0);
  if (r != obj || g_member != 1 || g_dtor != 1) { std::printf("FAIL nodelete m=%d d=%d\n", g_member, g_dtor); return 1; }
  r = CEngineSubPrimitiveText_scalar_deleting_destructor(obj, 0, 1);
  if (r != obj || g_member != 2 || g_dtor != 2) { std::printf("FAIL delete m=%d d=%d\n", g_member, g_dtor); return 1; }
  std::printf("CEngineSubPrimitiveText_00455cab_TEST PASS\n");
  return 0;
}