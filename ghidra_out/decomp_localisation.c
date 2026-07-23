//=== GetTextBySymbol @ 0041cfb2 ===

/* [bsim sim=0.9999999999999999 <- ego_r] */

void GetTextBySymbol(CIsThingAlive *param_1)

{
  char cVar1;
  CWideString *unaff_EDI;
  CCharString local_8 [4];
  
  if (DAT_013b86a0 == 0) {
LAB_0041cfe9:
    if (DAT_013b871c != 0) {
      NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b871c + 0x60),local_8);
      cVar1 = FUN_0099b0f0(&DAT_013bca24);
      if (cVar1 != '\0') goto LAB_0041d00e;
      CCharString::~CCharString(local_8);
    }
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)unaff_EDI,param_1);
  }
  else {
    NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b86a0 + 0x14),local_8);
    cVar1 = FUN_0099b0f0(&DAT_013bca24);
    if (cVar1 == '\0') {
      CCharString::~CCharString(local_8);
      goto LAB_0041cfe9;
    }
LAB_0041d00e:
    CWideString::CWideString(unaff_EDI,(CWideString *)local_8);
    CCharString::~CCharString(local_8);
  }
  return;
}



//=== GetTextWidthFromText @ 004372a0 ===

/* [bsim sim=0.6863702930585719 <- ego_r]
   private: float __thiscall CSubtitleRenderer::GetTextWidthFromText(class CWideString const &,class
   CFontBank const &)const  */

float __thiscall
CSubtitleRenderer::GetTextWidthFromText
          (CSubtitleRenderer *this,CWideString *param_1,CFontBank *param_2)

{
  float10 fVar1;
  undefined1 local_8 [8];
  
  fVar1 = (float10)CTexture::GetDimensions((CTexture *)this,(ulong)local_8);
  return (float)fVar1;
}



//=== GetText @ 004380a0 ===

/* [bsim sim=0.9337528896718994 <- ego_r]
   public: class CWideString __thiscall NPlayerGui::CScreenMessage::GetText(void)const  */

CWideString * __thiscall NPlayerGui::CScreenMessage::GetText(CScreenMessage *this)

{
  int *piVar1;
  char *_Str1;
  int iVar2;
  CCharString *this_00;
  CWideString *pCVar3;
  long lVar4;
  char *_Str2;
  size_t _MaxCount;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CWideString *in_stack_00000004;
  undefined4 **ppuVar5;
  ulong uVar6;
  CWideString local_10 [4];
  undefined4 *local_c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_4 [4];
  
  CWideString::CWideString(local_10,(CWideString *)this);
  CCharString::CCharString((CCharString *)&local_c,"(HC)",-1);
  piVar1 = (int *)FUN_0099b2c0(local_8);
  if (local_c == (undefined4 *)0x0) {
    _MaxCount = 0;
    _Str2 = "";
  }
  else {
    _MaxCount = local_c[1];
    _Str2 = (char *)*local_c;
  }
  if ((undefined4 *)*piVar1 == (undefined4 *)0x0) {
    _Str1 = "";
  }
  else {
    _Str1 = *(char **)*piVar1;
  }
  iVar2 = strncmp(_Str1,_Str2,_MaxCount);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_8,extraout_EDX,unaff_EBX);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_c,
             extraout_EDX_00,unaff_EBX);
  if (iVar2 == 0) {
    CCharString::GetLength((CCharString *)this);
    uVar6 = 4;
    ppuVar5 = &local_c;
    this_00 = (CCharString *)FUN_0099b2c0(local_4);
    CCharString::GetSubString(this_00,(ulong)ppuVar5,uVar6);
    pCVar3 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
    CWideString::operator=(local_10,pCVar3);
    CCharString::~CCharString((CCharString *)local_8);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_c,
               extraout_EDX_01,unaff_ESI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (local_4,extraout_EDX_02,unaff_ESI);
  }
  iVar2 = *(int *)(this + 8);
  lVar4 = CCharString::GetLength((CCharString *)local_10);
  if (iVar2 < lVar4) {
    CWideString::SetLength(local_10,iVar2);
  }
  CWideString::CWideString(in_stack_00000004,local_10);
  CCharString::~CCharString((CCharString *)local_10);
  return in_stack_00000004;
}



//=== FinishWithDynamicTextures @ 00441380 ===

/* [bsim sim=0.895339124521449 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    free(*(void **)(this + 0x1c));
  }
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00443140 ===

/* [bsim sim=0.5711667741706364 <- donor]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  void *_Memory;
  undefined4 *puVar1;
  list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *unaff_ESI;
  undefined4 *puVar2;
  
  CGuiControlTreePane::SortTreeRecursively((CGuiControlTreePane *)(this + 0x28),unaff_ESI);
  _Memory = *(void **)(this + 0x28);
  if (_Memory != (void *)0x0) {
    free(_Memory);
  }
  puVar1 = *(undefined4 **)(this + 0x20);
  for (puVar2 = *(undefined4 **)(this + 0x1c); puVar2 != puVar1; puVar2 = puVar2 + 2) {
    (**(code **)*puVar2)(0);
  }
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    free(*(void **)(this + 0x1c));
  }
  return;
}



//=== ?GetSizeofClass@CTextureReplacementDef@@UAEKXZ @ 00454690 ===

undefined4 _GetSizeofClass_CTextureReplacementDef__UAEKXZ(void)

{
  return 0x34;
}



//=== ?Copy@CTextureReplacementDef@@UAEXPBVCDefClassBase@@@Z @ 00456244 ===

void _Copy_CTextureReplacementDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  FUN_00456263(param_1 + 0x28);
  return;
}



//=== ?Transfer@CTextureReplacementDef@@UAEXAAVCPersistContext@@@Z @ 00469c2e ===

void _Transfer_CTextureReplacementDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== FinishWithDynamicTextures @ 0048d7c0 ===

/* [bsim sim=0.5699519256397553 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  CEngineScreenEffectDisplacementRenderer *pCVar1;
  undefined4 uStack_4;
  
  pCVar1 = this + 0x10;
  uStack_4 = this;
  if (*(int *)(this + 0x14) != 0) {
    FUN_0047c6a0(*(undefined4 *)(*(int *)pCVar1 + 4));
    *(int *)(*(int *)pCVar1 + 8) = *(int *)pCVar1;
    *(undefined4 *)(*(int *)pCVar1 + 4) = 0;
    *(int *)(*(int *)pCVar1 + 0xc) = *(int *)pCVar1;
    *(undefined4 *)(this + 0x14) = 0;
  }
  if (*(void **)pCVar1 != (void *)0x0) {
    free(*(void **)pCVar1);
  }
  std::
  _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
            (*(undefined4 *)this,*(undefined4 *)(this + 4),(int)&uStack_4 + 3);
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== GetTextOffsetFromEmbeddedTagGraphic @ 0048dc3c ===

/* [bsim sim=1.0 <- ego_r]
   public: class C2DVector const & __thiscall
   CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long)const  */

C2DVector * __thiscall
CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(CSubtitleRenderer *this,long param_1)

{
  CPlayerGuiDef *pCVar1;
  
  pCVar1 = CPlayerGui::PeekGuiDef();
  if ((param_1 != *(int *)(pCVar1 + 0x66c)) && (param_1 != *(int *)(pCVar1 + 0x670))) {
    if ((param_1 == *(int *)(pCVar1 + 0x674)) || (param_1 == *(int *)(pCVar1 + 0x678))) {
      return (C2DVector *)(pCVar1 + 0x6a4);
    }
    if ((param_1 == *(int *)(pCVar1 + 0x684)) || (param_1 == *(int *)(pCVar1 + 0x688))) {
      return (C2DVector *)(pCVar1 + 0x6ac);
    }
  }
  return (C2DVector *)(pCVar1 + 0x69c);
}



//=== GetTextFromEmbeddedTagGraphic @ 0048df77 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: class CWideString __thiscall CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(long)const
    */

long __thiscall
CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(CSubtitleRenderer *this,long param_1)

{
  char cVar1;
  CPlayerGuiDef *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar3;
  int in_stack_00000008;
  char *pcVar4;
  
  CCharString::CCharString((CCharString *)&stack0xfffffff8);
  pCVar2 = CPlayerGui::PeekGuiDef();
  if (in_stack_00000008 == *(int *)(pCVar2 + 0x66c)) {
LAB_0048dfb3:
    pcVar4 = "TEXT_GUI_CONTROLLER_LEFT";
  }
  else {
    if (in_stack_00000008 != *(int *)(pCVar2 + 0x670)) {
      if (in_stack_00000008 == *(int *)(pCVar2 + 0x674)) goto LAB_0048dfb3;
      if (in_stack_00000008 != *(int *)(pCVar2 + 0x678)) {
        if (in_stack_00000008 == *(int *)(pCVar2 + 0x684)) goto LAB_0048dfb3;
        if (in_stack_00000008 != *(int *)(pCVar2 + 0x688)) goto LAB_0048dfcf;
      }
    }
    pcVar4 = "TEXT_GUI_CONTROLLER_RIGHT";
  }
  CCharString::operator=((CCharString *)&stack0xfffffff8,pcVar4);
LAB_0048dfcf:
  cVar1 = FUN_0099e960(&DAT_0122d70e);
  if (cVar1 == '\0') {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
    ppVar3 = extraout_EDX_00;
  }
  else {
    NGameText::CDataBank::GetTextBySymbol
              (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)param_1);
    ppVar3 = extraout_EDX;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff8,
             ppVar3,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)this);
  return param_1;
}



//=== GetTextFromTextTag @ 0048f6bd ===

/* [bsim sim=0.9899037821860553 <- ego_r]
   private: bool __thiscall CSubtitleRenderer::GetTextFromTextTag(class CWideString const &,class
   CWideString &) */

bool __thiscall
CSubtitleRenderer::GetTextFromTextTag
          (CSubtitleRenderer *this,CWideString *param_1,CWideString *param_2)

{
  CBankFile *this_00;
  int iVar1;
  uint uVar2;
  bool bVar3;
  undefined4 uVar4;
  ulong uVar5;
  int *piVar6;
  CSubtitleRenderer *pCVar7;
  uint uVar8;
  undefined4 *puVar9;
  CWideString *pCVar10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar11;
  ulong uVar12;
  CSubtitleRenderer *local_8;
  
  this_00 = *(CBankFile **)(DAT_013b86a0 + 0x14);
  local_8 = this;
  FUN_0099b2c0(&local_8);
  uVar4 = FUN_00404310();
  uVar11 = FUN_009cc410(uVar4);
  uVar12 = (ulong)uVar11;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar11 >> 0x20),unaff_EDI);
  if (uVar12 == 0) {
LAB_0048f7ac:
    CWideString::operator=(param_2,param_1);
    bVar3 = false;
  }
  else {
    uVar5 = CBankFile::GetEntryDataType(this_00,uVar12);
    if (uVar5 != 0) {
      piVar6 = (int *)NGameText::CDataBank::GetPGroupEntry
                                ((CDataBank *)this_00,(ulong)&local_8,SUB81(uVar11,0));
      iVar1 = *piVar6;
      piVar6 = (int *)(iVar1 + 0x1c);
      if (local_8 != (CSubtitleRenderer *)0x0) {
        pCVar7 = local_8 + 4;
        *(int *)pCVar7 = *(int *)pCVar7 + -1;
        if (*(int *)pCVar7 == 0) {
          (**(code **)(*(int *)local_8 + 4))();
        }
        local_8 = (CSubtitleRenderer *)0x0;
      }
      CWideString::operator=((CWideString *)(this + 0x30),param_1);
      *(int *)(this + 0x2c) = *(int *)(this + 0x2c) + 1;
      uVar2 = *(uint *)(this + 0x2c);
      this[0x28] = (CSubtitleRenderer)0x0;
      uVar8 = *(int *)(iVar1 + 0x20) - *piVar6 >> 2;
      if (uVar8 <= uVar2) goto LAB_0048f7ac;
      if ((int)uVar2 < (int)(uVar8 - 1)) {
        this[0x28] = (CSubtitleRenderer)0x1;
      }
      uVar12 = *(ulong *)(*piVar6 + uVar2 * 4);
    }
    puVar9 = (undefined4 *)
             NGameText::CDataBank::GetPGroupEntry
                       ((CDataBank *)this_00,(ulong)&local_8,SUB41(uVar12,0));
    pCVar10 = (CWideString *)CBankFile::GetBankHandle((CBankFile *)*puVar9);
    CWideString::operator=(param_2,pCVar10);
    CCharString::~CCharString((CCharString *)&param_1);
    if (local_8 != (CSubtitleRenderer *)0x0) {
      pCVar7 = local_8 + 4;
      *(int *)pCVar7 = *(int *)pCVar7 + -1;
      if (*(int *)pCVar7 == 0) {
        (**(code **)(*(int *)local_8 + 4))();
      }
    }
    bVar3 = true;
  }
  return bVar3;
}



//=== FinishWithDynamicTextures @ 004ad8d0 ===

/* [bsim sim=0.5412657460630491 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  LTextTreeWalkThrough *this_00;
  byte in_stack_00000004;
  
  this_00 = (LTextTreeWalkThrough *)(this + 0x2c);
  if (*(int *)(this + 0x30) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    *(undefined4 *)(*(int *)this_00 + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 0x30) = 0;
  }
  if (*(void **)this_00 != (void *)0x0) {
    free(*(void **)this_00);
  }
  if ((in_stack_00000004 & 1) != 0) {
    operator_delete(this);
  }
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 004c2c70 ===

/* [bsim sim=0.7644746219403386 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if ((*(byte *)(param_1 + 8 + iVar4 * 0xc) & 1) != 0) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0xc);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0xc + iVar4 * 0xc);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<NGraphicAppearance::CMeshInfo*,int,NGraphicAppearance::CMeshInfo,CSortPrimaryMeshesFirst>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7);
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 004c2d10 ===

/* [bsim sim=0.7553268014594617 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if ((~*(byte *)(param_1 + 8 + iVar4 * 0xc) & 1) != 0) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0xc);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0xc + iVar4 * 0xc);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<NGraphicAppearance::CMeshInfo*,int,NGraphicAppearance::CMeshInfo,CSortPrimaryMeshesLast>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7);
  return;
}



//=== FinishWithDynamicTextures @ 004e25f1 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x20) != (void *)0x0) {
    free(*(void **)(this + 0x20));
  }
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  return;
}



//=== CPreallocTexturePool @ 005191e0 ===

/* [bsim sim=0.552023186487085 <- ego_r]
   public: __thiscall CPreallocTexturePool::CPreallocTexturePool(void) */

CPreallocTexturePool * __thiscall
CPreallocTexturePool::CPreallocTexturePool(CPreallocTexturePool *this)

{
  undefined1 *puVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01244678;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  puVar1 = malloc(0x14);
  *(undefined1 **)(this + 0x20) = puVar1;
  *(undefined4 *)(this + 0x24) = 0;
  *puVar1 = 0;
  *(undefined4 *)(*(int *)(this + 0x20) + 4) = 0;
  *(int *)(*(int *)(this + 0x20) + 8) = *(int *)(this + 0x20);
  *(int *)(*(int *)(this + 0x20) + 0xc) = *(int *)(this + 0x20);
  return this;
}



//=== FinishWithDynamicTextures @ 0051d530 ===

/* [bsim sim=0.7723005965966644 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  undefined4 uStack_4;
  
  uStack_4 = this;
  if (*(void **)(this + 0x24) != (void *)0x0) {
    free(*(void **)(this + 0x24));
  }
  if (*(void **)(this + 0x18) != (void *)0x0) {
    free(*(void **)(this + 0x18));
  }
  if (*(void **)(this + 0xc) != (void *)0x0) {
    free(*(void **)(this + 0xc));
  }
  std::
  _Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std::allocator<unsigned_int>_>
            (*(undefined4 *)this,*(undefined4 *)(this + 4),(int)&uStack_4 + 3);
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 0052c4b0 ===

/* [bsim sim=0.5767131960236795 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  LTextTreeWalkThrough *this_00;
  undefined4 uStack_4;
  
  this_00 = (LTextTreeWalkThrough *)(this + 0x18);
  uStack_4 = this;
  if (*(int *)(this + 0x1c) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    *(undefined4 *)(*(int *)this_00 + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 0x1c) = 0;
  }
  if (*(void **)this_00 != (void *)0x0) {
    free(*(void **)this_00);
  }
  std::_Copy_backward<CPixelFormat*,CPixelFormat*>
            (*(CPixelFormat **)(this + 0xc),*(CPixelFormat **)(this + 0x10),
             (CPixelFormat *)((int)&uStack_4 + 3));
  if (*(void **)(this + 0xc) != (void *)0x0) {
    free(*(void **)(this + 0xc));
  }
  std::_Copy_backward<CPixelFormat*,CPixelFormat*>
            (*(CPixelFormat **)this,*(CPixelFormat **)(this + 4),
             (CPixelFormat *)((int)&uStack_4 + 3));
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== GetTextComponent @ 005402c0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual class NUISystem::CComponent * __thiscall
   NUISystem::CEditBox::GetTextComponent(void) */

CComponent * __thiscall NUISystem::CEditBox::GetTextComponent(CEditBox *this)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  if ((*(int *)(this + 0xb4) - *(int *)(this + 0xb0) & 0xfffffff8U) != 0) {
    do {
      iVar1 = (**(code **)(**(int **)(*(int *)(this + 0xb0) + iVar2 * 8) + 0x104))();
      if (iVar1 == 6) {
        return *(CComponent **)(*(int *)(this + 0xb0) + iVar2 * 8);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 != *(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3);
  }
  return (CComponent *)0x0;
}



//=== ~CText @ 005424b0 ===

/* WARNING: Removing unreachable block (ram,0x00542661) */
/* WARNING: Removing unreachable block (ram,0x00542666) */
/* WARNING: Removing unreachable block (ram,0x0054266b) */
/* WARNING: Removing unreachable block (ram,0x005426b4) */
/* WARNING: Removing unreachable block (ram,0x005426b9) */
/* WARNING: Removing unreachable block (ram,0x005426be) */
/* [bsim sim=0.8253460311963253 <- ego_r]
   public: virtual __thiscall NUISystem::CText::~CText(void) */

void __thiscall NUISystem::CText::~CText(CText *this)

{
  int *piVar1;
  CBase *pCVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  ulong uVar6;
  CEngineInternalPrimitiveBase *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CThing *unaff_EBX;
  ulong unaff_EDI;
  undefined4 *puVar7;
  CCharString aCStack_8 [4];
  int *piStack_4;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__012470cc;
  *(undefined ***)(this + 4) = &PTR_LAB_012470a8;
  *(undefined ***)(this + 0x18) = &PTR_LAB_012470a0;
  piVar3 = *(int **)(this + 0x174);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(this + 0x170);
  }
  piVar3 = *(int **)(this + 0x174);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0x174) = 0;
  }
  pCVar2 = (CBase *)(this + 0x178);
  *(undefined4 *)(this + 0x174) = 0;
  piVar3 = *(int **)(this + 0x17c);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(pCVar2);
  }
  piVar3 = *(int **)(this + 0x17c);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0x17c) = 0;
  }
  *(undefined4 *)(this + 0x17c) = 0;
  std::_For_each<std::pair<long,CCharString>_const*,CAddDefNamesToComboBox>
            (&piStack_4,*(undefined4 *)(this + 0x180),*(undefined4 *)(this + 0x184),
             CEnginePrimitiveHandle::Clear);
  piVar3 = *(int **)(this + 0x168);
  if (piVar3 != (int *)0x0) {
    *piVar3 = *piVar3 + -1;
    if (**(int **)(this + 0x168) == 0) {
      (*(code *)(*(int **)(this + 0x168))[1])();
      operator_delete(*(void **)(this + 0x168));
    }
  }
  *(undefined4 *)(this + 0x168) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  NEntityEvents::CEventKilledCreature::CEventKilledCreature
            ((CEventKilledCreature *)this,(CThing *)&piStack_4,unaff_EBX);
  CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)aCStack_8);
  uVar6 = CEngineInternalPrimitiveBase::AddChildPrimitive
                    (this_00,(CEnginePrimitive *)unaff_EBX,unaff_EDI);
  CFontManager::ReleaseFont(*(CFontManager **)(uVar6 + 0x84),aCStack_8);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_8,extraout_EDX
             ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EBX);
  if ((piStack_4 != (int *)0x0) &&
     (iVar4 = piStack_4[1], piStack_4[1] = iVar4 + -1, iVar4 + -1 == 0)) {
    (**(code **)(*piStack_4 + 4))();
  }
  if (*(void **)(this + 0x18c) != (void *)0x0) {
    free(*(void **)(this + 0x18c));
  }
  puVar5 = *(undefined4 **)(this + 0x184);
  for (puVar7 = *(undefined4 **)(this + 0x180); puVar7 != puVar5; puVar7 = puVar7 + 2) {
    (**(code **)*puVar7)(0);
  }
  if (*(void **)(this + 0x180) != (void *)0x0) {
    free(*(void **)(this + 0x180));
  }
  *(undefined ***)pCVar2 = &PTR__scalar_deleting_destructor__0122f598;
  piVar3 = *(int **)(this + 0x17c);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(pCVar2);
  }
  piVar3 = *(int **)(this + 0x17c);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0x17c) = 0;
  }
  *(undefined4 *)(this + 0x17c) = 0;
  NHeroInformationScreens::CBase::CBase(pCVar2);
  pCVar2 = (CBase *)(this + 0x170);
  *(undefined ***)pCVar2 = &PTR__scalar_deleting_destructor__0122f598;
  piVar3 = *(int **)(this + 0x174);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(pCVar2);
  }
  piVar3 = *(int **)(this + 0x174);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0x174) = 0;
  }
  *(undefined4 *)(this + 0x174) = 0;
  NHeroInformationScreens::CBase::CBase(pCVar2);
  piVar3 = *(int **)(this + 0x168);
  if (piVar3 != (int *)0x0) {
    *piVar3 = *piVar3 + -1;
    if (**(int **)(this + 0x168) == 0) {
      (*(code *)(*(int **)(this + 0x168))[1])();
      operator_delete(*(void **)(this + 0x168));
    }
  }
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  CCharString::~CCharString((CCharString *)(this + 0x160));
  CCharString::~CCharString((CCharString *)(this + 0x15c));
  CTCIdleScheduler::Clear((CTCIdleScheduler *)this);
  return;
}



//=== CTextContainer @ 00546d30 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall NUISystem::CTextContainer::CTextContainer(long) */

CTextContainer * __thiscall
NUISystem::CTextContainer::CTextContainer(CTextContainer *this,long param_1)

{
  CDraggableInto::CDraggableInto((CDraggableInto *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01248064;
  *(undefined ***)(this + 4) = &PTR_LAB_0124803c;
  *(undefined ***)(this + 0x18) = &PTR_LAB_01248034;
  GetDataFromDef(this);
  return this;
}



//=== ~CTextSlider @ 00549a00 ===

/* [bsim sim=0.672053984012374 <- ego_r]
   public: virtual __thiscall NUISystem::CTextSlider::~CTextSlider(void) */

void __thiscall NUISystem::CTextSlider::~CTextSlider(CTextSlider *this)

{
  list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
  *plVar1;
  CFrontEndManager *pCVar2;
  uint uVar3;
  undefined4 uStack_4;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__01248a8c;
  *(undefined ***)(this + 4) = &PTR_LAB_01248a68;
  *(undefined ***)(this + 0x18) = &PTR_LAB_01248a60;
  uStack_4 = this;
  if (*(int *)(this + 0x164) != *(int *)(this + 0x15c)) {
    uVar3 = 0;
    if (*(int *)(this + 0x170) - *(int *)(this + 0x16c) >> 3 != 0) {
      do {
        *(undefined4 *)
         (*(int *)(**(int **)(*(int *)(uVar3 * 8 + *(int *)(this + 0x16c)) + 4) + 8) + 8) =
             *(undefined4 *)(this + 0x164);
        pCVar2 = CFrontEndManager::GetInstance();
        (**(code **)(*(int *)pCVar2 + 0x38))(*(int *)(this + 0x16c) + uVar3 * 8);
        uVar3 = uVar3 + 1;
      } while (uVar3 < (uint)(*(int *)(this + 0x170) - *(int *)(this + 0x16c) >> 3));
    }
    this[0x168] = (CTextSlider)0x1;
  }
  plVar1 = *(list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
             **)(this + 0x178);
  if (plVar1 != (list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
                 *)0x0) {
    std::
    list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
    ::remove_if<NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>_>(plVar1);
    if (*(void **)plVar1 != (void *)0x0) {
      free(*(void **)plVar1);
    }
    operator_delete(plVar1);
  }
  plVar1 = *(list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
             **)(this + 0x17c);
  if (plVar1 != (list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
                 *)0x0) {
    std::
    list<CCountedPointer<NEntityEvents::CEventBase>,std::allocator<CCountedPointer<NEntityEvents::CEventBase>_>_>
    ::remove_if<NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>_>(plVar1);
    if (*(void **)plVar1 != (void *)0x0) {
      free(*(void **)plVar1);
    }
    operator_delete(plVar1);
  }
  std::
  _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
            (*(undefined4 *)(this + 0x16c),*(undefined4 *)(this + 0x170),(int)&uStack_4 + 3);
  if (*(void **)(this + 0x16c) != (void *)0x0) {
    free(*(void **)(this + 0x16c));
  }
  CTCIdleScheduler::Clear((CTCIdleScheduler *)this);
  return;
}



//=== CTextSlider @ 00549f60 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: __thiscall NUISystem::CTextSlider::CTextSlider(long) */

CTextSlider * __thiscall NUISystem::CTextSlider::CTextSlider(CTextSlider *this,long param_1)

{
  CDraggableInto::CDraggableInto((CDraggableInto *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01248a8c;
  *(undefined ***)(this + 4) = &PTR_LAB_01248a68;
  *(undefined ***)(this + 0x18) = &PTR_LAB_01248a60;
  *(undefined4 *)(this + 0x16c) = 0;
  *(undefined4 *)(this + 0x170) = 0;
  *(undefined4 *)(this + 0x174) = 0;
  *(undefined4 *)(this + 0x178) = 0;
  *(undefined4 *)(this + 0x17c) = 0;
  *(undefined4 *)(this + 0x184) = 0;
  *(undefined4 *)(this + 0x188) = 0;
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x198) = 0;
  *(undefined4 *)(this + 0x19c) = 0;
  FUN_00549b20();
  return this;
}



//=== CText @ 0054f5c0 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall NUISystem::CText::CText(long) */

CText * __thiscall NUISystem::CText::CText(CText *this,long param_1)

{
  CDraggableInto::CDraggableInto((CDraggableInto *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01249ccc;
  *(undefined ***)(this + 4) = &PTR_LAB_01249ca4;
  *(undefined ***)(this + 0x18) = &PTR_LAB_01249c9c;
  CCharString::CCharString((CCharString *)(this + 0x15c));
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x168));
  *(undefined ***)(this + 0x168) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(this + 0x16c) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x170));
  *(undefined ***)(this + 0x170) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(this + 0x174) = 0;
  this[0x18a] = (CText)0x0;
  GetDataFromDef(this);
  this[0x189] = (CText)0x0;
  return this;
}



//=== ~CText @ 0054f6e0 ===

/* WARNING: Removing unreachable block (ram,0x0054f82d) */
/* WARNING: Removing unreachable block (ram,0x0054f832) */
/* WARNING: Removing unreachable block (ram,0x0054f837) */
/* WARNING: Removing unreachable block (ram,0x0054f87a) */
/* WARNING: Removing unreachable block (ram,0x0054f87f) */
/* WARNING: Removing unreachable block (ram,0x0054f884) */
/* [bsim sim=0.8996427449111177 <- ego_r]
   public: virtual __thiscall NUISystem::CText::~CText(void) */

void __thiscall NUISystem::CText::~CText(CText *this)

{
  int *piVar1;
  CBase *this_00;
  CBase *this_01;
  int *piVar2;
  int iVar3;
  ulong uVar4;
  CEngineInternalPrimitiveBase *this_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CThing *unaff_ESI;
  ulong unaff_EDI;
  CCharString aCStack_8 [4];
  int *piStack_4;
  
  this_00 = (CBase *)(this + 0x168);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01249ccc;
  *(undefined ***)(this + 4) = &PTR_LAB_01249ca4;
  *(undefined ***)(this + 0x18) = &PTR_LAB_01249c9c;
  piVar2 = *(int **)(this + 0x16c);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_00);
  }
  piVar2 = *(int **)(this + 0x16c);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x16c) = 0;
  }
  this_01 = (CBase *)(this + 0x170);
  *(undefined4 *)(this + 0x16c) = 0;
  piVar2 = *(int **)(this + 0x174);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_01);
  }
  piVar2 = *(int **)(this + 0x174);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x174) = 0;
  }
  *(undefined4 *)(this + 0x174) = 0;
  piVar2 = *(int **)(this + 0x164);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + -1;
    if (**(int **)(this + 0x164) == 0) {
      (*(code *)(*(int **)(this + 0x164))[1])();
      operator_delete(*(void **)(this + 0x164));
    }
  }
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x160) = 0;
  NEntityEvents::CEventKilledCreature::CEventKilledCreature
            ((CEventKilledCreature *)this,(CThing *)&piStack_4,unaff_ESI);
  CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)aCStack_8);
  uVar4 = CEngineInternalPrimitiveBase::AddChildPrimitive
                    (this_02,(CEnginePrimitive *)unaff_ESI,unaff_EDI);
  CFontManager::ReleaseFont(*(CFontManager **)(uVar4 + 0x84),aCStack_8);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_8,extraout_EDX
             ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_ESI);
  if ((piStack_4 != (int *)0x0) &&
     (iVar3 = piStack_4[1], piStack_4[1] = iVar3 + -1, iVar3 + -1 == 0)) {
    (**(code **)(*piStack_4 + 4))();
  }
  *(undefined ***)this_01 = &PTR__scalar_deleting_destructor__0122f598;
  piVar2 = *(int **)(this + 0x174);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_01);
  }
  piVar2 = *(int **)(this + 0x174);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x174) = 0;
  }
  *(undefined4 *)(this + 0x174) = 0;
  NHeroInformationScreens::CBase::CBase(this_01);
  *(undefined ***)this_00 = &PTR__scalar_deleting_destructor__0122f598;
  piVar2 = *(int **)(this + 0x16c);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_00);
  }
  piVar2 = *(int **)(this + 0x16c);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x16c) = 0;
  }
  *(undefined4 *)(this + 0x16c) = 0;
  NHeroInformationScreens::CBase::CBase(this_00);
  piVar2 = *(int **)(this + 0x164);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + -1;
    if (**(int **)(this + 0x164) == 0) {
      (*(code *)(*(int **)(this + 0x164))[1])();
      operator_delete(*(void **)(this + 0x164));
    }
  }
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  CCharString::~CCharString((CCharString *)(this + 0x15c));
  CTCIdleScheduler::Clear((CTCIdleScheduler *)this);
  return;
}



