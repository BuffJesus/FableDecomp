//=== C3DMeshFileXAnimatedPrimitiveChunk @ 008bbb10 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall C3DMeshFileXAnimatedPrimitiveChunk::C3DMeshFileXAnimatedPrimitiveChunk(void)
    */

C3DMeshFileXAnimatedPrimitiveChunk * __thiscall
C3DMeshFileXAnimatedPrimitiveChunk::C3DMeshFileXAnimatedPrimitiveChunk
          (C3DMeshFileXAnimatedPrimitiveChunk *this)

{
  CCombatActionBase::CCombatActionBase((CCombatActionBase *)this,0x96);
  *(undefined4 *)(this + 0x114) = 0;
  *(undefined4 *)(this + 0x118) = 0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__012645ec;
  return this;
}



//=== ~C3DMeshLODInfo @ 00a0b9a0 ===

/* [bsim sim=0.635990792892609 <- ego_r]
   public: virtual __thiscall C3DMeshLODInfo::~C3DMeshLODInfo(void) */

void __thiscall C3DMeshLODInfo::~C3DMeshLODInfo(C3DMeshLODInfo *this)

{
  int *piVar1;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129c9dc;
  if (*(undefined4 **)(this + 0x10) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(this + 0x10))();
    *(undefined4 *)(this + 0x10) = 0;
  }
  piVar1 = *(int **)(this + 0x18);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x18) == 0) {
      (*(code *)(*(int **)(this + 0x18))[1])();
      operator_delete(*(void **)(this + 0x18));
    }
  }
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  piVar1 = *(int **)(this + 8);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 8) == 0) {
      (*(code *)(*(int **)(this + 8))[1])();
      operator_delete(*(void **)(this + 8));
    }
  }
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== ?FreeDeviceSpecificResources@CMeshDataBankEntry@@UAEXXZ @ 00a23900 ===

void _FreeDeviceSpecificResources_CMeshDataBankEntry__UAEXXZ(void)

{
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



//=== C3DMeshLODInfo @ 00a24430 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   private: __thiscall C3DMeshLODInfo::C3DMeshLODInfo(class C3DMeshInfo const *,unsigned long) */

C3DMeshLODInfo * __thiscall
C3DMeshLODInfo::C3DMeshLODInfo(C3DMeshLODInfo *this,C3DMeshInfo *param_1,ulong param_2)

{
  CResource::CResource((CResource *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129cda4;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x31] = (C3DMeshLODInfo)0x0;
  *(C3DMeshInfo **)(this + 0x2c) = param_1;
  this[0x30] = param_2._0_1_;
  return this;
}



//=== ~C3DMeshLODInfo @ 00a24470 ===

/* [bsim sim=0.6934083957109157 <- ego_r]
   public: virtual __thiscall C3DMeshLODInfo::~C3DMeshLODInfo(void) */

void __thiscall C3DMeshLODInfo::~C3DMeshLODInfo(C3DMeshLODInfo *this)

{
  int *piVar1;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129cda4;
  piVar1 = *(int **)(this + 0x28);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x28) == 0) {
      (*(code *)(*(int **)(this + 0x28))[1])();
      operator_delete(*(void **)(this + 0x28));
    }
  }
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  piVar1 = *(int **)(this + 0x20);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x20) == 0) {
      (*(code *)(*(int **)(this + 0x20))[1])();
      operator_delete(*(void **)(this + 0x20));
    }
  }
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)this);
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



//=== ?DoUnlock@CVertexBufferWin32@@EAEXXZ @ 00a63120 ===

void _DoUnlock_CVertexBufferWin32__EAEXXZ(void)

{
  int in_ECX;
  
  (**(code **)(**(int **)(in_ECX + 4) + 0x30))(*(int **)(in_ECX + 4));
  return;
}



//=== ?DoIsBusy@CVertexBufferWin32@@EBE_NXZ @ 00a63130 ===

undefined1 _DoIsBusy_CVertexBufferWin32__EBE_NXZ(void)

{
  return 0;
}



//=== ?DoSizeof@CVertexBufferWin32@@EBEKXZ @ 00a63140 ===

int _DoSizeof_CVertexBufferWin32__EBEKXZ(void)

{
  int in_ECX;
  
  return *(int *)(in_ECX + 8) + 0x38;
}



//=== ?DoLock@CVertexBufferWin32@@EAEPAEKKK@Z @ 00a63240 ===

CEnginePrimitive *
_DoLock_CVertexBufferWin32__EAEPAEKKK_Z(undefined4 param_1,undefined4 param_2,int param_3)

{
  CEngineInternalPrimitiveBase *in_ECX;
  ulong unaff_ESI;
  CEnginePrimitive *unaff_EDI;
  int iVar1;
  
  iVar1 = param_3;
  if (param_3 == 0) {
    iVar1 = *(int *)(in_ECX + 8);
  }
  CEngineInternalPrimitiveBase::AddChildPrimitive(in_ECX,unaff_EDI,unaff_ESI);
  _E2(*(undefined4 *)(in_ECX + 4),0x3f800000);
  param_3 = 0;
  (**(code **)(**(int **)(in_ECX + 4) + 0x2c))(*(int **)(in_ECX + 4),param_2,iVar1,&param_3,param_1)
  ;
  return unaff_EDI;
}



//=== ?WriteToFile@C3DMeshFileXBoundingBoxChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa4b90 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXBoundingBoxChunk::WriteToFile(class CDataOutputStream
   &,class CChunkedFileSaver &)const  */

void __thiscall
C3DMeshFileXBoundingBoxChunk::
_WriteToFile_C3DMeshFileXBoundingBoxChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
          (C3DMeshFileXBoundingBoxChunk *this,CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x10));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x14));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x18));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x1c));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x20));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x24));
  return;
}



//=== ?WriteToFile@C3DMeshFileXAnimatedPrimitiveChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa4ca0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXAnimatedPrimitiveChunk::WriteToFile(class
   CDataOutputStream &,class CChunkedFileSaver &)const  */

void __thiscall
C3DMeshFileXAnimatedPrimitiveChunk::
_WriteToFile_C3DMeshFileXAnimatedPrimitiveChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
          (C3DMeshFileXAnimatedPrimitiveChunk *this,CDataOutputStream *param_1,
          CChunkedFileSaver *param_2)

