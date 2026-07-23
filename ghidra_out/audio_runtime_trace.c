//=== ?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z @ 017b7447 ===

void _PlayCollisionSound_CTCSoundPlayer__QAEXABVC3DVector__ABV__CDefPointer___CBVCMaterialDef____1_Z
               (undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  int *piVar5;
  undefined1 *puVar6;
  undefined *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 auStack_d4 [8];
  undefined1 auStack_cc [12];
  undefined1 auStack_c0 [8];
  undefined1 auStack_b8 [7];
  char cStack_b1;
  undefined1 auStack_98 [8];
  undefined1 auStack_90 [4];
  undefined1 auStack_8c [8];
  undefined1 auStack_84 [7];
  char cStack_7d;
  undefined1 auStack_74 [8];
  undefined1 auStack_6c [4];
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined1 auStack_58 [11];
  char cStack_4d;
  undefined1 auStack_44 [8];
  undefined1 auStack_3c [4];
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined1 auStack_28 [11];
  char cStack_1d;
  undefined4 uStack_14;
  void *pvStack_10;
  undefined1 *puStack_c;
  int iStack_8;
  
  iStack_8 = 0xffffffff;
  puStack_c = &LAB_03cafb8b;
  pvStack_10 = ExceptionList;
  if (DAT_04a4cbdf != '\0') {
    ExceptionList = &pvStack_10;
    _GetParentThing_CTCBase__QAEAAVCThing__XZ();
    uStack_14 = _GetSampleBank_CThing__QBEAAVCASoundBank__XZ();
    __0CCharString__QAE_PBDJ_Z("SE_COLLISION;",0xffffffff);
    iStack_8 = 0;
    cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
    if ((cVar2 != '\0') &&
       (cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ(), cVar2 != '\0')) {
      iStack_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      ExceptionList = pvStack_10;
      return;
    }
    cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
    if (cVar2 == '\0') {
      cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
      if (cVar2 == '\0') {
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = auStack_84;
        (**(code **)(*piVar5 + 0x28))(puVar9,auStack_8c);
        GetObject(puVar9);
        iStack_8._0_1_ = 0xb;
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = auStack_98;
        (**(code **)(*piVar5 + 0x28))(auStack_90,puVar9);
        GetObject(puVar9);
        iStack_8._0_1_ = 0xc;
        uVar11 = 0;
        uVar12 = 0;
        puVar9 = auStack_8c;
        puVar7 = &DAT_041172a8;
        __H_YI_AVCCharString__ABV0_0_Z(auStack_98);
        iStack_8._0_1_ = 0xd;
        __H_YI_AVCCharString__ABV0_PBD_Z(puVar7);
        iStack_8._0_1_ = 0xe;
        uVar10 = __H_YI_AVCCharString__ABV0_0_Z(puVar9);
        iStack_8._0_1_ = 0xf;
        iVar3 = _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                          (uStack_14,param_1,uVar10,uVar12,uVar11);
        cStack_7d = iVar3 == -1;
        iStack_8._0_1_ = 0xe;
        __1CCharString__QAE_XZ();
        iStack_8._0_1_ = 0xd;
        __1CCharString__QAE_XZ();
        iStack_8._0_1_ = 0xc;
        __1CCharString__QAE_XZ();
        iStack_8._0_1_ = 0xb;
        __1CCharString__QAE_XZ();
        iStack_8._0_1_ = 0;
        __1CCharString__QAE_XZ();
        if (cStack_7d != '\0') {
          piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
          puVar9 = auStack_c0;
          (**(code **)(*piVar5 + 0x28))(auStack_b8,puVar9);
          GetObject(puVar9);
          iStack_8._0_1_ = 0x10;
          uVar11 = 0;
          uVar12 = 0;
          uVar10 = __H_YI_AVCCharString__ABV0_0_Z(auStack_c0);
          iStack_8._0_1_ = 0x11;
          iVar3 = _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                            (uStack_14,param_1,uVar10,uVar12,uVar11);
          cStack_b1 = iVar3 == -1;
          iStack_8._0_1_ = 0x10;
          __1CCharString__QAE_XZ();
          iStack_8._0_1_ = 0;
          __1CCharString__QAE_XZ();
          if (cStack_b1 != '\0') {
            piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
            puVar9 = auStack_d4;
            (**(code **)(*piVar5 + 0x28))(auStack_cc,puVar9);
            GetObject(puVar9);
            iStack_8._0_1_ = 0x12;
            uVar11 = 0;
            uVar12 = 0;
            uVar10 = __H_YI_AVCCharString__ABV0_0_Z(auStack_d4);
            iStack_8._0_1_ = 0x13;
            _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                      (uStack_14,param_1,uVar10,uVar12,uVar11);
            iStack_8._0_1_ = 0x12;
            __1CCharString__QAE_XZ();
            iStack_8 = (uint)iStack_8._1_3_ << 8;
            __1CCharString__QAE_XZ();
          }
        }
      }
      else {
        iVar3 = _Get___CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        if (iVar3 == 0) {
          __0CWideString__QAE_PB_W_Z(L"tc_sound_player.cpp");
          iStack_8._0_1_ = 6;
          __0CCharString__QAE_PBDJ_Z(&DAT_0411722f,0xffffffff);
          iStack_8._0_1_ = 7;
          __0CCharString__QAE_PBDJ_Z("pmaterial1.Get()",0xffffffff);
          iStack_8._0_1_ = 8;
          uVar12 = 1;
          uVar10 = 0x5e;
          puVar9 = auStack_58;
          puVar8 = auStack_60;
          puVar6 = auStack_68;
          _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar8,puVar9,0x5e,1);
          _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
          cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                            (puVar6,puVar8,puVar9,uVar10,uVar12);
          cStack_4d = '\x01' - (cVar2 != '\x01');
          iStack_8._0_1_ = 7;
          __1CCharString__QAE_XZ();
          iStack_8._0_1_ = 6;
          __1CCharString__QAE_XZ();
          iStack_8 = (uint)iStack_8._1_3_ << 8;
          __1CWideString__QAE_XZ();
          if ((cStack_4d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
            pcVar1 = (code *)swi(3);
            (*pcVar1)();
            return;
          }
        }
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = auStack_6c;
        (**(code **)(*piVar5 + 0x28))(puVar9,auStack_74);
        GetObject(puVar9);
        iStack_8._0_1_ = 9;
        uVar11 = 0;
        uVar12 = 0;
        uVar10 = __H_YI_AVCCharString__ABV0_0_Z(auStack_74);
        iStack_8._0_1_ = 10;
        _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                  (uStack_14,param_1,uVar10,uVar12,uVar11);
        iStack_8._0_1_ = 9;
        __1CCharString__QAE_XZ();
        iStack_8 = (uint)iStack_8._1_3_ << 8;
        __1CCharString__QAE_XZ();
      }
    }
    else {
      iVar3 = _Get___CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
      if (iVar3 == 0) {
        __0CWideString__QAE_PB_W_Z(L"tc_sound_player.cpp");
        iStack_8._0_1_ = 1;
        __0CCharString__QAE_PBDJ_Z(&DAT_0411722e,0xffffffff);
        iStack_8._0_1_ = 2;
        __0CCharString__QAE_PBDJ_Z("pmaterial2.Get()",0xffffffff);
        iStack_8._0_1_ = 3;
        uVar12 = 1;
        uVar10 = 0x59;
        puVar9 = auStack_28;
        puVar8 = auStack_30;
        puVar6 = auStack_38;
        _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar8,puVar9,0x59,1);
        _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
        cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                          (puVar6,puVar8,puVar9,uVar10,uVar12);
        cStack_1d = '\x01' - (cVar2 != '\x01');
        iStack_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
        iStack_8._0_1_ = 1;
        __1CCharString__QAE_XZ();
        iStack_8 = (uint)iStack_8._1_3_ << 8;
        __1CWideString__QAE_XZ();
        if ((cStack_1d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
      piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
      puVar9 = auStack_3c;
      (**(code **)(*piVar5 + 0x28))(puVar9,auStack_44);
      GetObject(puVar9);
      iStack_8._0_1_ = 4;
      uVar11 = 0;
      uVar12 = 0;
      uVar10 = __H_YI_AVCCharString__ABV0_0_Z(auStack_44);
      iStack_8._0_1_ = 5;
      _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                (uStack_14,param_1,uVar10,uVar12,uVar11);
      iStack_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      iStack_8 = (uint)iStack_8._1_3_ << 8;
      __1CCharString__QAE_XZ();
    }
    iStack_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
  }
  ExceptionList = pvStack_10;
  return;
}



//=== ?PeekSampleBank@CTCBase@@IBEABVCASoundBank@@XZ @ 017c918d ===

void _PeekSampleBank_CTCBase__IBEABVCASoundBank__XZ(void)

{
  _GetThingManager_CTCBase__IBEABVCThingManager__XZ();
  _PeekComponent_CThingManager__QBEABVCMainGameComponent__XZ();
  _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ();
  return;
}



//=== ?GetSampleBank@CMainGameComponent@@QBEAAVCASoundBank@@XZ @ 017cdac6 ===

undefined4 _GetSampleBank_CMainGameComponent__QBEAAVCASoundBank__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x10);
}