//=== FinishWithDynamicTextures @ 00594fa9 ===

/* [bsim sim=0.5707458587279242 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  CVirtualKeyboard *this_00;
  char in_stack_00000004;
  
  this_00 = *(CVirtualKeyboard **)(this + 0x60);
  if (this_00 != (CVirtualKeyboard *)0x0) {
    CVirtualKeyboard::~CVirtualKeyboard(this_00);
    operator_delete(this_00);
    *(undefined4 *)(this + 0x60) = 0;
  }
  if (in_stack_00000004 != '\0') {
    NUISystem::CFrontEndManager::GotoPreviousScreen((CFrontEndManager *)this,false);
  }
  return;
}



//=== GetText @ 005e9e70 ===

/* [bsim sim=0.5401069498135852 <- ego_r]
   public: class CWideString __thiscall CTCBetting::GetText(unsigned long) */

ulong __thiscall CTCBetting::GetText(CTCBetting *this,ulong param_1)

{
  int iVar1;
  CWideString *pCVar2;
  bool bVar3;
  CCharString in_stack_00000008;
  CBankFile *local_4;
  
  local_4 = (CBankFile *)0x0;
  if ((DAT_013b8c4c & 1) == 0) {
    DAT_013b8c4c = DAT_013b8c4c | 1;
    FUN_0099b6b0(L"Missing text entry");
    _atexit((_func_4879 *)&LAB_01229160);
  }
  NGameText::CDataBank::GetPGroupEntry
            (*(CDataBank **)(DAT_013b86a0 + 0x14),(ulong)&local_4,(bool)in_stack_00000008);
  bVar3 = local_4 == (CBankFile *)0x0;
  if (bVar3) {
    pCVar2 = (CWideString *)&DAT_013b8c48;
  }
  else {
    pCVar2 = (CWideString *)CBankFile::GetBankHandle(local_4);
  }
  CWideString::CWideString((CWideString *)param_1,pCVar2);
  if (!bVar3) {
    CCharString::~CCharString(&stack0x00000008);
  }
  if ((local_4 != (CBankFile *)0x0) &&
     (iVar1 = *(int *)(local_4 + 4), *(int *)(local_4 + 4) = iVar1 + -1, iVar1 + -1 == 0)) {
    (**(code **)(*(int *)local_4 + 4))();
  }
  return param_1;
}



//=== GetTextWithNarrator @ 005eb3a0 ===

/* [bsim sim=0.5836586834153105 <- ego_r]
   public: class CWideString __thiscall CTCTavernGame::GetTextWithNarrator(unsigned long) */

ulong __thiscall CTCTavernGame::GetTextWithNarrator(CTCTavernGame *this,ulong param_1)

{
  CDataBank *this_00;
  CBaseObject *pCVar1;
  int *piVar2;
  CTextEntry *this_01;
  CWideString *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CTCTalk *this_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  ulong in_stack_00000008;
  bool local_5;
  ETCInterfaceType local_4;
  
  local_4 = 0;
  this_02 = (CTCTalk *)0x0;
  pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(this + 0xd0));
  if ((char)((uint)*(undefined4 *)(pCVar1 + 0x20) >> 8) < '\0') {
    local_4 = 0xf;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar1 + 0x44),&local_4);
    if ((piVar2 == *(int **)(pCVar1 + 0x48)) || (0xf < *piVar2)) {
      piVar2 = *(int **)(pCVar1 + 0x48);
    }
    this_02 = (CTCTalk *)piVar2[1];
  }
  CTCTalk::PlaySpeech(this_02,in_stack_00000008,0,true,true);
  if ((DAT_013b8c5c & 1) == 0) {
    DAT_013b8c5c = DAT_013b8c5c | 1;
    FUN_0099b6b0(L"Missing text entry");
    _atexit((_func_4879 *)&LAB_01229170);
  }
  this_00 = *(CDataBank **)(DAT_013b86a0 + 0x14);
  CCharString::CCharString((CCharString *)&stack0x00000008,(CCharString *)(this_02 + 0x38));
  this_01 = NGameText::CDataBank::GetTextEntryFromGroup
                      (this_00,in_stack_00000008,(CCharString *)&stack0x00000008,&local_5,
                       (long *)0x0,true);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000008,
             extraout_EDX,unaff_EDI);
  if (this_01 == (CTextEntry *)0x0) {
    pCVar3 = (CWideString *)&DAT_013b8c58;
  }
  else {
    pCVar3 = (CWideString *)CBankFile::GetBankHandle((CBankFile *)this_01);
  }
  CWideString::CWideString((CWideString *)param_1,pCVar3);
  if (this_01 != (CTextEntry *)0x0) {
    CCharString::~CCharString((CCharString *)&stack0x00000008);
  }
  return param_1;
}



//=== FinishWithDynamicTextures @ 00642a60 ===

/* [bsim sim=0.5794945033394789 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  if (*(void **)(this + 0x30) != (void *)0x0) {
    free(*(void **)(this + 0x30));
  }
  CTexture::Uninitialise((CTexture *)(this + 0x24));
  *(undefined ***)(this + 0x14) = &PTR__vector_deleting_destructor__0122f84c;
  CSurface::Uninitialise((CSurface *)(this + 0x14));
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00675b10 ===

/* [bsim sim=0.6459553571027007 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  undefined4 uStack_4;
  
  uStack_4 = this;
  if (*(void **)(this + 0x28) != (void *)0x0) {
    free(*(void **)(this + 0x28));
  }
  if (*(void **)(this + 0x18) != (void *)0x0) {
    free(*(void **)(this + 0x18));
  }
  FUN_006753b0();
  std::
  _Uninit_copy<std::_Vector_const_iterator<std::_Vector_val<unsigned_int,std::allocator<unsigned_int>_>_>,unsigned_int*,std::allocator<unsigned_int>_>
            (*(undefined4 *)this,*(undefined4 *)(this + 4),(int)&uStack_4 + 3);
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== CTextContainer @ 006aed80 ===

/* [bsim sim=0.5053976663219188 <- ego_r]
   public: __thiscall NUISystem::CTextContainer::CTextContainer(long) */

CTextContainer * __thiscall
NUISystem::CTextContainer::CTextContainer(CTextContainer *this,long param_1)

{
  void *pvVar1;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__scalar_deleting_destructor__0125d234;
  *(undefined4 *)(this + 0xc) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0xc) = pvVar1;
  return this;
}



//=== FinishWithDynamicTextures @ 006c6010 ===

/* [bsim sim=0.8274795949366568 <- ego_r]
   private: void __thiscall CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectRadialBlurRenderer *this)

{
  if (*(int *)(this + 0x28) != -1) {
    CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses
              ((CTCHeroOpinionDeedLog *)this,*(int *)(this + 0x28));
    FUN_006c59e0(*(undefined4 *)(this + 0x28));
    CTCHeroOpinionDeedLog::RemoveCrimeFromVillage
              ((CTCHeroOpinionDeedLog *)this,*(long *)(this + 0x28));
    *(undefined4 *)(this + 0x28) = 0xffffffff;
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 006f40f0 ===

/* [bsim sim=0.5808324194636016 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      FUN_006f20a0(param_3 + 0x10);
    }
    param_1 = param_1 + 0x1c;
  }
  return;
}



//=== FinishWithDynamicTextures @ 0070b800 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== FinishWithDynamicTextures @ 0070b830 ===

/* [bsim sim=0.6500918021870907 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  byte in_stack_00000004;
  
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  if ((in_stack_00000004 & 1) != 0) {
    operator_delete(this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 0070b870 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== _Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_> @ 007183e0 ===

/* [bsim sim=0.6882304618196631 <- ego_r]
   void __fastcall std::_Fill<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> const &) */

void __fastcall
std::
_Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_>
          (pair<EHeroMorphType,CTextureMorphEntry> *param_1,
          pair<EHeroMorphType,CTextureMorphEntry> *param_2,
          pair<enum_EHeroMorphType,class_CTextureMorphEntry> *param_3)

{
  for (; param_2 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
      *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 0x14);
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



//=== _Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_> @ 0071f710 ===

/* [bsim sim=0.7814929630641709 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,int,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>,class
   CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,int,int,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> &&,class CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry>)
    */

void __fastcall
std::
_Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
          (int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,int param_7,
          int param_8,int param_9)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    iVar3 = *(int *)(param_1 + iVar4 * 0x18);
    iVar1 = param_1 + iVar4 * 0x18;
    if (param_4 <= iVar3) break;
    piVar2 = (int *)(param_1 + param_2 * 0x18);
    *piVar2 = iVar3;
    piVar2[1] = *(int *)(iVar1 + 4);
    piVar2[2] = *(int *)(iVar1 + 8);
    piVar2[3] = *(int *)(iVar1 + 0xc);
    piVar2[4] = *(int *)(iVar1 + 0x10);
    piVar2[5] = *(int *)(iVar1 + 0x14);
    param_2 = iVar4;
  }
  piVar2 = (int *)(param_1 + param_2 * 0x18);
  *piVar2 = param_4;
  piVar2[1] = param_5;
  piVar2[2] = param_6;
  piVar2[3] = param_7;
  piVar2[4] = param_8;
  piVar2[5] = param_9;
  return;
}



//=== swap<CTCHeroMorph::CTextureLayer> @ 0071fdf0 ===

/* [bsim sim=0.8880965923552114 <- ego_r]
   void __fastcall std::swap<class CTCHeroMorph::CTextureLayer>(class CTCHeroMorph::CTextureLayer
   &,class CTCHeroMorph::CTextureLayer &) */

void __fastcall
std::swap<CTCHeroMorph::CTextureLayer>(CTextureLayer *param_1,CTextureLayer *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  uVar1 = *(undefined4 *)(param_1 + 4);
  uVar2 = *(undefined4 *)param_1;
  CCompositeBlendDesc::CCompositeBlendDesc
            ((CCompositeBlendDesc *)&local_10,(CCompositeBlendDesc *)(param_1 + 8));
  *(undefined4 *)param_1 = *(undefined4 *)param_2;
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_2 + 0xc);
  *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_2 + 0x14);
  *(undefined4 *)param_2 = uVar2;
  *(undefined4 *)(param_2 + 4) = uVar1;
  *(undefined4 *)(param_2 + 8) = local_10;
  *(undefined4 *)(param_2 + 0xc) = local_c;
  *(undefined4 *)(param_2 + 0x10) = local_8;
  *(undefined4 *)(param_2 + 0x14) = local_4;
  return;
}



//=== _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_> @ 0071ff00 ===

/* [bsim sim=0.8501437172028039 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,int,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>,class
   CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,int,int,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> &&,class CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry>)
    */

void __fastcall
std::
_Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 in_stack_00000020;
  CCompositeBlendDesc local_24 [16];
  undefined4 uStack_14;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if (*(int *)(param_1 + iVar4 * 0x18) < *(int *)(param_1 + -0x18 + iVar4 * 0x18)) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0x18);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0x18);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    puVar2[3] = puVar1[3];
    puVar2[4] = puVar1[4];
    puVar2[5] = puVar1[5];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0x18 + iVar4 * 0x18);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0x18);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    puVar1[3] = puVar2[3];
    puVar1[4] = puVar2[4];
    puVar1[5] = puVar2[5];
    iVar5 = iVar3 + 1;
  }
  uStack_14 = in_stack_00000020;
  CCompositeBlendDesc::CCompositeBlendDesc(local_24,(CCompositeBlendDesc *)&stack0x00000010);
  _Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
            (param_1,iVar5,param_2,param_4,param_5);
  return;
}



//=== _Make_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,int,std::pair<long,class_CTCHeroMorph::CTextureLayer>,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_> @ 00720710 ===

/* [bsim sim=0.5415758112652092 <- donor]
   void __fastcall std::_Make_heap<struct std::pair<long,class CTCHeroMorph::CTextureLayer>
   *,int,struct std::pair<long,class CTCHeroMorph::CTextureLayer>,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer> >(struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *,struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer>,int *,struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *) */

void __fastcall
std::
_Make_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,int,std::pair<long,class_CTCHeroMorph::CTextureLayer>,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_>
          (int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = (param_2 - param_1) / 0x18;
  if (1 < iVar1) {
    iVar3 = (iVar1 + -2) / 2;
    iVar2 = param_1 + 0x11 + iVar3 * 0x18;
    while( true ) {
      _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
                (param_1,iVar3,iVar1,*(undefined4 *)(iVar2 + -0x11),*(undefined4 *)(iVar2 + -0xd),
                 *(undefined4 *)(iVar2 + -9),*(undefined4 *)(iVar2 + -5),*(undefined1 *)(iVar2 + -1)
                 ,*(undefined1 *)(iVar2 + 3),param_3);
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      iVar2 = iVar2 + -0x18;
    }
  }
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00720d30 ===

/* [bsim sim=0.7569941204171718 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  if (1 < (param_2 - (int)param_1) / 0xc) {
    puVar4 = (undefined4 *)(param_2 + -0xc);
    do {
      uVar2 = puVar4[1];
      uVar1 = puVar4[2];
      uVar3 = *puVar4;
      *puVar4 = *param_1;
      puVar4[1] = param_1[1];
      puVar4[2] = param_1[2];
      _Adjust_heap<std::pair<CSkeletalMorphs::CEntry,float>*,int,std::pair<class_CSkeletalMorphs::CEntry,float>,CIsSkeletalMorphLowerPriorityThan>
                (param_1,0,(-(int)param_1 + (int)puVar4) / 0xc,uVar3,uVar2,uVar1,param_3);
      puVar4 = puVar4 + -3;
    } while (1 < (-(int)param_1 + 0xc + (int)puVar4) / 0xc);
  }
  return;
}



//=== _Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_> @ 007214a0 ===

/* [bsim sim=0.8593839224562976 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer> >(struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *,struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer>) */

void __fastcall
std::
_Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  
  if (1 < (param_2 - (int)param_1) / 0x18) {
    param_2 = param_2 + -7;
    do {
      uVar3 = *(undefined4 *)(param_2 + -5);
      uVar4 = *(undefined4 *)(param_2 + -0xd);
      uVar5 = *(undefined4 *)(param_2 + -0x11);
      uVar6 = *(undefined4 *)(param_2 + -9);
      uVar1 = *(undefined1 *)(param_2 + -1);
      uVar2 = *(undefined1 *)(param_2 + 3);
      *(undefined4 *)(param_2 + -0x11) = *param_1;
      *(undefined4 *)(param_2 + -0xd) = param_1[1];
      *(undefined4 *)(param_2 + -9) = param_1[2];
      *(undefined4 *)(param_2 + -5) = param_1[3];
      *(undefined4 *)(param_2 + -1) = param_1[4];
      *(undefined4 *)(param_2 + 3) = param_1[5];
      _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
                (param_1,0,((-0x11 - (int)param_1) + param_2) / 0x18,uVar5,uVar4,uVar6,uVar3,uVar1,
                 uVar2,param_3);
      param_2 = param_2 + -0x18;
    } while (1 < ((7 - (int)param_1) + param_2) / 0x18);
  }
  return;
}



//=== CTextContainer @ 00757640 ===

/* [bsim sim=0.561835441654661 <- ego_r]
   public: __thiscall NUISystem::CTextContainer::CTextContainer(long) */

CTextContainer * __thiscall
NUISystem::CTextContainer::CTextContainer(CTextContainer *this,long param_1)

{
  void *pvVar1;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__012698b4;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  pvVar1 = malloc(0x4c);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x10) = pvVar1;
  return this;
}



//=== FinishWithDynamicTextures @ 0075bee0 ===

/* [bsim sim=0.895339124521449 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  if (*(void **)(this + 0x1ac) != (void *)0x0) {
    free(*(void **)(this + 0x1ac));
  }
  if (*(void **)(this + 0xf8) != (void *)0x0) {
    free(*(void **)(this + 0xf8));
  }
  if (*(void **)(this + 0x44) != (void *)0x0) {
    free(*(void **)(this + 0x44));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00761bd0 ===

/* [bsim sim=0.692495528191472 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  *(undefined ***)this = &PTR__CPatchTesselationEdgeStrip_0126a208;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00762480 ===

/* [bsim sim=0.6859337640056924 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  undefined4 uStack_4;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126a31c;
  uStack_4 = this;
  std::
  _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
            (*(undefined4 *)(this + 0x20),*(undefined4 *)(this + 0x24),(int)&uStack_4 + 3);
  if (*(void **)(this + 0x20) != (void *)0x0) {
    free(*(void **)(this + 0x20));
  }
  *(undefined ***)this = &PTR__CPatchTesselationEdgeStrip_0126a208;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00764f70 ===

/* [bsim sim=0.692495528191472 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  *(undefined ***)this = &PTR__CPatchTesselationEdgeStrip_0126a208;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00765050 ===

/* [bsim sim=0.692495528191472 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  *(undefined ***)this = &PTR__CPatchTesselationEdgeStrip_0126a208;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  return;
}



//=== _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00769e20 ===

/* [bsim sim=0.7249339768732309 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,uint param_4,undefined4 param_5,uint param_6,
          uint param_7)

{
  int iVar1;
  uint *puVar2;
  uint *puVar3;
  
  while (param_3 < param_2) {
    iVar1 = (param_2 + -1) / 2;
    puVar3 = (uint *)(iVar1 * 0x10 + param_1);
    if (param_4 <= *puVar3) break;
    puVar2 = (uint *)(param_2 * 0x10 + param_1);
    *puVar2 = *puVar3;
    puVar2[2] = puVar3[2];
    puVar2[3] = puVar3[3];
    param_2 = iVar1;
  }
  puVar3 = (uint *)(param_2 * 0x10 + param_1);
  *puVar3 = param_4;
  puVar3[3] = param_7;
  puVar3[2] = param_6;
  NHeroInformationScreens::CBase::CBase((CBase *)&param_5);
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0076a4e0 ===

/* [bsim sim=0.7070488182205671 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = param_2;
  while( true ) {
    iVar5 = iVar4 * 2;
    iVar2 = iVar5 + 2;
    if (param_3 <= iVar2) break;
    if (*(uint *)(iVar2 * 0x10 + param_1) < *(uint *)(iVar2 * 0x10 + param_1 + -0x10)) {
      iVar2 = iVar5 + 1;
    }
    iVar5 = iVar2 * 0x10 + param_1;
    puVar3 = (undefined4 *)(iVar4 * 0x10 + param_1);
    *puVar3 = *(undefined4 *)(iVar2 * 0x10 + param_1);
    puVar3[2] = *(undefined4 *)(iVar5 + 8);
    puVar3[3] = *(undefined4 *)(iVar5 + 0xc);
    iVar4 = iVar2;
  }
  if (iVar2 == param_3) {
    iVar1 = iVar2 * 0x10 + -0x10 + param_1;
    puVar3 = (undefined4 *)(iVar4 * 0x10 + param_1);
    *puVar3 = *(undefined4 *)(iVar2 * 0x10 + -0x10 + param_1);
    puVar3[2] = *(undefined4 *)(iVar1 + 8);
    puVar3[3] = *(undefined4 *)(iVar1 + 0xc);
    iVar4 = iVar5 + 1;
  }
  _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
            (param_1,iVar4,param_2,param_4,&PTR__vector_deleting_destructor__0125fee4,param_6,
             param_7,param_8);
  NHeroInformationScreens::CBase::CBase((CBase *)&param_5);
  return;
}



//=== CTextContainer @ 00780070 ===

/* [bsim sim=0.5053976663219188 <- ego_r]
   public: __thiscall NUISystem::CTextContainer::CTextContainer(long) */

CTextContainer * __thiscall
NUISystem::CTextContainer::CTextContainer(CTextContainer *this,long param_1)

{
  void *pvVar1;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__scalar_deleting_destructor__0126b0f4;
  *(undefined4 *)(this + 0xc) = 0;
  pvVar1 = malloc(0x14);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0xc) = pvVar1;
  return this;
}



//=== GetText @ 007e4d70 ===

/* [bsim sim=0.5401069498135852 <- ego_r]
   public: class CWideString __thiscall CTCBetting::GetText(unsigned long) */

ulong __thiscall CTCBetting::GetText(CTCBetting *this,ulong param_1)

{
  int iVar1;
  CWideString *pCVar2;
  bool bVar3;
  CCharString in_stack_00000008;
  CBankFile *local_4;
  
  local_4 = (CBankFile *)0x0;
  if ((DAT_013bae88 & 1) == 0) {
    DAT_013bae88 = DAT_013bae88 | 1;
    FUN_0099b6b0(L"Missing text entry");
    _atexit((_func_4879 *)&LAB_01229dd0);
  }
  NGameText::CDataBank::GetPGroupEntry
            (*(CDataBank **)(DAT_013b86a0 + 0x14),(ulong)&local_4,(bool)in_stack_00000008);
  bVar3 = local_4 == (CBankFile *)0x0;
  if (bVar3) {
    pCVar2 = (CWideString *)&DAT_013bae84;
  }
  else {
    pCVar2 = (CWideString *)CBankFile::GetBankHandle(local_4);
  }
  CWideString::CWideString((CWideString *)param_1,pCVar2);
  if (!bVar3) {
    CCharString::~CCharString(&stack0x00000008);
  }
  if ((local_4 != (CBankFile *)0x0) &&
     (iVar1 = *(int *)(local_4 + 4), *(int *)(local_4 + 4) = iVar1 + -1, iVar1 + -1 == 0)) {
    (**(code **)(*(int *)local_4 + 4))();
  }
  return param_1;
}



//=== GetTextWithNarrator @ 007e5ff0 ===

/* [bsim sim=0.5587990941473137 <- ego_r]
   public: class CWideString __thiscall CTCOracleMinigame::GetTextWithNarrator(unsigned long) */

ulong __thiscall CTCOracleMinigame::GetTextWithNarrator(CTCOracleMinigame *this,ulong param_1)

{
  int iVar1;
  CDataBank *this_00;
  int *piVar2;
  CTextEntry *this_01;
  CWideString *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CTCTalk *this_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  ulong in_stack_00000008;
  ETCInterfaceType local_4;
  
  local_4 = 0;
  iVar1 = *(int *)(this + 0xe4);
  this_02 = (CTCTalk *)0x0;
  if ((char)((uint)*(undefined4 *)(iVar1 + 0x20) >> 8) < '\0') {
    local_4 = 0xf;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),&local_4);
    if ((piVar2 == *(int **)(iVar1 + 0x48)) || (0xf < *piVar2)) {
      piVar2 = *(int **)(iVar1 + 0x48);
    }
    this_02 = (CTCTalk *)piVar2[1];
  }
  CTCTalk::PlaySpeech(this_02,in_stack_00000008,0,true,true);
  if ((DAT_013bae90 & 1) == 0) {
    DAT_013bae90 = DAT_013bae90 | 1;
    FUN_0099b6b0(L"Missing text entry");
    _atexit((_func_4879 *)&LAB_01229de0);
  }
  this_00 = *(CDataBank **)(DAT_013b86a0 + 0x14);
  CCharString::CCharString((CCharString *)&local_4,(CCharString *)(this_02 + 0x38));
  this_01 = NGameText::CDataBank::GetTextEntryFromGroup
                      (this_00,in_stack_00000008,(CCharString *)&local_4,(bool *)&stack0x00000008,
                       (long *)0x0,true);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,extraout_EDX,
             unaff_EDI);
  if (this_01 == (CTextEntry *)0x0) {
    pCVar3 = (CWideString *)&DAT_013bae8c;
  }
  else {
    pCVar3 = (CWideString *)CBankFile::GetBankHandle((CBankFile *)this_01);
  }
  CWideString::CWideString((CWideString *)param_1,pCVar3);
  if (this_01 != (CTextEntry *)0x0) {
    CCharString::~CCharString((CCharString *)&local_4);
  }
  return param_1;
}



