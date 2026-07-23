//=== C3DMeshStats @ 00499ce0 ===

/* [bsim sim=0.7371397840669073 <- ego_r]
   public: __thiscall C3DMeshStats::C3DMeshStats(void) */

C3DMeshStats * __thiscall C3DMeshStats::C3DMeshStats(C3DMeshStats *this)

{
  *(undefined ***)this = &PTR__vector_deleting_destructor__012388b4;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  CCharString::CCharString((CCharString *)(this + 0x48));
  CCharString::CCharString((CCharString *)(this + 0x4c));
  CCharString::CCharString((CCharString *)(this + 0x50));
  *(undefined8 *)(this + 0x60) = 0;
  this[0x7a] = (C3DMeshStats)0x1;
  *(undefined8 *)(this + 0x68) = 0;
  this[0x7c] = (C3DMeshStats)0x1;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined8 *)(this + 0x70) = 0;
  this[0x78] = (C3DMeshStats)0x0;
  this[0x79] = (C3DMeshStats)0x0;
  this[0x7b] = (C3DMeshStats)0x0;
  this[0x7d] = (C3DMeshStats)0x0;
  this[0x7e] = (C3DMeshStats)0x0;
  this[0x7f] = (C3DMeshStats)0x0;
  this[0x80] = (C3DMeshStats)0x0;
  this[0x81] = (C3DMeshStats)0x0;
  return this;
}



//=== SetVertexFormat @ 00a0aa30 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CRenderManagerCore::SetVertexFormat(unsigned long) */

void __thiscall CRenderManagerCore::SetVertexFormat(CRenderManagerCore *this,ulong param_1)

{
  (**(code **)(**(int **)(this + 0x3cf0) + 0x164))(*(int **)(this + 0x3cf0),param_1);
  return;
}



//=== ?CanAbortResourceLoad@C3DMeshLODInfo@@UBE_NJ@Z @ 00a23c20 ===

bool _CanAbortResourceLoad_C3DMeshLODInfo__UBE_NJ_Z(int param_1)

{
  return 1 < param_1;
}



//=== ?IsResourceLoaded@C3DMeshLODInfo@@UBE_NXZ @ 00a23de0 ===

bool _IsResourceLoaded_C3DMeshLODInfo__UBE_NXZ(void)

{
  bool bVar1;
  int in_ECX;
  
  if (*(CBankFileAsyncData **)(in_ECX + 0x24) != (CBankFileAsyncData *)0x0) {
    bVar1 = CBankFileAsyncData::IsFinished(*(CBankFileAsyncData **)(in_ECX + 0x24));
    return bVar1;
  }
  return true;
}



//=== ?Reset@C3DMeshInfo@@EAEXXZ @ 00a24cd0 ===

/* [bsim sim=1.0 <- ego_r]
   private: virtual void __thiscall C3DMeshInfo::Reset(void) */

void __thiscall C3DMeshInfo::_Reset_C3DMeshInfo__EAEXXZ(C3DMeshInfo *this)

{
  int *piVar1;
  int *piVar2;
  
  piVar2 = *(int **)(this + 0xc);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0xc) = 0;
  }
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x34) = 0xbf800000;
  *(undefined4 *)(this + 0x3c) = 0xffffffff;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  return;
}



//=== ?OnResourceLoaded@C3DMeshLODInfo@@UAEXXZ @ 00a25470 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshLODInfo::OnResourceLoaded(void) */

void __thiscall C3DMeshLODInfo::_OnResourceLoaded_C3DMeshLODInfo__UAEXXZ(C3DMeshLODInfo *this)

{
  bool bVar1;
  
  if (*(int *)(this + 0x24) != 0) {
    bVar1 = BuildMesh(this,true,false);
    if (!bVar1) {
      C3DMeshInfo::RemoveLOD(*(C3DMeshInfo **)(this + 0x2c),(uint)(byte)this[0x30],false);
    }
  }
  return;
}



//=== ?OnResourceLoadAborted@C3DMeshLODInfo@@UAEXXZ @ 00a254a0 ===

/* [bsim sim=0.7898338609741783 <- ego_r]
   public: virtual void __thiscall C3DMeshLODInfo::OnResourceLoadAborted(void) */

void __thiscall C3DMeshLODInfo::_OnResourceLoadAborted_C3DMeshLODInfo__UAEXXZ(C3DMeshLODInfo *this)

