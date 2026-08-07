//=== IsIMEActive @ 00404c50 (seed 00404c50) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: bool __thiscall NUISystem::CIME::IsIMEActive(void)const  */

bool __thiscall NUISystem::CIME::IsIMEActive(CIME *this)

{
  bool bVar1;
  
  if (this[8] != (CIME)0x0) {
    bVar1 = CAIStateGroup_Freeze::IsRunnable((CAIStateGroup_Freeze *)this);
    if (bVar1) {
      return true;
    }
  }
  return false;
}


// CALLEES of IsIMEActive:
//   00cb28e0  IsRunnable

