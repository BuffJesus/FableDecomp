#include "rebuild_abi.h"
#include <cstdio>
struct CScriptThingInner {
  virtual bool m0(int a0){ return true; }
  virtual bool m1(int a0){ return true; }
  virtual bool m2(int a0){ return true; }
  virtual bool m3(int a0){ return true; }
  virtual bool m4(int a0){ return true; }
  virtual bool m5(int a0){ return true; }
  virtual bool m6(int a0){ return true; }
  virtual bool m7(int a0){ return true; }
  virtual bool m8(int a0){ return true; }
  virtual bool m9(int a0){ return true; }
  virtual bool m10(int a0){ return true; }
  virtual bool m11(int a0){ return true; }
  virtual bool m12(int a0){ return true; }
  virtual bool m13(int a0){ return true; }
  virtual bool m14(int a0){ return true; }
  virtual bool m15(int a0){ return true; }
  virtual bool m16(int a0){ return true; }
  virtual bool m17(int a0){ return true; }
  virtual bool m18(int a0){ return true; }
  virtual bool m19(int a0){ return true; }
  virtual bool m20(int a0){ return true; }
  virtual bool m21(int a0){ return true; }
  virtual bool m22(int a0){ return true; }
  virtual bool m23(int a0){ return true; }
  virtual bool m24(int a0){ return true; }
  virtual bool m25(int a0){ return true; }
  virtual bool m26(int a0){ return true; }
  virtual bool m27(int a0){ return true; }
  virtual bool m28(int a0){ return true; }
  virtual bool m29(int a0){ return true; }
  virtual bool m30(int a0){ return true; }
  virtual bool m31(int a0){ return true; }
  virtual bool m32(int a0){ return true; }
  virtual bool m33(int a0){ return true; }
  virtual bool m34(int a0){ return true; }
  virtual bool m35(int a0){ return true; }
  virtual bool m36(int a0){ return true; }
  virtual bool m37(int a0){ return true; }
};
struct CScriptThing { void *pad0; CScriptThingInner *m_impl; bool MsgPerformedSpecialAbility(int a0); };
bool CScriptThing::MsgPerformedSpecialAbility(int a0){ CScriptThingInner *p=m_impl; if(!p) return false; return p->m37(a0); }
int main(){
    CScriptThing t; t.m_impl=0;
    if(t.MsgPerformedSpecialAbility(0)){std::printf("BADNULL\n");return 1;}
    CScriptThingInner impl; CScriptThingInner *pi = &impl; t.m_impl=pi;
    if(!t.MsgPerformedSpecialAbility(1)){std::printf("BADFWD\n");return 1;}
    std::printf("MSG_4aaca0_OK\n"); return 0;
}