//=== ?WriteULONG@CDataOutputStream@@QAEXK@Z @ 0179f6d0 ===

void _WriteULONG_CDataOutputStream__QAEXK_Z(void)

{
  int *in_ECX;
  
  (**(code **)(*in_ECX + 0x10))(&stack0x00000004,4);
  return;
}



//=== ?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z @ 017af693 ===

void _WriteString_CDataOutputStream__QAEXABVCCharString___Z(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int *in_ECX;
  
  uVar1 = _GetAsCharArray_CCharString__QBEPBDXZ();
  uVar2 = _GetLength_CCharString__QBEJXZ();
  (**(code **)(*in_ECX + 0x10))(uVar1,uVar2);
  return;
}



//=== ?GetByteLength@CCharString@@QBEJXZ @ 01810cf4 ===

void _GetByteLength_CCharString__QBEJXZ(void)

{
  _GetLength_CCharString__QBEJXZ();
  return;
}



//=== ?ReadULONG@CDataInputStream@@QAEKXZ @ 01814458 ===

void _ReadULONG_CDataInputStream__QAEKXZ(void)

{
  ___ReadType_K_CDataInputStream__QAEKXZ();
  return;
}



//=== ??$GFSerialiseVectorBinaryOut@VCActionInputControl@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z @ 0184e171 ===

/* WARNING: Removing unreachable block (ram,0x0189c030) */

void ___GFSerialiseVectorBinaryOut_VCActionInputControl____YIXAAVCDataOutputStream__AAV__vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std___Z
               (void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 in_ECX;
  int iStack_34;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 uStack_8;
  
  uStack_8 = 0xffffffff;
  puStack_c = &LAB_03c09438;
  pvStack_10 = ExceptionList;
  ExceptionList = &pvStack_10;
  __0__CPersistTraits_VCActionInputControl____QAE_XZ();
  uStack_8 = 0;
  iVar1 = _size___vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QBEIXZ
                    ();
  uVar2 = _size___vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QBEIXZ
                    ();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  for (iStack_34 = 0; iStack_34 < iVar1; iStack_34 = iStack_34 + 1) {
    uVar2 = __A__vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QAEAAVCActionInputControl__I_Z
                      (iStack_34);
    _TransferBinaryOut___CPersistTraits_VCActionInputControl____QBEXAAVCDataOutputStream__ABVCActionInputControl___Z
              (in_ECX,uVar2);
  }
  uStack_8 = 0xffffffff;
  __1__CPersistTraits_VCActionInputControl____QAE_XZ();
  ExceptionList = pvStack_10;
  return;
}



//=== ?GetByteLength@CCharString@@QBEJXZ @ 0186d370 ===

/* [ported from ego_r via strfp] */

void _GetByteLength_CCharString__QBEJXZ(void)

{
  _GetLength_CCharString__QBEJXZ();
  return;
}



//=== ?ReadULONG@CDataInputStream@@QAEKXZ @ 01874a20 ===

/* [ported from ego_r via strfp] */

void _ReadULONG_CDataInputStream__QAEKXZ(void)

{
  ___ReadType_K_CDataInputStream__QAEKXZ();
  return;
}



//=== ??$GFSerialiseVectorBinaryOut@VCActionInputControl@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@@Z @ 0189bfe0 ===

/* WARNING: Removing unreachable block (ram,0x0189c030) */
/* [ported from ego_r via strfp] */

void ___GFSerialiseVectorBinaryOut_VCActionInputControl____YIXAAVCDataOutputStream__AAV__vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std___Z
               (void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 in_ECX;
  int local_34;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c09438;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0__CPersistTraits_VCActionInputControl____QAE_XZ();
  local_8 = 0;
  iVar1 = _size___vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QBEIXZ
                    ();
  uVar2 = _size___vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QBEIXZ
                    ();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  for (local_34 = 0; local_34 < iVar1; local_34 = local_34 + 1) {
    uVar2 = __A__vector_VCActionInputControl__V__allocator_VCActionInputControl___std___std__QAEAAVCActionInputControl__I_Z
                      (local_34);
    _TransferBinaryOut___CPersistTraits_VCActionInputControl____QBEXAAVCDataOutputStream__ABVCActionInputControl___Z
              (in_ECX,uVar2);
  }
  local_8 = 0xffffffff;
  __1__CPersistTraits_VCActionInputControl____QAE_XZ();
  ExceptionList = local_10;
  return;
}



//=== ?WriteULONG@CDataOutputStream@@QAEXK@Z @ 03082850 ===

/* [ported from ego_r via strfp] */

void _WriteULONG_CDataOutputStream__QAEXK_Z(void)

{
  int *in_ECX;
  
  (**(code **)(*in_ECX + 0x10))(&stack0x00000004,4);
  return;
}



//=== ?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z @ 03082f50 ===

/* [ported from ego_r via strfp] */

void _WriteString_CDataOutputStream__QAEXABVCCharString___Z(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int *in_ECX;
  
  uVar1 = _GetAsCharArray_CCharString__QBEPBDXZ();
  uVar2 = _GetLength_CCharString__QBEJXZ();
  (**(code **)(*in_ECX + 0x10))(uVar1,uVar2);
  return;
}



