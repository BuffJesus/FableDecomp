//=== GetInstance @ 0041e5f2 (seed 0041e5f2) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: static class NUISystem::CFrontEndManager * __fastcall
   NUISystem::CFrontEndManager::GetInstance(void) */

CFrontEndManager * __fastcall NUISystem::CFrontEndManager::GetInstance(void)

{
  CManager *this;
  
  if (DAT_013b8710 == (CFrontEndManager *)0x0) {
    this = ::operator_new(0xd0);
    if (this == (CManager *)0x0) {
      DAT_013b8710 = (CFrontEndManager *)0x0;
    }
    else {
      DAT_013b8710 = (CFrontEndManager *)CManager::CManager(this);
    }
  }
  return DAT_013b8710;
}


// CALLEES of GetInstance:
//   00bfea1a  operator_new
//   0041e3f6  CManager

//=== GetInstance @ 00595582 (seed 00595582) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: static class NUISystem::CFrontEndManager * __fastcall
   NUISystem::CFrontEndManager::GetInstance(void) */

CFrontEndManager * __fastcall NUISystem::CFrontEndManager::GetInstance(void)

{
  CFrontEndManager *this;
  
  if (DAT_013b8b5c == (CFrontEndManager *)0x0) {
    this = ::operator_new(0xe0);
    if (this == (CFrontEndManager *)0x0) {
      DAT_013b8b5c = (CFrontEndManager *)0x0;
    }
    else {
      DAT_013b8b5c = (CFrontEndManager *)CFrontEndManager(this);
    }
  }
  return DAT_013b8b5c;
}


// CALLEES of GetInstance:
//   00bfea1a  operator_new
//   005953e2  CFrontEndManager