{
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x2c));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x30));
  CChunkedFileSuperChunk::
  _WriteToFile_CChunkedFileSuperChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
            ((CChunkedFileSuperChunk *)this,param_1,param_2);
  return;
}



//=== ?CreateNewInstance@C3DMeshFileXAnimatedPrimitiveChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa4cd0 ===

void _CreateNewInstance_C3DMeshFileXAnimatedPrimitiveChunk__UBEPAVCChunkedFileChunk__XZ(void)

{
  CShopItemDef *this;
  int *in_ECX;
  
  this = operator_new(0x34);
  if (this != (CShopItemDef *)0x0) {
    CShopItemDef::CShopItemDef(this);
    *(undefined ***)this = &PTR__vector_deleting_destructor__0129e278;
    *(undefined4 *)(this + 0x2c) = 0;
    *(undefined4 *)(this + 0x30) = 0;
    (**(code **)(*in_ECX + 0x28))(this);
    return;
  }
  (**(code **)(*in_ECX + 0x28))(0);
  return;
}



//=== ?ReadFromFile@C3DMeshFileXBoundingBoxChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa5030 ===

void _ReadFromFile_C3DMeshFileXBoundingBoxChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
               (CDataInputStream *param_1)

{
  CDataInputStream *pCVar1;
  undefined4 uVar2;
  CDataInputStream *this;
  int in_ECX;
  
  this = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(param_1,(uchar *)&param_1,4);
    }
    else {
      pCVar1 = (CDataInputStream *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
      param_1 = pCVar1;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x10) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x14) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x18) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x1c) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x20) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (3 < *(int *)(this + 0x14)) {
      uVar2 = **(undefined4 **)(this + 0xc);
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
      *(undefined4 *)(in_ECX + 0x24) = uVar2;
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
  }
  *(CDataInputStream **)(in_ECX + 0x24) = param_1;
  return;
}



//=== ?CreateNewInstance@C3DMeshFileXBoundingBoxChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa5250 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual class CChunkedFileChunk * __thiscall
   C3DMeshFileXBoundingBoxChunk::CreateNewInstance(void)const  */

CChunkedFileChunk * __thiscall
C3DMeshFileXBoundingBoxChunk::
_CreateNewInstance_C3DMeshFileXBoundingBoxChunk__UBEPAVCChunkedFileChunk__XZ
          (C3DMeshFileXBoundingBoxChunk *this)

{
  CChunkedFileChunk *pCVar1;
  
  pCVar1 = ::operator_new(0x28);
  if (pCVar1 != (CChunkedFileChunk *)0x0) {
    CChunkedFileChunk::CChunkedFileChunk(pCVar1,"XBDB");
    *(undefined ***)pCVar1 = &PTR__vector_deleting_destructor__0129e2fc;
    *(undefined4 *)(pCVar1 + 0x10) = 0;
    *(undefined4 *)(pCVar1 + 0x14) = 0;
    *(undefined4 *)(pCVar1 + 0x18) = 0;
    *(undefined4 *)(pCVar1 + 0x1c) = 0;
    *(undefined4 *)(pCVar1 + 0x20) = 0;
    *(undefined4 *)(pCVar1 + 0x24) = 0;
    pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(pCVar1);
    return pCVar1;
  }
  pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(0);
  return pCVar1;
}



//=== ?ReadFromFile@C3DMeshFileXAnimatedPrimitiveChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa5370 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXAnimatedPrimitiveChunk::ReadFromFile(class
   CDataInputStream &,unsigned long,class CChunkedFileLoader &) */

void __thiscall
C3DMeshFileXAnimatedPrimitiveChunk::
_ReadFromFile_C3DMeshFileXAnimatedPrimitiveChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
          (C3DMeshFileXAnimatedPrimitiveChunk *this,CDataInputStream *param_1,ulong param_2,
          CChunkedFileLoader *param_3)

{
  CDataInputStream *this_00;
  int iVar1;
  int iVar2;
  
  this_00 = param_1;
  iVar1 = (**(code **)(*(int *)param_1 + 8))();
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 0x2c) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 0x30) = param_1;
  iVar2 = (**(code **)(*(int *)this_00 + 8))();
  CChunkedFileSuperChunk::
  _ReadFromFile_CChunkedFileSuperChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
            ((CChunkedFileSuperChunk *)this,this_00,param_2 - (iVar2 - iVar1),param_3);
  return;
}



//=== ?WriteToFile@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa5720 ===

void _WriteToFile_C3DMeshFileXAnimatedPrimitiveBlockChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
               (CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  int iVar1;
  char cVar2;
  CChunkedFileSuperChunk *in_ECX;
  uint uVar3;
  uint uVar4;
  
  uVar3 = *(int *)(in_ECX + 0x30) - *(int *)(in_ECX + 0x2c) >> 2;
  CDataOutputStream::WriteCBYTE(param_1,(char)uVar3);
  uVar4 = 0;
  if (uVar3 != 0) {
    do {
      CDataOutputStream::WriteCBYTE
                (param_1,(char)*(undefined4 *)(*(int *)(in_ECX + 0x2c) + uVar4 * 4));
      uVar4 = uVar4 + 1;
    } while (uVar4 < uVar3);
  }
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(in_ECX + 0x44));
  uVar3 = *(int *)(in_ECX + 0x4c) - *(int *)(in_ECX + 0x48) >> 2;
  CDataOutputStream::WriteCBYTE(param_1,(char)uVar3);
  uVar4 = 0;
  if (uVar3 != 0) {
    do {
      CDataOutputStream::WriteCBYTE
                (param_1,(char)*(undefined4 *)(*(int *)(in_ECX + 0x48) + uVar4 * 4));
      uVar4 = uVar4 + 1;
    } while (uVar4 < uVar3);
  }
  iVar1 = *(int *)(in_ECX + 0x44);
  if (iVar1 * uVar3 != 0) {
    uVar4 = 0;
    do {
      CDataOutputStream::WriteCBYTE
                (param_1,(char)*(undefined4 *)(*(int *)(in_ECX + 0x54) + uVar4 * 8));
      cVar2 = __ftol2();
      CDataOutputStream::WriteCBYTE(param_1,cVar2);
      uVar4 = uVar4 + 1;
    } while (uVar4 < iVar1 * uVar3);
  }
  CChunkedFileSuperChunk::
  _WriteToFile_CChunkedFileSuperChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
            (in_ECX,param_1,param_2);
  return;
}



