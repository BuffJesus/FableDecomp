#include <cstdio>
struct SubObj { int x; };
static int g_sub=0, g_real=0, g_del=0;
extern "C" void __fastcall SubObj_dtor(SubObj* p){ (void)p; g_sub++; }
struct CEngineSubPrimitiveText {
  char pad[0x3c];
  SubObj member;
  void realdtor(){ g_real++; }
  void* scalar_deleting_destructor(unsigned int flags);
};
extern "C" void operator_delete(void* p){ (void)p; g_del++; }

void* CEngineSubPrimitiveText::scalar_deleting_destructor(unsigned int flags) {
  SubObj_dtor((SubObj*)((char*)this + 0x3c));
  this->realdtor();
  if (flags & 1) {
    operator_delete((void*)this);
  }
  return this;
}

int main(){
  CEngineSubPrimitiveText obj;
  g_sub=g_real=g_del=0;
  void* r = obj.scalar_deleting_destructor(0);
  if (r != &obj || g_sub!=1 || g_real!=1 || g_del!=0){ std::printf("FAIL nodelete\n"); return 1; }
  g_sub=g_real=g_del=0;
  r = obj.scalar_deleting_destructor(1);
  if (r != &obj || g_sub!=1 || g_real!=1 || g_del!=1){ std::printf("FAIL delete\n"); return 1; }
  std::printf("CEngineSubPrimitiveText_00455c64_TEST PASS\n");
  return 0;
}