{
  int *piVar1;
  
  CBankFileAsyncData::DiscardAsyncData((CCountedPointer<CBankFileAsyncData> *)(this + 0x24));
  piVar1 = *(int **)(this + 0x28);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x28) == 0) {
      (*(code *)(*(int **)(this + 0x28))[1])();
      operator_delete(*(void **)(this + 0x28));
    }
  }
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  C3DMeshInfo::RemoveLOD(*(C3DMeshInfo **)(this + 0x2c),(uint)(byte)this[0x30],false);
  return;
}



//=== ?UnloadResource@C3DMeshLODInfo@@UAE_NXZ @ 00a254f0 ===

undefined4 _UnloadResource_C3DMeshLODInfo__UAE_NXZ(void)

{
  int iVar1;
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x1c) != 0) {
    if (*(int **)(in_ECX + 0x20) == (int *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = **(int **)(in_ECX + 0x20);
    }
    if (iVar1 == 1) {
      C3DMeshInfo::RemoveLOD(*(C3DMeshInfo **)(in_ECX + 0x2c),(uint)*(byte *)(in_ECX + 0x30),true);
      return 1;
    }
  }
  return 0;
}



//=== ?Sizeof@C3DMeshInfo@@UBEKXZ @ 00a26170 ===

/* [bsim sim=0.6369969667814156 <- ego_r]
   public: virtual unsigned long __thiscall C3DMeshInfo::Sizeof(void)const  */

ulong __thiscall C3DMeshInfo::_Sizeof_C3DMeshInfo__UBEKXZ(C3DMeshInfo *this)

{
  int iVar1;
  ulong uVar2;
  uint uVar3;
  int iVar4;
  C3DMeshInfo *local_8;
  int local_4;
  
  uVar2 = 0x60;
  local_4 = 0x60;
  if (*(int *)(this + 0xc) != 0) {
    uVar3 = 8;
    for (local_8 = this + 0x50; (uVar3 < 0x18 && (*(int *)local_8 != 0)); local_8 = local_8 + 4) {
      iVar1 = *(int *)(*(int *)(this + 0xc) + uVar3);
      if (iVar1 == 0) {
        iVar4 = 4;
      }
      else {
        if (*(C3DMesh2 **)(iVar1 + 0x1c) == (C3DMesh2 *)0x0) {
          iVar4 = 8;
        }
        else {
          uVar2 = C3DMesh2::Sizeof(*(C3DMesh2 **)(iVar1 + 0x1c));
          iVar4 = uVar2 + 8;
        }
        if (*(CVertexBufferWin32 **)(iVar1 + 0x24) == (CVertexBufferWin32 *)0x0) {
          iVar4 = iVar4 + 0x40;
        }
        else {
          uVar2 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(iVar1 + 0x24));
          iVar4 = uVar2 + 0x40 + iVar4;
        }
      }
      local_4 = local_4 + iVar4;
      uVar3 = uVar3 + 4;
    }
    iVar1 = *(int *)(*(int *)(this + 0xc) + 0x18);
    if (iVar1 != 0) {
      if (*(C3DMesh2 **)(iVar1 + 0x1c) == (C3DMesh2 *)0x0) {
        iVar4 = 8;
      }
      else {
        uVar2 = C3DMesh2::Sizeof(*(C3DMesh2 **)(iVar1 + 0x1c));
        iVar4 = uVar2 + 8;
      }
      if (*(CVertexBufferWin32 **)(iVar1 + 0x24) != (CVertexBufferWin32 *)0x0) {
        uVar2 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(iVar1 + 0x24));
        return local_4 + uVar2 + 0x40 + iVar4;
      }
      return local_4 + iVar4 + 0x40;
    }
    uVar2 = local_4 + 4;
  }
  return uVar2;
}



//=== ?RelocateData@C3DMeshStats@@UAEXJPAX0J@Z @ 00a2e820 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshStats::RelocateData(long,void *,void *,long) */

void __thiscall
C3DMeshStats::_RelocateData_C3DMeshStats__UAEXJPAX0J_Z
          (C3DMeshStats *this,long param_1,void *param_2,void *param_3,long param_4)

{
  if (param_1 == *(int *)(this + 8)) {
    if (*(int *)(this + 0x74) != 0) {
      *(int *)(this + 0x74) = (*(int *)(this + 0x74) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0x3c) != 0) {
      *(int *)(this + 0x3c) = (*(int *)(this + 0x3c) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0x4c) != 0) {
      *(int *)(this + 0x4c) = (*(int *)(this + 0x4c) - (int)param_2) + (int)param_3;
    }
  }
  return;
}



//=== C3DMeshStats @ 00a2f210 ===

