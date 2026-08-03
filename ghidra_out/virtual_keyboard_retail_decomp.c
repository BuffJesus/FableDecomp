//=== Cancel @ 00851860 (seed 00851860) ===

/* [bsim sim=0.8313540853237326 <- ego_r]
   public: void __thiscall CVirtualKeyboard::Cancel(void) */

void __thiscall CVirtualKeyboard::Cancel(CVirtualKeyboard *this)

{
  bool bVar1;
  CIME *pCVar2;
  CEngineInternalPrimitiveBase *this_00;
  CEnginePrimitive *unaff_retaddr;
  ulong in_stack_00000004;
  
  if (this[5] == (CVirtualKeyboard)0x0) {
    this[5] = (CVirtualKeyboard)0x1;
    this[4] = (CVirtualKeyboard)0x1;
    pCVar2 = (CIME *)CEngineInternalPrimitiveBase::AddChildPrimitive
                               ((CEngineInternalPrimitiveBase *)this,unaff_retaddr,in_stack_00000004
                               );
    bVar1 = NUISystem::CIME::IsIMEActive(pCVar2);
    if (bVar1) {
      bVar1 = false;
      pCVar2 = (CIME *)CEngineInternalPrimitiveBase::AddChildPrimitive
                                 (this_00,(CEnginePrimitive *)0x0,(ulong)unaff_retaddr);
      NUISystem::CIME::FinaliseString(pCVar2,bVar1);
    }
  }
  return;
}


// CALLEES of Cancel:
//   00404b50  FinaliseString
//   00404c50  IsIMEActive
//   00404a80  AddChildPrimitive

//=== Confirm @ 00851920 (seed 00851920) ===

/* [bsim sim=0.9146921646574626 <- ego_r]
   public: void __thiscall CVirtualKeyboard::Confirm(void) */

void __thiscall CVirtualKeyboard::Confirm(CVirtualKeyboard *this)

{
  bool bVar1;
  bool bVar2;
  CCharString *this_00;
  long lVar3;
  CIME *pCVar4;
  CVirtualKeyboard *extraout_ECX;
  CEngineInternalPrimitiveBase *extraout_ECX_00;
  CEngineInternalPrimitiveBase *this_01;
  CEngineInternalPrimitiveBase *this_02;
  ulong unaff_EBX;
  CEnginePrimitive *unaff_ESI;
  undefined4 local_4;
  
  bVar2 = false;
  local_4 = 0;
  this_01 = (CEngineInternalPrimitiveBase *)this;
  if (this[5] == (CVirtualKeyboard)0x0) {
    bVar2 = true;
    this_00 = (CCharString *)GetProfileName(this);
    lVar3 = CCharString::GetLength(this_00);
    bVar1 = true;
    this_01 = (CEngineInternalPrimitiveBase *)extraout_ECX;
    if (0 < lVar3) goto LAB_00851957;
  }
  bVar1 = false;
LAB_00851957:
  if (bVar2) {
    CCharString::~CCharString((CCharString *)&local_4);
    this_01 = extraout_ECX_00;
  }
  if (bVar1) {
    this[5] = (CVirtualKeyboard)0x1;
    this[4] = (CVirtualKeyboard)0x0;
    pCVar4 = (CIME *)CEngineInternalPrimitiveBase::AddChildPrimitive(this_01,unaff_ESI,unaff_EBX);
    bVar2 = NUISystem::CIME::IsIMEActive(pCVar4);
    if (bVar2) {
      bVar2 = true;
      pCVar4 = (CIME *)CEngineInternalPrimitiveBase::AddChildPrimitive
                                 (this_02,(CEnginePrimitive *)0x1,(ulong)unaff_ESI);
      NUISystem::CIME::FinaliseString(pCVar4,bVar2);
    }
  }
  return;
}


// CALLEES of Confirm:
//   0099b510  ~CCharString
//   00404b50  FinaliseString
//   00851890  GetProfileName
//   00404c50  IsIMEActive
//   00404a80  AddChildPrimitive
//   0099b220  GetLength

//=== GetProfileName @ 00851890 (seed 00851890) ===

/* [bsim sim=0.9522890386313249 <- ego_r]
   public: class CWideString __thiscall CVirtualKeyboard::GetProfileName(void)const  */

undefined4 __thiscall CVirtualKeyboard::GetProfileName(CVirtualKeyboard *this)

{
  int iVar1;
  int iVar2;
  long lVar3;
  char *pcVar4;
  int iVar5;
  undefined4 unaff_retaddr;
  CVirtualKeyboard *local_4;
  
  local_4 = this;
  (**(code **)(**(int **)(this + 0xc) + 0x238))(&local_4);
  iVar5 = 0;
  iVar2 = CCharString::GetLength((CCharString *)&stack0xfffffff8);
  lVar3 = CCharString::GetLength((CCharString *)&stack0xfffffff8);
  if (0 < lVar3) {
    iVar5 = 0;
    do {
      pcVar4 = CCharString::operator_char_const_((CCharString *)&stack0xfffffff8);
      if (*(short *)(pcVar4 + iVar5 * 2) != 0x20) break;
      iVar5 = iVar5 + 1;
      lVar3 = CCharString::GetLength((CCharString *)&stack0xfffffff8);
    } while (iVar5 < lVar3);
  }
  do {
    iVar1 = iVar2;
    iVar2 = iVar1 + -1;
    if (iVar2 <= iVar5) break;
    pcVar4 = CCharString::operator_char_const_((CCharString *)&stack0xfffffff8);
  } while (*(short *)(pcVar4 + iVar2 * 2) == 0x20);
  CCharString__SetSubstring(unaff_retaddr,iVar5,iVar1);
  CCharString::~CCharString((CCharString *)&stack0xfffffff8);
  return unaff_retaddr;
}


// CALLEES of GetProfileName:
//   0099af10  operator_char_const*
//   0099bdb0  CCharString::SetSubstring
//   0099b510  ~CCharString
//   0099b220  GetLength