//=== ?WriteToFile@C3DMeshFileXUVTangentChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa57f0 ===

/* [bsim sim=0.997030969642455 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXUVTangentChunk::WriteToFile(class CDataOutputStream
   &,class CChunkedFileSaver &)const  */

void __thiscall
C3DMeshFileXUVTangentChunk::
_WriteToFile_C3DMeshFileXUVTangentChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
          (C3DMeshFileXUVTangentChunk *this,CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  int iVar1;
  int iVar2;
  C3DMeshFileXUVTangentChunk *pCVar3;
  C3DMeshFileXUVTangentChunk *pCVar4;
  
  CDataOutputStream::WriteCBYTE(param_1,'\x01');
  pCVar4 = this + 0x10;
  CDataOutputStream::WriteCBYTE
            (param_1,(char)((*(int *)(this + 0x14) - *(int *)(this + 0x10)) / 0xc));
  iVar2 = *(int *)(this + 0x14) - (int)*(C3DMeshFileXUVTangentChunk **)pCVar4;
  iVar1 = iVar2 >> 0x1f;
  pCVar3 = *(C3DMeshFileXUVTangentChunk **)pCVar4;
  if (iVar2 / 0xc + iVar1 == iVar1) {
    pCVar3 = pCVar4;
  }
  (**(code **)(*(int *)param_1 + 0x10))
            (pCVar3,((*(int *)(this + 0x14) - *(int *)pCVar4) / 0xc) * 0xc);
  iVar2 = *(int *)(this + 0x20) - (int)*(C3DMeshFileXUVTangentChunk **)(this + 0x1c);
  iVar1 = iVar2 >> 0x1f;
  pCVar4 = *(C3DMeshFileXUVTangentChunk **)(this + 0x1c);
  if (iVar2 / 0xc + iVar1 == iVar1) {
    pCVar4 = this + 0x1c;
  }
  (**(code **)(*(int *)param_1 + 0x10))
            (pCVar4,((*(int *)(this + 0x20) - *(int *)(this + 0x1c)) / 0xc) * 0xc);
  return;
}



//=== ?WriteToFile@C3DMeshFileXVertexDataChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa58c0 ===

/* [bsim sim=0.9977382925444841 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXVertexDataChunk::WriteToFile(class CDataOutputStream
   &,class CChunkedFileSaver &)const  */

void __thiscall
C3DMeshFileXVertexDataChunk::
_WriteToFile_C3DMeshFileXVertexDataChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
          (C3DMeshFileXVertexDataChunk *this,CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  C3DMeshFileXVertexDataChunk *pCVar1;
  
  CDataOutputStream::WriteCBYTE(param_1,'\x02');
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x10));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x14));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x18));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x1c));
  WriteToFile((CVertexCompressionParams *)(this + 0x20),param_1);
  pCVar1 = *(C3DMeshFileXVertexDataChunk **)(this + 0x40);
  if (*(C3DMeshFileXVertexDataChunk **)(this + 0x44) ==
      *(C3DMeshFileXVertexDataChunk **)(this + 0x40)) {
    pCVar1 = this + 0x40;
  }
  (**(code **)(*(int *)param_1 + 0x10))(pCVar1,*(int *)(this + 0x44) - *(int *)(this + 0x40));
  return;
}



//=== ?WriteToFile@C3DMeshFileXTriangleStripChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00aa5930 ===

void _WriteToFile_C3DMeshFileXTriangleStripChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
               (CDataOutputStream *param_1)

{
  int *piVar1;
  int *piVar2;
  int in_ECX;
  
  piVar1 = (int *)(in_ECX + 0x10);
  CDataOutputStream::WriteCBYTE
            (param_1,(char)(*(int *)(in_ECX + 0x14) - *(int *)(in_ECX + 0x10) >> 1));
  piVar2 = (int *)*piVar1;
  if (*(int *)(in_ECX + 0x14) - *piVar1 >> 1 == 0) {
    piVar2 = piVar1;
  }
  (**(code **)(*(int *)param_1 + 0x10))(piVar2,(*(int *)(in_ECX + 0x14) - *piVar1 >> 1) << 1);
  return;
}



//=== ?CreateNewInstance@C3DMeshFileXVertexDataChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa59c0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual class CChunkedFileChunk * __thiscall
   C3DMeshFileXVertexDataChunk::CreateNewInstance(void)const  */

CChunkedFileChunk * __thiscall
C3DMeshFileXVertexDataChunk::
_CreateNewInstance_C3DMeshFileXVertexDataChunk__UBEPAVCChunkedFileChunk__XZ
          (C3DMeshFileXVertexDataChunk *this)

{
  CChunkedFileChunk *pCVar1;
  
  pCVar1 = ::operator_new(0x4c);
  if (pCVar1 != (CChunkedFileChunk *)0x0) {
    CChunkedFileChunk::CChunkedFileChunk(pCVar1,"XVBD");
    *(undefined ***)pCVar1 = &PTR__scalar_deleting_destructor__0129e33c;
    *(undefined4 *)(pCVar1 + 0x20) = 0x3f800000;
    *(undefined4 *)(pCVar1 + 0x24) = 0x3f800000;
    *(undefined4 *)(pCVar1 + 0x28) = 0x3f800000;
    *(undefined4 *)(pCVar1 + 0x2c) = 0x3f800000;
    *(undefined4 *)(pCVar1 + 0x30) = 0;
    *(undefined4 *)(pCVar1 + 0x34) = 0;
    *(undefined4 *)(pCVar1 + 0x38) = 0;
    *(undefined4 *)(pCVar1 + 0x3c) = 0;
    *(undefined4 *)(pCVar1 + 0x40) = 0;
    *(undefined4 *)(pCVar1 + 0x44) = 0;
    *(undefined4 *)(pCVar1 + 0x48) = 0;
    pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(pCVar1);
    return pCVar1;
  }
  pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(0);
  return pCVar1;
}



//=== ?CreateNewInstance@C3DMeshFileXTriangleStripChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa5a60 ===

void _CreateNewInstance_C3DMeshFileXTriangleStripChunk__UBEPAVCChunkedFileChunk__XZ(void)

