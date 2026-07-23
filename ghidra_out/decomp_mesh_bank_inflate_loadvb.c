INFO  DecompFuncs.java> ==================== LoadVertexBuffer @ 00a91560 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.6921914231236962 <- ego_r]
   private: void __thiscall C3DPrimitive2::LoadVertexBuffer(class CMemoryDataInputStream &,unsigned
   long,unsigned long,unsigned long,class CVertexCompressionParams &) */

void __thiscall
C3DPrimitive2::LoadVertexBuffer
          (C3DPrimitive2 *this,CMemoryDataInputStream *param_1,ulong param_2,ulong param_3,
          ulong param_4,CVertexCompressionParams *param_5)

{
  bool bVar1;
  int iVar2;
  undefined3 extraout_var;
  CThing *unaff_EBX;
  CThing *unaff_EBP;
  int iVar4;
  void *unaff_EDI;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  int *piStack_4;
  CThing *pCVar3;
  
  iVar2 = *(int *)(this + 8);
  if (iVar2 < 2) {
    iVar2 = 1;
  }
  iVar2 = *(int *)(this + 0x58) * iVar2;
  uVar7 = 0;
  uVar6 = 0;
  iVar4 = iVar2 * param_3;
  bVar1 = CThingFilter_IsValid::operator()(*(CThingFilter_IsValid **)(this + 0x30),(CThing *)0x0);
  pCVar3 = (CThing *)CONCAT31(extraout_var,bVar1);
  if ((param_2 == *(ulong *)(this + 0x68)) || (0 < *(int *)(this + 8))) {
    (**(code **)(*piStack_4 + 0x10))(pCVar3,iVar4,uVar6,uVar7);
  }
  else {
    FUN_00514e20(iVar4);
    if (unaff_EBX == unaff_EBP) {
      unaff_EBP = (CThing *)&stack0xffffffec;
    }
    (**(code **)(*piStack_4 + 0x10))(unaff_EBP,iVar4);
    if (*(int *)(this + 0x48) == 0) {
      FUN_00a91390(piStack_4,*(undefined4 *)(this + 0x68),iVar2,pCVar3);
      pCVar3 = unaff_EBP;
    }
    else {
      FUN_00a90fc0(piStack_4,*(undefined4 *)(this + 0x68),iVar2,pCVar3);
      pCVar3 = unaff_EBP;
    }
    if (unaff_EDI != (void *)0x0) {
      free(unaff_EDI);
    }
  }
  CThingFilter_IsValid::operator()(*(CThingFilter_IsValid **)(this + 0x30),pCVar3);
  puVar5 = (undefined4 *)(*(int *)(this + 0x30) + 0x14);
  for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar5 = *(undefined4 *)param_2;
    param_2 = (ulong)(param_2 + 4);
    puVar5 = puVar5 + 1;
  }
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== LoadBinary @ 00a92900 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.7836406936547198 <- ego_r]
   public: bool __thiscall C3DPrimitive2::LoadBinary(class CMemoryDataInputStream &,unsigned
   long,unsigned char * *,class CMovableResourceMemoryPool *) */

bool __thiscall
C3DPrimitive2::LoadBinary
          (C3DPrimitive2 *this,CMemoryDataInputStream *param_1,ulong param_2,uchar **param_3,
          CMovableResourceMemoryPool *param_4)