//=== GetText @ 007e7580 ===

/* [bsim sim=0.5401069498135852 <- ego_r]
   public: class CWideString __thiscall CTCBetting::GetText(unsigned long) */

ulong __thiscall CTCBetting::GetText(CTCBetting *this,ulong param_1)

{
  int iVar1;
  CWideString *pCVar2;
  bool bVar3;
  CCharString in_stack_00000008;
  CBankFile *local_4;
  
  local_4 = (CBankFile *)0x0;
  if ((DAT_013baea8 & 1) == 0) {
    DAT_013baea8 = DAT_013baea8 | 1;
    FUN_0099b6b0(L"Missing text entry");
    _atexit((_func_4879 *)&LAB_01229e10);
  }
  NGameText::CDataBank::GetPGroupEntry
            (*(CDataBank **)(DAT_013b86a0 + 0x14),(ulong)&local_4,(bool)in_stack_00000008);
  bVar3 = local_4 == (CBankFile *)0x0;
  if (bVar3) {
    pCVar2 = (CWideString *)&DAT_013baea4;
  }
  else {
    pCVar2 = (CWideString *)CBankFile::GetBankHandle(local_4);
  }
  CWideString::CWideString((CWideString *)param_1,pCVar2);
  if (!bVar3) {
    CCharString::~CCharString(&stack0x00000008);
  }
  if ((local_4 != (CBankFile *)0x0) &&
     (iVar1 = *(int *)(local_4 + 4), *(int *)(local_4 + 4) = iVar1 + -1, iVar1 + -1 == 0)) {
    (**(code **)(*(int *)local_4 + 4))();
  }
  return param_1;
}



//=== _Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_> @ 00819230 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall std::_Fill<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> const &) */

void __fastcall
std::
_Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_>
          (pair<EHeroMorphType,CTextureMorphEntry> *param_1,
          pair<EHeroMorphType,CTextureMorphEntry> *param_2,
          pair<enum_EHeroMorphType,class_CTextureMorphEntry> *param_3)

{
  for (; param_1 != param_2; param_1 = param_1 + 0x18) {
    *(undefined4 *)param_1 = *(undefined4 *)param_3;
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
    *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
    *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 0x14);
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 00819320 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_1 != param_2; param_1 = param_1 + 0x14) {
    *(undefined4 *)param_1 = *(undefined4 *)param_3;
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
    *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
    *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 008195e0 ===

/* [bsim sim=0.6737685012993061 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== _Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_> @ 00819b10 ===

/* [bsim sim=0.7814929630641709 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,int,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>,class
   CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,int,int,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> &&,class CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry>)
    */

void __fastcall
std::
_Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
          (int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,int param_7,
          int param_8,int param_9)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    iVar3 = *(int *)(param_1 + iVar4 * 0x18);
    iVar1 = param_1 + iVar4 * 0x18;
    if (param_4 <= iVar3) break;
    piVar2 = (int *)(param_1 + param_2 * 0x18);
    *piVar2 = iVar3;
    piVar2[1] = *(int *)(iVar1 + 4);
    piVar2[2] = *(int *)(iVar1 + 8);
    piVar2[3] = *(int *)(iVar1 + 0xc);
    piVar2[4] = *(int *)(iVar1 + 0x10);
    piVar2[5] = *(int *)(iVar1 + 0x14);
    param_2 = iVar4;
  }
  piVar2 = (int *)(param_1 + param_2 * 0x18);
  *piVar2 = param_4;
  piVar2[1] = param_5;
  piVar2[2] = param_6;
  piVar2[3] = param_7;
  piVar2[4] = param_8;
  piVar2[5] = param_9;
  return;
}



//=== _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00819c30 ===

/* [bsim sim=0.8009641593203382 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,uint param_4,uint param_5,uint param_6)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    uVar3 = *(uint *)(param_1 + iVar4 * 0xc);
    iVar1 = param_1 + iVar4 * 0xc;
    if (param_4 <= uVar3) break;
    puVar2 = (uint *)(param_1 + param_2 * 0xc);
    *puVar2 = uVar3;
    puVar2[1] = *(uint *)(iVar1 + 4);
    puVar2[2] = *(uint *)(iVar1 + 8);
    param_2 = iVar4;
  }
  puVar2 = (uint *)(param_1 + param_2 * 0xc);
  *puVar2 = param_4;
  puVar2[1] = param_5;
  puVar2[2] = param_6;
  return;
}



//=== _Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_> @ 0081a200 ===

/* [bsim sim=0.6882304618196631 <- ego_r]
   void __fastcall std::_Fill<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> const &) */

void __fastcall
std::
_Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_>
          (pair<EHeroMorphType,CTextureMorphEntry> *param_1,
          pair<EHeroMorphType,CTextureMorphEntry> *param_2,
          pair<enum_EHeroMorphType,class_CTextureMorphEntry> *param_3)

{
  for (; param_2 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
      *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 0x14);
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



//=== _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_> @ 0081aa50 ===

/* [bsim sim=0.8544802457777742 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,int,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>,class
   CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,int,int,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> &&,class CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry>)
    */

void __fastcall
std::
_Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
          undefined4 param_10)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if (*(int *)(param_1 + iVar4 * 0x18) < *(int *)(param_1 + -0x18 + iVar4 * 0x18)) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0x18);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0x18);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    puVar2[3] = puVar1[3];
    puVar2[4] = puVar1[4];
    puVar2[5] = puVar1[5];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0x18);
    puVar2 = (undefined4 *)(param_1 + -0x18 + iVar4 * 0x18);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    puVar1[3] = puVar2[3];
    puVar1[4] = puVar2[4];
    puVar1[5] = puVar2[5];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7,param_8,param_9,param_10);
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0081ac20 ===

/* [bsim sim=0.8590856811945937 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if (*(uint *)(param_1 + iVar4 * 0xc) < *(uint *)(param_1 + -0xc + iVar4 * 0xc)) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0xc);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0xc + iVar4 * 0xc);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7);
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0081c7d0 ===

/* [bsim sim=0.7569941204171718 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  if (1 < (param_2 - (int)param_1) / 0xc) {
    puVar4 = (undefined4 *)(param_2 + -0xc);
    do {
      uVar1 = puVar4[1];
      uVar2 = puVar4[2];
      uVar3 = *puVar4;
      *puVar4 = *param_1;
      puVar4[1] = param_1[1];
      puVar4[2] = param_1[2];
      _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                (param_1,0,(-(int)param_1 + (int)puVar4) / 0xc,uVar3,uVar1,uVar2,param_3);
      puVar4 = puVar4 + -3;
    } while (1 < (-(int)param_1 + 0xc + (int)puVar4) / 0xc);
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 0082f240 ===

/* [bsim sim=0.6737685012993061 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 0085d680 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_1 != param_2; param_1 = param_1 + 0x14) {
    *(undefined4 *)param_1 = *(undefined4 *)param_3;
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
    *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
    *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 0085d7a0 ===

/* [bsim sim=0.6737685012993061 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086a770 ===

/* [bsim sim=0.8009641593203382 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,uint param_4,uint param_5,uint param_6)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    uVar3 = *(uint *)(param_1 + iVar4 * 0xc);
    iVar1 = param_1 + iVar4 * 0xc;
    if (param_4 <= uVar3) break;
    puVar2 = (uint *)(param_1 + param_2 * 0xc);
    *puVar2 = uVar3;
    puVar2[1] = *(uint *)(iVar1 + 4);
    puVar2[2] = *(uint *)(iVar1 + 8);
    param_2 = iVar4;
  }
  puVar2 = (uint *)(param_1 + param_2 * 0xc);
  *puVar2 = param_4;
  puVar2[1] = param_5;
  puVar2[2] = param_6;
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086afb0 ===

/* [bsim sim=0.8590856811945937 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if (*(uint *)(param_1 + iVar4 * 0xc) < *(uint *)(param_1 + -0xc + iVar4 * 0xc)) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0xc);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0xc + iVar4 * 0xc);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7);
  return;
}



//=== _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_> @ 0086b070 ===

/* [bsim sim=0.7136581003859607 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,int,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>,class
   CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,int,int,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> &&,class CKeyPairCompareLess<enum EHeroMorphType,class CTextureMorphEntry>)
    */

void __fastcall
std::
_Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  
  iVar4 = param_2;
  while( true ) {
    iVar1 = iVar4 * 2;
    iVar2 = iVar1 + 2;
    if (param_3 <= iVar2) break;
    if (*(int *)(iVar2 * 0x10 + param_1) < *(int *)(iVar2 * 0x10 + param_1 + -0x10)) {
      iVar2 = iVar1 + 1;
    }
    puVar5 = (undefined4 *)(iVar2 * 0x10 + param_1);
    puVar3 = (undefined4 *)(iVar4 * 0x10 + param_1);
    *puVar3 = *puVar5;
    puVar3[1] = puVar5[1];
    puVar3[2] = puVar5[2];
    puVar3[3] = puVar5[3];
    iVar4 = iVar2;
  }
  if (iVar2 == param_3) {
    puVar3 = (undefined4 *)(iVar2 * 0x10 + -0x10 + param_1);
    puVar5 = (undefined4 *)(iVar4 * 0x10 + param_1);
    *puVar5 = *puVar3;
    puVar5[1] = puVar3[1];
    puVar5[2] = puVar3[2];
    puVar5[3] = puVar3[3];
    iVar4 = iVar1 + 1;
  }
  _Push_heap<std::pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,int,std::pair<long,class_CRandomAppearanceMorph::CSkeletalMorph>,CKeyPairCompareLess<long,CRandomAppearanceMorph::CSkeletalMorph>_>
            (param_1,iVar4,param_2,param_4,param_5,param_6,param_7,param_8);
  return;
}



//=== GFWriteVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph,unsigned_long,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086b210 ===

/* [bsim sim=1.0 <- ego_r]
   void __fastcall GFWriteVectorMapMemDump<class CRandomAppearanceMorph::CTextureMorph,unsigned
   long,class CKeyPairCompareLess<unsigned long,class CRandomAppearanceMorph::CTextureMorph> >(class
   CDataOutputStream &,class CVectorMap<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> > const &) */

void __fastcall
GFWriteVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph,unsigned_long,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (CDataOutputStream *param_1,
          CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          *param_2)

{
  int iVar1;
  CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
  *pCVar2;
  
  CDataOutputStream::WriteCBYTE(param_1,(char)((*(int *)(param_2 + 4) - *(int *)param_2) / 0xc));
  CDataOutputStream::WriteCBYTE
            (param_1,(char)((*(int *)(param_2 + 4) - *(int *)param_2) / 0xc) * '\f');
  iVar1 = (*(int *)(param_2 + 4) -
          (int)*(CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                 **)param_2) / 0xc;
  pCVar2 = *(CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
             **)param_2;
  if (iVar1 == 0) {
    pCVar2 = param_2;
  }
  (**(code **)(*(int *)param_1 + 0x10))(pCVar2,iVar1 * 0xc);
  CDataOutputStream::WriteEBOOL(param_1,(bool)param_2[0xd]);
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086c890 ===

/* [bsim sim=0.7569941204171718 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  if (1 < (param_2 - (int)param_1) / 0xc) {
    puVar4 = (undefined4 *)(param_2 + -0xc);
    do {
      uVar1 = puVar4[1];
      uVar2 = puVar4[2];
      uVar3 = *puVar4;
      *puVar4 = *param_1;
      puVar4[1] = param_1[1];
      puVar4[2] = param_1[2];
      _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                (param_1,0,(-(int)param_1 + (int)puVar4) / 0xc,uVar3,uVar1,uVar2,param_3);
      puVar4 = puVar4 + -3;
    } while (1 < (-(int)param_1 + 0xc + (int)puVar4) / 0xc);
  }
  return;
}



//=== GFReadVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph,unsigned_long,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086deb0 ===

/* [bsim sim=0.8136634826179111 <- ego_r]
   void __fastcall GFReadVectorMapMemDump<class CRandomAppearanceMorph::CTextureMorph,unsigned
   long,class CKeyPairCompareLess<unsigned long,class CRandomAppearanceMorph::CTextureMorph> >(class
   CMemoryDataInputStream &,class CVectorMap<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> > &) */

void __fastcall
GFReadVectorMapMemDump<CRandomAppearanceMorph::CTextureMorph,unsigned_long,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (CMemoryDataInputStream *param_1,
          CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
  *pCVar6;
  CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
  *pCVar7;
  uchar local_15;
  undefined4 local_14;
  uchar local_10 [4];
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_14,4);
    }
    else {
      local_14 = **(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,local_10,4);
    }
    else {
      *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 4;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  local_c = 0;
  local_8 = 0;
  local_4 = 0;
  std::vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_>::resize
            ((vector<CBrainBehaviour,std::allocator<CBrainBehaviour>_> *)param_2,local_14,&local_c);
  iVar2 = (*(int *)(param_2 + 4) -
          (int)*(CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                 **)param_2) / 0xc;
  pCVar7 = *(CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
             **)param_2;
  if (iVar2 == 0) {
    pCVar7 = param_2;
  }
  iVar3 = iVar2 * 0xc;
  if ((0 < iVar3) && ((uint)(*(int *)(param_1 + 4) + iVar3) < 0x80000000)) {
    if (*(int *)(param_1 + 0x14) < iVar3) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)pCVar7,iVar3);
    }
    else {
      pCVar6 = *(CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                 **)(param_1 + 0xc);
      for (uVar4 = iVar2 * 3 & 0x3fffffff; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pCVar7 = *(undefined4 *)pCVar6;
        pCVar6 = pCVar6 + 4;
        pCVar7 = pCVar7 + 4;
      }
      for (iVar5 = 0; iVar5 != 0; iVar5 = iVar5 + -1) {
        *pCVar7 = *pCVar6;
        pCVar6 = pCVar6 + 1;
        pCVar7 = pCVar7 + 1;
      }
      *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + iVar3;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + iVar2 * -0xc;
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + iVar3;
    }
  }
  if (*(int *)(param_1 + 4) + 1U < 0x80000000) {
    if (0 < *(int *)(param_1 + 0x14)) {
      cVar1 = **(char **)(param_1 + 0xc);
      *(char **)(param_1 + 0xc) = *(char **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -1;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 1U;
      param_2[0xd] = (CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                      )(cVar1 != '\0');
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,&local_15,1);
  }
  param_2[0xd] = (CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                  )(local_15 != '\0');
  return;
}



//=== _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086f100 ===

/* [bsim sim=0.8009641593203382 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    iVar3 = *(int *)(param_1 + iVar4 * 0xc);
    iVar1 = param_1 + iVar4 * 0xc;
    if (param_4 <= iVar3) break;
    piVar2 = (int *)(param_1 + param_2 * 0xc);
    *piVar2 = iVar3;
    piVar2[1] = *(int *)(iVar1 + 4);
    piVar2[2] = *(int *)(iVar1 + 8);
    param_2 = iVar4;
  }
  piVar2 = (int *)(param_1 + param_2 * 0xc);
  *piVar2 = param_4;
  piVar2[1] = param_5;
  piVar2[2] = param_6;
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 0086fb40 ===

/* [bsim sim=0.7569941204171718 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  if (1 < (param_2 - (int)param_1) / 0xc) {
    puVar4 = (undefined4 *)(param_2 + -0xc);
    do {
      uVar1 = puVar4[1];
      uVar2 = puVar4[2];
      uVar3 = *puVar4;
      *puVar4 = *param_1;
      puVar4[1] = param_1[1];
      puVar4[2] = param_1[2];
      _Adjust_heap<std::pair<EExpressionType,CExpressionSet::CEntry>*,int,std::pair<enum_EExpressionType,class_CExpressionSet::CEntry>,CKeyPairCompareLess<EExpressionType,CExpressionSet::CEntry>_>
                (param_1,0,(-(int)param_1 + (int)puVar4) / 0xc,uVar3,uVar1,uVar2,param_3);
      puVar4 = puVar4 + -3;
    } while (1 < (-(int)param_1 + 0xc + (int)puVar4) / 0xc);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00884b60 ===

/* [bsim sim=0.5101185053440758 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  LTextTreeWalkThrough *this_00;
  
  this_00 = (LTextTreeWalkThrough *)(this + 0x30);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01278060;
  if (*(int *)(this + 0x34) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    *(undefined4 *)(*(int *)this_00 + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 0x34) = 0;
  }
  if (*(void **)this_00 != (void *)0x0) {
    free(*(void **)this_00);
  }
  *(undefined ***)this = &PTR_LAB_01277ff0;
  return;
}



//=== CEnvironmentLookupTexture @ 00885f50 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.9686420614957213 <- ego_r]
   public: __thiscall
   NEnvironmentInternal::CEnvironmentLookupTexture::CEnvironmentLookupTexture(class CWideString) */

CEnvironmentLookupTexture * __thiscall
NEnvironmentInternal::CEnvironmentLookupTexture::CEnvironmentLookupTexture
          (CEnvironmentLookupTexture *this)

{
  void *pvVar1;
  CPixelFormat local_58 [4];
  int local_54;
  int local_50;
  undefined **local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined **local_3c [5];
  undefined1 local_28;
  CCharString local_24 [4];
  undefined4 local_20;
  undefined4 local_1c;
  undefined1 local_18 [12];
  ushort local_c;
  undefined1 local_a;
  undefined1 local_9;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  NHeroInformationScreens::CBase::CBase((CBase *)local_3c);
  local_3c[0] = &PTR__vector_deleting_destructor__0122d06c;
  CCharString::CCharString(local_24);
  local_28 = 0;
  CDiskFileWin32::_Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z
            ((CDiskFileWin32 *)local_3c,(CWideString *)&stack0x00000004,1,2);
  CDiskFileWin32::_Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z
            ((CDiskFileWin32 *)local_3c,local_18,0x12,(CASuspendableProcess *)0x0);
  *(uint *)this = (uint)local_c;
  *(uint *)(this + 4) = (uint)CONCAT11(local_9,local_a);
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_3c);
  local_4c = &PTR__vector_deleting_destructor__0122f84c;
  local_48 = 0;
  local_44 = 0;
  CPixelFormat::Initialise(local_58,8,8,8,8,false);
  local_1c = *(undefined4 *)(this + 4);
  local_20 = *(undefined4 *)this;
  NUISystem::CComponent::CanMove((CComponent *)&local_54,(C2DVector *)local_58);
  CPixelFormat::Initialise(local_58,8,8,8,8,false);
  CSurface::InitialiseAsImage((CSurface *)&local_4c,(C2DExtentsI *)&local_54,local_58,false);
  CSurface::LoadFromBitmapFileToPreallocatedSurface
            ((CSurface *)&local_4c,(CWideString *)&stack0x00000004,1);
  *(int *)(this + 8) = local_54 * 4;
  pvVar1 = operator_new__(local_54 * 4 * local_50);
  *(void **)(this + 0xc) = pvVar1;
  CSurface::CopyFromMemoryAutoSwizzle((CSurface *)&local_4c,pvVar1,*(long *)(this + 8));
  local_4c = &PTR__vector_deleting_destructor__0122f84c;
  CSurface::Uninitialise((CSurface *)&local_4c);
  local_3c[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_3c);
  CCharString::~CCharString(local_24);
  NHeroInformationScreens::CBase::CBase((CBase *)local_3c);
  CCharString::~CCharString((CCharString *)&stack0x00000004);
  return this;
}



//=== ?GetTextString@CGameScriptInterface@@UBE?AVCWideString@@ABVCCharString@@@Z @ 008913a0 ===

CCharString *
_GetTextString_CGameScriptInterface__UBE_AVCWideString__ABVCCharString___Z(CCharString *param_1)

{
  NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b86a0 + 0x14),param_1);
  return param_1;
}



//=== _Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_> @ 008ebae0 ===

/* [bsim sim=0.5695723567759243 <- ego_r]
   void __fastcall std::_Fill<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> const &) */

void __fastcall
std::
_Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_>
          (pair<EHeroMorphType,CTextureMorphEntry> *param_1,
          pair<EHeroMorphType,CTextureMorphEntry> *param_2,
          pair<enum_EHeroMorphType,class_CTextureMorphEntry> *param_3)

{
  for (; param_2 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0) {
      CCharString::CCharString((CCharString *)param_1,(CCharString *)param_3);
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
      *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 0x14);
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



//=== UpdateLocalisedSphereMapping @ 00988e80 ===

/* [bsim sim=0.9684147380045167 <- ego_r]
   private: void __thiscall CShaderRenderManager::UpdateLocalisedSphereMapping(void) */

void __thiscall CShaderRenderManager::UpdateLocalisedSphereMapping(CShaderRenderManager *this)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  CShaderRenderManager *pCVar19;
  float fVar20;
  float *pfVar21;
  float fVar22;
  int iVar23;
  float local_70;
  float local_6c;
  float local_68;
  undefined4 local_64;
  float local_60;
  float local_5c;
  float local_58;
  undefined4 local_54;
  float local_50 [7];
  float fStack_34;
  float local_30;
  float fStack_2c;
  float fStack_28;
  float fStack_24;
  float local_20;
  float fStack_1c;
  float fStack_18;
  float fStack_14;
  
  if (1 < *(uint *)(*(int *)(this + 4) + 0xa0)) {
    if (DAT_013d2880 == '\0') {
      iVar23 = 4;
      pCVar19 = this + 0x238;
      pfVar21 = local_50 + 2;
      do {
        iVar23 = iVar23 + -1;
        pfVar21[-2] = *(float *)(this + 0x210) * *(float *)pCVar19 +
                      *(float *)(this + 0x1f0) * *(float *)(pCVar19 + -8) +
                      *(float *)(this + 0x200) * *(float *)(pCVar19 + -4) +
                      *(float *)(this + 0x220) * *(float *)(pCVar19 + 4);
        pfVar21[-1] = *(float *)(pCVar19 + -8) * *(float *)(this + 500) +
                      *(float *)pCVar19 * *(float *)(this + 0x214) +
                      *(float *)(this + 0x204) * *(float *)(pCVar19 + -4) +
                      *(float *)(this + 0x224) * *(float *)(pCVar19 + 4);
        *pfVar21 = *(float *)pCVar19 * *(float *)(this + 0x218) +
                   *(float *)(this + 0x208) * *(float *)(pCVar19 + -4) +
                   *(float *)(this + 0x228) * *(float *)(pCVar19 + 4) +
                   *(float *)(this + 0x1f8) * *(float *)(pCVar19 + -8);
        pfVar21[1] = *(float *)(pCVar19 + -8) * *(float *)(this + 0x1fc) +
                     *(float *)(pCVar19 + -4) * *(float *)(this + 0x20c) +
                     *(float *)(pCVar19 + 4) * *(float *)(this + 0x22c) +
                     *(float *)(this + 0x21c) * *(float *)pCVar19;
        pCVar19 = pCVar19 + 0x10;
        pfVar21 = pfVar21 + 4;
      } while (iVar23 != 0);
    }
    else {
      fVar1 = *(float *)(this + 0x1f0);
      fVar20 = *(float *)(this + 500);
      fVar2 = *(float *)(this + 0x1f8);
      fVar3 = *(float *)(this + 0x1fc);
      fVar22 = *(float *)(this + 0x200);
      fVar4 = *(float *)(this + 0x204);
      fVar5 = *(float *)(this + 0x208);
      fVar6 = *(float *)(this + 0x20c);
      fVar7 = *(float *)(this + 0x210);
      fVar8 = *(float *)(this + 0x214);
      fVar9 = *(float *)(this + 0x218);
      fVar10 = *(float *)(this + 0x21c);
      fVar11 = *(float *)(this + 0x220);
      fVar12 = *(float *)(this + 0x224);
      fVar13 = *(float *)(this + 0x228);
      fVar14 = *(float *)(this + 0x22c);
      fVar15 = *(float *)(this + 0x230);
      fVar16 = *(float *)(this + 0x234);
      fVar17 = *(float *)(this + 0x238);
      fVar18 = *(float *)(this + 0x23c);
      local_50[0] = fVar15 * fVar1 + fVar16 * fVar22 + fVar17 * fVar7 + fVar18 * fVar11;
      local_50[1] = fVar15 * fVar20 + fVar16 * fVar4 + fVar17 * fVar8 + fVar18 * fVar12;
      local_50[2] = fVar15 * fVar2 + fVar16 * fVar5 + fVar17 * fVar9 + fVar18 * fVar13;
      local_50[3] = fVar15 * fVar3 + fVar16 * fVar6 + fVar17 * fVar10 + fVar18 * fVar14;
      fVar15 = *(float *)(this + 0x240);
      fVar16 = *(float *)(this + 0x244);
      fVar17 = *(float *)(this + 0x248);
      fVar18 = *(float *)(this + 0x24c);
      local_50[4] = fVar15 * fVar1 + fVar16 * fVar22 + fVar17 * fVar7 + fVar18 * fVar11;
      local_50[5] = fVar15 * fVar20 + fVar16 * fVar4 + fVar17 * fVar8 + fVar18 * fVar12;
      local_50[6] = fVar15 * fVar2 + fVar16 * fVar5 + fVar17 * fVar9 + fVar18 * fVar13;
      fStack_34 = fVar15 * fVar3 + fVar16 * fVar6 + fVar17 * fVar10 + fVar18 * fVar14;
      fVar15 = *(float *)(this + 0x250);
      fVar16 = *(float *)(this + 0x254);
      fVar17 = *(float *)(this + 600);
      fVar18 = *(float *)(this + 0x25c);
      local_30 = fVar15 * fVar1 + fVar16 * fVar22 + fVar17 * fVar7 + fVar18 * fVar11;
      fStack_2c = fVar15 * fVar20 + fVar16 * fVar4 + fVar17 * fVar8 + fVar18 * fVar12;
      fStack_28 = fVar15 * fVar2 + fVar16 * fVar5 + fVar17 * fVar9 + fVar18 * fVar13;
      fStack_24 = fVar15 * fVar3 + fVar16 * fVar6 + fVar17 * fVar10 + fVar18 * fVar14;
      fVar15 = *(float *)(this + 0x260);
      fVar16 = *(float *)(this + 0x264);
      fVar17 = *(float *)(this + 0x268);
      fVar18 = *(float *)(this + 0x26c);
      local_20 = fVar15 * fVar1 + fVar16 * fVar22 + fVar17 * fVar7 + fVar18 * fVar11;
      fStack_1c = fVar15 * fVar20 + fVar16 * fVar4 + fVar17 * fVar8 + fVar18 * fVar12;
      fStack_18 = fVar15 * fVar2 + fVar16 * fVar5 + fVar17 * fVar9 + fVar18 * fVar13;
      fStack_14 = fVar15 * fVar3 + fVar16 * fVar6 + fVar17 * fVar10 + fVar18 * fVar14;
    }
    local_64 = 0x3f000000;
    fVar1 = local_50[2] * local_50[2] + local_50[1] * local_50[1] + local_50[0] * local_50[0];
    fVar20 = (float)((0x17c - ((uint)fVar1 >> 0x17 & 0xff) & 0x3fe) << 0x16 |
                    (&DAT_013ce558)[(uint)fVar1 >> 0x11 & 0x7f] << 0xf);
    fVar1 = (3.0 - fVar20 * fVar20 * fVar1) * fVar20 * 0.5;
    local_70 = local_50[0] * fVar1 * 0.5;
    local_6c = local_50[1] * fVar1 * 0.5;
    local_68 = local_50[2] * fVar1 * 0.5;
    fVar1 = -local_50[4];
    fVar3 = -local_50[5];
    fVar2 = -local_50[6];
    fVar20 = fVar1 * fVar1 + fVar3 * fVar3 + fVar2 * fVar2;
    fVar22 = (float)((0x17c - ((uint)fVar20 >> 0x17 & 0xff) & 0x3fe) << 0x16 |
                    (&DAT_013ce558)[(uint)fVar20 >> 0x11 & 0x7f] << 0xf);
    local_54 = 0x3f000000;
    fVar20 = (3.0 - fVar22 * fVar22 * fVar20) * fVar22 * 0.5;
    local_60 = fVar20 * fVar1 * 0.5;
    local_5c = fVar20 * fVar3 * 0.5;
    local_58 = fVar20 * fVar2 * 0.5;
    (**(code **)(**(int **)(this + 0x1d4) + 0x178))
              (*(int **)(this + 0x1d4),*(undefined4 *)(*(int *)(this + 4) + 0x9c),&local_70,2);
  }
  *(uint *)(this + 0x3d8) = *(uint *)(this + 0x3d8) & 0xffff7fff;
  return;
}