{
  CChunkedFileChunk *this;
  int *in_ECX;
  
  this = operator_new(0x1c);
  if (this != (CChunkedFileChunk *)0x0) {
    CChunkedFileChunk::CChunkedFileChunk(this,"XTST");
    *(undefined ***)this = &PTR__scalar_deleting_destructor__0129e380;
    *(undefined4 *)(this + 0x10) = 0;
    *(undefined4 *)(this + 0x14) = 0;
    *(undefined4 *)(this + 0x18) = 0;
    (**(code **)(*in_ECX + 0x28))(this);
    return;
  }
  (**(code **)(*in_ECX + 0x28))(0);
  return;
}



//=== ?CreateNewInstance@C3DMeshFileXUVTangentChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa5b30 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual class CChunkedFileChunk * __thiscall
   C3DMeshFileXBoundingBoxChunk::CreateNewInstance(void)const  */

CChunkedFileChunk * __thiscall
C3DMeshFileXBoundingBoxChunk::
_CreateNewInstance_C3DMeshFileXUVTangentChunk__UBEPAVCChunkedFileChunk__XZ
          (C3DMeshFileXBoundingBoxChunk *this)

{
  CChunkedFileChunk *pCVar1;
  
  pCVar1 = ::operator_new(0x28);
  if (pCVar1 != (CChunkedFileChunk *)0x0) {
    CChunkedFileChunk::CChunkedFileChunk(pCVar1,"XUVT");
    *(undefined ***)pCVar1 = &PTR_FinishWithDynamicTextures_0129e3c0;
    *(undefined4 *)(pCVar1 + 0x10) = 0;
    *(undefined4 *)(pCVar1 + 0x14) = 0;
    *(undefined4 *)(pCVar1 + 0x18) = 0;
    *(undefined4 *)(pCVar1 + 0x1c) = 0;
    *(undefined4 *)(pCVar1 + 0x20) = 0;
    *(undefined4 *)(pCVar1 + 0x24) = 0;
    pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(pCVar1);
    return pCVar1;
  }
  pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(0);
  return pCVar1;
}



//=== ?CreateNewInstance@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00aa5c20 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual class CChunkedFileChunk * __thiscall
   C3DMeshFileXAnimatedPrimitiveBlockChunk::CreateNewInstance(void)const  */

CChunkedFileChunk * __thiscall
C3DMeshFileXAnimatedPrimitiveBlockChunk::
_CreateNewInstance_C3DMeshFileXAnimatedPrimitiveBlockChunk__UBEPAVCChunkedFileChunk__XZ
          (C3DMeshFileXAnimatedPrimitiveBlockChunk *this)

{
  CShopItemDef *this_00;
  CChunkedFileChunk *pCVar1;
  
  this_00 = ::operator_new(0x60);
  if (this_00 != (CShopItemDef *)0x0) {
    CShopItemDef::CShopItemDef(this_00);
    *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0129e404;
    FUN_00ad9030();
    *(undefined4 *)(this_00 + 0x48) = 0;
    *(undefined4 *)(this_00 + 0x4c) = 0;
    *(undefined4 *)(this_00 + 0x50) = 0;
    *(undefined4 *)(this_00 + 0x54) = 0;
    *(undefined4 *)(this_00 + 0x58) = 0;
    *(undefined4 *)(this_00 + 0x5c) = 0;
    pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(this_00);
    return pCVar1;
  }
  pCVar1 = (CChunkedFileChunk *)(**(code **)(*(int *)this + 0x28))(0);
  return pCVar1;
}



//=== ?ReadFromFile@C3DMeshFileXVertexDataChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa66b0 ===

void _ReadFromFile_C3DMeshFileXVertexDataChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
               (CDataInputStream *param_1)

{
  CDataInputStream *this;
  uint in_ECX;
  uint uVar1;
  uint uVar2;
  vector<CEditRegion,std::allocator<CEditRegion>_> *pvVar3;
  vector<CEditRegion,std::allocator<CEditRegion>_> *pvVar4;
  uint uStack_4;
  
  this = param_1;
  uStack_4 = in_ECX;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(param_1,(uchar *)&uStack_4,4);
    }
    else {
      uStack_4 = **(uint **)(param_1 + 0xc);
      *(uint **)(param_1 + 0xc) = *(uint **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x10) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x14) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x18) = param_1;
  if (*(int *)(this + 4) + 4U < 0x80000000) {
    if (*(int *)(this + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this + 0xc);
      *(undefined4 **)(this + 0xc) = *(undefined4 **)(this + 0xc) + 1;
      *(int *)(this + 0x14) = *(int *)(this + 0x14) + -4;
      *(uint *)(this + 4) = *(int *)(this + 4) + 4U;
    }
  }
  *(CDataInputStream **)(in_ECX + 0x1c) = param_1;
  if (1 < uStack_4) {
    ReadFromFile((CVertexCompressionParams *)(in_ECX + 0x20),this);
  }
  uVar2 = *(int *)(in_ECX + 0x14) * *(int *)(in_ECX + 0x18);
  pvVar3 = (vector<CEditRegion,std::allocator<CEditRegion>_> *)(in_ECX + 0x40);
  param_1 = (CDataInputStream *)((uint)param_1 & 0xffffff00);
  std::vector<CEditRegion,std::allocator<CEditRegion>_>::resize(pvVar3,uVar2);
  pvVar4 = *(vector<CEditRegion,std::allocator<CEditRegion>_> **)pvVar3;
  if (*(vector<CEditRegion,std::allocator<CEditRegion>_> **)(in_ECX + 0x44) ==
      *(vector<CEditRegion,std::allocator<CEditRegion>_> **)pvVar3) {
    pvVar4 = pvVar3;
  }
  if ((0 < (int)uVar2) && (*(int *)(this + 4) + uVar2 < 0x80000000)) {
    if (uVar2 - *(int *)(this + 0x14) == 0 || (int)uVar2 < *(int *)(this + 0x14)) {
      pvVar3 = *(vector<CEditRegion,std::allocator<CEditRegion>_> **)(this + 0xc);
      for (uVar1 = uVar2 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
        *(undefined4 *)pvVar4 = *(undefined4 *)pvVar3;
        pvVar3 = pvVar3 + 4;
        pvVar4 = pvVar4 + 4;
      }
      for (uVar1 = uVar2 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
        *pvVar4 = *pvVar3;
        pvVar3 = pvVar3 + 1;
        pvVar4 = pvVar4 + 1;
      }
      *(uint *)(this + 0xc) = *(int *)(this + 0xc) + uVar2;
      *(uint *)(this + 0x14) = *(int *)(this + 0x14) - uVar2;
      *(uint *)(this + 4) = *(int *)(this + 4) + uVar2;
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)pvVar4,uVar2);
  }
  return;
}