//=== ?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ @ 017f7425 ===

void _GetSampleBank_CThing__QBEAAVCASoundBank__XZ(void)

{
  _PeekComponent_CThing__QBEABVCMainGameComponent__XZ();
  _GetSampleBank_CMainGameComponent__QBEAAVCASoundBank__XZ();
  return;
}



//=== ?GetPSoundPair@CSoundMap@@QBEPBVCSoundPair@@ABVCCharString@@@Z @ 017f7d6c ===

int _GetPSoundPair_CSoundMap__QBEPBVCSoundPair__ABVCCharString___Z(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_30 [15];
  char cStack_21;
  undefined4 uStack_20;
  undefined1 auStack_1c [12];
  void *pvStack_10;
  undefined1 *puStack_c;
  int iStack_8;
  
  iStack_8 = 0xffffffff;
  puStack_c = &LAB_03cc7a70;
  pvStack_10 = ExceptionList;
  ExceptionList = &pvStack_10;
  uStack_20 = _GetCRC_CCharString__QBEKXZ();
  _Find___CVectorMap_KVCSoundPair__V__CKeyPairCompareLess_KVCSoundPair______QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__ABK_Z
            (auStack_1c,&uStack_20);
  iStack_8 = 0;
  uVar1 = _end___vector_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std__QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___2_XZ
                    (auStack_30);
  iStack_8._0_1_ = 1;
  cStack_21 = __9___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QBE_NABV01__Z
                        (uVar1);
  iStack_8 = (uint)iStack_8._1_3_ << 8;
  __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
            ();
  if (cStack_21 == '\0') {
    iStack_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
              ();
    iVar2 = 0;
  }
  else {
    iVar2 = __C___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QBEPBU__pair_KVCSoundPair___1_XZ
                      ();
    iVar2 = iVar2 + 4;
    iStack_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
              ();
  }
  ExceptionList = pvStack_10;
  return iVar2;
}