//=== FinishWithDynamicTextures @ 0098d9f0 ===

/* [bsim sim=0.5069797832838899 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  LTextTreeWalkThrough *this_00;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  
  this_00 = (LTextTreeWalkThrough *)(this + 0xc);
  if (*(int *)(this + 0x10) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    in_EDX = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)this_00;
    *(undefined4 *)(in_EDX + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 0x10) = 0;
  }
  if (*(void **)this_00 != (void *)0x0) {
    free(*(void **)this_00);
    in_EDX = extraout_EDX;
  }
  paVar1 = *(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)(this + 4);
  for (paVar2 = *(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)this;
      paVar2 != paVar1; paVar2 = paVar2 + 4) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (paVar2,in_EDX,unaff_EDI);
    in_EDX = extraout_EDX_00;
  }
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== ?Render@CStreamingFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z @ 009a25d0 ===

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Removing unreachable block (ram,0x009a2d95) */
/* WARNING: Removing unreachable block (ram,0x009a2c31) */
/* WARNING: Removing unreachable block (ram,0x009a2cc1) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _Render_CStreamingFontBank__UBEXABVCWideString__ABVC3DVector__MABVCRGBColour__KPBVCTexture___Z
               (CCharString *param_1,CEngineInternalPrimitiveBase *param_2,byte param_3,int *param_4
               )

{
  byte *pbVar1;
  float fVar2;
  float fVar3;
  CStreamingGlyphLookup CVar4;
  undefined4 uVar5;
  CRenderManagerCore *this;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  bool bVar14;
  CEngineInternalPrimitiveBase *pCVar15;
  char cVar16;
  ulong uVar17;
  char *pcVar18;
  short *psVar19;
  ushort *puVar20;
  CStreamingGlyphLookup *pCVar21;
  short sVar22;
  CStreamingFontBank *in_ECX;
  CEngineInternalPrimitiveBase *pCVar23;
  int iVar24;
  uint uVar25;
  uint uVar26;
  ulong unaff_EBX;
  uint *puVar27;
  CEnginePrimitive *unaff_EBP;
  ushort uVar28;
  CEnginePrimitive *unaff_ESI;
  float *pfVar29;
  CEnginePrimitive *unaff_EDI;
  float *pfVar30;
  double dVar31;
  float *unaff_retaddr;
  CCharString *pCVar32;
  float fVar33;
  undefined4 uVar34;
  float fStack_25e8;
  uint uStack_25dc;
  float fStack_25c4;
  long lStack_25b4;
  float *pfStack_25b0;
  float *pfStack_25ac;
  float fStack_25a0;
  float fStack_259c;
  CStreamingFontBank *pCStack_2598;
  float fStack_2594;
  CEngineInternalPrimitiveBase *pCStack_2590;
  float fStack_258c;
  undefined4 uStack_2588;
  float fStack_2584;
  float fStack_2580;
  float fStack_257c;
  undefined4 uStack_2578;
  float fStack_2574;
  CEngineInternalPrimitiveBase *pCStack_2570;
  float fStack_256c;
  undefined4 uStack_2568;
  float fStack_2564;
  CEngineInternalPrimitiveBase *pCStack_2560;
  float fStack_255c;
  undefined4 uStack_2558;
  float fStack_2554;
  CEngineInternalPrimitiveBase *pCStack_2550;
  float fStack_254c;
  undefined4 uStack_2548;
  ushort *puStack_2544;
  float fStack_2540;
  float fStack_253c;
  float fStack_2538;
  undefined4 uStack_2534;
  float fStack_2530;
  float fStack_252c;
  float fStack_2528;
  undefined4 uStack_2524;
  float fStack_2520;
  float fStack_251c;
  float fStack_2518;
  undefined4 uStack_2514;
  int iStack_2510;
  int iStack_250c;
  float afStack_2508 [5];
  float fStack_24f4;
  float fStack_24f0;
  float afStack_24ec [5];
  float fStack_24d8;
  float fStack_24d4;
  float fStack_24d0;
  CEngineInternalPrimitiveBase *pCStack_24cc;
  float fStack_24c8;
  undefined4 uStack_24c4;
  undefined4 uStack_24c0;
  float fStack_24bc;
  float fStack_24b8;
  float fStack_24b4;
  CEngineInternalPrimitiveBase *pCStack_24b0;
  float fStack_24ac;
  undefined4 uStack_24a8;
  undefined4 uStack_24a4;
  float fStack_24a0;
  float fStack_249c;
  float fStack_2498;
  CEngineInternalPrimitiveBase *pCStack_2494;
  float fStack_2490;
  undefined4 uStack_248c;
  undefined4 uStack_2488;
  float fStack_2484;
  float fStack_2480;
  float fStack_247c;
  float fStack_2478;
  float afStack_2474 [5];
  float fStack_2460;
  float fStack_245c;
  float fStack_2458;
  float fStack_2454;
  float fStack_2450;
  CEngineInternalPrimitiveBase *pCStack_244c;
  float fStack_2448;
  undefined4 uStack_2444;
  undefined4 uStack_2440;
  float fStack_243c;
  float fStack_2438;
  float fStack_2434;
  float fStack_2430;
  float afStack_242c [5];
  float fStack_2418;
  float fStack_2414;
  float fStack_2410;
  float fStack_240c;
  float afStack_2408 [2305];
  CCharString *pCStack_4;
  
  pCStack_4 = (CCharString *)0x9a25da;
  pCStack_2590 = (CEngineInternalPrimitiveBase *)in_ECX;
  uVar17 = CEngineInternalPrimitiveBase::AddChildPrimitive
                     ((CEngineInternalPrimitiveBase *)in_ECX,unaff_ESI,unaff_EBX);
  pCVar32 = param_1;
  cVar16 = (**(code **)(*(int *)in_ECX + 0x10))
                     (param_1,*(undefined1 *)(*(int *)(uVar17 + 0x84) + 0x38));
  if (cVar16 != '\0') {
    fVar2 = *unaff_retaddr;
    uVar5 = *(undefined4 *)param_2;
    fVar3 = unaff_retaddr[1];
    uVar17 = CEngineInternalPrimitiveBase::AddChildPrimitive(param_2,unaff_EBP,(ulong)pCVar32);
    this = *(CRenderManagerCore **)(*(int *)(uVar17 + 0x60) + 8);
    if (DAT_013c5a70 != '\0') {
      _anon_FE46A155::GFUpdateFontTextureCache();
    }
    fStack_25c4 = fVar3;
    if ((param_3 & 8) != 0) {
      iVar24 = *(int *)in_ECX;
      pcVar18 = CCharString::operator_char_const_(param_1);
      (**(code **)(iVar24 + 4))(&fStack_25a0,pcVar18);
      dVar31 = floor((double)(fVar3 - fStack_259c * 0.5));
      fStack_25c4 = (float)dVar31;
    }
    lStack_25b4 = 0x1c;
    uVar34 = 0;
    if (DAT_013bc800 == '\0') {
      if (param_4 == (int *)0x0) {
        CRenderManagerCore::SetVertexFormat(this,0x144);
      }
      else {
        CRenderManagerCore::SetVertexFormat(this,0x244);
        lStack_25b4 = 0x24;
        uVar34 = 0x1000000;
      }
    }
    pfStack_25b0 = afStack_2408;
    if (s___AVCStreamingFontBank___0138e09c[0x19] != '\0') {
      psVar19 = &DAT_013ca308;
      uVar25 = 0;
      do {
        sVar22 = (short)uVar25 * 4;
        *psVar19 = sVar22;
        psVar19[1] = sVar22 + 1;
        psVar19[2] = sVar22 + 2;
        psVar19[3] = sVar22 + 2;
        psVar19[4] = sVar22 + 3;
        psVar19[5] = sVar22;
        psVar19 = psVar19 + 6;
        uVar25 = uVar25 + 1;
      } while (uVar25 < 0x40);
      s___AVCStreamingFontBank___0138e09c[0x19] = '\0';
    }
    if (*(int *)(this + 0x3d00) != DAT_013c5a80) {
      *(int *)(this + 0x3d00) = DAT_013c5a80;
      (**(code **)(**(int **)(this + 0x3cf0) + 0x104))(*(int **)(this + 0x3cf0),0);
    }
    if (*(int *)(this + 0x3cb8) < 1) {
      *(undefined4 *)(this + 0x3cb8) = 1;
    }
    if (param_4 == (int *)0x0) {
      iVar24 = 0;
    }
    else {
      iVar24 = *param_4;
    }
    if (*(int *)(this + 0x3d04) != iVar24) {
      *(int *)(this + 0x3d04) = iVar24;
      (**(code **)(**(int **)(this + 0x3cf0) + 0x104))(*(int **)(this + 0x3cf0),1);
    }
    pCVar23 = *(CEngineInternalPrimitiveBase **)(this + 0x3cb8);
    if ((int)pCVar23 < 2) {
      *(undefined4 *)(this + 0x3cb8) = 2;
    }
    iVar24 = DAT_01396e74;
    if ((param_4 != (int *)0x0) && (DAT_013bc801 == '\0')) {
      if (*(int *)(this + 0x321c) != DAT_01396e74) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x3220)) == 0) {
          *(uint *)(this + 0x3220) = *(uint *)(this + 0x3220) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x3218;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x321c)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(int *)(this + 0x321c) = iVar24;
        if (this[0x3228] == (CRenderManagerCore)0x0) {
          this[0x3228] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x3218;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      iVar24 = DAT_01396ee8;
      if (*(int *)(this + 0x335c) != DAT_01396ee8) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x3360)) == 0) {
          *(uint *)(this + 0x3360) = *(uint *)(this + 0x3360) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x3358;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x335c)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(int *)(this + 0x335c) = iVar24;
        if (this[0x3368] == (CRenderManagerCore)0x0) {
          this[0x3368] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x3358;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      iVar24 = DAT_01396e7c;
      if (*(int *)(this + 0x32bc) != DAT_01396e7c) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x32c0)) == 0) {
          *(uint *)(this + 0x32c0) = *(uint *)(this + 0x32c0) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x32b8;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x32bc)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(int *)(this + 0x32bc) = iVar24;
        if (this[13000] == (CRenderManagerCore)0x0) {
          this[13000] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x32b8;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      iVar24 = DAT_01396ef0;
      if (*(int *)(this + 0x349c) != DAT_01396ef0) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x34a0)) == 0) {
          *(uint *)(this + 0x34a0) = *(uint *)(this + 0x34a0) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x3498;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x349c)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(int *)(this + 0x349c) = iVar24;
        if (this[0x34a8] == (CRenderManagerCore)0x0) {
          this[0x34a8] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x3498;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      iVar24 = DAT_01396ee8;
      if (*(int *)(this + 0x353c) != DAT_01396ee8) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x3540)) == 0) {
          *(uint *)(this + 0x3540) = *(uint *)(this + 0x3540) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x3538;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x353c)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(int *)(this + 0x353c) = iVar24;
        if (this[0x3548] == (CRenderManagerCore)0x0) {
          this[0x3548] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x3538;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      pCVar23 = DAT_01396e70;
      if (*(CEngineInternalPrimitiveBase **)(this + 0x3230) != DAT_01396e70) {
        if ((*(uint *)(this + 0x2814) & *(uint *)(this + 0x3234)) == 0) {
          *(uint *)(this + 0x3234) = *(uint *)(this + 0x3234) | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x322c;
          *(undefined4 *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = *(undefined4 *)(this + 0x3230)
          ;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        *(CEngineInternalPrimitiveBase **)(this + 0x3230) = pCVar23;
        if (this[0x323c] == (CRenderManagerCore)0x0) {
          this[0x323c] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x322c;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
      pCVar15 = DAT_01396e70;
      pCVar23 = DAT_01396e70;
      if (*(CEngineInternalPrimitiveBase **)(this + 0x32d0) != DAT_01396e70) {
        uVar25 = *(uint *)(this + 0x32d4);
        if ((*(uint *)(this + 0x2814) & uVar25) == 0) {
          *(uint *)(this + 0x32d4) = uVar25 | *(uint *)(this + 0x2814);
          *(CRenderManagerCore **)(this + *(int *)(this + 0x2808) * 8 + 8) = this + 0x32cc;
          uVar25 = *(uint *)(this + 0x32d0);
          *(uint *)(this + *(int *)(this + 0x2808) * 8 + 0xc) = uVar25;
          *(int *)(this + 0x2808) = *(int *)(this + 0x2808) + 1;
        }
        pCVar23 = (CEngineInternalPrimitiveBase *)CONCAT31((int3)(uVar25 >> 8),this[0x32dc]);
        *(CEngineInternalPrimitiveBase **)(this + 0x32d0) = pCVar15;
        if (this[0x32dc] == (CRenderManagerCore)0x0) {
          this[0x32dc] = (CRenderManagerCore)0x1;
          *(CRenderManagerCore **)(this + *(int *)(this + 0x280c) * 4 + 0x2008) = this + 0x32cc;
          *(int *)(this + 0x280c) = *(int *)(this + 0x280c) + 1;
        }
      }
    }
    uVar17 = CEngineInternalPrimitiveBase::AddChildPrimitive(pCVar23,unaff_EDI,(ulong)unaff_EBP);
    CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing
              (*(CMessageEventFilter_IsOriginatedByThing **)(uVar17 + 0x60),(CThing *)&iStack_2510);
    uStack_25dc = 0;
    bVar14 = true;
    fVar6 = 1.0 / ((float)iStack_2510 * 0.5);
    fVar7 = 1.0 / ((float)iStack_250c * 0.5);
    puVar20 = (ushort *)CCharString::operator_char_const_(pCStack_4);
    fStack_25e8 = fVar2;
    pfStack_25ac = pfStack_25b0;
    if (*puVar20 != 0) {
      do {
        if (bVar14) {
          if ((param_3 & 6) != 0) {
            (**(code **)(*(int *)in_ECX + 4))(&fStack_25a0,puVar20);
            if ((param_3 & 2) != 0) {
              dVar31 = floor((double)(fVar2 - fStack_25a0));
              fStack_25e8 = (float)dVar31;
            }
            if ((param_3 & 4) != 0) {
              dVar31 = floor((double)(fVar2 - fStack_25a0 * 0.5));
              fStack_25e8 = (float)dVar31;
            }
          }
          bVar14 = false;
        }
        uVar28 = *puVar20;
        puStack_2544 = puVar20 + 1;
        if (uVar28 == 10) {
          fVar8 = (float)*(int *)(in_ECX + 4);
          if (*(int *)(in_ECX + 4) < 0) {
            fVar8 = fVar8 + 4.2949673e+09;
          }
          fStack_25c4 = fVar8 + fStack_25c4;
          bVar14 = true;
          fStack_25e8 = fVar2;
        }
        else {
          if (uVar28 == 0xa0) {
            uVar28 = 0x20;
          }
          pCVar21 = CStreamingFontBank::GlyphLookup(in_ECX,uVar28);
          if (pCVar21 != (CStreamingGlyphLookup *)0x0) {
            pbVar1 = (byte *)(DAT_013bda40 + (uint)(byte)pCVar21[7] * 2);
            if (uVar28 == 0x20) {
              cVar16 = __ftol2();
              fStack_25e8 = (float)(int)cVar16 + fStack_25e8;
            }
            else {
              puVar27 = &DAT_013c7ac8;
              if (-1 < *(int *)(pCVar21 + 0xc)) {
                puVar27 = &DAT_013c5a98;
              }
              uVar25 = *puVar27;
              uVar26 = (uint)(*(ushort *)pCVar21 >> 4) % uVar25;
              fVar8 = (float)(int)uVar26;
              if ((int)uVar26 < 0) {
                fVar8 = fVar8 + 4.2949673e+09;
              }
              fVar13 = (float)(int)uVar25;
              if ((int)uVar25 < 0) {
                fVar13 = fVar13 + 4.2949673e+09;
              }
              fVar8 = fVar8 / fVar13;
              fVar13 = (float)(int)puVar27[1];
              if ((int)puVar27[1] < 0) {
                fVar13 = fVar13 + 4.2949673e+09;
              }
              fVar13 = (float)((*(ushort *)pCVar21 >> 4) / uVar25) / fVar13;
              fVar9 = (float)param_1 * (float)*pbVar1 * 0.01;
              CVar4 = pCVar21[10];
              fVar12 = ((float)(int)(char)pCVar21[8] * fVar9 +
                        (fStack_25e8 - fVar2) * (float)param_1 + fVar2) - 0.5;
              fVar11 = ((fStack_25c4 - fVar3) * (float)param_1 +
                        (float)((int)(char)pbVar1[1] + (int)(char)pCVar21[9]) * fVar9 + fVar3) - 0.5
              ;
              fVar10 = (float)(int)(char)CVar4 * fVar9 + fVar12;
              fVar33 = (float)(int)(char)pCVar21[0xb];
              pCVar23 = (CEngineInternalPrimitiveBase *)(fVar33 * fVar9 + fVar11);
              cVar16 = __ftol2();
              fStack_25e8 = (float)(int)cVar16 + fStack_25e8;
              fVar9 = (float)(int)(char)CVar4 * 0.0009765625 + fVar8;
              fVar33 = fVar33 * 0.001953125 + fVar13;
              if ((char)((uint)uVar34 >> 0x18) == '\0') {
                fStack_24d8 = fVar9;
                fStack_254c = unaff_retaddr[2];
                pCStack_24cc = pCVar23;
                fStack_24d0 = fVar12;
                fStack_24c8 = fStack_254c;
                uStack_2548 = 0x3f800000;
                uStack_24c4 = 0x3f800000;
                fStack_24b8 = fVar33;
                fStack_24bc = fVar8;
                uStack_24c0 = uVar5;
                pfVar29 = &fStack_24d0;
                pfVar30 = pfStack_25b0;
                for (iVar24 = 7; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                fStack_2538 = unaff_retaddr[2];
                afStack_2508[0] = fVar12;
                afStack_2508[1] = fVar11;
                afStack_2508[2] = fStack_2538;
                uStack_2534 = 0x3f800000;
                afStack_2508[3] = 1.0;
                fStack_24f4 = fVar8;
                fStack_24f0 = fVar13;
                afStack_2508[4] = (float)uVar5;
                pfVar29 = afStack_2508;
                pfVar30 = pfStack_25b0 + 7;
                for (iVar24 = 7; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                fStack_2518 = unaff_retaddr[2];
                afStack_24ec[0] = fVar10;
                afStack_24ec[1] = fVar11;
                afStack_24ec[2] = fStack_2518;
                uStack_2514 = 0x3f800000;
                afStack_24ec[3] = 1.0;
                fStack_24d4 = fVar13;
                afStack_24ec[4] = (float)uVar5;
                pfVar29 = afStack_24ec;
                pfVar30 = pfStack_25b0 + 0xe;
                for (iVar24 = 7; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                pfVar30 = pfStack_25b0 + 0x15;
                fStack_258c = unaff_retaddr[2];
                fStack_24a0 = fVar9;
                fStack_24b4 = fVar10;
                fStack_24ac = fStack_258c;
                uStack_2588 = 0x3f800000;
                pCStack_24b0 = pCVar23;
                pfStack_25b0 = pfStack_25b0 + 0x1c;
                fStack_249c = fVar33;
                uStack_24a8 = 0x3f800000;
                uStack_24a4 = uVar5;
                iVar24 = 7;
                pfVar29 = &fStack_24b4;
                fStack_2594 = fVar10;
                pCStack_2590 = pCVar23;
                fStack_2554 = fVar12;
                pCStack_2550 = pCVar23;
                fStack_2540 = fVar12;
                fStack_253c = fVar11;
                fStack_2520 = fVar10;
                fStack_251c = fVar11;
              }
              else {
                fStack_255c = unaff_retaddr[2];
                fStack_2498 = fVar12;
                fStack_247c = (fVar12 * fVar6 - 1.0) * _DAT_013ca2f8 + _DAT_013ca300;
                fStack_2490 = fStack_255c;
                pCStack_2494 = pCVar23;
                fStack_2484 = fVar8;
                uStack_2558 = 0x3f800000;
                uStack_248c = 0x3f800000;
                fStack_240c = (1.0 - fVar11 * fVar7) * _DAT_013ca2fc + _DAT_013ca304;
                fStack_2480 = fVar33;
                uStack_2488 = uVar5;
                uStack_2524 = 0x3f800000;
                afStack_242c[4] = (float)uVar5;
                fStack_2458 = (fVar10 * fVar6 - 1.0) * _DAT_013ca2f8 + _DAT_013ca300;
                fStack_2478 = (1.0 - (float)pCVar23 * fVar7) * _DAT_013ca2fc + _DAT_013ca304;
                pfVar29 = &fStack_2498;
                pfVar30 = pfStack_25ac;
                for (iVar24 = 9; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                fStack_2410 = fStack_247c;
                fStack_2528 = unaff_retaddr[2];
                afStack_242c[0] = fVar12;
                afStack_242c[1] = fVar11;
                afStack_242c[2] = fStack_2528;
                afStack_242c[3] = 1.0;
                fStack_2418 = fVar8;
                fStack_2414 = fVar13;
                fStack_2460 = fVar9;
                pfVar29 = afStack_242c;
                pfVar30 = pfStack_25ac + 9;
                for (iVar24 = 9; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                fStack_2454 = fStack_240c;
                fStack_243c = fVar9;
                fStack_257c = unaff_retaddr[2];
                fStack_256c = unaff_retaddr[2];
                afStack_2474[0] = fVar10;
                afStack_2474[1] = fVar11;
                afStack_2474[2] = fStack_257c;
                uStack_2578 = 0x3f800000;
                afStack_2474[3] = 1.0;
                fStack_245c = fVar13;
                afStack_2474[4] = (float)uVar5;
                pfVar29 = afStack_2474;
                pfVar30 = pfStack_25ac + 0x12;
                for (iVar24 = 9; iVar24 != 0; iVar24 = iVar24 + -1) {
                  *pfVar30 = *pfVar29;
                  pfVar29 = pfVar29 + 1;
                  pfVar30 = pfVar30 + 1;
                }
                fStack_2450 = fVar10;
                fStack_2448 = fStack_256c;
                pCStack_244c = pCVar23;
                pfVar30 = pfStack_25ac + 0x1b;
                uStack_2568 = 0x3f800000;
                fStack_2438 = fVar33;
                uStack_2444 = 0x3f800000;
                pfStack_25ac = pfStack_25ac + 0x24;
                fStack_2430 = fStack_2478;
                uStack_2440 = uVar5;
                fStack_2434 = fStack_2458;
                iVar24 = 9;
                pfVar29 = &fStack_2450;
                fStack_2584 = fVar10;
                fStack_2580 = fVar11;
                fStack_2574 = fVar10;
                pCStack_2570 = pCVar23;
                fStack_2564 = fVar12;
                pCStack_2560 = pCVar23;
                fStack_2530 = fVar12;
                fStack_252c = fVar11;
              }
              uStack_25dc = uStack_25dc + 4;
              for (; iVar24 != 0; iVar24 = iVar24 + -1) {
                *pfVar30 = *pfVar29;
                pfVar29 = pfVar29 + 1;
                pfVar30 = pfVar30 + 1;
              }
              if (0xfc < uStack_25dc) {
                CRenderManagerCore::DrawAsIndexedTris
                          (this,afStack_2408,lStack_25b4,uStack_25dc,&DAT_013ca308,uStack_25dc >> 1)
                ;
                pfStack_25b0 = afStack_2408;
                uStack_25dc = 0;
                pfStack_25ac = pfStack_25b0;
              }
            }
          }
        }
        in_ECX = pCStack_2598;
        puVar20 = puStack_2544;
      } while (*puStack_2544 != 0);
      if (uStack_25dc != 0) {
        CRenderManagerCore::DrawAsIndexedTris
                  (this,afStack_2408,lStack_25b4,uStack_25dc,&DAT_013ca308,uStack_25dc >> 1);
      }
    }
    if (*(int *)(this + 0x3d00) != 0) {
      *(undefined4 *)(this + 0x3d00) = 0;
      (**(code **)(**(int **)(this + 0x3cf0) + 0x104))(*(int **)(this + 0x3cf0),0,0);
    }
    if (*(int *)(this + 0x3cb8) < 1) {
      *(undefined4 *)(this + 0x3cb8) = 1;
    }
    if (*(int *)(this + 0x3d04) != 0) {
      *(undefined4 *)(this + 0x3d04) = 0;
      (**(code **)(**(int **)(this + 0x3cf0) + 0x104))(*(int **)(this + 0x3cf0),1,0);
    }
    if (*(int *)(this + 0x3cb8) < 2) {
      *(undefined4 *)(this + 0x3cb8) = 2;
    }
  }
  return;
}



//=== _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 009b1ca0 ===

/* [bsim sim=0.8009641593203382 <- ego_r]
   void __fastcall std::_Push_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,uint param_4,uint param_5,uint param_6)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  
  while (param_3 < param_2) {
    iVar4 = (param_2 + -1) / 2;
    uVar3 = *(uint *)(param_1 + iVar4 * 0xc);
    iVar1 = param_1 + iVar4 * 0xc;
    if (param_4 <= uVar3) break;
    puVar2 = (uint *)(param_1 + param_2 * 0xc);
    *puVar2 = uVar3;
    puVar2[1] = *(uint *)(iVar1 + 4);
    puVar2[2] = *(uint *)(iVar1 + 8);
    param_2 = iVar4;
  }
  puVar2 = (uint *)(param_1 + param_2 * 0xc);
  *puVar2 = param_4;
  puVar2[1] = param_5;
  puVar2[2] = param_6;
  return;
}



//=== _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 009b2f30 ===

/* [bsim sim=0.8590856811945937 <- ego_r]
   void __fastcall std::_Adjust_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,int,int,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> &&,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2;
  while( true ) {
    iVar3 = iVar5 * 2;
    iVar4 = iVar3 + 2;
    if (param_3 <= iVar4) break;
    if (*(uint *)(param_1 + iVar4 * 0xc) < *(uint *)(param_1 + -0xc + iVar4 * 0xc)) {
      iVar4 = iVar3 + 1;
    }
    puVar1 = (undefined4 *)(param_1 + iVar4 * 0xc);
    puVar2 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar2 = *puVar1;
    puVar2[1] = puVar1[1];
    puVar2[2] = puVar1[2];
    iVar5 = iVar4;
  }
  if (iVar4 == param_3) {
    puVar2 = (undefined4 *)(param_1 + -0xc + iVar4 * 0xc);
    puVar1 = (undefined4 *)(param_1 + iVar5 * 0xc);
    *puVar1 = *puVar2;
    puVar1[1] = puVar2[1];
    puVar1[2] = puVar2[2];
    iVar5 = iVar3 + 1;
  }
  _Push_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
            (param_1,iVar5,param_2,param_4,param_5,param_6,param_7);
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 009b5af0 ===

/* [bsim sim=0.7569941204171718 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  if (1 < (param_2 - (int)param_1) / 0xc) {
    puVar4 = (undefined4 *)(param_2 + -0xc);
    do {
      uVar1 = puVar4[1];
      uVar2 = puVar4[2];
      uVar3 = *puVar4;
      *puVar4 = *param_1;
      puVar4[1] = param_1[1];
      puVar4[2] = param_1[2];
      _Adjust_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,int,std::pair<unsigned_long,class_CRandomAppearanceMorph::CTextureMorph>,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
                (param_1,0,(-(int)param_1 + (int)puVar4) / 0xc,uVar3,uVar1,uVar2,param_3);
      puVar4 = puVar4 + -3;
    } while (1 < (-(int)param_1 + 0xc + (int)puVar4) / 0xc);
  }
  return;
}



//=== CPreallocTexturePool @ 009bdb00 ===

/* [bsim sim=0.5696867170588596 <- ego_r]
   public: __thiscall CPreallocTexturePool::CPreallocTexturePool(void) */