{
  CStaticBlock *pCVar1;
  undefined4 *puVar2;
  int *piVar3;
  CMemoryDataInputStream *this_00;
  bool bVar4;
  undefined4 *extraout_EAX;
  uchar *puVar5;
  ulong uVar6;
  CIndexBuffer *this_01;
  undefined4 uVar7;
  ulong uVar8;
  CMemoryDataInputStream *pCVar9;
  CCountedPointer<CGuiVarTransferBase> *pCVar10;
  CBase *this_02;
  undefined4 *puVar11;
  int iVar12;
  int iVar13;
  EIndexBufferPool EVar14;
  uint uVar15;
  CBrainUpdateZone *unaff_EDI;
  uchar **ppuVar16;
  CMemoryDataInputStream **ppCVar17;
  uint local_34;
  EVertexType local_30;
  ulong local_2c [3];
  CMemoryDataInputStream *local_20 [8];
  
  this_00 = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&param_1,4);
    }
    else {
      pCVar9 = (CMemoryDataInputStream *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
      param_1 = pCVar9;
    }
  }
  *(CMemoryDataInputStream **)(this + 4) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 8) = param_1;
  CPersistTraits<CBrainUpdateZone>::TransferBinaryIn
            ((CPersistTraits<CBrainUpdateZone> *)this_00,(CMemoryDataInputStream *)local_2c,
             unaff_EDI);
  *(undefined4 *)(this + 0xc) = *extraout_EAX;
  *(undefined4 *)(this + 0x10) = extraout_EAX[1];
  *(undefined4 *)(this + 0x14) = extraout_EAX[2];
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x18) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x1c) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x40) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x48) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x58) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x5c) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x60) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&local_34,4);
    }
    else {
      local_34 = **(uint **)(this_00 + 0xc);
      *(uint **)(this_00 + 0xc) = *(uint **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(uint *)(this + 0x68) = local_34 & 0xfffffffb;
  this[0x65] = (C3DPrimitive2)0x0;
  if (*(int *)(this + 0x48) != 0) {
    this[0x65] = (C3DPrimitive2)0x1;
  }
  if ((local_34 & 2) != 0) {
    this[0x65] = (C3DPrimitive2)((byte)this[0x65] | 2);
  }
  if ((local_34 & 0x10) != 0) {
    this[0x65] = (C3DPrimitive2)((byte)this[0x65] | 4);
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x40) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  ppuVar16 = param_3;
  *(CMemoryDataInputStream **)(this + 0x48) = param_1;
  pCVar1 = (CStaticBlock *)*param_3;
  *(CStaticBlock **)(this + 0x44) = pCVar1;
  GFExplicitNew<N3DPrimitive::CStaticBlock>(pCVar1,*(ulong *)(this + 0x40));
  *ppuVar16 = *ppuVar16 + *(int *)(this + 0x40) * 0x20;
  uVar15 = 0;
  if (*(int *)(this + 0x40) != 0) {
    param_1 = (CMemoryDataInputStream *)0x0;
    do {
      N3DPrimitive::CStaticBlock::LoadBinary
                ((CStaticBlock *)(param_1 + *(int *)(this + 0x44)),this_00);
      uVar15 = uVar15 + 1;
      param_1 = param_1 + 0x20;
    } while (uVar15 < *(uint *)(this + 0x40));
  }
  iVar12 = *(int *)(this + 0x48);
  puVar5 = *ppuVar16;
  uVar15 = 0;
  *(uchar **)(this + 0x4c) = puVar5;
  if (iVar12 != 0) {
    puVar5 = puVar5 + 8;
    do {
      if (puVar5 != &DAT_00000008) {
        puVar5[-0xffffffff00000008] = '\0';
        puVar5[-0xffffffff00000007] = '\0';
        puVar5[-0xffffffff00000006] = '\0';
        puVar5[-0xffffffff00000005] = '\0';
        puVar5[-0xffffffff00000004] = '\0';
        puVar5[-0xffffffff00000003] = '\0';
        puVar5[-0xffffffff00000002] = '\0';
        puVar5[-0xffffffff00000001] = '\0';
        *puVar5 = '\0';
        puVar5[1] = '\0';
        puVar5[2] = '\0';
        puVar5[0x24] = '\0';
        puVar5[0x25] = '\0';
        puVar5[0x26] = '\0';
        puVar5[0x27] = '\0';
      }
      puVar5 = puVar5 + 0x30;
      iVar12 = iVar12 + -1;
    } while (iVar12 != 0);
  }
  *ppuVar16 = *ppuVar16 + *(int *)(this + 0x48) * 0x30;
  if (*(int *)(this + 0x48) != 0) {
    iVar12 = 0;
    do {
      N3DPrimitive::CAnimatedBlock::LoadBinary
                ((CAnimatedBlock *)(*(int *)(this + 0x4c) + iVar12),this_00);
      uVar15 = uVar15 + 1;
      iVar12 = iVar12 + 0x30;
    } while (uVar15 < *(uint *)(this + 0x48));
  }
  ppuVar16 = *(uchar ***)(this + 8);
  param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,*(int *)(this + 0x48) != 0);
  if ((int)ppuVar16 < 2) {
    ppuVar16 = (uchar **)0x1;
  }
  iVar12 = *(int *)(this_00 + 4);
  local_20[0] = (CMemoryDataInputStream *)0x3f800000;
  local_20[1] = (CMemoryDataInputStream *)0x3f800000;
  local_20[2] = (CMemoryDataInputStream *)0x3f800000;
  local_20[3] = (CMemoryDataInputStream *)0x3f800000;
  local_20[4] = (CMemoryDataInputStream *)0x0;
  local_20[5] = (CMemoryDataInputStream *)0x0;
  local_20[6] = (CMemoryDataInputStream *)0x0;
  local_20[7] = (CMemoryDataInputStream *)0x0;
  param_3 = ppuVar16;
  if (iVar12 + 0x20U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 0x20) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)local_20,0x20)
      ;
    }
    else {
      puVar2 = *(undefined4 **)(this_00 + 0xc);
      puVar11 = puVar2;
      ppCVar17 = local_20;
      for (iVar13 = 8; iVar13 != 0; iVar13 = iVar13 + -1) {
        *ppCVar17 = (CMemoryDataInputStream *)*puVar11;
        puVar11 = puVar11 + 1;
        ppCVar17 = ppCVar17 + 1;
      }
      *(undefined4 **)(this_00 + 0xc) = puVar2 + 8;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -0x20;
      *(uint *)(this_00 + 4) = iVar12 + 0x20U;
    }
  }
  uVar15 = (uint)param_3 >> 8;
  param_3 = (uchar **)((uint)param_3 & 0xffffff00);
  if (0 < *(int *)(this + 8)) {
    param_3 = (uchar **)CONCAT31((int3)uVar15,1);
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)local_2c,4);
    }
    else {
      local_2c[0] = **(ulong **)(this_00 + 0xc);
      *(ulong **)(this_00 + 0xc) = *(ulong **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&local_30,4);
    }
    else {
      local_30 = **(EVertexType **)(this_00 + 0xc);
      *(EVertexType **)(this_00 + 0xc) = *(EVertexType **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  uVar6 = CalculateVertexSize(*(ulong *)(this + 0x68),param_1._0_1_,SUB41(param_3,0));
  bVar4 = SetupVertexBuffer(this,local_30,uVar6,*(int *)(this + 0x58) * (int)ppuVar16,
                            *(ulong *)(this + 0x68));
  if (!bVar4) {
    return false;
  }
  LoadVertexBuffer(this,this_00,uVar6,local_2c[0],local_34,(CVertexCompressionParams *)local_20);
  uVar6 = *(int *)(this + 0x60) * (int)ppuVar16 * 2;
  this_01 = ::operator_new(0x14);
  if (this_01 == (CIndexBuffer *)0x0) {
    uVar7 = 0;
  }
  else {
    uVar7 = ::CIndexBuffer::CIndexBuffer(this_01);
  }
  puVar2 = *(undefined4 **)(this + 0x34);
  *(undefined4 *)(this + 0x34) = uVar7;
  if (puVar2 != (undefined4 *)0x0) {
    (**(code **)*puVar2)(1);
  }
  EVar14 = 1;
  uVar8 = 8;
  if ((param_2 & 1) != 0) {
    uVar8 = 0x18;
    EVar14 = 2;
  }
  ::CIndexBuffer::Initialise(*(CIndexBuffer **)(this + 0x34),uVar6,uVar8,0x65,EVar14,4);
  puVar5 = ::CIndexBuffer::Lock(*(CIndexBuffer **)(this + 0x34),0,0,0);
  (**(code **)(*(int *)this_00 + 0x10))(puVar5,uVar6);
  CVertexBufferWin32::DoUnlock(*(CVertexBufferWin32 **)(this + 0x34));
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)this_00,(uchar *)(local_20 + 7),4);
    }
    else {
      local_20[7] = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  iVar12 = *(int *)(this + 0x24);
  local_34 = 0;
  local_30 = 0;
  pCVar9 = (CMemoryDataInputStream *)(iVar12 - *(int *)(this + 0x20) >> 3);
  param_1 = local_20[7];
  if (local_20[7] < pCVar9) {
    pCVar10 = std::
              _Move<CCountedPointer<CGuiVarTransferBase>*,CCountedPointer<class_CGuiVarTransferBase>*>
                        (iVar12,iVar12,*(int *)(this + 0x20) + (int)local_20[7] * 8,local_20 + 7,0);
    std::
    _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
              (pCVar10,*(undefined4 *)(this + 0x24),local_20 + 7);
    *(CCountedPointer<CGuiVarTransferBase> **)(this + 0x24) = pCVar10;
  }
  else {
    FUN_00a97e40(iVar12,(int)local_20[7] - (int)pCVar9,&local_34);
  }
  local_20[7] = (CMemoryDataInputStream *)0x0;
  if (param_1 != (CMemoryDataInputStream *)0x0) {
    do {
      puVar2 = (undefined4 *)(*(int *)(this + 0x20) + (int)local_20[7] * 8);
      this_02 = ::operator_new(0x14);
      if (this_02 == (CBase *)0x0) {
        this_02 = (CBase *)0x0;
      }
      else {
        NHeroInformationScreens::CBase::CBase(this_02);
        *(undefined ***)this_02 = &PTR__vector_deleting_destructor__0129df74;
        *(undefined4 *)(this_02 + 4) = 0;
        *(undefined4 *)(this_02 + 8) = 0;
      }
      piVar3 = (int *)puVar2[1];
      if (piVar3 != (int *)0x0) {
        *piVar3 = *piVar3 + -1;
        if (*(int *)puVar2[1] == 0) {
          (*(code *)((int *)puVar2[1])[1])();
          operator_delete((void *)puVar2[1]);
        }
      }
      puVar2[1] = 0;
      *puVar2 = this_02;
      if (this_02 != (CBase *)0x0) {
        puVar11 = ::operator_new(0xc);
        if (puVar11 == (undefined4 *)0x0) {
          puVar11 = (undefined4 *)0x0;
        }
        else {
          uVar7 = *puVar2;
          *puVar11 = 1;
          puVar11[1] = &LAB_00a93a60;
          puVar11[2] = uVar7;
        }
        puVar2[1] = puVar11;
      }
      bVar4 = C3DClothPrimitive::LoadBinary
                        ((C3DClothPrimitive *)*puVar2,this_00,(CMovableResourceMemoryPool *)param_2)
      ;
      if (!bVar4) {
        pCVar10 = std::
                  _Move<CCountedPointer<CGuiVarTransferBase>*,CCountedPointer<class_CGuiVarTransferBase>*>
                            (*(undefined4 *)(this + 0x24),*(undefined4 *)(this + 0x24),
                             *(undefined4 *)(this + 0x20),local_20 + 7,0);
        std::
        _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
                  (pCVar10,*(undefined4 *)(this + 0x24),local_20 + 7);
        *(CCountedPointer<CGuiVarTransferBase> **)(this + 0x24) = pCVar10;
        break;
      }
      local_20[7] = local_20[7] + 1;
    } while (local_20[7] < param_1);
  }
  *(undefined4 *)(this + 0x38) = 1;
  *(undefined4 *)(this + 0x3c) = 1;
  *(undefined4 *)(this + 0x50) = 1;
  *(undefined4 *)(this + 0x54) = 1;
  this[100] = (C3DPrimitive2)0x1;
  return true;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /Fable.exe (HeadlessAnalyzer)  
