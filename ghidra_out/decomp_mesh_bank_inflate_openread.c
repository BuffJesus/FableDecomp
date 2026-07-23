INFO  DecompFuncs.java> ==================== ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z @ 009d06f0 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.9387805522643481 <- ego_r]
   public: virtual bool __thiscall CBankFile::OpenReadOnly(class CCharString const &,unsigned long)
    */

bool __thiscall
CBankFile::_OpenReadOnly_CBankFile__UAE_NABVCCharString__K_Z
          (CBankFile *this,CCharString *param_1,ulong param_2)

{
  CCharString *pCVar1;
  bool bVar2;
  undefined1 uVar3;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar4;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_00;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_01;
  undefined8 uVar5;
  int local_48 [2];
  undefined4 uStack_40;
  int *piStack_3c;
  int local_38;
  ulong local_34;
  ulong local_30;
  undefined4 local_2c;
  undefined4 local_28;
  CFileDataInputStream aCStack_24 [32];
  CCharString aCStack_4 [4];
  
  pCVar1 = param_1;
  this[0x8d] = (CBankFile)0x1;
  *(ulong *)(this + 0x78) = param_2;
  CCharString::operator=((CCharString *)(this + 0x88),param_1);
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  this[0x8f] = (CBankFile)0x0;
  this[0x95] = (CBankFile)0x0;
  if ((DAT_013ca7b0 != '\0') && ((param_2 & 4) == 0)) {
    this[0x8c] = (CBankFile)0x1;
    local_38 = 0;
    local_34 = 0;
    local_30 = 0;
    local_2c = 0;
    local_28 = 0;
    local_48[0] = 0;
    local_48[1] = 0;
    bVar2 = CBankFileManager::FindContainedBank
                      ((CBankFileManager *)&DAT_013ca79c,pCVar1,(CContainedBank *)&local_38,
                       (CCountedPointer<NBankFileManager::CRetailBank> *)local_48);
    pCVar4 = extraout_EDX;
    if (bVar2) {
      uVar5 = (**(code **)(*(int *)this + 0x20))();
      pCVar4 = (CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *)
               ((ulonglong)uVar5 >> 0x20);
      if (local_38 == (int)uVar5) {
        *(undefined4 *)(this + 0x98) = local_28;
        uStack_40 = *(undefined4 *)(local_48[0] + 4);
        piStack_3c = *(int **)(local_48[0] + 8);
        if (piStack_3c != (int *)0x0) {
          *piStack_3c = *piStack_3c + 1;
        }
        CCountedPointer<CDiskFileWin32>::operator=
                  ((CCountedPointer<CDiskFileWin32> *)(this + 0x7c),
                   (CCountedPointer<class_CDiskFileWin32> *)&uStack_40);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   &uStack_40,extraout_EDX_00);
        CFileDataInputStream::CFileDataInputStream(aCStack_24,*(CAFile **)(this + 0x7c),0x4000);
        CDataInputStream::_SetPosition_CDataInputStream__UAEXK_Z
                  ((CDataInputStream *)aCStack_24,local_30);
        ReadEntryList(this,aCStack_24,local_34 + 1,local_34);
        (**(code **)(*(int *)this + 0x2c))();
        CFileDataInputStream::~CFileDataInputStream(aCStack_24);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_48,extraout_EDX_01);
        return true;
      }
    }
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_48,pCVar4);
    return false;
  }
  FUN_009a7ca0(&param_1,pCVar1);
  uVar3 = (**(code **)(*(int *)this + 0xc))(&param_1,param_2);
  CCharString::~CCharString(aCStack_4);
  return (bool)uVar3;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== ?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z @ 009d1090 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.7481268866523866 <- ego_r]
   public: virtual bool __thiscall CBankFile::OpenFileReadOnly(class CWideString const &,unsigned
   long) */

bool __thiscall
CBankFile::_OpenFileReadOnly_CBankFile__UAE_NABVCWideString__K_Z
          (CBankFile *this,CWideString *param_1,ulong param_2)

{
  bool bVar1;
  CBase *this_00;
  uint unaff_EBX;
  CIPhysicsFacesSegmentBase *pCVar2;
  
  this[0x8d] = (CBankFile)0x1;
  *(ulong *)(this + 0x78) = param_2;
  *(undefined4 *)(this + 0x90) = 0;
  this[0x94] = (CBankFile)0x0;
  this[0x8e] = (CBankFile)0x0;
  this[0x8f] = (CBankFile)0x0;
  this[0x95] = (CBankFile)0x0;
  this[0x8c] = (CBankFile)0x0;
  bVar1 = CAFile::PathExists(param_1);
  if (bVar1) {
    this_00 = ::operator_new(0x1c);
    pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    if (this_00 != (CBase *)0x0) {
      NHeroInformationScreens::CBase::CBase(this_00);
      *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0122d06c;
      CCharString::CCharString((CCharString *)(this_00 + 0x18));
      this_00[0x14] = (CBase)0x0;
      pCVar2 = (CIPhysicsFacesSegmentBase *)this_00;
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0x7c),pCVar2);
    (**(code **)(**(int **)(this + 0x7c) + 4))(param_1,1,4);
    CWideString::operator=((CWideString *)(this + 0x84),param_1);
    bVar1 = ReadFileInformation(this,(bool)(~(byte)(unaff_EBX >> 1) & 1));
    if (bVar1) {
      (**(code **)(*(int *)this + 0x2c))();
      return true;
    }
    FUN_009cd850();
  }
  return false;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /Fable.exe (HeadlessAnalyzer)  