CPreallocTexturePool * __thiscall
CPreallocTexturePool::CPreallocTexturePool(CPreallocTexturePool *this)

{
  void *pvVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b3bc;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  pvVar1 = malloc(0x24);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x58) = pvVar1;
  return this;
}



//=== GetTextInfo @ 009c7d00 ===

/* [bsim sim=1.0 <- ego_r]
   public: class NGameText::CDataBankTextEntry const * __thiscall
   NGameText::CDataBank::GetTextInfo(unsigned long)const  */

CDataBankTextEntry * __thiscall NGameText::CDataBank::GetTextInfo(CDataBank *this,ulong param_1)

{
  ulong uVar1;
  
  uVar1 = CBankFile::GetEntryDataType((CBankFile *)this,param_1);
  if (uVar1 == 0) {
    return *(CDataBankTextEntry **)(*(int *)(this + 0x1e0) + param_1 * 4);
  }
  return (CDataBankTextEntry *)0x0;
}



//=== CTextEntry @ 009c8250 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall NGameText::CTextEntry::CTextEntry(class CCountedPointer<class
   CBankFileAsyncData> &,class NGameText::CDataBankTextEntry const *) */

CTextEntry * __thiscall
NGameText::CTextEntry::CTextEntry
          (CTextEntry *this,CCountedPointer<CBankFileAsyncData> *param_1,CDataBankTextEntry *param_2
          )

{
  int *piVar1;
  ulong uVar2;
  
  CResource::CResource((CResource *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129b4a4;
  CCharString::CCharString((CCharString *)(this + 0x1c));
  CCharString::CCharString((CCharString *)(this + 0x20));
  CCharString::CCharString((CCharString *)(this + 0x24));
  CCharString::CCharString((CCharString *)(this + 0x28));
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = *(undefined4 *)param_1;
  piVar1 = *(int **)(param_1 + 4);
  *(int **)(this + 0x3c) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  *(CDataBankTextEntry **)(this + 0x40) = param_2;
  uVar2 = Sizeof(this);
  CResource::SetResourceSize((CResource *)this,uVar2);
  return this;
}



//=== GetTextEntryFromGroup @ 009c9280 ===

/* [bsim sim=0.7757466826789848 <- ego_r]
   public: class NGameText::CTextEntry const * __thiscall
   NGameText::CDataBank::GetTextEntryFromGroup(unsigned long,class CCharString const &,bool &,long
   *,bool)const  */

CTextEntry * __thiscall
NGameText::CDataBank::GetTextEntryFromGroup
          (CDataBank *this,ulong param_1,CCharString *param_2,bool *param_3,long *param_4,
          bool param_5)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  CBankFileAsyncData *this_00;
  CTextEntry *pCVar4;
  bool bVar5;
  bool bVar6;
  ulong uVar7;
  int iVar8;
  long lVar9;
  uint uVar10;
  CDataBankTextEntry *pCVar11;
  undefined4 *puVar12;
  ulong uVar13;
  pair<long,CMusicManager::CRegisteredMusicEntry> *ppVar14;
  pair<long,CMusicManager::CRegisteredMusicEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar15;
  pair<long,CMusicManager::CRegisteredMusicEntry> *extraout_EDX_03;
  pair<long,CMusicManager::CRegisteredMusicEntry> *extraout_EDX_04;
  pair<long,CMusicManager::CRegisteredMusicEntry> *ppVar16;
  ulong *puVar17;
  uint uVar18;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar19;
  undefined3 in_stack_00000015;
  pair<long,CMusicManager::CRegisteredMusicEntry> *ppStack_1c;
  int *local_18;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_14 [4];
  ulong uStack_10;
  ulong *local_c;
  ulong *local_8;
  undefined4 local_4;
  
  uVar13 = param_1;
  bVar6 = false;
  *param_3 = false;
  if ((param_1 != 0) && (uVar7 = CBankFile::GetEntryDataType((CBankFile *)this,param_1), uVar7 == 1)
     ) {
    local_c = (ulong *)0x0;
    local_8 = (ulong *)0x0;
    local_4 = 0;
    param_1 = 0;
    uVar7 = CBankFile::GetEntryDataType((CBankFile *)this,uVar13);
    if ((uVar7 == 1) && (*(int *)(*(int *)(this + 0x1e0) + uVar13 * 4) != 0)) {
      uVar19 = GetPGroupEntry(this,(ulong)&local_18,SUB41(uVar13,0));
      ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)((ulonglong)uVar19 >> 0x20);
      uVar13 = *(ulong *)uVar19;
      if (uVar13 != 0) {
        *(int *)(uVar13 + 4) = *(int *)(uVar13 + 4) + 1;
        param_1 = uVar13;
      }
      if (local_18 != (int *)0x0) {
        ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)(local_18[1] - 1);
        local_18[1] = (int)ppVar14;
        if (ppVar14 == (pair<long,CMusicManager::CRegisteredMusicEntry> *)0x0) {
          (**(code **)(*local_18 + 4))();
          ppVar14 = extraout_EDX;
        }
        local_18 = (int *)0x0;
      }
      if (param_1 == 0) {
        *param_3 = true;
        return (CTextEntry *)0x0;
      }
      iVar8 = *(int *)(param_1 + 0x1c);
      ppStack_1c = (pair<long,CMusicManager::CRegisteredMusicEntry> *)0x0;
      puVar17 = (ulong *)0x0;
      if (*(int *)(param_1 + 0x20) - iVar8 >> 2 != 0) {
        do {
          uVar13 = *(ulong *)(iVar8 + (int)ppStack_1c * 4);
          uStack_10 = uVar13;
          uVar7 = CBankFile::GetEntryDataType((CBankFile *)this,uVar13);
          if (uVar7 == 0) {
            iVar8 = *(int *)(*(int *)(this + 0x1e0) + uVar13 * 4);
          }
          else {
            iVar8 = 0;
          }
          iVar2 = *(int *)(this + 0x204);
          puVar12 = *(undefined4 **)param_2;
          iVar8 = *(int *)(iVar8 + 4) * 4;
          puVar3 = *(undefined4 **)(iVar2 + iVar8);
          ppVar15 = extraout_EDX_00;
          if ((puVar3 == puVar12) ||
             ((((puVar3 != (undefined4 *)0x0 && (puVar12 != (undefined4 *)0x0)) &&
               (puVar3[1] == puVar12[1])) &&
              (lVar9 = CBasicString<char>::Compare((char *)*puVar3,(char *)*puVar12),
              ppVar15 = extraout_EDX_01, lVar9 == 0)))) {
LAB_009c9405:
            bVar5 = true;
          }
          else {
            bVar6 = true;
            uVar19 = FUN_0099f600(&DAT_012783c8);
            ppVar15 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar19 >> 0x20);
            puVar12 = *(undefined4 **)(iVar2 + iVar8);
            puVar3 = (undefined4 *)*(int *)uVar19;
            if ((puVar12 == puVar3) ||
               (((puVar12 != (undefined4 *)0x0 && (puVar3 != (undefined4 *)0x0)) &&
                ((ppVar15 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar12[1],
                 ppVar15 == (pair<EHeroMorphType,CParticleMorphs::CEntry> *)puVar3[1] &&
                 (lVar9 = CBasicString<char>::Compare((char *)*puVar12,(char *)*puVar3),
                 ppVar15 = extraout_EDX_02, lVar9 == 0)))))) goto LAB_009c9405;
            bVar5 = false;
          }
          if (bVar6) {
            bVar6 = false;
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_14,ppVar15,unaff_EDI);
          }
          if (bVar5) {
            FUN_005178a0(&uStack_10);
          }
          iVar8 = *(int *)(param_1 + 0x1c);
          ppStack_1c = ppStack_1c + 1;
          ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)
                    (*(int *)(param_1 + 0x20) - iVar8 >> 2);
          puVar17 = local_8;
        } while (ppStack_1c < ppVar14);
      }
      piVar1 = (int *)(param_1 + 4);
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*(int *)param_1 + 4))();
        ppVar14 = extraout_EDX_03;
      }
      if (local_c != puVar17) {
        uVar18 = (int)puVar17 - (int)local_c >> 2;
        *param_3 = true;
        uVar13 = *local_c;
        ppVar16 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)0x0;
        if (1 < (int)uVar18) {
          do {
            uVar13 = *(int *)(this + 0x210) * 0x24a1 + 0x24df;
            *(ulong *)(this + 0x210) = uVar13;
            uVar10 = GFROR13(uVar13);
            ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)0x0;
            *(uint *)(this + 0x210) = uVar10;
            if (uVar18 != 0) {
              ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)(uVar10 % uVar18);
            }
            uVar13 = local_c[(int)ppVar14];
            ppVar16 = ppVar14;
          } while ((param_4 != (long *)0x0) && (uVar13 == *param_4));
        }
        std::
        _Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>
                  ((allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_> *)this,ppVar14)
        ;
        pCVar11 = GetTextInfo(this,uVar13);
        if (((*(int *)pCVar11 != 0) &&
            ((this_00 = *(CBankFileAsyncData **)(*(int *)pCVar11 + 0x38),
             this_00 == (CBankFileAsyncData *)0x0 ||
             (bVar6 = CBankFileAsyncData::IsFinished(this_00), bVar6)))) || (param_5 != false)) {
          if (param_4 != (long *)0x0) {
            *param_4 = uVar13;
          }
LAB_009c9581:
          puVar12 = (undefined4 *)GetPGroupEntry(this,(ulong)&param_3,SUB41(uVar13,0));
          pCVar4 = (CTextEntry *)*puVar12;
          if (param_3 != (bool *)0x0) {
            iVar8 = *(int *)(param_3 + 4);
            *(int *)(param_3 + 4) = iVar8 + -1;
            if (iVar8 + -1 == 0) {
              (**(code **)(*(int *)param_3 + 4))();
            }
            param_3 = (bool *)0x0;
          }
          free(local_c);
          return pCVar4;
        }
        BeginLoadingText(this,uVar13);
        param_3 = (bool *)0x0;
        ppVar14 = extraout_EDX_04;
        if (0 < (int)uVar18) {
          do {
            uVar13 = local_c[(int)ppVar16];
            std::
            _Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>
                      ((allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_> *)this,
                       ppVar14);
            pCVar11 = GetTextInfo(this,uVar13);
            bVar6 = CDataBankTextEntry::IsLoaded(pCVar11);
            if (bVar6) {
              if (param_4 != (long *)0x0) {
                *param_4 = uVar13;
              }
              goto LAB_009c9581;
            }
            ppVar14 = (pair<long,CMusicManager::CRegisteredMusicEntry> *)
                      ((int)(ppVar16 + 1) % (int)uVar18);
            param_3 = param_3 + 1;
            ppVar16 = ppVar14;
          } while ((int)param_3 < (int)uVar18);
        }
      }
      if (local_c != (ulong *)0x0) {
        free(local_c);
      }
    }
  }
  return (CTextEntry *)0x0;
}



//=== GetTextBySymbol @ 009c95e0 ===

/* [bsim sim=0.5095848107478536 <- ego_r]
   public: class CWideString const __thiscall NGameText::CDataBank::GetTextBySymbol(class
   CCharString const &)const  */

CCharString * __thiscall NGameText::CDataBank::GetTextBySymbol(CDataBank *this,CCharString *param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  ulong uVar4;
  ulong uVar5;
  int *in_stack_00000008;
  
  puVar2 = (undefined4 *)*in_stack_00000008;
  if (puVar2 == (undefined4 *)0x0) {
    uVar5 = 0;
    puVar3 = &DAT_0129aaf4;
  }
  else {
    uVar5 = puVar2[1];
    puVar3 = (undefined1 *)*puVar2;
  }
  uVar5 = CCRC::Calc(0,puVar3,uVar5);
  uVar5 = FUN_009cc410(uVar5);
  if (((uVar5 != 0) && (uVar4 = CBankFile::GetEntryDataType((CBankFile *)this,uVar5), uVar4 == 0))
     && (GetPGroupEntry(this,(ulong)&stack0x00000008,SUB41(uVar5,0)),
        in_stack_00000008 != (int *)0x0)) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)(in_stack_00000008 + 10));
    piVar1 = in_stack_00000008 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*in_stack_00000008 + 4))();
    }
    return param_1;
  }
  CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
  return param_1;
}



//=== FinishWithDynamicTextures @ 009f7330 ===

/* [bsim sim=0.5871331824267586 <- donor]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  FUN_009f7250();
  FUN_009f7830();
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  FUN_009f7830();
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== CTextContainer @ 009f8690 ===

/* [bsim sim=0.5053976663219188 <- donor]
   public: __thiscall NUISystem::CTextContainer::CTextContainer(class NUISystem::CTextContainer
   const &) */

CTextContainer * __thiscall
NUISystem::CTextContainer::CTextContainer(CTextContainer *this,CTextContainer *param_1)

{
  void *pvVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129c7e4;
  *(undefined4 *)(this + 4) = 0;
  pvVar1 = malloc(0x40);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 4) = pvVar1;
  return this;
}



//=== CTexture @ 009f9d00 ===

/* [bsim sim=0.8464702549207161 <- ego_r]
   public: __thiscall CTexture::CTexture(class CTexture const &) */

CTexture * __thiscall CTexture::CTexture(CTexture *this,CTexture *param_1)

{
  int *piVar1;
  uint uVar2;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  uVar2 = *(uint *)(this + 4) ^ (*(uint *)(this + 4) ^ *(uint *)(param_1 + 4)) & 0xfffffff;
  *(uint *)(this + 4) = uVar2;
  piVar1 = *(int **)this;
  *(uint *)(this + 4) = (*(uint *)(param_1 + 4) ^ uVar2) & 0xfffffff ^ *(uint *)(param_1 + 4);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 4))(piVar1);
  }
  return this;
}



//=== GetTextureType @ 009fd2b0 ===

/* [bsim sim=1.0 <- ego_r]
   private: enum CGraphicDataBank::ETextureType __thiscall CGraphicDataBank::GetTextureType(class
   CGraphicDataBank::CGraphicHeader const &,bool)const  */

ETextureType __thiscall
CGraphicDataBank::GetTextureType(CGraphicDataBank *this,CGraphicHeader *param_1,bool param_2)

{
  CGraphicHeader CVar1;
  bool bVar2;
  bool bVar3;
  ETextureType EVar4;
  ETextureType EVar5;
  ETextureType EVar6;
  ETextureType EVar7;
  
  CVar1 = param_1[0x10];
  bVar2 = CPixelFormat::IsCompressed((CPixelFormat *)(param_1 + 0xc));
  bVar3 = CPixelFormat::IsSigned((CPixelFormat *)(param_1 + 0xc));
  if (bVar3) {
    EVar5 = 10;
    EVar6 = 10;
    EVar4 = 10;
    EVar7 = 10;
  }
  else if (param_2) {
    if (bVar2) {
      EVar5 = 5;
      EVar6 = 5;
      EVar4 = 5;
      EVar7 = 5;
    }
    else {
      EVar5 = 9;
      EVar6 = 9;
      EVar4 = 9;
      EVar7 = 9;
    }
  }
  else if (bVar2) {
    EVar5 = 1;
    EVar6 = 2;
    EVar4 = 1;
    EVar7 = 3;
  }
  else {
    EVar5 = 7;
    EVar6 = 7;
    EVar4 = 7;
    EVar7 = 7;
  }
  switch(CVar1) {
  case (CGraphicHeader)0x1:
    return EVar5;
  case (CGraphicHeader)0x2:
    return EVar6;
  case (CGraphicHeader)0x3:
    return EVar7;
  default:
    return EVar4;
  }
}



//=== CPreallocTexturePool @ 00a3aea0 ===

/* [bsim sim=0.6199039603577671 <- ego_r]
   public: __thiscall CPreallocTexturePool::CPreallocTexturePool(void) */

CPreallocTexturePool * __thiscall
CPreallocTexturePool::CPreallocTexturePool(CPreallocTexturePool *this)

{
  vector<unsigned_char,std::allocator<unsigned_char>_> *this_00;
  uint in_stack_00000004;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  this_00 = (vector<unsigned_char,std::allocator<unsigned_char>_> *)(this + 0x10);
  *(undefined ***)this = &PTR__scalar_deleting_destructor__0129b424;
  *(undefined4 *)this_00 = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  std::vector<unsigned_char,std::allocator<unsigned_char>_>::resize(this_00,in_stack_00000004);
  *(uint *)(this + 8) = in_stack_00000004;
  *(undefined4 *)(this + 4) = *(undefined4 *)this_00;
  return this;
}



//=== CTextureRenderer @ 00a3b510 ===

/* [bsim sim=0.569559329002901 <- donor]
   public: __thiscall CTextureRenderer::CTextureRenderer(struct IUnknown *,long *,class CTexture *)
    */

CTextureRenderer * __thiscall
CTextureRenderer::CTextureRenderer
          (CTextureRenderer *this,IUnknown *param_1,long *param_2,CTexture *param_3)

{
  NUISystem::CSpellContainerList::CSpellContainerList((CSpellContainerList *)this,0x129d150);
  *(undefined ***)this = &PTR_LAB_0129d08c;
  *(undefined ***)(this + 0xc) = &PTR_LAB_0129d04c;
  *(undefined ***)(this + 0x10) = &PTR_LAB_0129d030;
  *(undefined ***)(this + 0xe0) = &PTR_LAB_0129d008;
  *(undefined ***)(this + 0xe4) = &PTR_LAB_0129cff0;
  *(undefined4 *)(this + 0x160) = 0;
  this[0x178] = (CTextureRenderer)0x0;
  *param_2 = 0;
  *(CTexture **)(this + 0x164) = param_3;
  if (param_3 == (CTexture *)0x0) {
    this[0x178] = (CTextureRenderer)0x1;
  }
  return this;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 00a3cbe0 ===

/* [bsim sim=0.5471529583600682 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  undefined4 uVar1;
  
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined ***)param_1 = &PTR_LAB_012354d0;
      uVar1 = *(undefined4 *)(param_3 + 4);
      *(undefined ***)param_1 = &PTR_LAB_01249f50;
      *(undefined4 *)(param_1 + 4) = uVar1;
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined ***)param_1 = &PTR_LAB_01265128;
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 00a3cee0 ===

/* [bsim sim=0.9020090206280044 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_1 != param_2; param_1 = param_1 + 0x14) {
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
    *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
    *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
  }
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00a41c20 ===

/* [bsim sim=0.6419571714072203 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,undefined4 param_3)

{
  CCharString *pCVar1;
  CCharString aCStack_24 [4];
  CCharString local_20 [4];
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  if (1 < (param_2 - param_1) / 0xc) {
    pCVar1 = (CCharString *)(param_2 + -0xc);
    do {
      uStack_14 = 0;
      uStack_18 = param_3;
      CCharString::CCharString(aCStack_24,pCVar1);
      CCharString::CCharString(local_20,pCVar1 + 4);
      local_1c = *(undefined4 *)(pCVar1 + 8);
      _Pop_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>(param_1,pCVar1,pCVar1);
      pCVar1 = pCVar1 + -0xc;
    } while (1 < (int)(pCVar1 + (0xc - param_1)) / 0xc);
  }
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00a466f0 ===

/* [bsim sim=0.657341010056407 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (CFlammableDef *param_1,int param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  CCharString aCStack_30 [4];
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  if (1 < (param_2 - (int)param_1) / 0x14) {
    puVar1 = (undefined4 *)(param_2 + -4);
    do {
      uStack_18 = 0;
      uStack_1c = param_3;
      CCharString::CCharString(aCStack_30,(CCharString *)(puVar1 + -4));
      local_2c = puVar1[-3];
      local_28 = puVar1[-2];
      local_24 = puVar1[-1];
      local_20 = *puVar1;
      CFlammableDef::operator=(param_1,(CFlammableDef *)(puVar1 + -4));
      puVar1 = puVar1 + -5;
    } while (1 < ((4 - (int)param_1) + (int)puVar1) / 0x14);
  }
  return;
}



//=== GetTexturesInUse @ 00a69ed0 ===

/* [bsim sim=1.0 <- ego_r]
   public: unsigned long __thiscall CPreallocTexturePool::GetTexturesInUse(unsigned long)const  */

ulong __thiscall CPreallocTexturePool::GetTexturesInUse(CPreallocTexturePool *this,ulong param_1)

{
  CResource *this_00;
  ulong uVar1;
  ulong uVar2;
  uint uVar3;
  
  uVar2 = 0;
  uVar3 = 0;
  if (*(int *)(this + 0x3c) != 0) {
    do {
      this_00 = *(CResource **)(*(int *)(this + 0x48) + uVar3 * 4);
      if ((this_00[0x48] != (CResource)0x0) &&
         (uVar1 = CResource::GetUnusedFrames(this_00), uVar1 <= param_1)) {
        uVar2 = uVar2 + 1;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < *(uint *)(this + 0x3c));
  }
  return uVar2;
}



//=== CTextureManager @ 00a6a360 ===

/* [bsim sim=0.8324199570525386 <- ego_r]
   public: __thiscall CTextureManager::CTextureManager(void) */

CTextureManager * __thiscall CTextureManager::CTextureManager(CTextureManager *this)

{
  CTextureManager *pCVar1;
  CTextureManager *this_00;
  int local_4;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  this_00 = this + 0x4c;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129dc5c;
  this[4] = (CTextureManager)0x0;
  *(undefined4 *)(this + 8) = 0;
  pCVar1 = this + 0x9c;
  local_4 = 0x10;
  do {
    CResourceList::CResourceList((CResourceList *)this_00);
    *(undefined ***)this_00 = &PTR_FUN_0129dc50;
    *(undefined4 *)(pCVar1 + -0x14) = 0;
    *(undefined4 *)(pCVar1 + -0x10) = 0;
    *(undefined4 *)(pCVar1 + -0xc) = 0xffffffff;
    *(undefined4 *)pCVar1 = 0;
    *(undefined4 *)(pCVar1 + -8) = 0;
    *(undefined4 *)pCVar1 = 0;
    *(undefined4 *)(pCVar1 + -4) = 0;
    this_00 = this_00 + 0x54;
    pCVar1 = pCVar1 + 0x54;
    local_4 = local_4 + -1;
  } while (local_4 != 0);
  *(undefined4 *)(this + 0x5cc) = 0;
  *(undefined4 *)(this + 0x5d0) = 0;
  *(undefined4 *)(this + 0x58c) = 0;
  *(undefined4 *)(this + 0x590) = 0;
  *(undefined4 *)(this + 0x594) = 0;
  *(undefined4 *)(this + 0x598) = 0;
  *(undefined4 *)(this + 0x59c) = 0;
  *(undefined4 *)(this + 0x5a0) = 0;
  *(undefined4 *)(this + 0x5a4) = 0;
  *(undefined4 *)(this + 0x5a8) = 0;
  *(undefined4 *)(this + 0x5ac) = 0;
  *(undefined4 *)(this + 0x5b0) = 0;
  *(undefined4 *)(this + 0x5b4) = 0;
  *(undefined4 *)(this + 0x5b8) = 0;
  *(undefined4 *)(this + 0x5bc) = 0;
  *(undefined4 *)(this + 0x5c0) = 0;
  *(undefined4 *)(this + 0x5c4) = 0;
  *(undefined4 *)(this + 0x5c8) = 0;
  return this;
}



//=== ~CTextureManager @ 00a6a440 ===

/* [bsim sim=0.7020253117106521 <- ego_r]
   public: virtual __thiscall CTextureManager::~CTextureManager(void) */

void __thiscall CTextureManager::~CTextureManager(CTextureManager *this)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  CTextureManager *pCVar5;
  int local_4;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129dc5c;
  if (*(int *)(this + 0x5d0) != 0) {
    *(undefined4 *)(this + 0x5d0) = 0;
  }
  this[4] = (CTextureManager)0x0;
  local_4 = 0x10;
  pCVar5 = this + 0x58c;
  do {
    piVar2 = *(int **)(pCVar5 + -8);
    for (piVar4 = *(int **)(pCVar5 + -0xc); piVar4 != piVar2; piVar4 = piVar4 + 1) {
      piVar3 = (int *)*piVar4;
      if (piVar3 != (int *)0x0) {
        piVar1 = piVar3 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piVar3 + 4))();
        }
        *piVar4 = 0;
      }
    }
    if (*(void **)(pCVar5 + -0xc) != (void *)0x0) {
      free(*(void **)(pCVar5 + -0xc));
    }
    ::Stack<unsigned_long,64>___Stack<unsigned_long,64>();
    local_4 = local_4 + -1;
    pCVar5 = pCVar5 + -0x54;
  } while (local_4 != 0);
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00a719a0 ===

