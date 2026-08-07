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
  virtual bool m38(int a0){ return true; }
  virtual bool m39(int a0){ return true; }
  virtual bool m40(int a0){ return true; }
  virtual bool m41(int a0){ return true; }
  virtual bool m42(int a0){ return true; }
  virtual bool m43(int a0){ return true; }
  virtual bool m44(int a0){ return true; }
  virtual bool m45(int a0){ return true; }
  virtual bool m46(int a0){ return true; }
  virtual bool m47(int a0){ return true; }
  virtual bool m48(int a0){ return true; }
  virtual bool m49(int a0){ return true; }
  virtual bool m50(int a0){ return true; }
  virtual bool m51(int a0){ return true; }
  virtual bool m52(int a0){ return true; }
  virtual bool m53(int a0){ return true; }
  virtual bool m54(int a0){ return true; }
  virtual bool m55(int a0){ return true; }
  virtual bool m56(int a0){ return true; }
  virtual bool m57(int a0){ return true; }
  virtual bool m58(int a0){ return true; }
  virtual bool m59(int a0){ return true; }
};
struct CScriptThing { void *pad0; CScriptThingInner *m_impl; bool MsgOnCutSceneAnimEvent(int a0); };
bool CScriptThing::MsgOnCutSceneAnimEvent(int a0){ CScriptThingInner *p=m_impl; if(!p) return false; return p->m59(a0); }
int main(){
    CScriptThing t; t.m_impl=0;
    if(t.MsgOnCutSceneAnimEvent(0)){std::printf("BADNULL\n");return 1;}
    CScriptThingInner impl; CScriptThingInner *pi = &impl; t.m_impl=pi;
    if(!t.MsgOnCutSceneAnimEvent(1)){std::printf("BADFWD\n");return 1;}
    std::printf("MSG_4aaf60_OK\n"); return 0;
}