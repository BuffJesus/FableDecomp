#include "rebuild_abi.h"
struct M { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(){} virtual void Disp(void* a){} };
extern M* g_mgr_9e9f90;
struct C { void Set(); };
void C::Set(){ if(g_mgr_9e9f90) g_mgr_9e9f90->Disp(this); }