/* [bsim sim=0.657341010056407 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (CFlammableDef *param_1,int param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  CCharString aCStack_30 [4];
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  if (1 < (param_2 - (int)param_1) / 0x14) {
    puVar1 = (undefined4 *)(param_2 + -8);
    do {
      uStack_18 = 0;
      uStack_1c = param_3;
      CCharString::CCharString(aCStack_30,(CCharString *)(puVar1 + -3));
      local_2c = puVar1[-2];
      local_28 = puVar1[-1];
      local_24 = *puVar1;
      local_20 = puVar1[1];
      CFlammableDef::operator=(param_1,(CFlammableDef *)(puVar1 + -3));
      puVar1 = puVar1 + -5;
    } while (1 < ((8 - (int)param_1) + (int)puVar1) / 0x14);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00a851e0 ===

/* [bsim sim=0.5155588620640611 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  undefined4 uStack_4;
  
  uStack_4 = this;
  std::
  _Move_backward<std::pair<unsigned_long,CSoundPair>*,std::pair<unsigned_long,class_CSoundPair>*>
            (*(undefined4 *)(this + 0x18),*(undefined4 *)(this + 0x1c),(int)&uStack_4 + 3);
  if (*(void **)(this + 0x18) != (void *)0x0) {
    free(*(void **)(this + 0x18));
  }
  FUN_00a827d0((int)&uStack_4 + 3);
  if (*(void **)(this + 0xc) != (void *)0x0) {
    free(*(void **)(this + 0xc));
  }
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00a96da0 ===

/* [bsim sim=0.5952916403734707 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  LTextTreeWalkThrough *this_00;
  
  this_00 = (LTextTreeWalkThrough *)(this + 0xc);
  if (*(int *)(this + 0x10) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    *(undefined4 *)(*(int *)this_00 + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 0x10) = 0;
  }
  if (*(void **)this_00 != (void *)0x0) {
    free(*(void **)this_00);
  }
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00a97850 ===

/* [bsim sim=0.6780076129905211 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  *(undefined ***)this = &PTR_LAB_012354d0;
  return;
}



//=== FinishWithDynamicTextures @ 00a978c0 ===

/* [bsim sim=0.6780076129905211 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 8) != (void *)0x0) {
    free(*(void **)(this + 8));
  }
  *(undefined ***)this = &PTR_LAB_012354d0;
  return;
}



//=== FinishWithDynamicTextures @ 00aa7370 ===

/* [bsim sim=0.5921183276935108 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  byte in_stack_00000004;
  
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    free(*(void **)(this + 0x1c));
  }
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)this);
  if ((in_stack_00000004 & 1) != 0) {
    operator_delete(this);
  }
  return;
}



//=== ?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z @ 00ab7c20 ===

void _Render_CStaticFontBank__UBEXABVCWideString__ABVC3DVector__MABVCRGBColour__KPBVCTexture___Z
               (CCharString *param_1,float *param_2,byte param_3,undefined4 *param_4,byte param_5,
               float param_6)

{
  ushort *puVar1;
  float fVar2;
  ushort uVar3;
  float fVar4;
  bool bVar5;
  ulong uVar6;
  CThingFilter_IsValid *this;
  undefined3 extraout_var;
  ushort *puVar8;
  CVertexBufferWin32 *this_00;
  undefined3 extraout_var_00;
  int *in_ECX;
  uint uVar9;
  int iVar10;
  int iVar11;
  float unaff_EBX;
  ulong unaff_ESI;
  float *pfVar12;
  CTexture **ppCVar13;
  CRenderManagerCore *unaff_EDI;
  float *pfVar14;
  undefined4 *puVar15;
  double dVar16;
  int unaff_retaddr;
  CThing *pCVar17;
  undefined8 uVar18;
  CRenderManagerCore *pCVar19;
  CRenderManagerCore *pCStack_158;
  int iStack_154;
  float fStack_150;
  CRenderManagerCore *pCStack_148;
  float fStack_144;
  ulong uStack_13c;
  CThingFilter_IsValid *pCStack_134;
  float fStack_120;
  CEngineInternalPrimitiveBase *pCStack_11c;
  float fStack_118;
  float fStack_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  CTexture *pCStack_108;
  float fStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  CTexture *pCStack_f0;
  float fStack_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  float fStack_e0;
  float fStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  float fStack_d0;
  float fStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  CTexture *pCStack_c0;
  float fStack_bc;
  undefined4 uStack_b8;
  undefined4 uStack_b4;
  CTexture *pCStack_b0;
  float fStack_ac;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  int iStack_a0;
  float fStack_9c;
  float fStack_98;
  float afStack_94 [4];
  int iStack_84;
  float fStack_80;
  float fStack_7c;
  CTexture *pCStack_78;
  float fStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  int iStack_68;
  float fStack_64;
  float fStack_60;
  float afStack_5c [4];
  int iStack_4c;
  float fStack_48;
  float fStack_44;
  float afStack_40 [4];
  int iStack_30;
  float fStack_2c;
  float fStack_28;
  CTexture *pCStack_24;
  float fStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  int iStack_14;
  float fStack_10;
  float fStack_c;
  undefined4 *puVar7;
  
  pCStack_158 = (CRenderManagerCore *)*param_2;
  fVar2 = param_2[1];
  pCStack_11c = (CEngineInternalPrimitiveBase *)*param_4;
  uVar6 = CEngineInternalPrimitiveBase::AddChildPrimitive
                    (pCStack_11c,(CEnginePrimitive *)unaff_EDI,unaff_ESI);
  iStack_154 = *(int *)(*(int *)(uVar6 + 0x60) + 8);
  pCStack_f0 = (CTexture *)in_ECX[0x80c];
  fStack_ec = param_6;
  iVar11 = (param_6 != 0.0) + 1;
  this = (CThingFilter_IsValid *)CFontManager::GetFreeVertexBuffer((CFontManager *)in_ECX[3]);
  if ((param_5 & 8) != 0) {
    iVar10 = *in_ECX;
    CCharString::operator_char_const_(param_1);
    (**(code **)(iVar10 + 4))();
    floor((double)(fVar2 - fStack_114 * 0.5));
  }
  uVar18 = 0;
  bVar5 = CThingFilter_IsValid::operator()(this,(CThing *)0x0);
  puVar7 = (undefined4 *)CONCAT31(extraout_var,bVar5);
  uStack_13c = 1;
  puVar8 = (ushort *)CCharString::operator_char_const_(param_1);
  pCVar17 = (CThing *)uVar18;
  uVar3 = *puVar8;
  pCStack_148 = pCStack_158;
  while (uVar3 != 0) {
    if (uStack_13c != 0) {
      if ((param_3 & 6) != 0) {
        (**(code **)(*in_ECX + 4))(&fStack_120,puVar8);
        if ((param_3 & 2) != 0) {
          dVar16 = floor((double)(fStack_150 - fStack_120));
          unaff_EBX = (float)dVar16;
        }
        if ((param_3 & 4) != 0) {
          dVar16 = floor((double)(fStack_150 - fStack_120 * 0.5));
          unaff_EBX = (float)dVar16;
        }
      }
      uStack_13c = 0;
    }
    uVar3 = *puVar8;
    puVar8 = puVar8 + 1;
    if (uVar3 == 10) {
      fVar2 = (float)in_ECX[1];
      if (in_ECX[1] < 0) {
        fVar2 = fVar2 + 4.2949673e+09;
      }
      pCStack_148 = (CRenderManagerCore *)(fVar2 + (float)pCStack_148);
      uStack_13c = 1;
      unaff_EBX = fStack_150;
    }
    uVar9 = (uint)uVar3;
    pCVar19 = unaff_EDI;
    if ((in_ECX[9] <= (int)uVar9) && ((int)uVar9 <= in_ECX[10])) {
      puVar1 = (ushort *)(in_ECX + (uint)(uVar3 >> 6) * 2 + 0xc);
      if ((*(int *)(puVar1 + 2) != 0) &&
         ((uVar9 = (uVar9 & 0x3f) - (uint)*puVar1, uVar9 < puVar1[1] &&
          (pfVar12 = (float *)(*(int *)(puVar1 + 2) + uVar9 * 0x18), pfVar12 != (float *)0x0)))) {
        pCStack_108 = (CTexture *)
                      (((((float)(int)*(short *)(pfVar12 + 4) + unaff_EBX) - fStack_150) *
                        (float)param_1 + fStack_150) - 0.5);
        fStack_118 = ((float)(int)*(short *)((int)pfVar12 + 0x12) + 1.0) * (float)param_1 +
                     (float)pCStack_108;
        fStack_114 = (((float)pCStack_148 - fStack_144) * (float)param_1 + fStack_144) - 0.5;
        fVar2 = (float)in_ECX[1];
        if (in_ECX[1] < 0) {
          fVar2 = fVar2 + 4.2949673e+09;
        }
        uStack_b4 = 0x3f800000;
        fStack_dc = (fVar2 + 1.0) * (float)param_1 + fStack_114;
        unaff_EBX = (float)(int)*(short *)(pfVar12 + 5) +
                    (float)(int)*(short *)(pfVar12 + 4) + unaff_EBX;
        uStack_e4 = 0x3f800000;
        fVar2 = (float)(in_ECX[7] + -1);
        fStack_2c = (float)in_ECX[7];
        fStack_9c = (fVar2 * *pfVar12) / fStack_2c;
        pCStack_158 = (CRenderManagerCore *)(float)(in_ECX[8] + -1);
        fVar4 = (float)in_ECX[8];
        fStack_60 = ((float)pCStack_158 * pfVar12[1]) / fVar4;
        fStack_2c = (fVar2 * pfVar12[2]) / fStack_2c;
        uStack_b8 = *(undefined4 *)(unaff_retaddr + 8);
        uStack_a8 = uStack_b8;
        fVar4 = ((float)pCStack_158 * pfVar12[3]) / fVar4;
        pCStack_b0 = pCStack_108;
        fStack_ac = fStack_dc;
        fStack_98 = fVar4;
        uStack_a4 = 0x3f800000;
        iStack_a0 = iVar11;
        ppCVar13 = &pCStack_b0;
        puVar15 = puVar7;
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *puVar15 = *ppCVar13;
          ppCVar13 = ppCVar13 + 1;
          puVar15 = puVar15 + 1;
        }
        uStack_e8 = *(undefined4 *)(unaff_retaddr + 8);
        pCStack_78 = pCStack_108;
        fStack_74 = fStack_114;
        uStack_70 = uStack_e8;
        fStack_64 = fStack_9c;
        uStack_6c = 0x3f800000;
        fStack_28 = fVar4;
        fStack_80 = fStack_2c;
        iStack_68 = iVar11;
        ppCVar13 = &pCStack_78;
        puVar15 = puVar7 + 7;
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *puVar15 = *ppCVar13;
          ppCVar13 = ppCVar13 + 1;
          puVar15 = puVar15 + 1;
        }
        uStack_c8 = *(undefined4 *)(unaff_retaddr + 8);
        afStack_40[0] = fStack_118;
        afStack_40[1] = fStack_dc;
        afStack_40[2] = (float)uStack_c8;
        uStack_c4 = 0x3f800000;
        afStack_40[3] = 1.0;
        iStack_30 = iVar11;
        pfVar12 = afStack_40;
        pfVar14 = (float *)(puVar7 + 0xe);
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *pfVar14 = *pfVar12;
          pfVar12 = pfVar12 + 1;
          pfVar14 = pfVar14 + 1;
        }
        uStack_110 = *(undefined4 *)(unaff_retaddr + 8);
        afStack_94[0] = fStack_118;
        afStack_94[1] = fStack_114;
        afStack_94[2] = (float)uStack_110;
        uStack_10c = 0x3f800000;
        afStack_94[3] = 1.0;
        fStack_7c = fStack_60;
        iStack_84 = iVar11;
        pfVar12 = afStack_94;
        pfVar14 = (float *)(puVar7 + 0x15);
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *pfVar14 = *pfVar12;
          pfVar12 = pfVar12 + 1;
          pfVar14 = pfVar14 + 1;
        }
        uStack_d8 = *(undefined4 *)(unaff_retaddr + 8);
        afStack_5c[0] = fStack_118;
        afStack_5c[1] = fStack_dc;
        afStack_5c[2] = (float)uStack_d8;
        uStack_d4 = 0x3f800000;
        afStack_5c[3] = 1.0;
        fStack_48 = fStack_2c;
        iStack_4c = iVar11;
        fStack_44 = fVar4;
        pfVar12 = afStack_5c;
        pfVar14 = (float *)(puVar7 + 0x1c);
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *pfVar14 = *pfVar12;
          pfVar12 = pfVar12 + 1;
          pfVar14 = pfVar14 + 1;
        }
        uStack_100 = *(undefined4 *)(unaff_retaddr + 8);
        fStack_20 = fStack_114;
        uStack_fc = 0x3f800000;
        pCStack_24 = pCStack_108;
        uStack_18 = 0x3f800000;
        uStack_1c = uStack_100;
        iStack_154 = iStack_154 + 2;
        puVar15 = puVar7 + 0x23;
        iStack_14 = iVar11;
        fStack_10 = fStack_9c;
        puVar7 = puVar7 + 0x2a;
        fStack_c = fStack_60;
        ppCVar13 = &pCStack_24;
        for (iVar10 = 7; iVar10 != 0; iVar10 = iVar10 + -1) {
          *puVar15 = *ppCVar13;
          ppCVar13 = ppCVar13 + 1;
          puVar15 = puVar15 + 1;
        }
        fStack_104 = fStack_114;
        pCStack_f0 = pCStack_108;
        fStack_ec = fStack_114;
        fStack_e0 = fStack_118;
        fStack_d0 = fStack_118;
        fStack_cc = fStack_dc;
        pCStack_c0 = pCStack_108;
        fStack_bc = fStack_dc;
        if (0x126 < (uint)(iStack_154 * 3)) {
          CThingFilter_IsValid::operator()(pCStack_134,(CThing *)uVar18);
          if (pCStack_134 == (CThingFilter_IsValid *)0x0) {
            (**(code **)(**(int **)(pCStack_158 + 0x3cf0) + 400))
                      (*(int **)(pCStack_158 + 0x3cf0),0,0,0,0);
            pCVar19 = unaff_EDI;
          }
          else {
            pCVar19 = unaff_EDI;
            (**(code **)(**(int **)(pCStack_158 + 0x3cf0) + 400))
                      (*(int **)(pCStack_158 + 0x3cf0),0,*(undefined4 *)(pCStack_134 + 4),0,
                       pCStack_134[0x10]);
            unaff_EDI = pCStack_158;
          }
          CRenderManagerCore::DrawCurrentBuffersAsTris
                    (unaff_EDI,&pCStack_108,uStack_13c,0,(ulong)fVar4);
          (**(code **)(**(int **)(unaff_EDI + 0x3cf0) + 400))(*(int **)(unaff_EDI + 0x3cf0),0,0,0,0)
          ;
          this_00 = CFontManager::GetFreeVertexBuffer((CFontManager *)in_ECX[3]);
          uVar18 = 0;
          bVar5 = CThingFilter_IsValid::operator()((CThingFilter_IsValid *)this_00,(CThing *)0x0);
          puVar7 = (undefined4 *)CONCAT31(extraout_var_00,bVar5);
          iStack_154 = 0;
        }
      }
    }
    pCVar17 = (CThing *)uVar18;
    this = pCStack_134;
    unaff_EDI = pCVar19;
    uVar3 = *puVar8;
  }
  CThingFilter_IsValid::operator()(this,pCVar17);
  if (fStack_150 != 0.0) {
    if (this == (CThingFilter_IsValid *)0x0) {
      pCVar19 = pCStack_158 + 0x3cf0;
      pCStack_158 = unaff_EDI;
      (**(code **)(**(int **)pCVar19 + 400))(*(int **)pCVar19,0,0,0,0);
    }
    else {
      (**(code **)(**(int **)(pCStack_158 + 0x3cf0) + 400))
                (*(int **)(pCStack_158 + 0x3cf0),0,*(undefined4 *)(this + 4),0,this[0x10]);
    }
    CRenderManagerCore::DrawCurrentBuffersAsTris
              (pCStack_158,&pCStack_108,uStack_13c,0,(ulong)fStack_150);
    (**(code **)(**(int **)(pCStack_158 + 0x3cf0) + 400))(*(int **)(pCStack_158 + 0x3cf0),0,0,0,0);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00ac2a30 ===

/* [bsim sim=0.6648654970810962 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  CEngineScreenEffectDisplacementRenderer *pCVar1;
  
  pCVar1 = *(CEngineScreenEffectDisplacementRenderer **)(this + 0x3c);
  if (pCVar1 != (CEngineScreenEffectDisplacementRenderer *)0x0) {
    FinishWithDynamicTextures(pCVar1);
    operator_delete(pCVar1);
  }
  pCVar1 = *(CEngineScreenEffectDisplacementRenderer **)(this + 0x40);
  if (pCVar1 != (CEngineScreenEffectDisplacementRenderer *)0x0) {
    FinishWithDynamicTextures(pCVar1);
    operator_delete(pCVar1);
  }
  if (*(void **)(this + 0x30) != (void *)0x0) {
    free(*(void **)(this + 0x30));
  }
  return;
}



//=== ?AddCustomMorphTexture@CIEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z @ 00b23460 ===

undefined4 _AddCustomMorphTexture_CIEngine__UBEKAAVCTexture__ABVCCharString___Z(void)

{
  return 0;
}



//=== ?SetRenderTargetTexture@CIEngine@@UAEXPBVCTexture@@@Z @ 00b23660 ===

void _SetRenderTargetTexture_CIEngine__UAEXPBVCTexture___Z(void)

{
  return;
}



//=== ?AllocateTextureFromEngineResourcePools@CIEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z @ 00b23670 ===

undefined4
_AllocateTextureFromEngineResourcePools_CIEngine__UAEPAVCTexture__KABVC2DExtentsI__JABVCPixelFormat___Z
          (void)

{
  return 0;
}



//=== ?FreeTextureFromEngineResourcePools@CIEngine@@UAEXPAVCTexture@@@Z @ 00b23680 ===

void _FreeTextureFromEngineResourcePools_CIEngine__UAEXPAVCTexture___Z(void)

{
  return;
}



//=== ?RenderColorFilteredQuad@CIEngine@@UAEXMMABVC4DVector@@MMPAVCTexture@@@Z @ 00b23740 ===

void _RenderColorFilteredQuad_CIEngine__UAEXMMABVC4DVector__MMPAVCTexture___Z(void)

{
  return;
}



//=== ?AddCustomMorphTexture@CEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z @ 00b23ce0 ===

void _AddCustomMorphTexture_CEngine__UBEKAAVCTexture__ABVCCharString___Z
               (CTexture *param_1,CCharString *param_2)

{
  CEngineSurfaceCompositionManager::AddCustomMorphTexture(DAT_01436eac,param_1,param_2);
  return;
}



//=== ?SetRenderTargetTexture@CEngine@@UAEXPBVCTexture@@@Z @ 00b23ef0 ===

void _SetRenderTargetTexture_CEngine__UAEXPBVCTexture___Z(void)

{
  return;
}



//=== ?AllocateTextureFromEngineResourcePools@CEngine@@UAEPAVCTexture@@KABVC2DExtentsI@@JABVCPixelFormat@@@Z @ 00b23f40 ===

CTexture *
_AllocateTextureFromEngineResourcePools_CEngine__UAEPAVCTexture__KABVC2DExtentsI__JABVCPixelFormat___Z
          (undefined4 param_1,C2DExtentsI *param_2,long param_3,CPixelFormat *param_4)

{
  CTexture *this;
  
  this = operator_new(8);
  if (this == (CTexture *)0x0) {
    this = (CTexture *)0x0;
  }
  else {
    *(undefined4 *)this = 0;
    *(undefined4 *)(this + 4) = 0;
  }
  CTexture::Initialise(this,param_2,param_3,param_4,0,1,false,false);
  return this;
}



//=== ?FreeTextureFromEngineResourcePools@CEngine@@UAEXPAVCTexture@@@Z @ 00b23f50 ===

void _FreeTextureFromEngineResourcePools_CEngine__UAEXPAVCTexture___Z(CTexture *param_1)

{
  if (param_1 != (CTexture *)0x0) {
    CTexture::Uninitialise(param_1);
    operator_delete(param_1);
  }
  return;
}



//=== ?RenderColorFilteredQuad@CEngine@@UAEXMMABVC4DVector@@MMPAVCTexture@@@Z @ 00b25c00 ===

void _RenderColorFilteredQuad_CEngine__UAEXMMABVC4DVector__MMPAVCTexture___Z
               (float param_1,float param_2,C4DVector *param_3,float param_4,float param_5,
               CTexture *param_6)

{
  CRenderStateManager *pCVar1;
  long lVar2;
  CRenderStateManager *pCVar3;
  CEngine *in_ECX;
  
  pCVar3 = DAT_013cb508;
  pCVar1 = DAT_013cb508 + 0x2808;
  *(CRenderStateManager **)(DAT_013cb508 + *(int *)pCVar1 * 8 + 8) = DAT_013cb508 + 0x3a24;
  *(int *)pCVar1 = *(int *)pCVar1 + 1;
  *(int *)(pCVar3 + 0x2814) = *(int *)(pCVar3 + 0x2814) << 1;
  CEngine::SetDefaultRenderStates(in_ECX);
  lVar2 = DAT_013bc478;
  CShaderRenderManager::SetVertexShaderConstantLayout((CShaderRenderManager *)&DAT_013bc470,3);
  CEngineScreenEffectColourFilter::RenderColorFilteredQuad
            (DAT_01436e40,param_1,param_2,param_3,param_4,param_5,param_6);
  CShaderRenderManager::SetVertexShaderConstantLayout((CShaderRenderManager *)&DAT_013bc470,lVar2);
  CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 00b3e390 ===

/* [bsim sim=0.6737685012993061 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== BuildDepthPackingLookupTexture @ 00b4f420 ===

/* [bsim sim=1.0 <- ego_r]
   private: void __thiscall CEngineShadowRenderer::BuildDepthPackingLookupTexture(void) */

void __thiscall CEngineShadowRenderer::BuildDepthPackingLookupTexture(CEngineShadowRenderer *this)

{
  byte bVar1;
  int iVar2;
  CRGBColour local_30;
  byte local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  int local_2c [2];
  CPixelFormat local_24 [4];
  undefined **local_20 [4];
  CSurfaceLockedRect local_10 [16];
  
  CPixelFormat::Initialise(local_24,8,8,8,8,false);
  local_2c[0] = 0x100;
  local_2c[1] = 0x100;
  CTexture::Initialise
            ((CTexture *)(this + 0x32c),(C2DExtentsI *)local_2c,1,local_24,0,1,false,false);
  CTexture::GetNoLevels((CTexture *)(this + 0x32c));
  CSurface::Lock((CSurface *)local_20,(ulong)local_10);
  local_2c[0] = 0;
  do {
    iVar2 = 0;
    bVar1 = (byte)local_2c[0]._0_1_ & 0xfe;
    do {
      local_2e = (undefined1)iVar2;
      local_30 = (CRGBColour)0x0;
      local_2d = 0;
      local_2f = bVar1;
      CSurface::SetPixel((CSurface *)local_20,iVar2,local_2c[0],&local_30,local_10);
      iVar2 = iVar2 + 1;
    } while (iVar2 < 0x100);
    local_2c[0] = local_2c[0] + 1;
  } while (local_2c[0] < 0x100);
  CVertexBufferWin32::DoUnlock((CVertexBufferWin32 *)local_20);
  local_20[0] = &PTR__vector_deleting_destructor__0122f84c;
  CSurface::Uninitialise((CSurface *)local_20);
  return;
}



//=== FinishWithDynamicTextures @ 00b5bd30 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   private: void __thiscall CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectRadialBlurRenderer *this)

{
  if (*(int *)(this + 0xc) != 0) {
    ::_E2(*(undefined4 *)(this + 0x14));
    ::_E2(*(undefined4 *)(this + 0x18));
    *(undefined4 *)(this + 0xc) = 0;
    *(undefined4 *)(this + 0x10) = 0;
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b6c380 ===

/* [bsim sim=0.7192212874556934 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  if (*(void **)(this + 0x18) != (void *)0x0) {
    free(*(void **)(this + 0x18));
  }
  if (*(void **)(this + 0xc) != (void *)0x0) {
    free(*(void **)(this + 0xc));
  }
  if (*(void **)this != (void *)0x0) {
    free(*(void **)this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b6c9e0 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== InitialiseDynamicTextures @ 00b6d510 ===

/* [bsim sim=1.0 <- ego_r]
   private: void __thiscall CEngineWaterRenderer::InitialiseDynamicTextures(void) */

void __thiscall CEngineWaterRenderer::InitialiseDynamicTextures(CEngineWaterRenderer *this)

{
  long lVar1;
  CGPUScratchBufferTextureManager *this_00;
  CPixelFormat local_c [4];
  undefined4 local_8;
  undefined4 local_4;
  
  CPixelFormat::Initialise(local_c,0x15);
  this_00 = (CGPUScratchBufferTextureManager *)(DAT_01436e98 + 0x3190);
  lVar1 = CGPUScratchBufferTextureManager::AllocteMemoryPage(this_00,0,true);
  *(long *)(this + 0x1d8) = lVar1;
  local_8 = 0x80;
  local_4 = 0x80;
  lVar1 = CGPUScratchBufferTextureManager::AllocateTexture
                    (this_00,lVar1,(C2DExtentsI *)&local_8,3,local_c,1,false);
  *(long *)(this + 0x1ec) = lVar1;
  local_8 = 0x80;
  local_4 = 0x80;
  lVar1 = CGPUScratchBufferTextureManager::AllocateTexture
                    (this_00,*(long *)(this + 0x1d8),(C2DExtentsI *)&local_8,3,local_c,1,false);
  *(long *)(this + 500) = lVar1;
  local_8 = 0x80;
  local_4 = 0x80;
  lVar1 = CGPUScratchBufferTextureManager::AllocateTexture
                    (this_00,*(long *)(this + 0x1d8),(C2DExtentsI *)&local_8,3,local_c,1,false);
  *(long *)(this + 0x1e4) = lVar1;
  local_8 = 0x100;
  local_4 = 0x100;
  lVar1 = CGPUScratchBufferTextureManager::AllocateTexture
                    (this_00,*(long *)(this + 0x1d8),(C2DExtentsI *)&local_8,1,local_c,1,false);
  *(long *)(this + 0x1dc) = lVar1;
  return;
}



//=== FinishUsingDynamicTextures @ 00b6d630 ===

/* [bsim sim=1.0 <- ego_r]
   private: void __thiscall CEngineWaterRenderer::FinishUsingDynamicTextures(void) */

void __thiscall CEngineWaterRenderer::FinishUsingDynamicTextures(CEngineWaterRenderer *this)

{
  ::_E2(*(undefined4 *)(this + 0x1dc));
  *(undefined4 *)(this + 0x1e0) = 0;
  ::_E2(*(undefined4 *)(this + 0x1ec));
  *(undefined4 *)(this + 0x1f0) = 0;
  ::_E2(*(undefined4 *)(this + 0x1e4));
  *(undefined4 *)(this + 0x1e8) = 0;
  ::_E2(*(undefined4 *)(this + 500));
  *(undefined4 *)(this + 0x1f8) = 0;
  return;
}



//=== CreateDynamicTextures @ 00b72180 ===

/* WARNING: Removing unreachable block (ram,0x00b73704) */
/* WARNING: Removing unreachable block (ram,0x00b73708) */
/* [bsim sim=0.6374708292303011 <- ego_r]
   private: bool __thiscall CEngineWaterRenderer::CreateDynamicTextures(bool,bool,bool) */

bool __thiscall
CEngineWaterRenderer::CreateDynamicTextures
          (CEngineWaterRenderer *this,bool param_1,bool param_2,bool param_3)

{
  int *piVar1;
  CRenderStateManager *pCVar2;
  CTCActionUseReadable *pCVar3;
  uint uVar4;
  uint uVar5;
  CResource *pCVar6;
  CGraphicFrameData *pCVar7;
  int *piVar8;
  CRenderStateManager *pCVar9;
  CTCActionUseReadable *pCVar10;
  CEngineWaterRenderer *this_00;
  byte bVar11;
  byte bVar12;
  CFrameData *pCVar13;
  CSurface *pCVar14;
  long lVar15;
  uint *puVar16;
  int iVar17;
  float *pfVar18;
  float extraout_ECX;
  float extraout_ECX_00;
  float extraout_ECX_01;
  float extraout_ECX_02;
  float extraout_ECX_03;
  float extraout_ECX_04;
  float extraout_ECX_05;
  float extraout_ECX_06;
  byte bVar19;
  int unaff_EBX;
  int unaff_ESI;
  CGraphicInfoFrame *pCVar20;
  CGPUScratchBufferTextureManager *this_01;
  int iVar21;
  CEngineWaterRenderer *this_02;
  char cStack_d4;
  char cStack_d3;
  CGraphicFrameData *pCStack_d0;
  CEngineWaterRenderer *pCStack_c4;
  undefined4 uStack_ac;
  int iStack_a4;
  undefined **ppuStack_a0;
  undefined4 local_9c;
  CGraphicFrameData *local_98;
  CGraphicFrameData *local_94;
  CGraphicFrameData *local_90;
  CGraphicFrameData *local_8c;
  CGraphicFrameData *local_88;
  CEngineWaterRenderer *pCStack_84;
  undefined **ppuStack_80;
  undefined **ppuStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined **ppuStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined **appuStack_5c [4];
  undefined1 auStack_4c [8];
  undefined4 uStack_44;
  undefined4 uStack_40;
  float fStack_3c;
  CEngineWaterRenderer *pCStack_38;
  undefined1 auStack_34 [8];
  undefined1 auStack_2c [40];
  
  uVar5 = *(uint *)(DAT_01436e24 + 0xd8);
  iVar21 = *(int *)(DAT_01436e98 + 0x954);
  local_88 = (CGraphicFrameData *)0x0;
  local_94 = (CGraphicFrameData *)0x0;
  local_8c = (CGraphicFrameData *)0x0;
  local_90 = (CGraphicFrameData *)0x0;
  local_98 = (CGraphicFrameData *)0x0;
  if (uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c)) {
    uVar4 = uVar5 * 0x2c + *(int *)(iVar21 + 0x1e0);
    if (uVar4 == 0) goto LAB_00b7221d;
    pCVar20 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c
                                       )) & uVar4) + 0x28);
  }
  else {
LAB_00b7221d:
    pCVar20 = (CGraphicInfoFrame *)0x0;
  }
  pCVar13 = *(CFrameData **)(pCVar20 + 8);
  if (pCVar13 == (CFrameData *)0x0) {
    pCVar13 = ::operator_new(0x44);
    if (pCVar13 == (CFrameData *)0x0) {
      local_9c = (CFrameData *)0x0;
    }
    else {
      local_9c = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar13,pCVar20);
    }
    piVar8 = *(int **)(pCVar20 + 8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar8 + 4))();
      }
      *(undefined4 *)(pCVar20 + 8) = 0;
    }
    *(CFrameData **)(pCVar20 + 8) = local_9c;
    pCVar13 = local_9c;