//=== ?ReadFromFile@C3DMeshFileXTriangleStripChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa6960 ===

void _ReadFromFile_C3DMeshFileXTriangleStripChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
               (CDataInputStream *param_1)

{
  CDataInputStream *pCVar1;
  CDataInputStream *this;
  uint uVar2;
  uint uVar3;
  int in_ECX;
  uint uVar4;
  vector<unsigned_int,std::allocator<unsigned_int>_> *pvVar5;
  vector<unsigned_int,std::allocator<unsigned_int>_> *pvVar6;
  undefined4 uStack_4;
  
  this = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(param_1,(uchar *)&param_1,4);
    }
    else {
      pCVar1 = (CDataInputStream *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
      param_1 = pCVar1;
    }
  }
  pvVar5 = (vector<unsigned_int,std::allocator<unsigned_int>_> *)(in_ECX + 0x10);
  uStack_4 = 0;
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            (pvVar5,(uint)param_1,(uint)&uStack_4);
  pvVar6 = *(vector<unsigned_int,std::allocator<unsigned_int>_> **)pvVar5;
  if (*(int *)(in_ECX + 0x14) - (int)*(vector<unsigned_int,std::allocator<unsigned_int>_> **)pvVar5
      >> 1 == 0) {
    pvVar6 = pvVar5;
  }
  uVar2 = *(int *)(in_ECX + 0x14) - *(int *)pvVar5 >> 1;
  uVar3 = uVar2 * 2;
  if ((0 < (int)uVar3) && (*(int *)(this + 4) + uVar3 < 0x80000000)) {
    if (uVar3 - *(int *)(this + 0x14) == 0 || (int)uVar3 < *(int *)(this + 0x14)) {
      pvVar5 = *(vector<unsigned_int,std::allocator<unsigned_int>_> **)(this + 0xc);
      for (uVar4 = (uVar2 & 0x7fffffff) >> 1; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pvVar6 = *(undefined4 *)pvVar5;
        pvVar5 = pvVar5 + 4;
        pvVar6 = pvVar6 + 4;
      }
      for (uVar4 = uVar3 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pvVar6 = *pvVar5;
        pvVar5 = pvVar5 + 1;
        pvVar6 = pvVar6 + 1;
      }
      *(uint *)(this + 0xc) = *(int *)(this + 0xc) + uVar3;
      *(uint *)(this + 0x14) = *(int *)(this + 0x14) + uVar2 * -2;
      *(uint *)(this + 4) = *(int *)(this + 4) + uVar3;
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow(this,(uchar *)pvVar6,uVar3);
  }
  return;
}



//=== ?ReadFromFile@C3DMeshFileXUVTangentChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa6a60 ===

/* [bsim sim=0.7463128949555322 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXUVTangentChunk::ReadFromFile(class CDataInputStream
   &,unsigned long,class CChunkedFileLoader &) */

void __thiscall
C3DMeshFileXUVTangentChunk::
_ReadFromFile_C3DMeshFileXUVTangentChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
          (C3DMeshFileXUVTangentChunk *this,CDataInputStream *param_1,ulong param_2,
          CChunkedFileLoader *param_3)

{
  int iVar1;
  CDataInputStream *this_00;
  int iVar2;
  uint uVar3;
  int iVar4;
  vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *pvVar5;
  vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *pvVar6;
  vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *pvVar7;
  CDataInputStream *pCVar8;
  undefined1 local_c [12];
  
  this_00 = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(param_1,(uchar *)&param_1,4);
    }
    else {
      *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 4;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  pCVar8 = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
      pCVar8 = param_1;
    }
    else {
      pCVar8 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  pvVar6 = (vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *)(this + 0x10);
  std::vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_>::resize(pvVar6,pCVar8,local_c);
  pvVar5 = (vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *)(this + 0x1c);
  std::vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_>::resize(pvVar5,pCVar8,local_c);
  iVar2 = *(int *)(this + 0x14) -
          (int)*(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)pvVar6;
  iVar1 = iVar2 >> 0x1f;
  pvVar7 = *(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)pvVar6;
  if (iVar2 / 0xc + iVar1 == iVar1) {
    pvVar7 = pvVar6;
  }
  iVar1 = (*(int *)(this + 0x14) - *(int *)pvVar6) / 0xc;
  iVar2 = iVar1 * 0xc;
  if ((0 < iVar2) && ((uint)(*(int *)(this_00 + 4) + iVar2) < 0x80000000)) {
    if (*(int *)(this_00 + 0x14) < iVar2) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)pvVar7,iVar2);
    }
    else {
      pvVar6 = *(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)(this_00 + 0xc);
      for (uVar3 = iVar1 * 3 & 0x3fffffff; uVar3 != 0; uVar3 = uVar3 - 1) {
        *(undefined4 *)pvVar7 = *(undefined4 *)pvVar6;
        pvVar6 = pvVar6 + 4;
        pvVar7 = pvVar7 + 4;
      }
      for (iVar4 = 0; iVar4 != 0; iVar4 = iVar4 + -1) {
        *pvVar7 = *pvVar6;
        pvVar6 = pvVar6 + 1;
        pvVar7 = pvVar7 + 1;
      }
      *(int *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + iVar2;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + iVar1 * -0xc;
      *(int *)(this_00 + 4) = *(int *)(this_00 + 4) + iVar2;
    }
  }
  iVar2 = *(int *)(this + 0x20) -
          (int)*(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)pvVar5;
  iVar1 = iVar2 >> 0x1f;
  pvVar6 = *(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)pvVar5;
  if (iVar2 / 0xc + iVar1 == iVar1) {
    pvVar6 = pvVar5;
  }
  iVar1 = (*(int *)(this + 0x20) - *(int *)pvVar5) / 0xc;
  iVar2 = iVar1 * 0xc;
  if ((0 < iVar2) && ((uint)(*(int *)(this_00 + 4) + iVar2) < 0x80000000)) {
    if (iVar2 <= *(int *)(this_00 + 0x14)) {
      pvVar7 = *(vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> **)(this_00 + 0xc);
      for (uVar3 = iVar1 * 3 & 0x3fffffff; uVar3 != 0; uVar3 = uVar3 - 1) {
        *(undefined4 *)pvVar6 = *(undefined4 *)pvVar7;
        pvVar7 = pvVar7 + 4;
        pvVar6 = pvVar6 + 4;
      }
      for (iVar4 = 0; iVar4 != 0; iVar4 = iVar4 + -1) {
        *pvVar6 = *pvVar7;
        pvVar7 = pvVar7 + 1;
        pvVar6 = pvVar6 + 1;
      }
      *(int *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + iVar2;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + iVar1 * -0xc;
      *(int *)(this_00 + 4) = *(int *)(this_00 + 4) + iVar2;
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)pvVar6,iVar2);
  }
  return;
}