//=== ?GetPCriteriaString@CSoundMap@@QBEPBVCDefString@@ABVCCharString@@@Z @ 01816e47 ===

int _GetPCriteriaString_CSoundMap__QBEPBVCDefString__ABVCCharString___Z(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_30 [15];
  char cStack_21;
  undefined4 uStack_20;
  undefined1 auStack_1c [12];
  void *pvStack_10;
  undefined1 *puStack_c;
  int iStack_8;
  
  iStack_8 = 0xffffffff;
  puStack_c = &LAB_03cc6cd0;
  pvStack_10 = ExceptionList;
  ExceptionList = &pvStack_10;
  uStack_20 = _GetCRC_CCharString__QBEKXZ();
  _Find___CVectorMap_KVCDefString__V__CKeyPairCompareLess_KVCDefString______QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__ABK_Z
            (auStack_1c,&uStack_20);
  iStack_8 = 0;
  uVar1 = _end___vector_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std__QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___2_XZ
                    (auStack_30);
  iStack_8._0_1_ = 1;
  cStack_21 = __9___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QBE_NABV01__Z
                        (uVar1);
  iStack_8 = (uint)iStack_8._1_3_ << 8;
  __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
            ();
  if (cStack_21 == '\0') {
    iStack_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
              ();
    iVar2 = 0;
  }
  else {
    iVar2 = __C___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QBEPBU__pair_KVCDefString___1_XZ
                      ();
    iVar2 = iVar2 + 4;
    iStack_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
              ();
  }
  ExceptionList = pvStack_10;
  return iVar2;
}