LAB_00b72340:
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
    uStack_ac._3_1_ = 0;
  }
  else {
    if (*(int *)(pCVar13 + 0x1c) == 0) goto LAB_00b72340;
    *(undefined4 *)(*(int *)(pCVar20 + 8) + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(pCVar20 + 0xc) + 0x20) + 0x194);
    pCVar6 = *(CResource **)(*(int *)(pCVar20 + 8) + 0x24);
    if (((pCVar6 != (CResource *)0x0) && (*(int *)(pCVar6 + 8) != 0)) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    pCVar6 = *(CResource **)(pCVar20 + 8);
    if ((*(int *)(pCVar6 + 8) != 0) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(pCVar20 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(pCVar20 + 8) + 0x3a) = 0;
    }
    pCVar13 = *(CFrameData **)(pCVar20 + 8);
    if (pCVar13[0x38] == (CFrameData)0x0) {
      local_88 = *(CGraphicFrameData **)(pCVar13 + 0x1c);
      if (local_88 != (CGraphicFrameData *)0x0) {
        *(int *)local_88 = *(int *)local_88 + 1;
      }
      uStack_ac._3_1_ = 1;
    }
    else {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
      local_88 = *(CGraphicFrameData **)(*(int *)(pCVar20 + 8) + 0x1c);
      if (local_88 != (CGraphicFrameData *)0x0) {
        *(int *)local_88 = *(int *)local_88 + 1;
      }
      uStack_ac._3_1_ = 1;
    }
  }
  uVar5 = *(uint *)(DAT_01436e24 + 0xe0);
  if (uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c)) {
    uVar4 = uVar5 * 0x2c + *(int *)(iVar21 + 0x1e0);
    if (uVar4 == 0) goto LAB_00b723b1;
    pCVar20 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c
                                       )) & uVar4) + 0x28);
  }
  else {
LAB_00b723b1:
    pCVar20 = (CGraphicInfoFrame *)0x0;
  }
  pCVar13 = *(CFrameData **)(pCVar20 + 8);
  if (pCVar13 == (CFrameData *)0x0) {
    pCVar13 = ::operator_new(0x44);
    if (pCVar13 == (CFrameData *)0x0) {
      pCVar13 = (CFrameData *)0x0;
    }
    else {
      pCVar13 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar13,pCVar20);
    }
    piVar8 = *(int **)(pCVar20 + 8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar8 + 4))();
      }
      *(undefined4 *)(pCVar20 + 8) = 0;
    }
    *(CFrameData **)(pCVar20 + 8) = pCVar13;
LAB_00b724cc:
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
    bVar11 = 0;
  }
  else {
    if (*(int *)(pCVar13 + 0x1c) == 0) goto LAB_00b724cc;
    *(undefined4 *)(*(int *)(pCVar20 + 8) + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(pCVar20 + 0xc) + 0x20) + 0x194);
    pCVar6 = *(CResource **)(*(int *)(pCVar20 + 8) + 0x24);
    if (((pCVar6 != (CResource *)0x0) && (*(int *)(pCVar6 + 8) != 0)) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    pCVar6 = *(CResource **)(pCVar20 + 8);
    if ((*(int *)(pCVar6 + 8) != 0) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(pCVar20 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(pCVar20 + 8) + 0x3a) = 0;
    }
    pCVar13 = *(CFrameData **)(pCVar20 + 8);
    if (pCVar13[0x38] == (CFrameData)0x0) {
      local_8c = *(CGraphicFrameData **)(pCVar13 + 0x1c);
      if (local_8c != (CGraphicFrameData *)0x0) {
        *(int *)local_8c = *(int *)local_8c + 1;
      }
      bVar11 = 1;
    }
    else {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
      local_8c = *(CGraphicFrameData **)(*(int *)(pCVar20 + 8) + 0x1c);
      if (local_8c != (CGraphicFrameData *)0x0) {
        *(int *)local_8c = *(int *)local_8c + 1;
      }
      bVar11 = 1;
    }
  }
  uVar5 = *(uint *)(DAT_01436e24 + 200);
  if (uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c)) {
    uVar4 = uVar5 * 0x2c + *(int *)(iVar21 + 0x1e0);
    if (uVar4 == 0) goto LAB_00b72547;
    pCVar20 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c
                                       )) & uVar4) + 0x28);
  }
  else {
LAB_00b72547:
    pCVar20 = (CGraphicInfoFrame *)0x0;
  }
  pCVar13 = *(CFrameData **)(pCVar20 + 8);
  if (pCVar13 == (CFrameData *)0x0) {
    pCVar13 = ::operator_new(0x44);
    if (pCVar13 == (CFrameData *)0x0) {
      pCVar13 = (CFrameData *)0x0;
    }
    else {
      pCVar13 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar13,pCVar20);
    }
    piVar8 = *(int **)(pCVar20 + 8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar8 + 4))();
      }
      *(undefined4 *)(pCVar20 + 8) = 0;
    }
    *(CFrameData **)(pCVar20 + 8) = pCVar13;
LAB_00b7265f:
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
    bVar12 = 0;
  }
  else {
    if (*(int *)(pCVar13 + 0x1c) == 0) goto LAB_00b7265f;
    *(undefined4 *)(pCVar13 + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(pCVar20 + 0xc) + 0x20) + 0x194);
    pCVar6 = *(CResource **)(*(int *)(pCVar20 + 8) + 0x24);
    if (((pCVar6 != (CResource *)0x0) && (*(int *)(pCVar6 + 8) != 0)) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    pCVar6 = *(CResource **)(pCVar20 + 8);
    if ((*(int *)(pCVar6 + 8) != 0) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(pCVar20 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(pCVar20 + 8) + 0x3a) = 0;
    }
    pCVar13 = *(CFrameData **)(pCVar20 + 8);
    if (pCVar13[0x38] == (CFrameData)0x0) {
      local_98 = *(CGraphicFrameData **)(pCVar13 + 0x1c);
      if (local_98 != (CGraphicFrameData *)0x0) {
        *(int *)local_98 = *(int *)local_98 + 1;
      }
      bVar12 = 1;
    }
    else {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
      local_98 = *(CGraphicFrameData **)(*(int *)(pCVar20 + 8) + 0x1c);
      if (local_98 != (CGraphicFrameData *)0x0) {
        *(int *)local_98 = *(int *)local_98 + 1;
      }
      bVar12 = 1;
    }
  }
  uVar5 = *(uint *)(DAT_01436e24 + 0xdc);
  if (uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c)) {
    uVar4 = uVar5 * 0x2c + *(int *)(iVar21 + 0x1e0);
    if (uVar4 == 0) goto LAB_00b726d9;
    pCVar20 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c
                                       )) & uVar4) + 0x28);
  }
  else {
LAB_00b726d9:
    pCVar20 = (CGraphicInfoFrame *)0x0;
  }
  pCVar13 = *(CFrameData **)(pCVar20 + 8);
  if (pCVar13 == (CFrameData *)0x0) {
    pCVar13 = ::operator_new(0x44);
    if (pCVar13 == (CFrameData *)0x0) {
      pCVar13 = (CFrameData *)0x0;
    }
    else {
      pCVar13 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar13,pCVar20);
    }
    piVar8 = *(int **)(pCVar20 + 8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar8 + 4))();
      }
      *(undefined4 *)(pCVar20 + 8) = 0;
    }
    *(CFrameData **)(pCVar20 + 8) = pCVar13;
LAB_00b727f4:
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
    bVar19 = 0;
  }
  else {
    if (*(int *)(pCVar13 + 0x1c) == 0) goto LAB_00b727f4;
    *(undefined4 *)(*(int *)(pCVar20 + 8) + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(pCVar20 + 0xc) + 0x20) + 0x194);
    pCVar6 = *(CResource **)(*(int *)(pCVar20 + 8) + 0x24);
    if (((pCVar6 != (CResource *)0x0) && (*(int *)(pCVar6 + 8) != 0)) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    pCVar6 = *(CResource **)(pCVar20 + 8);
    if ((*(int *)(pCVar6 + 8) != 0) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(pCVar20 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(pCVar20 + 8) + 0x3a) = 0;
    }
    pCVar13 = *(CFrameData **)(pCVar20 + 8);
    if (pCVar13[0x38] == (CFrameData)0x0) {
      local_94 = *(CGraphicFrameData **)(pCVar13 + 0x1c);
      if (local_94 != (CGraphicFrameData *)0x0) {
        *(int *)local_94 = *(int *)local_94 + 1;
      }
      bVar19 = 1;
    }
    else {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
      local_94 = *(CGraphicFrameData **)(*(int *)(pCVar20 + 8) + 0x1c);
      if (local_94 != (CGraphicFrameData *)0x0) {
        *(int *)local_94 = *(int *)local_94 + 1;
      }
      bVar19 = 1;
    }
  }
  bVar19 = uStack_ac._3_1_ & bVar11 & bVar12 & bVar19;
  uVar5 = *(uint *)(DAT_01436e24 + 0xe4);
  uStack_ac = CONCAT13(bVar19,(undefined3)uStack_ac);
  if (uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c)) {
    uVar4 = uVar5 * 0x2c + *(int *)(iVar21 + 0x1e0);
    if (uVar4 == 0) goto LAB_00b7286f;
    pCVar20 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar5 < (uint)((*(int *)(iVar21 + 0x1e4) - *(int *)(iVar21 + 0x1e0)) / 0x2c
                                       )) & uVar4) + 0x28);
  }
  else {
LAB_00b7286f:
    pCVar20 = (CGraphicInfoFrame *)0x0;
  }
  pCVar13 = *(CFrameData **)(pCVar20 + 8);
  if (pCVar13 == (CFrameData *)0x0) {
    pCVar13 = ::operator_new(0x44);
    if (pCVar13 == (CFrameData *)0x0) {
      pCVar13 = (CFrameData *)0x0;
    }
    else {
      pCVar13 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar13,pCVar20);
    }
    piVar8 = *(int **)(pCVar20 + 8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar8 + 4))();
      }
      *(undefined4 *)(pCVar20 + 8) = 0;
    }
    *(CFrameData **)(pCVar20 + 8) = pCVar13;
  }
  else if (*(int *)(pCVar13 + 0x1c) != 0) {
    *(undefined4 *)(pCVar13 + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(pCVar20 + 0xc) + 0x20) + 0x194);
    pCVar6 = *(CResource **)(*(int *)(pCVar20 + 8) + 0x24);
    if (((pCVar6 != (CResource *)0x0) && (*(int *)(pCVar6 + 8) != 0)) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    pCVar6 = *(CResource **)(pCVar20 + 8);
    if ((*(int *)(pCVar6 + 8) != 0) &&
       (*(int *)(pCVar6 + 0x18) != *(int *)(*(int *)(pCVar6 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar6);
      CResource::ListInsert
                (pCVar6,*(CResource **)(*(int *)(pCVar6 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar6 + 8) + 4));
      *(undefined4 *)(pCVar6 + 0x18) = *(undefined4 *)(*(int *)(pCVar6 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(pCVar20 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(pCVar20 + 8) + 0x3a) = 0;
    }
    pCVar13 = *(CFrameData **)(pCVar20 + 8);
    if (pCVar13[0x38] == (CFrameData)0x0) {
      local_90 = *(CGraphicFrameData **)(pCVar13 + 0x1c);
    }
    else {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
      local_90 = *(CGraphicFrameData **)(*(int *)(pCVar20 + 8) + 0x1c);
    }
    if (local_90 != (CGraphicFrameData *)0x0) {
      *(int *)local_90 = *(int *)local_90 + 1;
    }
    bVar11 = 1;
    goto LAB_00b7298a;
  }
  CGraphicInfoFrame::CFrameData::BeginLoading(pCVar13,0,0);
  bVar11 = 0;
LAB_00b7298a:
  pCVar7 = local_90;
  if ((bVar19 & bVar11) == 0) {
    if ((local_98 != (CGraphicFrameData *)0x0) &&
       (*(int *)local_98 = *(int *)local_98 + -1, *(int *)local_98 == 0)) {
      CGraphicFrameData::_scalar_deleting_destructor_(local_98,1);
    }
    if ((pCVar7 != (CGraphicFrameData *)0x0) &&
       (*(int *)pCVar7 = *(int *)pCVar7 + -1, *(int *)pCVar7 == 0)) {
      CGraphicFrameData::_scalar_deleting_destructor_(pCVar7,1);
    }
    if ((local_8c != (CGraphicFrameData *)0x0) &&
       (*(int *)local_8c = *(int *)local_8c + -1, *(int *)local_8c == 0)) {
      CGraphicFrameData::_scalar_deleting_destructor_(local_8c,1);
    }
    if ((local_94 != (CGraphicFrameData *)0x0) &&
       (*(int *)local_94 = *(int *)local_94 + -1, *(int *)local_94 == 0)) {
      CGraphicFrameData::_scalar_deleting_destructor_(local_94,1);
    }
    if ((local_88 != (CGraphicFrameData *)0x0) &&
       (*(int *)local_88 = *(int *)local_88 + -1, *(int *)local_88 == 0)) {
      CGraphicFrameData::_scalar_deleting_destructor_(local_88,1);
    }
    return false;
  }
  this_01 = (CGPUScratchBufferTextureManager *)(DAT_01436e98 + 0x3190);
  pCVar14 = CGPUScratchBufferTextureManager::StartSurfaceUse(this_01,*(long *)(this + 0x1dc));
  *(CSurface **)(this + 0x1e0) = pCVar14;
  pCVar14 = CGPUScratchBufferTextureManager::StartSurfaceUse(this_01,*(long *)(this + 0x1ec));
  *(CSurface **)(this + 0x1f0) = pCVar14;
  pCVar14 = CGPUScratchBufferTextureManager::StartSurfaceUse(this_01,*(long *)(this + 0x1e4));
  *(CSurface **)(this + 0x1e8) = pCVar14;
  pCVar14 = CGPUScratchBufferTextureManager::StartSurfaceUse(this_01,*(long *)(this + 500));
  *(CSurface **)(this + 0x1f8) = pCVar14;
  pCVar9 = DAT_013cb508;
  pCVar2 = DAT_013cb508 + 0x2808;
  *(CRenderStateManager **)(DAT_013cb508 + *(int *)(DAT_013cb508 + 0x2808) * 8 + 8) =
       DAT_013cb508 + 0x3a24;
  iVar21 = *(int *)(pCVar9 + 0x2814);
  *(int *)pCVar2 = *(int *)pCVar2 + 1;
  *(int *)(pCVar9 + 0x2814) = iVar21 << 1;
  local_9c = (CFrameData *)CONCAT31(local_9c._1_3_,DAT_013bc800);
  if (DAT_013bc800 != '\x01') {
    CShaderRenderManager::EnableVertexShaders((CShaderRenderManager *)&DAT_013bc470);
  }
  local_9c._0_2_ = CONCAT11(DAT_013bc801,local_9c._0_1_);
  if (DAT_013bc801 != '\x01') {
    CShaderRenderManager::EnablePixelShaders((CShaderRenderManager *)&DAT_013bc470);
  }
  pCVar10 = DAT_01436e18;
  pCVar3 = DAT_01436e18 + 0x2868;
  if (*(int *)(DAT_01436e18 + 0x286c) != 0) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x2870)) == 0) {
      *(uint *)(DAT_01436e18 + 0x2870) =
           *(uint *)(DAT_01436e18 + 0x2870) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x286c);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(undefined4 *)(pCVar10 + 0x286c) = 0;
    if (pCVar10[0x2878] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2878] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  pCVar3 = DAT_01436e18 + 0x28cc;
  if (*(int *)(DAT_01436e18 + 0x28d0) != 0) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x28d4)) == 0) {
      *(uint *)(DAT_01436e18 + 0x28d4) =
           *(uint *)(DAT_01436e18 + 0x28d4) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x28d0);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(undefined4 *)(pCVar10 + 0x28d0) = 0;
    if (pCVar10[0x28dc] == (CTCActionUseReadable)0x0) {
      pCVar10[0x28dc] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  pCVar3 = DAT_01436e18 + 0x29e4;
  if (*(int *)(DAT_01436e18 + 0x29e8) != 0) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x29ec)) == 0) {
      *(uint *)(DAT_01436e18 + 0x29ec) =
           *(uint *)(DAT_01436e18 + 0x29ec) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x29e8);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(undefined4 *)(pCVar10 + 0x29e8) = 0;
    if (pCVar10[0x29f4] == (CTCActionUseReadable)0x0) {
      pCVar10[0x29f4] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  pCVar3 = DAT_01436e18 + 0x28b8;
  if (*(int *)(DAT_01436e18 + 0x28bc) != 0) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x28c0)) == 0) {
      *(uint *)(DAT_01436e18 + 0x28c0) =
           *(uint *)(DAT_01436e18 + 0x28c0) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x28bc);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(undefined4 *)(pCVar10 + 0x28bc) = 0;
    if (pCVar10[0x28c8] == (CTCActionUseReadable)0x0) {
      pCVar10[0x28c8] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  pCVar3 = DAT_01436e18 + 0x2944;
  if (*(int *)(DAT_01436e18 + 0x2948) != 0) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x294c)) == 0) {
      *(uint *)(DAT_01436e18 + 0x294c) =
           *(uint *)(DAT_01436e18 + 0x294c) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x2948);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(undefined4 *)(pCVar10 + 0x2948) = 0;
    if (pCVar10[0x2954] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2954] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar7 = *(CGraphicFrameData **)(DAT_01436e18 + 0x3cf0);
  if (this == (CEngineWaterRenderer *)0xffffffcc) {
    pCStack_c4 = (CEngineWaterRenderer *)0x0;
  }
  else {
    pCStack_c4 = *(CEngineWaterRenderer **)(this + 0x38);
  }
  this_00 = pCStack_c4;
  this_02 = (CEngineWaterRenderer *)0x0;
  (**(code **)(*(int *)pCVar7 + 400))();
  pCVar3 = DAT_01436e18;
  *(CEngineWaterRenderer **)(DAT_01436e18 + 0x3cf4) = this + 0x20;
  piVar8 = *(int **)(pCVar3 + 0x3cf0);
  if (this + 0x20 == (CEngineWaterRenderer *)0x0) {
    pCStack_d0 = (CGraphicFrameData *)0x0;
  }
  else {
    pCStack_d0 = *(CGraphicFrameData **)(this + 0x24);
  }
  (**(code **)(*piVar8 + 0x1a0))();
  NHeroInformationScreens::CBase::CBase((CBase *)&ppuStack_80);
  ppuStack_80 = &PTR__vector_deleting_destructor__0129c800;
  ppuStack_7c = &PTR__vector_deleting_destructor__0122f84c;
  uStack_78 = 0;
  uStack_74 = 0;
  ppuStack_6c = &PTR__vector_deleting_destructor__0122f84c;
  uStack_68 = 0;
  uStack_64 = 0;
  ppuStack_a0 = &PTR__vector_deleting_destructor__0122f84c;
  local_9c = (CFrameData *)0x0;
  local_98 = (CGraphicFrameData *)0x0;
  if ((this[0x28f] != (CEngineWaterRenderer)0x0) && (this[0x274] != (CEngineWaterRenderer)0x0)) {
    pCVar14 = (CSurface *)CTexture::GetNoLevels(*(CTexture **)(this + 0x1e0));
    CSurface::operator=((CSurface *)&ppuStack_a0,pCVar14);
    local_90 = (CGraphicFrameData *)&PTR__vector_deleting_destructor__0122f84c;
    CSurface::Uninitialise((CSurface *)&local_90);
    CRenderTarget::AttachTarget((CRenderTarget *)&ppuStack_80,(CSurface *)&ppuStack_a0);
    CDisplayManager::SetRenderTarget(DAT_01436e1c,(CRenderTarget *)&ppuStack_80);
    CSurface::GetDimensions((CSurface *)&ppuStack_a0);
    CSurface::GetDimensions((CSurface *)&ppuStack_a0);
    this_02 = (CEngineWaterRenderer *)GFAYToVY(extraout_ECX);
    fStack_3c = GFAXToVX(extraout_ECX_00);
    uStack_44 = 0;
    uStack_40 = 0;
    pCStack_38 = this_02;
    CTCActionUseReadable::OnCreate(DAT_01436e18);
    RenderEnvironmentMap(this);
    CRenderTarget::DetachTarget((CRenderTarget *)&ppuStack_80);
    CTexture::GenerateMipmapsWithGPU(*(CTexture **)(this + 0x1e0));
  }
  pCVar10 = DAT_01436e18;
  iVar21 = DAT_01396dc8;
  pCVar3 = DAT_01436e18 + 0x2ee4;
  if (*(int *)(DAT_01436e18 + 0x2ee8) != DAT_01396dc8) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x2eec)) == 0) {
      *(uint *)(DAT_01436e18 + 0x2eec) =
           *(uint *)(DAT_01436e18 + 0x2eec) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x2ee8);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(int *)(pCVar10 + 0x2ee8) = iVar21;
    if (pCVar10[0x2ef4] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2ef4] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  iVar21 = DAT_01396dc8;
  pCVar3 = DAT_01436e18 + 0x2f84;
  if (*(int *)(DAT_01436e18 + 0x2f88) != DAT_01396dc8) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x2f8c)) == 0) {
      *(uint *)(DAT_01436e18 + 0x2f8c) =
           *(uint *)(DAT_01436e18 + 0x2f8c) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x2f88);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(int *)(pCVar10 + 0x2f88) = iVar21;
    if (pCVar10[0x2f94] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2f94] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  iVar21 = DAT_01396dc8;
  pCVar3 = DAT_01436e18 + 0x2ef8;
  if (*(int *)(DAT_01436e18 + 0x2efc) != DAT_01396dc8) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x2f00)) == 0) {
      *(uint *)(DAT_01436e18 + 0x2f00) =
           *(uint *)(DAT_01436e18 + 0x2f00) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x2efc);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(int *)(pCVar10 + 0x2efc) = iVar21;
    if (pCVar10[0x2f08] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2f08] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  pCVar10 = DAT_01436e18;
  iVar21 = DAT_01396dc8;
  pCVar3 = DAT_01436e18 + 0x2f98;
  if (*(int *)(DAT_01436e18 + 0x2f9c) != DAT_01396dc8) {
    if ((*(uint *)(DAT_01436e18 + 0x2814) & *(uint *)(DAT_01436e18 + 0x2fa0)) == 0) {
      *(uint *)(DAT_01436e18 + 0x2fa0) =
           *(uint *)(DAT_01436e18 + 0x2fa0) | *(uint *)(DAT_01436e18 + 0x2814);
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 8) = pCVar3;
      *(undefined4 *)(pCVar10 + *(int *)(pCVar10 + 0x2808) * 8 + 0xc) =
           *(undefined4 *)(pCVar10 + 0x2f9c);
      *(int *)(pCVar10 + 0x2808) = *(int *)(pCVar10 + 0x2808) + 1;
    }
    *(int *)(pCVar10 + 0x2f9c) = iVar21;
    if (pCVar10[0x2fa8] == (CTCActionUseReadable)0x0) {
      pCVar10[0x2fa8] = (CTCActionUseReadable)0x1;
      *(CTCActionUseReadable **)(pCVar10 + *(int *)(pCVar10 + 0x280c) * 4 + 0x2008) = pCVar3;
      *(int *)(pCVar10 + 0x280c) = *(int *)(pCVar10 + 0x280c) + 1;
    }
  }
  CShaderRenderManager::ApplyVertexShader(DAT_01436e14,(CVertexShader *)(this + 0x178),false);
  CShaderRenderManager::ApplyPixelShader(DAT_01436e14,(CPixelShader *)(this + 0x180));
  pCVar3 = DAT_01436e18;
  if (param_1) {
    if (*(int **)(iStack_a4 + 8) == (int *)0x0) {
      iVar21 = 0;
    }
    else {
      iVar21 = **(int **)(iStack_a4 + 8);
    }
    if (*(int *)(DAT_01436e18 + 0x3d00) != iVar21) {
      *(int *)(DAT_01436e18 + 0x3d00) = iVar21;
      (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x104))(*(int **)(pCVar3 + 0x3cf0),0,iVar21);
    }
    if (*(int *)(pCVar3 + 0x3cb8) < 1) {
      *(undefined4 *)(pCVar3 + 0x3cb8) = 1;
    }
    pCVar3 = DAT_01436e18;
    if (*(int **)(unaff_EBX + 8) == (int *)0x0) {
      iVar21 = 0;
    }
    else {
      iVar21 = **(int **)(unaff_EBX + 8);
    }
    if (*(int *)(DAT_01436e18 + 0x3d04) != iVar21) {
      *(int *)(DAT_01436e18 + 0x3d04) = iVar21;
      (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x104))(*(int **)(pCVar3 + 0x3cf0),1,iVar21);
    }
    if (*(int *)(pCVar3 + 0x3cb8) < 2) {
      *(undefined4 *)(pCVar3 + 0x3cb8) = 2;
    }
    iVar21 = 0;
    lVar15 = CTexture::GetNoLevels(*(CTexture **)(this + 0x1f0));
    if (0 < lVar15) {
      do {
        puVar16 = (uint *)CTexture::GetDimensions
                                    (*(CTexture **)(pCStack_c4 + 0x1f0),(ulong)auStack_34);
        if ((*puVar16 < 0x20) ||
           (iVar17 = CTexture::GetDimensions(*(CTexture **)(pCStack_c4 + 0x1f0),(ulong)auStack_4c),
           *(uint *)(iVar17 + 4) < 0x20)) break;
        pCVar14 = (CSurface *)CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1f0));
        CSurface::operator=((CSurface *)&ppuStack_a0,pCVar14);
        appuStack_5c[0] = &PTR__vector_deleting_destructor__0122f84c;
        CSurface::Uninitialise((CSurface *)appuStack_5c);
        CRenderTarget::AttachTarget((CRenderTarget *)&ppuStack_80,(CSurface *)&ppuStack_a0);
        CDisplayManager::SetRenderTarget(DAT_01436e1c,(CRenderTarget *)&ppuStack_80);
        CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        this_02 = (CEngineWaterRenderer *)GFAYToVY(extraout_ECX_01);
        local_88 = (CGraphicFrameData *)GFAXToVX(extraout_ECX_02);
        local_90 = (CGraphicFrameData *)0x0;
        local_8c = (CGraphicFrameData *)0x0;
        pCStack_84 = this_02;
        CTCActionUseReadable::OnCreate(DAT_01436e18);
        RenderBumpMap(pCStack_c4,0);
        CRenderTarget::DetachTarget((CRenderTarget *)&ppuStack_80);
        iVar21 = iVar21 + 1;
        lVar15 = CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1f0));
      } while (iVar21 < lVar15);
    }
  }
  pCVar3 = DAT_01436e18;
  if (param_2) {
    if (*(int **)(this + 8) == (int *)0x0) {
      iVar21 = 0;
    }
    else {
      iVar21 = **(int **)(this + 8);
    }
    if (*(int *)(DAT_01436e18 + 0x3d00) != iVar21) {
      *(int *)(DAT_01436e18 + 0x3d00) = iVar21;
      (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x104))(*(int **)(pCVar3 + 0x3cf0),0,iVar21);
    }
    if (*(int *)(pCVar3 + 0x3cb8) < 1) {
      *(undefined4 *)(pCVar3 + 0x3cb8) = 1;
    }
    pCVar3 = DAT_01436e18;
    if (*(int **)(uStack_ac + 8) == (int *)0x0) {
      iVar21 = 0;
    }
    else {
      iVar21 = **(int **)(uStack_ac + 8);
    }
    if (*(int *)(DAT_01436e18 + 0x3d04) != iVar21) {
      *(int *)(DAT_01436e18 + 0x3d04) = iVar21;
      (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x104))(*(int **)(pCVar3 + 0x3cf0),1,iVar21);
    }
    if (*(int *)(pCVar3 + 0x3cb8) < 2) {
      *(undefined4 *)(pCVar3 + 0x3cb8) = 2;
    }
    iVar21 = 0;
    lVar15 = CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1e8));
    if (0 < lVar15) {
      do {
        puVar16 = (uint *)CTexture::GetDimensions
                                    (*(CTexture **)(pCStack_c4 + 0x1e8),(ulong)&uStack_44);
        if ((*puVar16 < 0x20) ||
           (iVar17 = CTexture::GetDimensions(*(CTexture **)(pCStack_c4 + 0x1e8),(ulong)auStack_2c),
           *(uint *)(iVar17 + 4) < 0x20)) break;
        pCVar14 = (CSurface *)CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1e8));
        CSurface::operator=((CSurface *)&ppuStack_a0,pCVar14);
        appuStack_5c[0] = &PTR__vector_deleting_destructor__0122f84c;
        CSurface::Uninitialise((CSurface *)appuStack_5c);
        CRenderTarget::AttachTarget((CRenderTarget *)&ppuStack_80,(CSurface *)&ppuStack_a0);
        CDisplayManager::SetRenderTarget(DAT_01436e1c,(CRenderTarget *)&ppuStack_80);
        CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        this_02 = (CEngineWaterRenderer *)GFAYToVY(extraout_ECX_03);
        local_88 = (CGraphicFrameData *)GFAXToVX(extraout_ECX_04);
        local_90 = (CGraphicFrameData *)0x0;
        local_8c = (CGraphicFrameData *)0x0;
        pCStack_84 = this_02;
        CTCActionUseReadable::OnCreate(DAT_01436e18);
        RenderBumpMap(pCStack_c4,1);
        CRenderTarget::DetachTarget((CRenderTarget *)&ppuStack_80);
        iVar21 = iVar21 + 1;
        lVar15 = CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1e8));
      } while (iVar21 < lVar15);
    }
  }
  pCVar3 = DAT_01436e18;
  if (param_3) {
    if (*(int **)(unaff_ESI + 8) == (int *)0x0) {
      iVar21 = 0;
    }
    else {
      iVar21 = **(int **)(unaff_ESI + 8);
    }
    if (*(int *)(DAT_01436e18 + 0x3d00) != iVar21) {
      *(int *)(DAT_01436e18 + 0x3d00) = iVar21;
      (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x104))(*(int **)(pCVar3 + 0x3cf0),0,iVar21);
    }
    if (*(int *)(pCVar3 + 0x3cb8) < 1) {
      *(undefined4 *)(pCVar3 + 0x3cb8) = 1;
    }
    iVar21 = 0;
    lVar15 = CTexture::GetNoLevels(*(CTexture **)(pCStack_c4 + 0x1f8));
    if (0 < lVar15) {
      do {
        puVar16 = (uint *)CTexture::GetDimensions(*(CTexture **)(this_00 + 0x1f8),(ulong)&uStack_44)
        ;
        if ((*puVar16 < 0x20) ||
           (iVar17 = CTexture::GetDimensions(*(CTexture **)(this_00 + 0x1f8),(ulong)auStack_2c),
           *(uint *)(iVar17 + 4) < 0x20)) break;
        pCVar14 = (CSurface *)CTexture::GetNoLevels(*(CTexture **)(this_00 + 0x1f8));
        CSurface::operator=((CSurface *)&ppuStack_a0,pCVar14);
        appuStack_5c[0] = &PTR__vector_deleting_destructor__0122f84c;
        CSurface::Uninitialise((CSurface *)appuStack_5c);
        CRenderTarget::AttachTarget((CRenderTarget *)&ppuStack_80,(CSurface *)&ppuStack_a0);
        CDisplayManager::SetRenderTarget(DAT_01436e1c,(CRenderTarget *)&ppuStack_80);
        CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        pfVar18 = (float *)CSurface::GetDimensions((CSurface *)&ppuStack_a0);
        this_02 = (CEngineWaterRenderer *)*pfVar18;
        pCStack_c4 = (CEngineWaterRenderer *)GFAYToVY(extraout_ECX_05);
        local_88 = (CGraphicFrameData *)GFAXToVX(extraout_ECX_06);
        local_90 = (CGraphicFrameData *)0x0;
        local_8c = (CGraphicFrameData *)0x0;
        pCStack_84 = pCStack_c4;
        CTCActionUseReadable::OnCreate(DAT_01436e18);
        RenderIceBumpMap(this_00);
        CRenderTarget::DetachTarget((CRenderTarget *)&ppuStack_80);
        iVar21 = iVar21 + 1;
        lVar15 = CTexture::GetNoLevels(*(CTexture **)(this_00 + 0x1f8));
      } while (iVar21 < lVar15);
    }
  }
  (**(code **)(**(int **)(DAT_01436e18 + 0x3cf0) + 400))(*(int **)(DAT_01436e18 + 0x3cf0),0,0,0,0);
  pCVar3 = DAT_01436e18;
  *(undefined4 *)(DAT_01436e18 + 0x3cf4) = 0;
  (**(code **)(**(int **)(pCVar3 + 0x3cf0) + 0x1a0))(*(int **)(pCVar3 + 0x3cf0),0);
  CDisplayManager::SetRenderTargetAsBackScreen(DAT_01436e1c);
  CRenderTarget::DetachTarget((CRenderTarget *)&local_9c);
  CRenderTarget::DetachTarget((CRenderTarget *)&local_9c);
  CEngineCamera::RefreshD3DTransforms(DAT_01436ea0);
  CSurface::Uninitialise((CSurface *)&stack0xffffff44);
  CSurface::Uninitialise((CSurface *)&stack0xffffff44);
  local_88 = (CGraphicFrameData *)&PTR__vector_deleting_destructor__0122f84c;
  CSurface::Uninitialise((CSurface *)&local_88);
  local_98 = (CGraphicFrameData *)&PTR__vector_deleting_destructor__0122f84c;
  CSurface::Uninitialise((CSurface *)&local_98);
  NHeroInformationScreens::CBase::CBase((CBase *)&local_9c);
  cStack_d4 = (char)piVar8;
  if (cStack_d4 != DAT_013bc800) {
    if (cStack_d4 == '\0') {
      CShaderRenderManager::DisableVertexShaders((CShaderRenderManager *)&DAT_013bc470);
    }
    else {
      CShaderRenderManager::EnableVertexShaders((CShaderRenderManager *)&DAT_013bc470);
    }
  }
  cStack_d3 = (char)((uint)piVar8 >> 8);
  if (cStack_d3 != DAT_013bc801) {
    if (cStack_d3 == '\0') {
      CShaderRenderManager::DisablePixelShaders((CShaderRenderManager *)&DAT_013bc470);
    }
    else {
      CShaderRenderManager::EnablePixelShaders((CShaderRenderManager *)&DAT_013bc470);
    }
  }
  CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
  if ((pCStack_d0 != (CGraphicFrameData *)0x0) &&
     (*(int *)pCStack_d0 = *(int *)pCStack_d0 + -1, *(int *)pCStack_d0 == 0)) {
    CGraphicFrameData::_scalar_deleting_destructor_(pCStack_d0,1);
  }
  if ((pCStack_c4 != (CEngineWaterRenderer *)0x0) &&
     (*(int *)pCStack_c4 = *(int *)pCStack_c4 + -1, *(int *)pCStack_c4 == 0)) {
    CGraphicFrameData::_scalar_deleting_destructor_((CGraphicFrameData *)pCStack_c4,1);
  }
  if ((pCVar7 != (CGraphicFrameData *)0x0) &&
     (*(int *)pCVar7 = *(int *)pCVar7 + -1, *(int *)pCVar7 == 0)) {
    CGraphicFrameData::_scalar_deleting_destructor_(pCVar7,1);
  }
  if ((this_02 != (CEngineWaterRenderer *)0x0) &&
     (*(int *)this_02 = *(int *)this_02 + -1, *(int *)this_02 == 0)) {
    CGraphicFrameData::_scalar_deleting_destructor_((CGraphicFrameData *)this_02,1);
  }
  return true;
}