//=== ?ReadFromFile@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00aa6c70 ===

/* WARNING: Removing unreachable block (ram,0x00aa6ed3) */
/* WARNING: Removing unreachable block (ram,0x00aa6e0a) */
/* [bsim sim=0.6970479443748954 <- ego_r]
   public: virtual void __thiscall C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile(class
   CDataInputStream &,unsigned long,class CChunkedFileLoader &) */

void __thiscall
C3DMeshFileXAnimatedPrimitiveBlockChunk::
_ReadFromFile_C3DMeshFileXAnimatedPrimitiveBlockChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
          (C3DMeshFileXAnimatedPrimitiveBlockChunk *this,CDataInputStream *param_1,ulong param_2,
          CChunkedFileLoader *param_3)

{
  CDataInputStream *this_00;
  CDataInputStream *pCVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  CDataInputStream *pCVar6;
  C3DMeshFileXAnimatedPrimitiveBlockChunk *pCVar7;
  uint uVar8;
  C3DMeshFileXAnimatedPrimitiveBlockChunk *pCVar9;
  int iStack_10;
  int iStack_c;
  undefined1 auStack_8 [8];
  
  this_00 = param_1;
  iStack_c = (**(code **)(*(int *)param_1 + 8))();
  pCVar6 = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
      pCVar6 = param_1;
    }
    else {
      pCVar6 = (CDataInputStream *)**(int **)(this_00 + 0xc);
      *(int **)(this_00 + 0xc) = *(int **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  for (; pCVar6 != (CDataInputStream *)0x0; pCVar6 = pCVar6 + -1) {
    if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
      if (*(int *)(this_00 + 0x14) < 4) {
        CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
      }
      else {
        param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
        *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
        *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
        *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
      }
    }
    FUN_00ad91f0(param_1);
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 0x44) = param_1;
  pCVar6 = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
      pCVar6 = param_1;
    }
    else {
      pCVar6 = (CDataInputStream *)**(uint **)(this_00 + 0xc);
      *(uint **)(this_00 + 0xc) = *(uint **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  pCVar1 = (CDataInputStream *)(*(int *)(this + 0x4c) - *(int *)(this + 0x48) >> 2);
  param_1 = (CDataInputStream *)0x0;
  if (pCVar6 < pCVar1) {
    *(int *)(this + 0x4c) = *(int *)(this + 0x48) + (int)pCVar6 * 4;
  }
  else {
    FUN_004649a3(*(int *)(this + 0x4c),(int)pCVar6 - (int)pCVar1,&param_1);
  }
  pCVar9 = *(C3DMeshFileXAnimatedPrimitiveBlockChunk **)(this + 0x48);
  if (*(int *)(this + 0x4c) - (int)pCVar9 >> 2 == 0) {
    pCVar9 = this + 0x48;
  }
  iVar3 = (int)pCVar6 * 4;
  if ((0 < iVar3) && ((uint)(*(int *)(this_00 + 4) + iVar3) < 0x80000000)) {
    if (iVar3 - *(int *)(this_00 + 0x14) == 0 || iVar3 < *(int *)(this_00 + 0x14)) {
      pCVar7 = *(C3DMeshFileXAnimatedPrimitiveBlockChunk **)(this_00 + 0xc);
      for (uVar4 = (uint)pCVar6 & 0x3fffffff; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(int *)pCVar9 = *(int *)pCVar7;
        pCVar7 = pCVar7 + 4;
        pCVar9 = pCVar9 + 4;
      }
      for (iVar5 = 0; iVar5 != 0; iVar5 = iVar5 + -1) {
        *pCVar9 = *pCVar7;
        pCVar7 = pCVar7 + 1;
        pCVar9 = pCVar9 + 1;
      }
      *(int *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + iVar3;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + (int)pCVar6 * -4;
      *(int *)(this_00 + 4) = *(int *)(this_00 + 4) + iVar3;
    }
    else {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)pCVar9,iVar3);
    }
  }
  uVar8 = *(int *)(this + 0x44) * (int)pCVar6;
  uVar4 = *(int *)(this + 0x58) - *(int *)(this + 0x54) >> 3;
  if (uVar8 < uVar4) {
    *(uint *)(this + 0x58) = *(int *)(this + 0x54) + uVar8 * 8;
  }
  else {
    FUN_00a97180(*(int *)(this + 0x58),uVar8 - uVar4,auStack_8);
  }
  uVar4 = 0;
  if (uVar8 != 0) {
    do {
      if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
        if (*(int *)(this_00 + 0x14) < 4) {
          CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
        }
        else {
          param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
          *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
          *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
          *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
        }
      }
      if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
        if (*(int *)(this_00 + 0x14) < 4) {
          CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&iStack_10,4);
        }
        else {
          iStack_10 = **(int **)(this_00 + 0xc);
          *(int **)(this_00 + 0xc) = *(int **)(this_00 + 0xc) + 1;
          *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
          *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
        }
      }
      *(CDataInputStream **)(*(int *)(this + 0x54) + uVar4 * 8) = param_1;
      uVar2 = __ftol2();
      *(undefined4 *)(*(int *)(this + 0x54) + 4 + uVar4 * 8) = uVar2;
      uVar4 = uVar4 + 1;
    } while (uVar4 < uVar8);
  }
  iVar3 = (**(code **)(*(int *)this_00 + 8))();
  CChunkedFileSuperChunk::
  _ReadFromFile_CChunkedFileSuperChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
            ((CChunkedFileSuperChunk *)this,this_00,param_2 - (iVar3 - iStack_c),param_3);
  return;
}