/* [bsim sim=0.9316478635228377 <- ego_r]
   public: __thiscall C3DMeshStats::C3DMeshStats(void) */

C3DMeshStats * __thiscall C3DMeshStats::C3DMeshStats(C3DMeshStats *this)

{
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129cf04;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0xffffffff;
  CCharString::CCharString((CCharString *)(this + 0xc));
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  this[0x44] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  this[0x54] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  this[0x65] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined2 *)(this + 0x78) = 0;
  this[0x7a] = (C3DMeshStats)0x0;
  return this;
}



//=== C3DMeshStats @ 00a2f270 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall C3DMeshStats::C3DMeshStats(bool) */

C3DMeshStats * __thiscall C3DMeshStats::C3DMeshStats(C3DMeshStats *this,bool param_1)

{
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129cf04;
  CCharString::CCharString((CCharString *)(this + 0xc));
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  this[0x44] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  this[0x54] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  this[0x65] = (C3DMeshStats)0x0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  this[0x7a] = (C3DMeshStats)param_1;
  return this;
}



//=== ?RelocateData@C3DMesh2@@UAEXJPAX0J@Z @ 00a8b4c0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall C3DMesh2::RelocateData(long,void *,void *,long) */

void __thiscall
C3DMesh2::_RelocateData_C3DMesh2__UAEXJPAX0J_Z
          (C3DMesh2 *this,long param_1,void *param_2,void *param_3,long param_4)

{
  int iVar1;
  uint uVar2;
  
  if (param_1 == *(int *)(this + 0x7c)) {
    if (*(int *)(this + 0x8c) != 0) {
      *(int *)(this + 0x8c) = (*(int *)(this + 0x8c) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0x94) != 0) {
      *(int *)(this + 0x94) = (*(int *)(this + 0x94) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0x9c) != 0) {
      *(int *)(this + 0x9c) = (*(int *)(this + 0x9c) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0xa0) != 0) {
      *(int *)(this + 0xa0) = (*(int *)(this + 0xa0) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0xe0) != 0) {
      *(int *)(this + 0xe0) = (*(int *)(this + 0xe0) - (int)param_2) + (int)param_3;
      return;
    }
  }
  else if (param_1 == *(int *)(this + 0x80)) {
    uVar2 = 0;
    if (*(int *)(this + 0x90) != 0) {
      iVar1 = 0;
      do {
        (**(code **)(*(int *)(*(int *)(this + 0x94) + iVar1) + 4))(param_1,param_2,param_3,param_4);
        uVar2 = uVar2 + 1;
        iVar1 = iVar1 + 0xa0;
      } while (uVar2 < *(uint *)(this + 0x90));
    }
  }
  else {
    if (param_1 != *(int *)(this + 0x84)) {
      C3DMeshStats::_RelocateData_C3DMeshStats__UAEXJPAX0J_Z
                ((C3DMeshStats *)this,param_1,param_2,param_3,param_4);
      return;
    }
    if (*(int *)(this + 0xa4) != 0) {
      *(int *)(this + 0xa4) = (*(int *)(this + 0xa4) - (int)param_2) + (int)param_3;
    }
    if (*(int *)(this + 0xa8) != 0) {
      *(int *)(this + 0xa8) = (*(int *)(this + 0xa8) - (int)param_2) + (int)param_3;
      return;
    }
  }
  return;
}



//=== ?CanRelocateData@C3DMesh2@@UBE_NJ@Z @ 00a8ca00 ===

bool _CanRelocateData_C3DMesh2__UBE_NJ_Z(void)

{
  int in_ECX;
  
  return *(char *)(in_ECX + 0xe5) == '\0';
}



//=== ?RelocateData@C3DPrimitive2@@UAEXJPAX0J@Z @ 00a8e870 ===

void _RelocateData_C3DPrimitive2__UAEXJPAX0J_Z(undefined4 param_1,int param_2,int param_3)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x44) != 0) {
    *(int *)(in_ECX + 0x44) = (*(int *)(in_ECX + 0x44) - param_2) + param_3;
  }
  if (*(int *)(in_ECX + 0x4c) != 0) {
    *(int *)(in_ECX + 0x4c) = (*(int *)(in_ECX + 0x4c) - param_2) + param_3;
  }
  return;
}



//=== ?RelocateData@CIndexBuffer@@UAEXJPAX0J@Z @ 00ac1f50 ===

void _RelocateData_CIndexBuffer__UAEXJPAX0J_Z(void)

{
  return;
}