//=== ?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z @ 01823133 ===

undefined4
_InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
          (void)

{
  return 0xffffffff;
}



//=== ?InternalPlaySoundEvent@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVCCharString@@_N22J@Z @ 01823a52 ===

undefined4 _InternalPlaySoundEvent_CTCSoundPlayer__AAEKAAVCASoundBank__ABVCCharString___N22J_Z(void)

{
  return 0xffffffff;
}



//=== ?PeekSampleBank@CMainGameComponent@@QBEABVCASoundBank@@XZ @ 0182d8bd ===

undefined4 _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x10);
}



//=== ?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z @ 0182fd16 ===

undefined4 _GetSoundIndexFromSymbol_CASoundBank__QBEJABVCCharString___Z(undefined4 param_1)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 uStack_8;
  
  uStack_8 = 0;
  cVar1 = _IsNull___CCountedPointer_VCCRCSymbolMap____QBE_NXZ();
  if (cVar1 == '\0') {
    puVar2 = &uStack_8;
    __C__CCountedPointer_VCCRCSymbolMap____QBEPAVCCRCSymbolMap__XZ(param_1,puVar2);
    _FindSymbol_CCRCSymbolMap__QBE_NABVCCharString__AAJ_Z(param_1,puVar2);
  }
  return uStack_8;
}



//=== ?PlayCriteriaSoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 01851498 ===

undefined4
_PlayCriteriaSoundOnThing_CGameScriptInterface__UBEKABVCScriptThing__ABVCCharString___Z(void)

{
  return 0;
}



//=== ?GetSampleBank@CWorld@@QBEAAVCASoundBank@@XZ @ 01b502f0 ===

/* [ported from ego_r via strfp] */

void _GetSampleBank_CWorld__QBEAAVCASoundBank__XZ(void)

{
  _GetSampleBank_CMainGameComponent__QBEAAVCASoundBank__XZ();
  return;
}



//=== ?GetSampleBank@CMainGameComponent@@QBEAAVCASoundBank@@XZ @ 01b50310 ===

/* [ported from ego_r via strfp] */

undefined4 _GetSampleBank_CMainGameComponent__QBEAAVCASoundBank__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x10);
}



//=== ?PeekSampleBank@CWorld@@QBEABVCASoundBank@@XZ @ 01b50330 ===

/* [ported from ego_r via strfp] */

void _PeekSampleBank_CWorld__QBEABVCASoundBank__XZ(void)

{
  _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ();
  return;
}



//=== ?PeekSampleBank@CMainGameComponent@@QBEABVCASoundBank@@XZ @ 01b50350 ===

/* [ported from ego_r via strfp] */

undefined4 _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x10);
}



//=== ?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ @ 01cf6380 ===

/* [ported from ego_r via strfp] */

void _GetSampleBank_CThing__QBEAAVCASoundBank__XZ(void)

{
  _PeekComponent_CThing__QBEABVCMainGameComponent__XZ();
  _GetSampleBank_CMainGameComponent__QBEAAVCASoundBank__XZ();
  return;
}



//=== ?PeekSampleBank@CThing@@IBEABVCASoundBank@@XZ @ 01cf63a0 ===

/* [ported from ego_r via strfp] */

void _PeekSampleBank_CThing__IBEABVCASoundBank__XZ(void)

{
  _PeekComponent_CThing__QBEABVCMainGameComponent__XZ();
  _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ();
  return;
}



//=== ?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z @ 01e7e0a0 ===

/* [ported from ego_r via strfp] */