//=== ?GetChunkDescription@C3DMeshFileXAnimatedPrimitiveChunk@@UBE?BVCCharString@@XZ @ 00aa7090 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXAnimatedPrimitiveChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Animated Primitive",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFileXBoundingBoxChunk@@UBE?BVCCharString@@XZ @ 00aa7140 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXBoundingBoxChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Bounding Box",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFileXVertexDataChunk@@UBE?BVCCharString@@XZ @ 00aa7230 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXVertexDataChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Vertex Buffer Data",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFileXTriangleStripChunk@@UBE?BVCCharString@@XZ @ 00aa7280 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXTriangleStripChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Triangle Strip",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFileXUVTangentChunk@@UBE?BVCCharString@@XZ @ 00aa7350 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXUVTangentChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"UV Tangent Vectors",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFileXAnimatedPrimitiveBlockChunk@@UBE?BVCCharString@@XZ @ 00aa73b0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileXAnimatedPrimitiveBlockChunk__UBE_BVCCharString__XZ
          (CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Animated Primitive Block",-1);
  return in_stack_00000004;
}



//=== C3DMeshFileSubMeshChunk @ 00ab9a70 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall C3DMeshFileSubMeshChunk::C3DMeshFileSubMeshChunk(void) */

C3DMeshFileSubMeshChunk * __thiscall
C3DMeshFileSubMeshChunk::C3DMeshFileSubMeshChunk(C3DMeshFileSubMeshChunk *this)

{
  CShopItemDef::CShopItemDef((CShopItemDef *)this);
  *(undefined ***)this = &PTR__scalar_deleting_destructor__0129eb18;
  CCharString::CCharString((CCharString *)(this + 0x2c));
  *(undefined4 *)(this + 0x30) = 0xffffffff;
  *(undefined4 *)(this + 0x34) = 0xffffffff;
  *(undefined4 *)(this + 0x38) = 0xffffffff;
  *(undefined4 *)(this + 0x3c) = 0xffffffff;
  return this;
}



//=== ?WriteToFile@C3DMeshFileSubMeshChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00ab9af0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshFileSubMeshChunk::WriteToFile(class CDataOutputStream
   &,class CChunkedFileSaver &)const  */

void __thiscall
C3DMeshFileSubMeshChunk::
_WriteToFile_C3DMeshFileSubMeshChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
          (C3DMeshFileSubMeshChunk *this,CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  CDataOutputStream::WriteNullTerminatedString(param_1,(CCharString *)(this + 0x2c));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x30));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x34));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x38));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x3c));
  CChunkedFileSuperChunk::
  _WriteToFile_CChunkedFileSuperChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
            ((CChunkedFileSuperChunk *)this,param_1,param_2);
  return;
}



//=== ?CreateNewInstance@C3DMeshFileSubMeshChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00ab9b50 ===

void _CreateNewInstance_C3DMeshFileSubMeshChunk__UBEPAVCChunkedFileChunk__XZ(void)

{
  CShopItemDef *this;
  int *in_ECX;
  
  this = operator_new(0x40);
  if (this != (CShopItemDef *)0x0) {
    CShopItemDef::CShopItemDef(this);
    *(undefined ***)this = &PTR__scalar_deleting_destructor__0129eb18;
    CCharString::CCharString((CCharString *)(this + 0x2c));
    *(undefined4 *)(this + 0x30) = 0xffffffff;
    *(undefined4 *)(this + 0x34) = 0xffffffff;
    *(undefined4 *)(this + 0x38) = 0xffffffff;
    *(undefined4 *)(this + 0x3c) = 0xffffffff;
    (**(code **)(*in_ECX + 0x28))(this);
    return;
  }
  (**(code **)(*in_ECX + 0x28))(0);
  return;
}



//=== ?ReadFromFile@C3DMeshFileSubMeshChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00ab9fc0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall C3DMeshFileSubMeshChunk::ReadFromFile(class CDataInputStream
   &,unsigned long,class CChunkedFileLoader &) */

void __thiscall
C3DMeshFileSubMeshChunk::
_ReadFromFile_C3DMeshFileSubMeshChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
          (C3DMeshFileSubMeshChunk *this,CDataInputStream *param_1,ulong param_2,
          CChunkedFileLoader *param_3)