//=== FinishWithDynamicTextures @ 00b7ec90 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b7ed40 ===

/* [bsim sim=0.783936610529488 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(void **)(this + 0x14) != (void *)0x0) {
    free(*(void **)(this + 0x14));
  }
  if (*(void **)(this + 4) != (void *)0x0) {
    free(*(void **)(this + 4));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b86a30 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(int *)(this + 0xc) != 0) {
    *(undefined4 *)(this + 0xc) = 0;
  }
  if (*(int *)(this + 0x10) != 0) {
    ::_E2(*(undefined4 *)(this + 0x14));
    *(undefined4 *)(this + 0x10) = 0;
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b89080 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  if (*(int *)(this + 0xc) != 0) {
    ::_E2(*(undefined4 *)(this + 0x18));
    *(undefined4 *)(this + 0xc) = 0;
  }
  if (*(int *)(this + 0x10) != 0) {
    ::_E2(*(undefined4 *)(this + 0x1c));
    *(undefined4 *)(this + 0x10) = 0;
  }
  if (*(int *)(this + 0x14) != 0) {
    ::_E2(*(undefined4 *)(this + 0x20));
    *(undefined4 *)(this + 0x14) = 0;
  }
  return;
}



//=== FinishWithDynamicTextures @ 00b98230 ===

/* [bsim sim=0.5412657460630491 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  CEngineScreenEffectOutlineGlow *pCVar1;
  byte in_stack_00000004;
  
  pCVar1 = this + 4;
  if (*(int *)(this + 8) != 0) {
    FUN_00b97580(*(undefined4 *)(*(int *)pCVar1 + 4));
    *(int *)(*(int *)pCVar1 + 8) = *(int *)pCVar1;
    *(undefined4 *)(*(int *)pCVar1 + 4) = 0;
    *(int *)(*(int *)pCVar1 + 0xc) = *(int *)pCVar1;
    *(undefined4 *)(this + 8) = 0;
  }
  if (*(void **)pCVar1 != (void *)0x0) {
    free(*(void **)pCVar1);
  }
  if ((in_stack_00000004 & 1) != 0) {
    operator_delete(this);
  }
  return;
}



//=== FinishWithDynamicTextures @ 00beaaa0 ===

/* [bsim sim=0.6941172246138106 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  *(undefined ***)this = &PTR_FinishWithDynamicTextures_012a8134;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x14));
    *(undefined4 *)(this + 0x14) = 0;
  }
  if (*(int *)(this + 0x10) != 0) {
    FUN_00bda0d0(*(int *)(this + 0x10));
    *(undefined4 *)(this + 0x10) = 0;
  }
  *(undefined ***)this = &PTR_LAB_0129b860;
  return;
}



//=== FinishWithDynamicTextures @ 00beb4d0 ===

/* [bsim sim=0.6628018580486938 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  byte in_stack_00000004;
  
  *(undefined ***)this = &PTR_FinishWithDynamicTextures_012a8134;
  if (*(void **)(this + 0x14) != (void *)0x0) {
    operator_delete(*(void **)(this + 0x14));
    *(undefined4 *)(this + 0x14) = 0;
  }
  if (*(int *)(this + 0x10) != 0) {
    FUN_00bda0d0(*(int *)(this + 0x10));
    *(undefined4 *)(this + 0x10) = 0;
  }
  *(undefined ***)this = &PTR_LAB_0129b860;
  if ((in_stack_00000004 & 1) != 0) {
    CEngineLandscapeMemoryManager::FreeVirtual
              (*(CEngineLandscapeMemoryManager **)(DAT_01436ea8 + 0x6b0),this);
  }
  return;
}



//=== GetTexturePaletteIndex @ 00bf9150 ===

/* [bsim sim=0.79410278176426 <- ego_r]
   public: long __thiscall CEngineTexturePalette::GetTexturePaletteIndex(unsigned long) */

long __thiscall
CEngineTexturePalette::GetTexturePaletteIndex(CEngineTexturePalette *this,ulong param_1)

{
  int iVar1;
  int iVar2;
  ulong uVar3;
  uint uVar4;
  CCharString *pCVar5;
  ulong *puVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  ulong local_8;
  CCharString local_4 [4];
  
  uVar3 = param_1;
  if (this[0x14] != (CEngineTexturePalette)0x0) {
    return param_1;
  }
  puVar6 = *(ulong **)(this + 4);
  uVar4 = 0;
  if (*(int *)(this + 8) - (int)puVar6 >> 3 != 0) {
    do {
      if (*puVar6 == param_1) {
        return uVar4;
      }
      uVar4 = uVar4 + 1;
      puVar6 = puVar6 + 2;
    } while (uVar4 < (uint)(*(int *)(this + 8) - *(int *)(this + 4) >> 3));
  }
  CCharString::CCharString(local_4);
  pCVar5 = (CCharString *)CBankFile::GetEntrySymbol(*(CBankFile **)this,(ulong)&param_1);
  CCharString::operator=(local_4,pCVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,extraout_EDX,
             unaff_EDI);
  puVar6 = *(ulong **)(this + 8);
  local_8 = uVar3;
  if (puVar6 == *(ulong **)(this + 0xc)) {
    FUN_00bf94d0(puVar6,&local_8,&param_1,1,1);
    ppVar7 = extraout_EDX_02;
  }
  else {
    ppVar7 = extraout_EDX_00;
    if (puVar6 != (ulong *)0x0) {
      *puVar6 = uVar3;
      CCharString::CCharString((CCharString *)(puVar6 + 1),local_4);
      ppVar7 = extraout_EDX_01;
    }
    *(int *)(this + 8) = *(int *)(this + 8) + 8;
  }
  iVar1 = *(int *)(this + 8);
  iVar2 = *(int *)(this + 4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4,ppVar7,
             unaff_EDI);
  return (iVar1 - iVar2 >> 3) + -1;
}



//=== GetTexturesInUse @ 00c125e0 ===

/* [bsim sim=0.50995451132491 <- ego_r]
   public: unsigned long __thiscall CPreallocTexturePool::GetTexturesInUse(unsigned long)const  */

ulong __thiscall CPreallocTexturePool::GetTexturesInUse(CPreallocTexturePool *this,ulong param_1)

{
  wchar_t wVar1;
  CManager *this_00;
  undefined2 extraout_var;
  int iVar2;
  uint uVar3;
  ulong local_18;
  CTCHeroExperience local_14 [8];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01211715;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CTCHeroExperience::AddSuccessfulBlockParry(local_14);
  uVar3 = 0;
  local_4 = 0;
  local_18 = 0;
  if (*(int *)(this + 0x60) != 0) {
    do {
      this_00 = (CManager *)FUN_00c13620(uVar3);
      wVar1 = NUISystem::CManager::GetKeyPressed(this_00);
      if ((CONCAT22(extraout_var,wVar1) != 0) && (iVar2 = FUN_00c20ef0(), iVar2 != 0)) {
        local_18 = local_18 + 1;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < *(uint *)(this + 0x60));
  }
  local_4 = 0xffffffff;
  FUN_00c0ffe0();
  ExceptionList = local_c;
  return local_18;
}



//=== FinishWithDynamicTextures @ 00c1c230 ===

/* [bsim sim=0.9153830318329608 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *extraout_ECX;
  CThingFilter_IsValid *extraout_ECX_00;
  CThingFilter_IsValid *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  
  *(undefined ***)this = &PTR__CAIStateGroup_JackOfBladesBase_012b3290;
  this_00 = (CThingFilter_IsValid *)this;
  if (*(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x10) !=
      (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
               *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x10));
    *(undefined4 *)(this + 0x10) = 0;
    this_00 = (CThingFilter_IsValid *)extraout_ECX;
    in_EDX = extraout_EDX;
  }
  if (*(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0xc) !=
      (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this_00,in_EDX,
               *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0xc));
    *(undefined4 *)(this + 0xc) = 0;
    this_00 = extraout_ECX_00;
  }
  if (*(CThing **)(this + 8) != (CThing *)0x0) {
    CThingFilter_IsValid::operator()(this_00,*(CThing **)(this + 8));
    *(undefined4 *)(this + 8) = 0;
  }
  return;
}



//=== FinishWithDynamicTextures @ 00c1e850 ===

/* [bsim sim=0.5931039778672995 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_01212676;
  pvStack_c = ExceptionList;
  local_4 = 1;
  ExceptionList = &pvStack_c;
  FUN_00c1e800();
  (**(code **)(*(int *)(*(int *)this + 0x74) + 0x20))(this);
  puStack_8 = (undefined1 *)((uint)puStack_8 & 0xffffff00);
  if (*(void **)(this + 0x14) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x14));
    *(undefined4 *)(this + 0x14) = 0;
  }
  puStack_8 = (undefined1 *)0xffffffff;
  if (*(void **)(this + 8) != (void *)0x0) {
    operator_delete__(*(void **)(this + 8));
    *(undefined4 *)(this + 8) = 0;
  }
  ExceptionList = this;
  return;
}



//=== FinishWithDynamicTextures @ 00c4cef0 ===

/* [bsim sim=0.5975981700864549 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  if (*(int *)(this + 4) != 0) {
    *(undefined4 *)(*(int *)(this + 4) + 8) = *(undefined4 *)(this + 8);
  }
  if (*(int *)(this + 8) != 0) {
    *(undefined4 *)(*(int *)(this + 8) + 4) = *(undefined4 *)(this + 4);
  }
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  return;
}



//=== FinishWithDynamicTextures @ 00c5d610 ===

/* [bsim sim=0.6334606110749658 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  CTCHeroStats *extraout_ECX;
  CTCHeroStats *extraout_ECX_00;
  CTCHeroStats *this_00;
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  this_00 = (CTCHeroStats *)this;
  if (*(HWAVEOUT *)(this + 8) != (HWAVEOUT)0x0) {
    waveOutReset(*(HWAVEOUT *)(this + 8));
    if (*(int *)(this + 0x10) != 0) {
      iVar1 = 0;
      do {
        waveOutUnprepareHeader(*(HWAVEOUT *)(this + 8),(LPWAVEHDR)(*(int *)(this + 4) + iVar1),0x20)
        ;
        uVar2 = uVar2 + 1;
        iVar1 = iVar1 + 0x20;
      } while (uVar2 < *(uint *)(this + 0x10));
    }
    waveOutClose(*(HWAVEOUT *)(this + 8));
    *(undefined4 *)(this + 8) = 0;
    *(undefined4 *)(this + 0x10) = 0;
    this_00 = extraout_ECX;
  }
  if (*(CBase **)(this + 4) != (CBase *)0x0) {
    CTCHeroStats::InformOfEvent(this_00,*(CBase **)(this + 4));
    *(undefined4 *)(this + 4) = 0;
    this_00 = extraout_ECX_00;
  }
  if (*(CBase **)(this + 0xc) != (CBase *)0x0) {
    CTCHeroStats::InformOfEvent(this_00,*(CBase **)(this + 0xc));
    *(undefined4 *)(this + 0xc) = 0;
  }
  return;
}



//=== FinishWithDynamicTextures @ 00cb0d20 ===

/* [bsim sim=0.5072222081760475 <- ego_r]
   private: void __thiscall CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(void) */

void __thiscall
CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures(CEngineScreenEffectOutlineGlow *this)

{
  HANDLE hHandle;
  
  hHandle = (HANDLE)InterlockedExchange((LONG *)(this + 0x14),0);
  if (hHandle != (HANDLE)0x0) {
    WaitForSingleObject(hHandle,0xffffffff);
    CloseHandle(hHandle);
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 0x30));
  DeleteCriticalSection((LPCRITICAL_SECTION)(this + 0x18));
  if (*(HANDLE *)(this + 8) != (HANDLE)0x0) {
    CloseHandle(*(HANDLE *)(this + 8));
  }
  if (*(HANDLE *)(this + 4) != (HANDLE)0x0) {
    CloseHandle(*(HANDLE *)(this + 4));
  }
  return;
}



//=== FinishWithDynamicTextures @ 00cb1cc0 ===

/* [bsim sim=0.6446070813467596 <- ego_r]
   private: void __thiscall CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures(void)
    */

void __thiscall
CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
          (CEngineScreenEffectDisplacementRenderer *this)

{
  int *piVar1;
  
  if (*(int *)(this + 0x40) != 0) {
    CoTaskMemFree(*(LPVOID *)(this + 0x44));
    *(undefined4 *)(this + 0x40) = 0;
    *(undefined4 *)(this + 0x44) = 0;
  }
  piVar1 = *(int **)(this + 0x3c);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0x3c) = 0;
  }
  return;
}



//=== _Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_> @ 00cb6f10 ===

/* [bsim sim=0.6419571714072203 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> >(struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,struct std::pair<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph> *,class CKeyPairCompareLess<unsigned long,class
   CRandomAppearanceMorph::CTextureMorph>) */

void __fastcall
std::
_Sort_heap<std::pair<unsigned_long,CRandomAppearanceMorph::CTextureMorph>*,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>
          (int param_1,int param_2,undefined4 param_3)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  CCharString aCStack_34 [4];
  undefined4 local_30;
  undefined4 local_2c;
  CCharString local_28 [4];
  undefined4 local_24;
  CCharString local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  
  if (1 < (param_2 - param_1) / 0x18) {
    pCVar2 = (CCharString *)(param_2 + -0xc);
    do {
      uStack_18 = 0;
      uStack_1c = param_3;
      pCVar1 = pCVar2 + -0xc;
      CCharString::CCharString(aCStack_34,pCVar1);
      local_30 = *(undefined4 *)(pCVar2 + -8);
      local_2c = *(undefined4 *)(pCVar2 + -4);
      CCharString::CCharString(local_28,pCVar2);
      local_24 = *(undefined4 *)(pCVar2 + 4);
      local_20 = pCVar2[8];
      _Pop_heap_0<std::pair<CCharString,CScriptInfo>*,std::pair<class_CCharString,class_CScriptInfo>,CKeyPairCompareLess<CCharString,CScriptInfo>_>
                (param_1,pCVar1,pCVar1);
      pCVar2 = pCVar2 + -0x18;
    } while (1 < (int)(pCVar2 + (0xc - param_1)) / 0x18);
  }
  return;
}



//=== fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo> @ 00cb9b50 ===

/* [bsim sim=0.5808324194636016 <- ego_r]
   void __fastcall std::fill<class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo>(class
   CTextureManagerPoolInfo *,class CTextureManagerPoolInfo *,class CTextureManagerPoolInfo const &)
    */

void __fastcall
std::fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>
          (CTextureManagerPoolInfo *param_1,CTextureManagerPoolInfo *param_2,
          CTextureManagerPoolInfo *param_3)

{
  for (; param_2 != (CTextureManagerPoolInfo *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (CTextureManagerPoolInfo *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      CCharString::CCharString((CCharString *)(param_1 + 0x10),(CCharString *)(param_3 + 0x10));
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



//=== _Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_> @ 00f3b220 ===

/* [bsim sim=0.6882304618196631 <- ego_r]
   void __fastcall std::_Fill<struct std::pair<enum EHeroMorphType,class CTextureMorphEntry>
   *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> >(struct std::pair<enum
   EHeroMorphType,class CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class
   CTextureMorphEntry> *,struct std::pair<enum EHeroMorphType,class CTextureMorphEntry> const &) */

void __fastcall
std::
_Fill<std::pair<EHeroMorphType,CTextureMorphEntry>*,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>_>
          (pair<EHeroMorphType,CTextureMorphEntry> *param_1,
          pair<EHeroMorphType,CTextureMorphEntry> *param_2,
          pair<enum_EHeroMorphType,class_CTextureMorphEntry> *param_3)

{
  for (; param_2 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (pair<EHeroMorphType,CTextureMorphEntry> *)0x0) {
      *(undefined4 *)param_1 = *(undefined4 *)param_3;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_3 + 0xc);
      *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_3 + 0x10);
      *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_3 + 0x14);
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