void _PlayCollisionSound_CTCSoundPlayer__QAEXABVC3DVector__ABV__CDefPointer___CBVCMaterialDef____1_Z
               (undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  int *piVar5;
  undefined1 *puVar6;
  undefined *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined1 local_d4 [8];
  undefined1 local_cc [12];
  undefined1 local_c0 [8];
  undefined1 local_b8 [7];
  char local_b1;
  undefined1 local_98 [8];
  undefined1 local_90 [4];
  undefined1 local_8c [8];
  undefined1 local_84 [7];
  char local_7d;
  undefined1 local_74 [8];
  undefined1 local_6c [4];
  undefined1 local_68 [8];
  undefined1 local_60 [8];
  undefined1 local_58 [11];
  char local_4d;
  undefined1 local_44 [8];
  undefined1 local_3c [4];
  undefined1 local_38 [8];
  undefined1 local_30 [8];
  undefined1 local_28 [11];
  char local_1d;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cafb8b;
  local_10 = ExceptionList;
  if (DAT_04a4cbdf != '\0') {
    ExceptionList = &local_10;
    _GetParentThing_CTCBase__QAEAAVCThing__XZ();
    local_14 = _GetSampleBank_CThing__QBEAAVCASoundBank__XZ();
    __0CCharString__QAE_PBDJ_Z("SE_COLLISION;",0xffffffff);
    local_8 = 0;
    cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
    if ((cVar2 != '\0') &&
       (cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ(), cVar2 != '\0')) {
      local_8 = 0xffffffff;
      __1CCharString__QAE_XZ();
      ExceptionList = local_10;
      return;
    }
    cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
    if (cVar2 == '\0') {
      cVar2 = _IsNull___CDefPointer___CBVCMaterialDef____QBE_NXZ();
      if (cVar2 == '\0') {
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = local_84;
        (**(code **)(*piVar5 + 0x28))(puVar9,local_8c);
        GetObject(puVar9);
        local_8._0_1_ = 0xb;
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = local_98;
        (**(code **)(*piVar5 + 0x28))(local_90,puVar9);
        GetObject(puVar9);
        local_8._0_1_ = 0xc;
        uVar11 = 0;
        uVar12 = 0;
        puVar9 = local_8c;
        puVar7 = &DAT_041172a8;
        __H_YI_AVCCharString__ABV0_0_Z(local_98);
        local_8._0_1_ = 0xd;
        __H_YI_AVCCharString__ABV0_PBD_Z(puVar7);
        local_8._0_1_ = 0xe;
        uVar10 = __H_YI_AVCCharString__ABV0_0_Z(puVar9);
        local_8._0_1_ = 0xf;
        iVar3 = _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                          (local_14,param_1,uVar10,uVar12,uVar11);
        local_7d = iVar3 == -1;
        local_8._0_1_ = 0xe;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0xd;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0xc;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0xb;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0;
        __1CCharString__QAE_XZ();
        if (local_7d != '\0') {
          piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
          puVar9 = local_c0;
          (**(code **)(*piVar5 + 0x28))(local_b8,puVar9);
          GetObject(puVar9);
          local_8._0_1_ = 0x10;
          uVar11 = 0;
          uVar12 = 0;
          uVar10 = __H_YI_AVCCharString__ABV0_0_Z(local_c0);
          local_8._0_1_ = 0x11;
          iVar3 = _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                            (local_14,param_1,uVar10,uVar12,uVar11);
          local_b1 = iVar3 == -1;
          local_8._0_1_ = 0x10;
          __1CCharString__QAE_XZ();
          local_8._0_1_ = 0;
          __1CCharString__QAE_XZ();
          if (local_b1 != '\0') {
            piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
            puVar9 = local_d4;
            (**(code **)(*piVar5 + 0x28))(local_cc,puVar9);
            GetObject(puVar9);
            local_8._0_1_ = 0x12;
            uVar11 = 0;
            uVar12 = 0;
            uVar10 = __H_YI_AVCCharString__ABV0_0_Z(local_d4);
            local_8._0_1_ = 0x13;
            _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                      (local_14,param_1,uVar10,uVar12,uVar11);
            local_8._0_1_ = 0x12;
            __1CCharString__QAE_XZ();
            local_8 = (uint)local_8._1_3_ << 8;
            __1CCharString__QAE_XZ();
          }
        }
      }
      else {
        iVar3 = _Get___CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        if (iVar3 == 0) {
          __0CWideString__QAE_PB_W_Z(L"tc_sound_player.cpp");
          local_8._0_1_ = 6;
          __0CCharString__QAE_PBDJ_Z(&DAT_0411722f,0xffffffff);
          local_8._0_1_ = 7;
          __0CCharString__QAE_PBDJ_Z("pmaterial1.Get()",0xffffffff);
          local_8._0_1_ = 8;
          uVar12 = 1;
          uVar10 = 0x5e;
          puVar9 = local_58;
          puVar8 = local_60;
          puVar6 = local_68;
          _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar8,puVar9,0x5e,1);
          _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
          cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                            (puVar6,puVar8,puVar9,uVar10,uVar12);
          local_4d = '\x01' - (cVar2 != '\x01');
          local_8._0_1_ = 7;
          __1CCharString__QAE_XZ();
          local_8._0_1_ = 6;
          __1CCharString__QAE_XZ();
          local_8 = (uint)local_8._1_3_ << 8;
          __1CWideString__QAE_XZ();
          if ((local_4d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
            pcVar1 = (code *)swi(3);
            (*pcVar1)();
            return;
          }
        }
        piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
        puVar9 = local_6c;
        (**(code **)(*piVar5 + 0x28))(puVar9,local_74);
        GetObject(puVar9);
        local_8._0_1_ = 9;
        uVar11 = 0;
        uVar12 = 0;
        uVar10 = __H_YI_AVCCharString__ABV0_0_Z(local_74);
        local_8._0_1_ = 10;
        _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                  (local_14,param_1,uVar10,uVar12,uVar11);
        local_8._0_1_ = 9;
        __1CCharString__QAE_XZ();
        local_8 = (uint)local_8._1_3_ << 8;
        __1CCharString__QAE_XZ();
      }
    }
    else {
      iVar3 = _Get___CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
      if (iVar3 == 0) {
        __0CWideString__QAE_PB_W_Z(L"tc_sound_player.cpp");
        local_8._0_1_ = 1;
        __0CCharString__QAE_PBDJ_Z(&DAT_0411722e,0xffffffff);
        local_8._0_1_ = 2;
        __0CCharString__QAE_PBDJ_Z("pmaterial2.Get()",0xffffffff);
        local_8._0_1_ = 3;
        uVar12 = 1;
        uVar10 = 0x59;
        puVar9 = local_28;
        puVar8 = local_30;
        puVar6 = local_38;
        _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar6,puVar8,puVar9,0x59,1);
        _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
        cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                          (puVar6,puVar8,puVar9,uVar10,uVar12);
        local_1d = '\x01' - (cVar2 != '\x01');
        local_8._0_1_ = 2;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 1;
        __1CCharString__QAE_XZ();
        local_8 = (uint)local_8._1_3_ << 8;
        __1CWideString__QAE_XZ();
        if ((local_1d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
      piVar5 = (int *)__C__CDefPointer___CBVCMaterialDef____QBEPBVCMaterialDef__XZ();
      puVar9 = local_3c;
      (**(code **)(*piVar5 + 0x28))(puVar9,local_44);
      GetObject(puVar9);
      local_8._0_1_ = 4;
      uVar11 = 0;
      uVar12 = 0;
      uVar10 = __H_YI_AVCCharString__ABV0_0_Z(local_44);
      local_8._0_1_ = 5;
      _InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
                (local_14,param_1,uVar10,uVar12,uVar11);
      local_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      local_8 = (uint)local_8._1_3_ << 8;
      __1CCharString__QAE_XZ();
    }
    local_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
  }
  ExceptionList = local_10;
  return;
}



//=== ?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z @ 01e80320 ===

/* [ported from ego_r via strfp] */

undefined4
_InternalPlaySoundEventAtPos_CTCSoundPlayer__AAEKAAVCASoundBank__ABVC3DVector__ABVCCharString___N3_Z
          (void)

{
  return 0xffffffff;
}



//=== ?InternalPlaySoundEvent@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVCCharString@@_N22J@Z @ 01e80340 ===

/* [ported from ego_r via strfp] */

undefined4 _InternalPlaySoundEvent_CTCSoundPlayer__AAEKAAVCASoundBank__ABVCCharString___N22J_Z(void)

{
  return 0xffffffff;
}



//=== ?GetPCriteriaString@CSoundMap@@QBEPBVCDefString@@ABVCCharString@@@Z @ 01f519f0 ===

/* [ported from ego_r via strfp] */

int _GetPCriteriaString_CSoundMap__QBEPBVCDefString__ABVCCharString___Z(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_30 [15];
  char local_21;
  undefined4 local_20;
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cc6cd0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_20 = _GetCRC_CCharString__QBEKXZ();
  _Find___CVectorMap_KVCDefString__V__CKeyPairCompareLess_KVCDefString______QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__ABK_Z
            (local_1c,&local_20);
  local_8 = 0;
  uVar1 = _end___vector_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std__QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___2_XZ
                    (local_30);
  local_8._0_1_ = 1;
  local_21 = __9___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QBE_NABV01__Z
                       (uVar1);
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
            ();
  if (local_21 == '\0') {
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
              ();
    iVar2 = 0;
  }
  else {
    iVar2 = __C___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QBEPBU__pair_KVCDefString___1_XZ
                      ();
    iVar2 = iVar2 + 4;
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCDefString___std__V__allocator_U__pair_KVCDefString___std___2__std___std__QAE_XZ
              ();
  }
  ExceptionList = local_10;
  return iVar2;
}



//=== ?GetPSoundPair@CSoundMap@@QBEPBVCSoundPair@@ABVCCharString@@@Z @ 01f57cc0 ===

/* [ported from ego_r via strfp] */

int _GetPSoundPair_CSoundMap__QBEPBVCSoundPair__ABVCCharString___Z(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 local_30 [15];
  char local_21;
  undefined4 local_20;
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03cc7a70;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_20 = _GetCRC_CCharString__QBEKXZ();
  _Find___CVectorMap_KVCSoundPair__V__CKeyPairCompareLess_KVCSoundPair______QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__ABK_Z
            (local_1c,&local_20);
  local_8 = 0;
  uVar1 = _end___vector_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std__QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___2_XZ
                    (local_30);
  local_8._0_1_ = 1;
  local_21 = __9___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QBE_NABV01__Z
                       (uVar1);
  local_8 = (uint)local_8._1_3_ << 8;
  __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
            ();
  if (local_21 == '\0') {
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
              ();
    iVar2 = 0;
  }
  else {
    iVar2 = __C___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QBEPBU__pair_KVCSoundPair___1_XZ
                      ();
    iVar2 = iVar2 + 4;
    local_8 = 0xffffffff;
    __1___Vector_const_iterator_V___Vector_val_U__pair_KVCSoundPair___std__V__allocator_U__pair_KVCSoundPair___std___2__std___std__QAE_XZ
              ();
  }
  ExceptionList = local_10;
  return iVar2;
}



//=== ?PeekSampleBank@CTCBase@@IBEABVCASoundBank@@XZ @ 02191580 ===

/* [ported from ego_r via strfp] */

void _PeekSampleBank_CTCBase__IBEABVCASoundBank__XZ(void)

{
  _GetThingManager_CTCBase__IBEABVCThingManager__XZ();
  _PeekComponent_CThingManager__QBEABVCMainGameComponent__XZ();
  _PeekSampleBank_CMainGameComponent__QBEABVCASoundBank__XZ();
  return;
}



//=== ?PlayCriteriaSoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 02aacb40 ===

/* [ported from ego_r via strfp] */

undefined4
_PlayCriteriaSoundOnThing_CGameScriptInterface__UBEKABVCScriptThing__ABVCCharString___Z(void)

{
  return 0;
}



//=== ?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z @ 031ded60 ===

/* [ported from ego_r via strfp] */

undefined4 _GetSoundIndexFromSymbol_CASoundBank__QBEJABVCCharString___Z(undefined4 param_1)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 local_8;
  
  local_8 = 0;
  cVar1 = _IsNull___CCountedPointer_VCCRCSymbolMap____QBE_NXZ();
  if (cVar1 == '\0') {
    puVar2 = &local_8;
    __C__CCountedPointer_VCCRCSymbolMap____QBEPAVCCRCSymbolMap__XZ(param_1,puVar2);
    _FindSymbol_CCRCSymbolMap__QBE_NABVCCharString__AAJ_Z(param_1,puVar2);
  }
  return local_8;
}