{
  CDataInputStream *this_00;
  int iVar1;
  CCharString *pCVar2;
  int iVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  undefined4 unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  
  this_00 = param_1;
  iVar1 = (**(code **)(*(int *)param_1 + 8))();
  ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&param_1;
  pCVar2 = (CCharString *)(**(code **)(*(int *)this_00 + 0x18))();
  CCharString::operator=((CCharString *)(this + 0x2c),pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000000,
             extraout_EDX,ppVar4);
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,&stack0x00000000,4);
    }
    else {
      unaff_retaddr = **(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(undefined4 *)(this + 0x30) = unaff_retaddr;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,&stack0x00000000,4);
    }
    else {
      unaff_retaddr = **(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(undefined4 *)(this + 0x34) = unaff_retaddr;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,&stack0x00000000,4);
    }
    else {
      unaff_retaddr = **(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(undefined4 *)(this + 0x38) = unaff_retaddr;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,&stack0x00000000,4);
    }
    else {
      unaff_retaddr = **(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(undefined4 *)(this + 0x3c) = unaff_retaddr;
  iVar3 = (**(code **)(*(int *)this_00 + 8))();
  CChunkedFileSuperChunk::
  _ReadFromFile_CChunkedFileSuperChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
            ((CChunkedFileSuperChunk *)this,this_00,(int)param_1 - (iVar3 - iVar1),
             (CChunkedFileLoader *)param_2);
  return;
}



//=== ?GetChunkInfo@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ @ 00aba390 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

CCharString * _GetChunkInfo_C3DMeshFileSubMeshChunk__UBE_BVCCharString__XZ(CCharString *param_1)

{
  undefined1 *puVar1;
  int in_ECX;
  char acStack_404 [1023];
  undefined1 uStack_5;
  undefined4 uStack_4;
  
  uStack_4 = DAT_0139c8a8;
  if (*(undefined4 **)(in_ECX + 0x2c) == (undefined4 *)0x0) {
    puVar1 = &DAT_0129aaf4;
  }
  else {
    puVar1 = (undefined1 *)**(undefined4 **)(in_ECX + 0x2c);
  }
  _snprintf(acStack_404,0x400,"Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",puVar1,
            *(undefined4 *)(in_ECX + 0x30),*(undefined4 *)(in_ECX + 0x34),
            *(undefined4 *)(in_ECX + 0x38),*(undefined4 *)(in_ECX + 0x3c));
  uStack_5 = 0;
  CCharString::CCharString(param_1,acStack_404,-1);
  return param_1;
}



//=== ?WriteToFile@C3DMeshFilePrimitiveChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z @ 00abc390 ===

void _WriteToFile_C3DMeshFilePrimitiveChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
               (CDataOutputStream *param_1,CChunkedFileSaver *param_2)

{
  CChunkedFileSuperChunk *in_ECX;
  int iVar1;
  int iVar2;
  
  if ((*(int *)(in_ECX + 0x30) - (int)*(undefined4 **)(in_ECX + 0x2c) & 0xfffffffcU) != 4) {
    iVar1 = *(int *)(in_ECX + 0x30) - *(int *)(in_ECX + 0x2c) >> 2;
    CDataOutputStream::WriteCBYTE(param_1,(char)iVar1);
    iVar2 = 0;
    if (0 < iVar1) {
      do {
        CDataOutputStream::WriteCBYTE
                  (param_1,(char)*(undefined4 *)(*(int *)(in_ECX + 0x2c) + iVar2 * 4));
        iVar2 = iVar2 + 1;
      } while (iVar2 < iVar1);
    }
    CChunkedFileSuperChunk::
    _WriteToFile_CChunkedFileSuperChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
              (in_ECX,param_1,param_2);
    return;
  }
  CDataOutputStream::WriteCBYTE(param_1,(char)**(undefined4 **)(in_ECX + 0x2c));
  CChunkedFileSuperChunk::
  _WriteToFile_CChunkedFileSuperChunk__UBEXAAVCDataOutputStream__AAVCChunkedFileSaver___Z
            (in_ECX,param_1,param_2);
  return;
}



//=== ?ReadFromFile@C3DMeshFilePrimitiveChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z @ 00abcf40 ===

/* [bsim sim=0.8634601131525939 <- ego_r]
   public: virtual void __thiscall C3DMeshFilePrimitiveChunk::ReadFromFile(class CDataInputStream
   &,unsigned long,class CChunkedFileLoader &) */

void __thiscall
C3DMeshFilePrimitiveChunk::
_ReadFromFile_C3DMeshFilePrimitiveChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
          (C3DMeshFilePrimitiveChunk *this,CDataInputStream *param_1,ulong param_2,
          CChunkedFileLoader *param_3)

{
  CDataInputStream *this_00;
  int iVar1;
  int iVar2;
  CDataInputStream *pCVar3;
  uint uVar4;
  
  this_00 = param_1;
  iVar1 = (**(code **)(*(int *)param_1 + 8))();
  iVar2 = 0;
  pCVar3 = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
      pCVar3 = param_1;
    }
    else {
      pCVar3 = (CDataInputStream *)**(uint **)(this_00 + 0xc);
      *(uint **)(this_00 + 0xc) = *(uint **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  if ((pCVar3 == (CDataInputStream *)0xffffffff) || (-1 < (int)pCVar3)) {
    param_1 = (CDataInputStream *)0x0;
    std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
              ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x2c),1,(uint)&param_1)
    ;
    **(uint **)(this + 0x2c) = (uint)pCVar3;
  }
  else {
    uVar4 = (uint)pCVar3 & 0x7fffffff;
    param_1 = (CDataInputStream *)0x0;
    std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
              ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x2c),uVar4,
               (uint)&param_1);
    if (uVar4 != 0) {
      do {
        if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
          if (*(int *)(this_00 + 0x14) < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
          }
          else {
            param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
            *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
            *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
            *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
          }
        }
        *(CDataInputStream **)(*(int *)(this + 0x2c) + iVar2 * 4) = param_1;
        iVar2 = iVar2 + 1;
      } while (iVar2 < (int)uVar4);
    }
  }
  iVar2 = (**(code **)(*(int *)this_00 + 8))();
  CChunkedFileSuperChunk::
  _ReadFromFile_CChunkedFileSuperChunk__UAEXAAVCDataInputStream__KAAVCChunkedFileLoader___Z
            ((CChunkedFileSuperChunk *)this,this_00,param_2 - (iVar2 - iVar1),param_3);
  return;
}



//=== ?CreateNewInstance@C3DMeshFilePrimitiveChunk@@UBEPAVCChunkedFileChunk@@XZ @ 00abd0d0 ===

void _CreateNewInstance_C3DMeshFilePrimitiveChunk__UBEPAVCChunkedFileChunk__XZ(void)

{
  vector<unsigned_int,std::allocator<unsigned_int>_> *this;
  CShopItemDef *this_00;
  int *in_ECX;
  undefined4 uStack_4;
  
  this_00 = operator_new(0x38);
  if (this_00 != (CShopItemDef *)0x0) {
    CShopItemDef::CShopItemDef(this_00);
    this = (vector<unsigned_int,std::allocator<unsigned_int>_> *)(this_00 + 0x2c);
    *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0129ed74;
    *(undefined4 *)this = 0;
    *(undefined4 *)(this_00 + 0x30) = 0;
    *(undefined4 *)(this_00 + 0x34) = 0;
    uStack_4 = 0;
    std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize(this,1,(uint)&uStack_4);
    **(undefined4 **)this = 0xffffffff;
    (**(code **)(*in_ECX + 0x28))(this_00);
    return;
  }
  (**(code **)(*in_ECX + 0x28))(0);
  return;
}



//=== ?GetChunkDescription@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ @ 00abd3c0 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFileSubMeshChunk__UBE_BVCCharString__XZ(CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Sub-Mesh",-1);
  return in_stack_00000004;
}



//=== ?GetChunkDescription@C3DMeshFilePrimitiveChunk@@UBE?BVCCharString@@XZ @ 00abee90 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall
   NTCScriptedControl::CActionDoCreatureAction::GetActionName(void)const  */

CCharString * __thiscall
NTCScriptedControl::CActionDoCreatureAction::
_GetChunkDescription_C3DMeshFilePrimitiveChunk__UBE_BVCCharString__XZ(CActionDoCreatureAction *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,"Primitive",-1);
  return in_stack_00000004;
}



