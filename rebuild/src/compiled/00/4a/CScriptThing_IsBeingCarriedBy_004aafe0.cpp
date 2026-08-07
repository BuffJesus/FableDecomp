#include "rebuild_abi.h"
// CScriptThing::IsBeingCarriedBy @ 0x004aafe0
// mov ecx,[ecx+4]; test; jne L; xor al,al; ret 4; L: mov eax,[ecx]; jmp [eax+0xfc]
// Proxy: forward to the wrapped implementation object at this->f4; false when absent.
struct CScriptThingInner {
  virtual bool m0(int a0);
  virtual bool m1(int a0);
  virtual bool m2(int a0);
  virtual bool m3(int a0);
  virtual bool m4(int a0);
  virtual bool m5(int a0);
  virtual bool m6(int a0);
  virtual bool m7(int a0);
  virtual bool m8(int a0);
  virtual bool m9(int a0);
  virtual bool m10(int a0);
  virtual bool m11(int a0);
  virtual bool m12(int a0);
  virtual bool m13(int a0);
  virtual bool m14(int a0);
  virtual bool m15(int a0);
  virtual bool m16(int a0);
  virtual bool m17(int a0);
  virtual bool m18(int a0);
  virtual bool m19(int a0);
  virtual bool m20(int a0);
  virtual bool m21(int a0);
  virtual bool m22(int a0);
  virtual bool m23(int a0);
  virtual bool m24(int a0);
  virtual bool m25(int a0);
  virtual bool m26(int a0);
  virtual bool m27(int a0);
  virtual bool m28(int a0);
  virtual bool m29(int a0);
  virtual bool m30(int a0);
  virtual bool m31(int a0);
  virtual bool m32(int a0);
  virtual bool m33(int a0);
  virtual bool m34(int a0);
  virtual bool m35(int a0);
  virtual bool m36(int a0);
  virtual bool m37(int a0);
  virtual bool m38(int a0);
  virtual bool m39(int a0);
  virtual bool m40(int a0);
  virtual bool m41(int a0);
  virtual bool m42(int a0);
  virtual bool m43(int a0);
  virtual bool m44(int a0);
  virtual bool m45(int a0);
  virtual bool m46(int a0);
  virtual bool m47(int a0);
  virtual bool m48(int a0);
  virtual bool m49(int a0);
  virtual bool m50(int a0);
  virtual bool m51(int a0);
  virtual bool m52(int a0);
  virtual bool m53(int a0);
  virtual bool m54(int a0);
  virtual bool m55(int a0);
  virtual bool m56(int a0);
  virtual bool m57(int a0);
  virtual bool m58(int a0);
  virtual bool m59(int a0);
  virtual bool m60(int a0);
  virtual bool m61(int a0);
  virtual bool m62(int a0);
  virtual bool m63(int a0);
};
struct CScriptThing { void *pad0; CScriptThingInner *m_impl; bool IsBeingCarriedBy(int a0); };
bool CScriptThing::IsBeingCarriedBy(int a0)
{
    CScriptThingInner *p = m_impl;
    if (!p) return false;
    return p->m63(a0);
}