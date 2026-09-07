//=== CTCInventoryMap::BuildMapTeleportUI @ 0060c900 (seed 0x0060c900) ===

/* [forge-autoname2 conf=high <- wf] thiscall CTCInventoryMap; massive callees list includes UI
   strings (UI_WORLD_MAP, UI_LIST_TELEPORT), region building, markers */

CComponent * CTCInventoryMap__BuildMapTeleportUI(undefined4 param_1,char param_2,int param_3)

{
  LTextTreeWalkThrough *this;
  char cVar1;
  undefined4 *puVar2;
  CManager *pCVar3;
  CDataBank *this_00;
  void *pvVar4;
  undefined1 *puVar5;
  int *piVar6;
  CManager *_Memory;
  CAIStateGroup_SummonerCharge *pCVar7;
  char cVar8;
  CDisplayEngine *pCVar9;
  CTCInventoryBase *pCVar10;
  CCategory *pCVar11;
  long lVar12;
  undefined1 *puVar13;
  CWorld *this_01;
  CIDrawEnvironment *pCVar14;
  CManager *pCVar15;
  CBaseObject *pCVar16;
  COptimisedPrimitive *this_02;
  CDefinitionManager *this_03;
  CFrontEndManager *this_04;
  CComponent *pCVar17;
  CWorldMap *pCVar18;
  CRegion *pCVar19;
  CDisplayEngine *pCVar20;
  long lVar21;
  CComponent *pCVar22;
  CCharString *pCVar23;
  undefined4 *puVar24;
  void *pvVar25;
  char *pcVar26;
  int *piVar27;
  CThing *pCVar28;
  float *pfVar29;
  int *piVar30;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar31;
  CTCInventoryMap *in_ECX;
  undefined1 *puVar32;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  int *extraout_ECX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  int iVar33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  int iVar34;
  CCategory *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_55;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_56;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_57;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_58;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_59;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_60;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_61;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_62;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_63;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_64;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_65;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_66;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_67;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_68;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_69;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_70;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_71;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_72;
  CComponent *unaff_EBX;
  CRegion *unaff_EBP;
  uint uVar36;
  int *unaff_ESI;
  CIDrawEnvironment *pCVar37;
  CAIStateGroup_SummonerCharge *pCVar38;
  CRegion *pCVar39;
  CAIStateGroup_SummonerCharge *pCVar40;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CAIStateGroup_SummonerCharge *pCVar41;
  CRegion *pCVar42;
  bool bVar43;
  bool bVar44;
  undefined8 uVar45;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar46;
  CTCInventoryMap *pCVar47;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar48;
  CComponent **ppCVar49;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *this_05;
  CRegion *pCVar50;
  CTCInventoryMap *pCVar51;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar52;
  CViewBase *this_06;
  CViewBase *pCVar53;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *this_07;
  int *piStack_298;
  CManager *pCStack_294;
  CTCInventoryMap *local_290;
  CComponent *pCStack_28c;
  CCategory *pCStack_288;
  CAIStateGroup_SummonerCharge *local_284;
  CAIStateGroup_SummonerCharge *local_280;
  CRegion *local_27c;
  CComponent *pCStack_278;
  CComponent *pCStack_274;
  CManager *pCStack_270;
  CComponent *pCStack_26c;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_268;
  undefined4 uStack_264;
  CComponent *pCStack_260;
  CRegion *pCStack_25c;
  int *piStack_258;
  undefined4 uStack_254;
  CComponent *pCStack_250;
  int *local_24c;
  CComponent *pCStack_248;
  int *piStack_244;
  CComponent *pCStack_240;
  int *piStack_23c;
  int *piStack_238;
  CComponent *pCStack_234;
  undefined4 *puStack_230;
  int iStack_22c;
  CComponent *pCStack_228;
  undefined4 *puStack_224;
  undefined4 uStack_220;
  int *piStack_21c;
  CComponent *pCStack_218;
  undefined4 *puStack_214;
  int *piStack_210;
  undefined4 uStack_208;
  int *piStack_204;
  uint uStack_200;
  undefined4 uStack_1f8;
  CComponent *pCStack_1f4;
  int *piStack_1f0;
  CComponent *pCStack_1e8;
  int *piStack_1e4;
  int *piStack_1e0;
  CComponent *pCStack_1dc;
  undefined4 *puStack_1d8;
  undefined4 local_1d0;
  CComponent *pCStack_1cc;
  undefined4 *puStack_1c8;
  CCharString aCStack_1c4 [4];
  undefined1 *puStack_1c0;
  CComponent *pCStack_1bc;
  CAIStateGroup_SummonerCharge *pCStack_1b8;
  undefined4 *puStack_1b4;
  CComponent *pCStack_1b0;
  undefined4 *puStack_1ac;
  int *piStack_1a8;
  CCharString aCStack_1a4 [4];
  CCharString aCStack_1a0 [4];
  CCharString aCStack_19c [4];
  CCharString aCStack_198 [4];
  CViewBase *pCStack_194;
  undefined4 uStack_190;
  CRegion *pCStack_18c;
  CRegion *pCStack_188;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_184 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_180 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_17c [12];
  int *piStack_170;
  undefined4 uStack_16c;
  undefined4 uStack_168;
  CRegion *pCStack_164;
  CRegion *pCStack_160;
  undefined4 uStack_15c;
  int *piStack_158;
  undefined1 uStack_154;
  undefined1 uStack_153;
  undefined1 uStack_152;
  undefined1 uStack_151;
  undefined4 uStack_150;
  CCharString aCStack_14c [20];
  CCharString aCStack_138 [8];
  CViewBase *pCStack_130;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  CRegion *pCStack_124;
  CRegion *pCStack_120;
  undefined4 uStack_11c;
  int *piStack_118;
  undefined1 uStack_114;
  undefined1 uStack_113;
  undefined1 uStack_112;
  undefined1 uStack_111;
  undefined4 uStack_110;
  CCharString CStack_10c;
  CRegion aCStack_108 [12];
  CRegion aCStack_fc [4];
  CRegion aCStack_f8 [4];
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  int iStack_ec;
  int iStack_e8;
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  CEngineInit aCStack_d0 [4];
  CRegion *apCStack_cc [5];
  CCharString aCStack_b8 [8];
  CViewBase *pCStack_b0;
  CRegion aCStack_ac [56];
  CAIStateGroup_SummonerCharge aCStack_74 [116];
  
  this = (LTextTreeWalkThrough *)(in_ECX + 0x218);
  in_ECX[0x248] = (CTCInventoryMap)0x0;
  pCVar41 = (CAIStateGroup_SummonerCharge *)0x0;
  local_1d0 = 0;
  local_284 = (CAIStateGroup_SummonerCharge *)0x0;
  local_280 = (CAIStateGroup_SummonerCharge *)0x0;
  local_27c = (CRegion *)0x0;
  local_290 = in_ECX;
  if (*(int *)(in_ECX + 0x21c) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this,*(LTextBinNode **)(*(int *)this + 4));
    *(int *)(*(int *)this + 8) = *(int *)this;
    *(undefined4 *)(*(int *)this + 4) = 0;
    *(int *)(*(int *)this + 0xc) = *(int *)this;
    *(undefined4 *)(in_ECX + 0x21c) = 0;
  }
  CCharString::CCharString((CCharString *)&local_24c,"",-1);
  cVar8 = (**(code **)(*(int *)(in_ECX + 0x130) + 0xc))();
  if (cVar8 == '\0') {
    pCVar37 = (CIDrawEnvironment *)0x1;
    pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
    pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
    lVar12 = CWorldMap::DrawGetNoMaps(pCVar18);
    if (1 < lVar12) {
      do {
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        this_01 = (CWorld *)(**(code **)(*(int *)pCVar9 + 0x34))();
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        pCVar14 = pCVar37;
        pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
        pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,(long)pCVar14);
        pCVar28 = CTCInventoryMap::GetRegionTeleporter(in_ECX,(CRegion *)pCVar11);
        if ((pCVar28 != (CThing *)0x0) &&
           (pCVar14 = CWorld::DrawGetEnvironment(this_01), pCVar14 != pCVar37)) {
          pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
          pCVar14 = pCVar37;
          pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
          pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,(long)pCVar14);
          if (pCVar11[0x54] != (CCategory)0x0) {
            pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
            pCVar14 = pCVar37;
            pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
            NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,(long)pCVar14);
            Vector_PushBack_Or_Reallocate();
          }
        }
        pCVar37 = pCVar37 + 1;
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
        lVar12 = CWorldMap::DrawGetNoMaps(pCVar18);
      } while ((int)pCVar37 < lVar12);
    }
    pCVar15 = malloc(0x10);
    *(CManager **)pCVar15 = pCVar15;
    *(CManager **)(pCVar15 + 4) = pCVar15;
    pCStack_270 = pCVar15;
    Campfire_InsertRegionNode();
    pCStack_294 = *(CManager **)pCVar15;
    if (pCStack_294 != pCVar15) {
      do {
        pCVar16 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCStack_294 + 8));
        piStack_298 = (int *)(CONCAT13((char)(*(uint *)(pCVar16 + 0x2c) >> 0xc),piStack_298._0_3_) &
                             0x1ffffff);
        if ((*(uint *)(pCVar16 + 0x2c) >> 0xc & 1) != 0) {
          pCStack_28c = (CComponent *)0x6c;
          piVar27 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                           ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                         *)(pCVar16 + 0x44),(ETCInterfaceType *)&pCStack_28c);
          if ((piVar27 == *(int **)(pCVar16 + 0x48)) || (0x6c < *piVar27)) {
            piVar27 = *(int **)(pCVar16 + 0x48);
          }
          pCStack_288 = (CCategory *)piVar27[1];
        }
        CViewBase__GetMarkerRegionName();
        CViewBase__GetMarkerRegionName();
        cVar8 = CCharString__NotEqual();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_188,
                   extraout_EDX_06,unaff_EDI);
        pCVar51 = local_290;
        ppVar35 = extraout_EDX_07;
        if (cVar8 != '\0') {
          pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)local_290);
          pCVar20 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar51);
          pCVar23 = (CCharString *)CViewBase__GetMarkerRegionName();
          pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
          lVar12 = CWorldMap::GetRegionNumberFromName(pCVar18,pCVar23);
          pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar20 + 0x34))();
          pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,lVar12);
          pCVar28 = CTCInventoryMap::GetRegionTeleporter(pCVar51,(CRegion *)pCVar11);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_184,extraout_EDX_08,unaff_EDI);
          pCVar51 = local_290;
          ppVar35 = extraout_EDX_09;
          if (pCVar28 != (CThing *)0x0) {
            pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)local_290);
            pCVar20 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar51);
            pCVar23 = (CCharString *)CViewBase__GetMarkerRegionName();
            pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
            lVar12 = CWorldMap::GetRegionNumberFromName(pCVar18,pCVar23);
            pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar20 + 0x34))();
            pCStack_278 = (CComponent *)
                          NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,lVar12);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_17c,extraout_EDX_10,unaff_EDI);
            pCVar23 = (CCharString *)CViewBase__GetMarkerRegionName();
            CCharString::operator=((CCharString *)&local_24c,pCVar23);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_180,extraout_EDX_11,unaff_EDI);
            iVar33 = (int)local_280 - (int)local_284 >> 0x1f;
            iVar34 = ((int)local_280 - (int)local_284) / 0x58 + iVar33;
            uVar36 = 0;
            if (iVar34 != iVar33) {
              puVar24 = *(undefined4 **)(pCStack_278 + 0x20);
              pCVar41 = local_284 + 0x20;
              ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)pCStack_278;
              do {
                puVar2 = *(undefined4 **)pCVar41;
                if ((puVar2 == puVar24) ||
                   ((((puVar2 != (undefined4 *)0x0 && (puVar24 != (undefined4 *)0x0)) &&
                     (puVar2[1] == puVar24[1])) &&
                    (lVar12 = CBasicString<char>::Compare((char *)*puVar2,(char *)*puVar24),
                    ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)extraout_EDX_12,
                    lVar12 == 0)))) goto LAB_0060ce4b;
                uVar36 = uVar36 + 1;
                pCVar41 = pCVar41 + 0x58;
              } while (uVar36 < (uint)(iVar34 - iVar33));
            }
            pCVar51 = local_290;
            pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)local_290);
            pCVar37 = CWorld::DrawGetEnvironment((CWorld *)pCVar9);
            pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar51);
            pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                                ((CTCInventoryBase *)pCVar9,(long)pCVar37);
            pCVar17 = pCStack_278;
            bVar43 = std::less<CCharString>::operator()
                               ((less<CCharString> *)(pCStack_278 + 0x20),
                                (CCharString *)(pCVar11 + 0x20),(CCharString *)unaff_EDI);
            pCVar41 = local_280;
            ppVar35 = extraout_EDX_13;
            if (bVar43) {
              if (local_280 == (CAIStateGroup_SummonerCharge *)local_27c) {
                VectorReallocateWithRange();
                ppVar35 = extraout_EDX_15;
              }
              else {
                if (local_280 != (CAIStateGroup_SummonerCharge *)0x0) {
                  CRegion::CRegion((CRegion *)local_280,(CRegion *)pCVar17);
                  ppVar35 = extraout_EDX_14;
                }
                local_280 = pCVar41 + 0x58;
              }
            }
          }
        }
LAB_0060ce4b:
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_18c,
                   ppVar35,unaff_EDI);
        pCStack_294 = *(CManager **)pCStack_294;
      } while (pCStack_294 != pCStack_270);
    }
    pCVar51 = local_290;
    cVar8 = (**(code **)(*(int *)(local_290 + 0x130) + 0xc))();
    if ((cVar8 == '\0') && (lVar12 = CTCInventoryMap::GetGuildSealRecallRegion(pCVar51), 0 < lVar12)
       ) {
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar51);
      pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
      pCStack_288 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,lVar12);
      iVar33 = (int)local_280 - (int)local_284 >> 0x1f;
      iVar34 = ((int)local_280 - (int)local_284) / 0x58 + iVar33;
      uVar36 = 0;
      if (iVar34 != iVar33) {
        puVar24 = *(undefined4 **)(pCStack_288 + 0x20);
        pCVar41 = local_284 + 0x20;
        do {
          puVar2 = *(undefined4 **)pCVar41;
          if ((puVar2 == puVar24) ||
             (((puVar2 != (undefined4 *)0x0 && (puVar24 != (undefined4 *)0x0)) &&
              ((puVar2[1] == puVar24[1] &&
               (lVar12 = CBasicString<char>::Compare((char *)*puVar2,(char *)*puVar24), lVar12 == 0)
               ))))) goto LAB_0060cf4a;
          uVar36 = uVar36 + 1;
          pCVar41 = pCVar41 + 0x58;
        } while (uVar36 < (uint)(iVar34 - iVar33));
      }
      pCVar41 = local_280;
      if (local_280 == (CAIStateGroup_SummonerCharge *)local_27c) {
        VectorReallocateWithRange();
      }
      else {
        if (local_280 != (CAIStateGroup_SummonerCharge *)0x0) {
          CRegion::CRegion((CRegion *)local_280,(CRegion *)pCStack_288);
        }
        local_280 = pCVar41 + 0x58;
      }
    }
LAB_0060cf4a:
    _Memory = pCStack_270;
    pCVar15 = *(CManager **)pCStack_270;
    while (pCVar15 != _Memory) {
      pCVar3 = *(CManager **)pCVar15;
      (*(code *)**(undefined4 **)(pCVar15 + 8))();
      free(pCVar15);
      pCVar15 = pCVar3;
    }
    *(CManager **)_Memory = _Memory;
    *(CManager **)(_Memory + 4) = _Memory;
    free(_Memory);
    ppVar35 = extraout_EDX_16;
    in_ECX = local_290;
    pCVar41 = local_280;
  }
  else {
    puStack_1c0 = (undefined1 *)0x0;
    puStack_1c0 = malloc(0x14);
    pCStack_1bc = (CComponent *)0x0;
    *puStack_1c0 = 0;
    *(undefined4 *)(puStack_1c0 + 4) = 0;
    *(undefined1 **)(puStack_1c0 + 8) = puStack_1c0;
    *(undefined1 **)(puStack_1c0 + 0xc) = puStack_1c0;
    CTCInventoryMap::GetRegionsToDisplay
              (in_ECX,(set<long,std::less<long>,std::allocator<long>_> *)&puStack_1c0);
    puVar13 = *(undefined1 **)(puStack_1c0 + 8);
    ppVar35 = extraout_EDX;
    if (puVar13 != puStack_1c0) {
      do {
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        lVar12 = *(long *)(puVar13 + 0x10);
        pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
        pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,lVar12);
        if (pCVar11[0x54] == (CCategory)0x0) {
          iVar33 = (int)*(short *)(*(int *)(in_ECX + 4) + 0x8e);
          pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
          lVar12 = CWorldMap::GetRegionNumberMapIsIn((CWorldMap *)pCVar9,iVar33);
          ppVar35 = extraout_EDX_00;
          if (*(int *)(puVar13 + 0x10) == lVar12) goto LAB_0060ca21;
        }
        else {
LAB_0060ca21:
          pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
          lVar12 = *(long *)(puVar13 + 0x10);
          pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
          pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,lVar12);
          if (pCVar41 == (CAIStateGroup_SummonerCharge *)local_27c) {
            VectorReallocateWithRange();
            ppVar35 = extraout_EDX_03;
            pCVar41 = local_280;
          }
          else {
            ppVar35 = extraout_EDX_01;
            if (pCVar41 != (CAIStateGroup_SummonerCharge *)0x0) {
              CRegion::CRegion((CRegion *)pCVar41,(CRegion *)pCVar11);
              ppVar35 = extraout_EDX_02;
            }
            local_280 = pCVar41 + 0x58;
            pCVar41 = local_280;
          }
        }
        puVar32 = *(undefined1 **)(puVar13 + 0xc);
        if (puVar32 == (undefined1 *)0x0) {
          puVar32 = *(undefined1 **)(puVar13 + 4);
          if (puVar13 == *(undefined1 **)(puVar32 + 0xc)) {
            do {
              puVar13 = puVar32;
              puVar32 = *(undefined1 **)(puVar13 + 4);
            } while (puVar13 == *(undefined1 **)(puVar32 + 0xc));
          }
          if (*(undefined1 **)(puVar13 + 0xc) != puVar32) {
            puVar13 = puVar32;
          }
        }
        else {
          for (puVar5 = *(undefined1 **)(puVar32 + 8); puVar13 = puVar32,
              puVar5 != (undefined1 *)0x0; puVar5 = *(undefined1 **)(puVar5 + 8)) {
            puVar32 = puVar5;
          }
        }
      } while (puVar13 != puStack_1c0);
    }
    if (pCStack_1bc != (CComponent *)0x0) {
      LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
                ((LTextTreeWalkThrough *)&puStack_1c0,*(LTextBinNode **)(puStack_1c0 + 4));
      *(undefined1 **)(puStack_1c0 + 8) = puStack_1c0;
      *(undefined4 *)(puStack_1c0 + 4) = 0;
      *(undefined1 **)(puStack_1c0 + 0xc) = puStack_1c0;
      pCStack_1bc = (CComponent *)0x0;
      ppVar35 = extraout_EDX_04;
    }
    if (puStack_1c0 != (undefined1 *)0x0) {
      free(puStack_1c0);
      ppVar35 = extraout_EDX_05;
    }
  }
  pCVar40 = local_284;
  if ((param_2 != '\0') && (param_3 != 0)) {
    *(bool *)param_3 = local_284 != pCVar41;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_24c,ppVar35,
               unaff_EDI);
    for (pCVar38 = pCVar40; pCVar38 != pCVar41; pCVar38 = pCVar38 + 0x58) {
      CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(pCVar38);
    }
LAB_0060d00e:
    if (pCVar40 != (CAIStateGroup_SummonerCharge *)0x0) {
      free(pCVar40);
    }
    return (CComponent *)0x0;
  }
  iVar33 = (int)pCVar41 - (int)local_284 >> 0x1f;
  ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
            (((int)pCVar41 - (int)local_284) / 0x58 + iVar33);
  pCStack_250 = (CComponent *)(ppVar35 + -iVar33);
  if (pCStack_250 == (CComponent *)0x0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_24c,ppVar35,
               unaff_EDI);
    pCVar40 = local_284;
    for (pCVar38 = local_284; pCVar38 != pCVar41; pCVar38 = pCVar38 + 0x58) {
      CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(pCVar38);
    }
    goto LAB_0060d00e;
  }
  pCStack_288 = (CCategory *)0x0;
  CCharString::CCharString((CCharString *)&pCStack_278,"UI_MISC_THINGS_DEF",-1);
  ppCVar49 = &pCStack_278;
  this_02 = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
  lVar12 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                     (this_02,(ulong)ppCVar49);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_278,
             extraout_EDX_17,unaff_EDI);
  pCVar52 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&pCStack_288;
  this_03 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_03,lVar12,pCVar52);
  this_04 = NUISystem::CFrontEndManager::GetInstance();
  pCStack_278 = (CComponent *)this_04;
  CCharString::CCharString((CCharString *)&pCStack_294,"UI_WORLD_MAP",-1);
  pCStack_240 = NUISystem::CManager::CreateComponent
                          ((CManager *)this_04,(CCharString *)&pCStack_294,false);
  ppVar35 = extraout_EDX_18;
  if ((pCStack_240 == (CComponent *)0x0) ||
     (piStack_23c = operator_new(0xc), ppVar35 = extraout_EDX_19, piStack_23c == (int *)0x0)) {
    piStack_23c = (int *)0x0;
  }
  else {
    *piStack_23c = 1;
    piStack_23c[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
    piStack_23c[2] = (int)pCStack_240;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_294,ppVar35,
             unaff_EDI);
  *(CComponent **)(in_ECX + 0x188) = pCStack_240;
  iStack_ec = *(int *)(pCStack_288 + 0xe0);
  iStack_e8 = *(int *)(pCStack_288 + 0xe4);
  this_06 = (CViewBase *)&uStack_f4;
  uStack_e4 = 0x3e4ccccd;
  uStack_e0 = 0x3e4ccccd;
  uStack_f4 = 0x3fc00000;
  uStack_f0 = 0x3fc00000;
  (**(code **)(*(int *)pCStack_240 + 0x240))();
  pCVar19 = (CRegion *)&iStack_e8;
  (**(code **)(**(int **)(in_ECX + 0x188) + 0x23c))();
  ppVar31 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&uStack_f4;
  (**(code **)(**(int **)(in_ECX + 0x188) + 0x244))();
  cVar8 = (**(code **)(*(int *)(in_ECX + 0x130) + 0xc))();
  if (cVar8 == '\0') {
    CCharString::CCharString((CCharString *)&pCStack_1e8,"UI_LIST_TELEPORT",-1);
  }
  else {
    CCharString::CCharString((CCharString *)&pCStack_1e8,"UI_LIST_MAP",-1);
  }
  pCVar17 = NUISystem::CManager::CreateComponent
                      ((CManager *)this_04,(CCharString *)&pCStack_1e8,false);
  pCStack_278 = pCVar17;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1e8,
             extraout_EDX_20,ppVar31);
  *(CComponent **)(in_ECX + 0x214) = pCVar17;
  CCharString::CCharString((CCharString *)&stack0xfffffd60,"UI_ZOOM_STICK",-1);
  pCStack_1b8 = (CAIStateGroup_SummonerCharge *)
                NUISystem::CManager::CreateComponent
                          ((CManager *)this_04,(CCharString *)&stack0xfffffd60,false);
  ppVar35 = extraout_EDX_21;
  if ((pCStack_1b8 == (CAIStateGroup_SummonerCharge *)0x0) ||
     (puStack_1b4 = operator_new(0xc), ppVar35 = extraout_EDX_22, puStack_1b4 == (undefined4 *)0x0))
  {
    puStack_1b4 = (undefined4 *)0x0;
  }
  else {
    *puStack_1b4 = 1;
    puStack_1b4[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
    puStack_1b4[2] = pCStack_1b8;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd60,
             ppVar35,ppVar31);
  ppVar31 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_1b8;
  (**(code **)(*(int *)pCVar17 + 0x244))();
  CCharString::CCharString((CCharString *)&stack0xfffffd5c,"UI_MINI_MAP_VIEWPORT_FOR_MAP_SCREEN",-1)
  ;
  pCStack_228 = NUISystem::CManager::CreateComponent
                          ((CManager *)this_04,(CCharString *)&stack0xfffffd5c,false);
  ppVar35 = extraout_EDX_23;
  if ((pCStack_228 == (CComponent *)0x0) ||
     (puStack_224 = operator_new(0xc), ppVar35 = extraout_EDX_24, puStack_224 == (undefined4 *)0x0))
  {
    puStack_224 = (undefined4 *)0x0;
  }
  else {
    *puStack_224 = 1;
    puStack_224[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
    puStack_224[2] = pCStack_228;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd5c,
             ppVar35,ppVar31);
  ppVar31 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_228;
  (**(code **)(*(int *)pCVar17 + 0x244))();
  CCharString::CCharString((CCharString *)&stack0xfffffd58,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1);
  pCStack_1b0 = NUISystem::CManager::CreateComponent
                          ((CManager *)this_04,(CCharString *)&stack0xfffffd58,false);
  ppVar35 = extraout_EDX_25;
  if ((pCStack_1b0 == (CComponent *)0x0) ||
     (puStack_1ac = operator_new(0xc), ppVar35 = extraout_EDX_26, puStack_1ac == (undefined4 *)0x0))
  {
    puStack_1ac = (undefined4 *)0x0;
  }
  else {
    *puStack_1ac = 1;
    puStack_1ac[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
    puStack_1ac[2] = pCStack_1b0;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd58,
             ppVar35,ppVar31);
  pCVar51 = (CTCInventoryMap *)&pCStack_1b0;
  (**(code **)(*(int *)pCVar17 + 0x244))();
  (**(code **)(*(int *)pCVar17 + 0x270))();
  pCVar42 = (CRegion *)0x0;
  piStack_204 = (int *)0x0;
  piStack_204 = malloc(0x14);
  uStack_200 = 0;
  *(undefined1 *)piStack_204 = 0;
  piStack_204[1] = 0;
  piStack_204[2] = (int)piStack_204;
  piStack_204[3] = (int)piStack_204;
  uStack_190 = 0;
  pCStack_18c = (CRegion *)0x0;
  pCStack_188 = (CRegion *)0x0;
  CTCInventoryMap::GetRegionsToDisplay
            (in_ECX,(set<long,std::less<long>,std::allocator<long>_> *)&piStack_204);
  cVar8 = (**(code **)(*(int *)(in_ECX + 0x130) + 0xc))();
  ppVar31 = ppStack_268;
  if ((cVar8 == '\0') && (pCStack_26c != (CComponent *)0x0)) {
    pCVar39 = unaff_EBP + 0x18;
    pCVar17 = pCStack_26c;
    do {
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      pCVar50 = pCVar39;
      pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
      unaff_EDI = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                  CWorldMap::GetRegionNumberFromName(pCVar18,(CCharString *)pCVar50);
      BinarySearchTree_Insert();
      pCVar39 = pCVar39 + 0x58;
      pCVar17 = pCVar17 + -1;
    } while (pCVar17 != (CComponent *)0x0);
  }
  piVar27 = (int *)piStack_204[2];
  if (piVar27 != piStack_204) {
    do {
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      iVar33 = piVar27[4];
      pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
      pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,iVar33);
      if (pCVar11[0x54] != (CCategory)0x0) {
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        iVar33 = piVar27[4];
        pCVar10 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar9 + 0x34))();
        pCVar11 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar10,iVar33);
        if (pCVar42 == pCStack_188) {
          VectorReallocateWithRange();
          pCVar42 = pCStack_18c;
        }
        else {
          if (pCVar42 != (CRegion *)0x0) {
            CRegion::CRegion(pCVar42,(CRegion *)pCVar11);
          }
          pCStack_18c = pCVar42 + 0x58;
          pCVar42 = pCStack_18c;
        }
      }
      piVar30 = (int *)piVar27[3];
      if (piVar30 == (int *)0x0) {
        piVar30 = (int *)piVar27[1];
        if (piVar27 == (int *)piVar30[3]) {
          do {
            piVar27 = piVar30;
            piVar30 = (int *)piVar27[1];
          } while (piVar27 == (int *)piVar30[3]);
        }
        if ((int *)piVar27[3] != piVar30) {
          piVar27 = piVar30;
        }
      }
      else {
        for (piVar6 = (int *)piVar30[2]; piVar27 = piVar30, piVar6 != (int *)0x0;
            piVar6 = (int *)piVar6[2]) {
          piVar30 = piVar6;
        }
      }
    } while (piVar27 != piStack_204);
  }
  CTCInventoryMap::CreateMapMarkers
            (in_ECX,(CComponent *)pCStack_25c,
             (vector<CRegion,std::allocator<CRegion>_> *)&uStack_190);
  pCStack_28c = (CComponent *)0xffffffff;
  cVar8 = (**(code **)(*(int *)(in_ECX + 0x130) + 0xc))();
  if (cVar8 != '\0') goto LAB_0060d723;
  pCStack_1e8 = (CComponent *)0xffffffff;
  pCStack_274 = (CComponent *)0x0;
  if (pCStack_26c != (CComponent *)0x0) {
    pCVar19 = unaff_EBP + 0x18;
    do {
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      pCVar20 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      if (*(float *)(pCVar20 + 0x15c) * *(float *)(pCVar20 + 0x15c) +
          *(float *)(pCVar20 + 0x158) * *(float *)(pCVar20 + 0x158) +
          *(float *)(pCVar20 + 0x154) * *(float *)(pCVar20 + 0x154) <= 9.999999e-09) {
        lVar12 = 0;
      }
      else {
        pCVar20 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar20 + 0x34))();
        lVar12 = (**(code **)(*(int *)pCVar18 + 0x40))();
        lVar12 = CWorldMap::GetRegionNumberMapIsIn(pCVar18,lVar12);
      }
      pCVar42 = pCVar19;
      pCVar39 = pCVar19;
      pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
      lVar21 = CWorldMap::GetRegionNumberFromName(pCVar18,(CCharString *)pCVar42);
      if (lVar21 == lVar12) {
        pCStack_28c = pCStack_274;
      }
      uVar45 = CCharString::CCharString((CCharString *)&stack0xfffffd58,"HeroGuildComplexInside",-1)
      ;
      ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar45 >> 0x20);
      piVar27 = *(int **)pCVar19;
      piVar30 = (int *)*(undefined4 *)uVar45;
      if (piVar27 == piVar30) {
        bVar43 = true;
      }
      else if (((piVar27 == (int *)0x0) || (piVar30 == (int *)0x0)) ||
              (ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)piVar27[1],
              ppVar35 != (pair<EHeroMorphType,CParticleMorphs::CEntry> *)piVar30[1])) {
        bVar43 = false;
      }
      else {
        pcVar26 = (char *)*piVar30;
        iVar33 = *piVar27 - (int)pcVar26;
        while( true ) {
          cVar8 = pcVar26[iVar33];
          if ((cVar8 == '\0') && (*pcVar26 == '\0')) {
            bVar43 = true;
            goto LAB_0060d63e;
          }
          cVar1 = *pcVar26;
          ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                    CONCAT31((int3)((uint)ppVar35 >> 8),cVar1);
          if (cVar8 < cVar1) {
            bVar43 = false;
            goto LAB_0060d63e;
          }
          if (cVar1 < cVar8) break;
          pcVar26 = pcVar26 + 1;
        }
        bVar43 = false;
      }
LAB_0060d63e:
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd58,
                 ppVar35,ppVar31);
      if (bVar43) {
        pCStack_1e8 = pCStack_274;
      }
      pCStack_274 = pCStack_274 + 1;
      pCVar19 = pCVar39 + 0x58;
    } while (pCStack_274 < pCStack_26c);
  }
  CEngineInit::CEngineInit(aCStack_d0);
  pCVar17 = pCStack_28c;
  bVar43 = pCStack_28c != (CComponent *)0xffffffff;
  if (bVar43) {
    iVar33 = (int)pCStack_28c * 0x58;
    CRegion::operator=((CRegion *)aCStack_d0,unaff_EBP + iVar33);
    CRegion::operator=(unaff_EBP + iVar33,unaff_EBP);
    CRegion::operator=(unaff_EBP,(CRegion *)aCStack_d0);
  }
  if (pCStack_1e8 != (CComponent *)0xffffffff) {
    if (pCVar17 == (CComponent *)0x0) {
      bVar44 = pCStack_1e8 == (CComponent *)0x1;
LAB_0060d6de:
      if (bVar44) goto LAB_0060d70f;
    }
    else if (pCVar17 == (CComponent *)0x1) {
      bVar44 = pCStack_1e8 == (CComponent *)0x0;
      goto LAB_0060d6de;
    }
    iVar33 = (int)pCStack_1e8 * 0x58;
    CRegion::operator=((CRegion *)aCStack_d0,unaff_EBP + iVar33);
    CRegion::operator=(unaff_EBP + iVar33,unaff_EBP + (uint)bVar43 * 0x58);
    CRegion::operator=(unaff_EBP + (uint)bVar43 * 0x58,(CRegion *)aCStack_d0);
  }
LAB_0060d70f:
  pCStack_28c = (CComponent *)0xffffffff;
  CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge
            ((CAIStateGroup_SummonerCharge *)aCStack_d0);
LAB_0060d723:
  pCStack_274 = (CComponent *)0x0;
  if (pCStack_26c != (CComponent *)0x0) {
    do {
      CRegion::CRegion((CRegion *)aCStack_d0,unaff_EBP + (int)pCStack_274 * 0x58);
      this_00 = *(CDataBank **)(DAT_013b86a0 + 0x14);
      CRegion::GetDisplayName((CRegion *)aCStack_d0);
      NGameText::CDataBank::GetTextBySymbol(this_00,(CCharString *)&stack0xfffffd50);
      pCStack_194 = this_06 + 0x130;
      cVar8 = (**(code **)(*(int *)(this_06 + 0x130) + 0xc))();
      pCVar53 = this_06;
      if (cVar8 == '\0') {
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine(this_06);
        pCVar23 = aCStack_b8;
        pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
        pCVar17 = (CComponent *)CWorldMap::GetRegionNumberFromName(pCVar18,pCVar23);
        pCVar9 = NDisplayView::CViewBase::GetDisplayEngine(this_06);
        if (*(float *)(pCVar9 + 0x15c) * *(float *)(pCVar9 + 0x15c) +
            *(float *)(pCVar9 + 0x158) * *(float *)(pCVar9 + 0x158) +
            *(float *)(pCVar9 + 0x154) * *(float *)(pCVar9 + 0x154) <= 9.999999e-09) {
          pCVar22 = (CComponent *)0x0;
        }
        else {
          pCVar9 = NDisplayView::CViewBase::GetDisplayEngine(this_06);
          pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
          lVar12 = (**(code **)(*(int *)pCVar18 + 0x40))();
          pCVar22 = (CComponent *)CWorldMap::GetRegionNumberMapIsIn(pCVar18,lVar12);
        }
        if (pCVar17 == pCVar22) {
          CCharString::CCharString((CCharString *)&pCStack_1e8,"TEXT_GUI_MENU_RECALL",-1);
          NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b86a0 + 0x14),aCStack_138);
          pCVar23 = (CCharString *)CCharString__SetFromEnum();
          CCharString::operator+=((CCharString *)&stack0xfffffd50,pCVar23);
          CCharString::~CCharString(aCStack_14c);
          CCharString::~CCharString(aCStack_138);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1e8,
                     extraout_EDX_27,ppVar31);
          pCStack_28c = pCVar17;
        }
      }
      CCharString::CCharString
                ((CCharString *)&piStack_158,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",-1);
      CCharString::CCharString((CCharString *)&stack0xfffffd38,"PC_BUTTON_BASTARD",-1);
      CCharString::CCharString((CCharString *)&stack0xfffffd30,"",-1);
      pCVar15 = pCStack_294;
      local_284 = (CAIStateGroup_SummonerCharge *)
                  NUISystem::CManager::CreateMenuEntryComponent(pCStack_294);
      ppVar35 = extraout_EDX_28;
      if ((local_284 == (CAIStateGroup_SummonerCharge *)0x0) ||
         (local_280 = operator_new(0xc), ppVar35 = extraout_EDX_29,
         local_280 == (CAIStateGroup_SummonerCharge *)0x0)) {
        local_280 = (CAIStateGroup_SummonerCharge *)0x0;
        this_07 = unaff_EDI;
      }
      else {
        *(int *)local_280 = 1;
        *(code **)((int)local_280 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
        *(CAIStateGroup_SummonerCharge **)((int)local_280 + 8) = local_284;
        this_07 = unaff_EDI;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_158,
                 ppVar35,ppVar31);
      cVar8 = (**(code **)(*(int *)pCStack_194 + 0xc))();
      if (cVar8 == '\0') {
        puVar24 = operator_new(0x10);
        if (puVar24 == (undefined4 *)0x0) {
          puVar24 = (undefined4 *)0x0;
        }
        else {
          *puVar24 = 0;
          puVar24[1] = 0;
          pvVar25 = malloc(0xc);
          *(void **)pvVar25 = pvVar25;
          *(void **)((int)pvVar25 + 4) = pvVar25;
          puVar24[1] = pvVar25;
          puVar24[2] = 0;
          pvVar25 = malloc(0xc);
          *(void **)pvVar25 = pvVar25;
          *(void **)((int)pvVar25 + 4) = pvVar25;
          puVar24[2] = pvVar25;
          *(undefined1 *)(puVar24 + 3) = 1;
        }
        piStack_1e4 = puVar24;
        if ((puVar24 == (undefined4 *)0x0) ||
           (piStack_1e0 = operator_new(0xc), piStack_1e0 == (int *)0x0)) {
          piStack_1e0 = (int *)0x0;
        }
        else {
          *piStack_1e0 = 1;
          piStack_1e0[1] = (int)CTCMapwho::OnDie;
          piStack_1e0[2] = (int)piStack_1e4;
        }
        *piStack_1e4 = 0x50;
        (**(code **)(*(int *)local_284 + 0x110))();
        if ((piStack_1e0 != (int *)0x0) && (*piStack_1e0 = *piStack_1e0 + -1, *piStack_1e0 == 0)) {
          (*(code *)piStack_1e0[1])();
          operator_delete(piStack_1e0);
        }
        piStack_1e4 = (int *)0x0;
        piStack_1e0 = (int *)0x0;
      }
      ppVar46 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_284;
      (**(code **)(*(int *)pCStack_288 + 0x260))();
      CCharString::CCharString((CCharString *)&pCStack_164,"UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW",-1);
      pCStack_218 = NUISystem::CManager::CreateComponent(pCVar15,(CCharString *)&pCStack_164,false);
      ppVar35 = extraout_EDX_30;
      if ((pCStack_218 == (CComponent *)0x0) ||
         (puStack_214 = operator_new(0xc), ppVar35 = extraout_EDX_31,
         puStack_214 == (undefined4 *)0x0)) {
        puStack_214 = (undefined4 *)0x0;
      }
      else {
        *puStack_214 = 1;
        puStack_214[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
        puStack_214[2] = pCStack_218;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_164,
                 ppVar35,ppVar46);
      pCVar10 = (CTCInventoryBase *)&stack0xfffffd4c;
      (**(code **)(*(int *)pCStack_218 + 0x240))();
      this_05 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&puStack_224;
      (**(code **)(*(int *)pCStack_294 + 0xec))();
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine(this_06);
      pCVar23 = (CCharString *)apCStack_cc;
      pCVar18 = (CWorldMap *)(**(code **)(*(int *)pCVar9 + 0x34))();
      pCVar42 = (CRegion *)CWorldMap::GetRegionNumberFromName(pCVar18,pCVar23);
      iVar33 = (int)*(short *)(*(int *)(this_06 + 4) + 0x8e);
      pCVar9 = NDisplayView::CViewBase::GetDisplayEngine(this_06);
      pCVar39 = (CRegion *)CWorldMap::GetRegionNumberMapIsIn((CWorldMap *)pCVar9,iVar33);
      cVar8 = (**(code **)(*piStack_1a8 + 0xc))();
      if (cVar8 == '\0') {
        if (pCVar42 == unaff_EBP) {
          if (unaff_EBP != (CRegion *)0xffffffff) goto LAB_0060dc5e;
LAB_0060db53:
          uStack_200 = uStack_200 | 1;
          uVar45 = CCharString::CCharString
                             ((CCharString *)&stack0xfffffd5c,"HeroGuildComplexInside",-1);
          ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar45 >> 0x20);
          pCVar42 = (CRegion *)*(undefined4 *)uVar45;
          if (apCStack_cc[0] != pCVar42) {
            if (((apCStack_cc[0] != (CRegion *)0x0) && (pCVar42 != (CRegion *)0x0)) &&
               (ppVar35 = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(apCStack_cc[0] + 4),
               ppVar35 == *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(pCVar42 + 4))) {
              pcVar26 = *(char **)pCVar42;
              iVar33 = *(int *)apCStack_cc[0] - (int)pcVar26;
              while( true ) {
                cVar8 = pcVar26[iVar33];
                if ((cVar8 == '\0') && (*pcVar26 == '\0')) {
                  iVar33 = 0;
                  goto LAB_0060dbbc;
                }
                cVar1 = *pcVar26;
                ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                          CONCAT31((int3)((uint)ppVar35 >> 8),cVar1);
                if (cVar8 < cVar1) {
                  iVar33 = -1;
                  goto LAB_0060dbbc;
                }
                if (cVar1 < cVar8) break;
                pcVar26 = pcVar26 + 1;
              }
              iVar33 = 1;
LAB_0060dbbc:
              if (iVar33 == 0) goto LAB_0060dbc5;
            }
            goto LAB_0060dbc9;
          }
LAB_0060dbc5:
          bVar43 = true;
        }
        else {
          ppVar35 = extraout_EDX_32;
          if (unaff_EBP == (CRegion *)0xffffffff) goto LAB_0060db53;
LAB_0060dbc9:
          bVar43 = false;
        }
        if ((uStack_200 & 1) != 0) {
          uStack_200 = uStack_200 & 0xfffffffe;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                     &stack0xfffffd5c,ppVar35,this_05);
        }
        if (bVar43) goto LAB_0060dc5e;
        cVar8 = CCharString__NotEqual();
        if (cVar8 != '\0') {
          if (apCStack_cc[0] == local_27c) goto LAB_0060dc5e;
          if (((apCStack_cc[0] != (CRegion *)0x0) && (local_27c != (CRegion *)0x0)) &&
             (*(int *)(apCStack_cc[0] + 4) == *(int *)(local_27c + 4))) {
            pcVar26 = *(char **)local_27c;
            iVar33 = *(int *)apCStack_cc[0] - (int)pcVar26;
            while( true ) {
              cVar8 = pcVar26[iVar33];
              if ((cVar8 == '\0') && (*pcVar26 == '\0')) {
                iVar33 = 0;
                goto LAB_0060dc55;
              }
              if (cVar8 < *pcVar26) {
                iVar33 = -1;
                goto LAB_0060dc55;
              }
              if (*pcVar26 < cVar8) break;
              pcVar26 = pcVar26 + 1;
            }
            iVar33 = 1;
LAB_0060dc55:
            if (iVar33 == 0) goto LAB_0060dc5e;
          }
        }
      }
      else {
        if (pCVar42 != pCVar39) goto LAB_0060dc6f;
LAB_0060dc5e:
        (**(code **)(*(int *)unaff_EBX + 0x278))();
      }
LAB_0060dc6f:
      CCharString::CCharString
                ((CCharString *)&pCStack_188,"UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE",-1);
      unaff_EDI = this_07;
      pCStack_250 = NUISystem::CManager::CreateComponent
                              ((CManager *)this_07,(CCharString *)&pCStack_188,false);
      ppVar35 = extraout_EDX_33;
      if ((pCStack_250 == (CComponent *)0x0) ||
         (local_24c = operator_new(0xc), ppVar35 = extraout_EDX_34, local_24c == (int *)0x0)) {
        local_24c = (int *)0x0;
      }
      else {
        *local_24c = 1;
        local_24c[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        local_24c[2] = (int)pCStack_250;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_188,
                 ppVar35,this_05);
      CTCInventoryMap::ConstructRegionDescription(pCVar51,pCStack_250,(CRegion *)&uStack_e4,false);
      ppCVar49 = &pCStack_250;
      (**(code **)(*piStack_298 + 0xec))();
      iVar33 = *(int *)unaff_EBP;
      ppVar48 = extraout_ECX;
      CCharString::CCharString((CCharString *)&stack0xfffffd20,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1)
      ;
      piVar27 = (int *)(**(code **)(iVar33 + 0xc))();
      if (piVar27 != (int *)0x0) {
        (**(code **)(*piVar27 + 0x238))();
      }
      cVar8 = (**(code **)(*(int *)(ppVar46 + 0x130) + 0xc))();
      if (cVar8 == '\0') {
        CCharString::CCharString((CCharString *)aaStack_184,"UI_SUB_LIST",-1);
        pCStack_260 = NUISystem::CManager::CreateComponent
                                ((CManager *)this_07,(CCharString *)aaStack_184,false);
        ppVar35 = extraout_EDX_48;
        if ((pCStack_260 == (CComponent *)0x0) ||
           (pCStack_25c = operator_new(0xc), ppVar35 = extraout_EDX_49,
           pCStack_25c == (CRegion *)0x0)) {
          pCStack_25c = (CRegion *)0x0;
        }
        else {
          *(int *)pCStack_25c = 1;
          *(code **)(pCStack_25c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_25c + 8) = pCStack_260;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_184,ppVar35,ppVar48);
        pCStack_160 = (CRegion *)0x0;
        pCVar17 = (CComponent *)0x0;
        if (pCStack_25c != (CRegion *)0x0) {
          pCStack_160 = pCStack_25c;
          *(int *)pCStack_25c = *(int *)pCStack_25c + 1;
          pCVar17 = pCStack_260;
        }
        CCharString::CCharString(aCStack_1c4,"UI_TITLE_TELEPORT",-1);
        pCStack_1cc = NUISystem::CManager::CreateComponent((CManager *)this_07,aCStack_1c4,false);
        ppVar35 = extraout_EDX_50;
        if ((pCStack_1cc == (CComponent *)0x0) ||
           (puStack_1c8 = operator_new(0xc), ppVar35 = extraout_EDX_51,
           puStack_1c8 == (undefined4 *)0x0)) {
          puStack_1c8 = (undefined4 *)0x0;
        }
        else {
          *puStack_1c8 = 1;
          puStack_1c8[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
          puStack_1c8[2] = pCStack_1cc;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1c4,
                   ppVar35,ppVar48);
        pCVar47 = (CTCInventoryMap *)&pCStack_1cc;
        (**(code **)(*(int *)pCVar17 + 0x244))();
        ppVar46 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&uStack_264;
        (**(code **)(*unaff_ESI + 0xec))();
        CCharString::CCharString((CCharString *)&puStack_1c0,"TEXT_GUI_MENU_TELEPORT",-1);
        NGameText::CDataBank::GetTextBySymbol
                  (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&stack0xfffffd34);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&puStack_1c0,
                   extraout_EDX_52,ppVar46);
        CCharString::CCharString
                  ((CCharString *)&pCStack_1bc,"UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffd14,"PC_BUTTON_BASTARD",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffd0c,"",-1);
        pCStack_248 = NUISystem::CManager::CreateMenuEntryComponent
                                ((CManager *)this_07,&stack0xfffffd34,0,0x3f800000,&pCStack_1bc);
        ppVar35 = extraout_EDX_53;
        if ((pCStack_248 == (CComponent *)0x0) ||
           (piStack_244 = operator_new(0xc), ppVar35 = extraout_EDX_54,
           piStack_244 == (undefined4 *)0x0)) {
          piStack_244 = (undefined4 *)0x0;
        }
        else {
          *piStack_244 = 1;
          piStack_244[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_244[2] = (int)pCStack_248;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1bc,
                   ppVar35,ppVar46);
        (**(code **)(*(int *)pCVar17 + 0x240))();
        unaff_ESI = operator_new(0x10);
        if (unaff_ESI == (int *)0x0) {
          unaff_ESI = (int *)0x0;
        }
        else {
          *unaff_ESI = 0;
          unaff_ESI[1] = 0;
          pvVar25 = malloc(0xc);
          *(void **)pvVar25 = pvVar25;
          *(void **)((int)pvVar25 + 4) = pvVar25;
          unaff_ESI[1] = (int)pvVar25;
          unaff_ESI[2] = 0;
          pvVar25 = malloc(0xc);
          *(void **)pvVar25 = pvVar25;
          *(void **)((int)pvVar25 + 4) = pvVar25;
          unaff_ESI[2] = (int)pvVar25;
          *(undefined1 *)(unaff_ESI + 3) = 1;
        }
        if ((unaff_ESI == (int *)0x0) ||
           (unaff_EBP = operator_new(0xc), unaff_EBP == (CRegion *)0x0)) {
          unaff_EBP = (CRegion *)0x0;
        }
        else {
          *(int *)unaff_EBP = 1;
          *(code **)(unaff_EBP + 4) = CTCMapwho::OnDie;
          *(int **)(unaff_EBP + 8) = unaff_ESI;
        }
        if ((char)((uint)ppCVar49 >> 0x18) == '\0') {
          pCVar28 = CTCInventoryMap::GetRegionTeleporter((CTCInventoryMap *)pCVar10,aCStack_f8);
          if (pCVar28 != (CThing *)0x0) {
            *unaff_ESI = 5;
            pCVar9 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar10);
            lVar12 = CWorldMap::GetRegionNumberMapIsIn
                               ((CWorldMap *)pCVar9,(int)*(short *)(pCVar28 + 0x8e));
            iVar33 = unaff_ESI[2];
            piVar27 = malloc(0xc);
            if (piVar27 + 2 != (int *)0x0) {
              piVar27[2] = lVar12;
            }
            puVar24 = *(undefined4 **)(iVar33 + 4);
            *piVar27 = iVar33;
            piVar27[1] = (int)puVar24;
            *puVar24 = piVar27;
            *(int **)(iVar33 + 4) = piVar27;
          }
        }
        else {
          *unaff_ESI = 0x3d;
        }
        ppVar46 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffd5c;
        (**(code **)(*local_24c + 0x108))();
        CCharString::CCharString((CCharString *)&pCStack_1cc,"UI_MAP_MENU_MINI_MAP_TEMPLATE",-1);
        local_280 = (CAIStateGroup_SummonerCharge *)
                    NUISystem::CManager::CreateComponent
                              ((CManager *)this_07,(CCharString *)&pCStack_1cc,false);
        ppVar35 = extraout_EDX_55;
        if ((local_280 == (CAIStateGroup_SummonerCharge *)0x0) ||
           (local_27c = operator_new(0xc), ppVar35 = extraout_EDX_56, local_27c == (CRegion *)0x0))
        {
          local_27c = (CRegion *)0x0;
        }
        else {
          *(undefined4 *)local_27c = 1;
          *(code **)((int)local_27c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CAIStateGroup_SummonerCharge **)((int)local_27c + 8) = local_280;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1cc,
                   ppVar35,ppVar46);
        pCVar17 = pCStack_260;
        CCharString::CCharString
                  ((CCharString *)&local_1d0,"UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE",-1);
        pCStack_248 = NUISystem::CManager::CreateComponent
                                ((CManager *)this_07,(CCharString *)&local_1d0,false);
        ppVar35 = extraout_EDX_57;
        if ((pCStack_248 == (CComponent *)0x0) ||
           (piStack_244 = operator_new(0xc), ppVar35 = extraout_EDX_58,
           piStack_244 == (undefined4 *)0x0)) {
          piStack_244 = (undefined4 *)0x0;
        }
        else {
          *piStack_244 = 1;
          piStack_244[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_244[2] = (int)pCStack_248;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_1d0,
                   ppVar35,ppVar46);
        iVar33 = (**(code **)(*(int *)pCStack_248 + 0x104))();
        if (iVar33 == 0) {
          iVar33 = *(int *)pCStack_248;
          NInventory::CTCInventoryBase::GetMiniMapGraphic((CTCInventoryBase *)this_05,aCStack_fc);
          (**(code **)(iVar33 + 0x244))();
        }
        piVar27 = piStack_170;
        (**(code **)(*(int *)local_280 + 0xec))();
        (**(code **)(*(int *)pCVar17 + 0x238))();
        (**(code **)(*piStack_258 + 0xec))(&pCStack_288);
        CTCInventoryMap::CreateMiniMapMarkers(pCVar47,pCStack_28c,aCStack_108);
        if ((pCStack_250 != (CComponent *)0x0) &&
           (*(int *)pCStack_250 = *(int *)pCStack_250 + -1, *(int *)pCStack_250 == 0)) {
          (**(code **)(pCStack_250 + 4))();
          operator_delete(pCStack_250);
        }
        uStack_254 = 0;
        pCStack_250 = (CComponent *)0x0;
        if ((pCStack_288 != (CCategory *)0x0) &&
           (*(int *)pCStack_288 = *(int *)pCStack_288 + -1, *(int *)pCStack_288 == 0)) {
          (**(code **)(pCStack_288 + 4))();
          operator_delete(pCStack_288);
        }
        pCStack_28c = (CComponent *)0x0;
        pCStack_288 = (CCategory *)0x0;
        if ((pCVar19 != (CRegion *)0x0) &&
           (*(int *)pCVar19 = *(int *)pCVar19 + -1, *(int *)pCVar19 == 0)) {
          (**(code **)(pCVar19 + 4))();
          operator_delete(pCVar19);
        }
        pCVar19 = (CRegion *)0x0;
        if ((piStack_258 != (int *)0x0) && (*piStack_258 = *piStack_258 + -1, *piStack_258 == 0)) {
          (*(code *)piStack_258[1])();
          operator_delete(piStack_258);
        }
        pCStack_25c = (CRegion *)0x0;
        piStack_258 = (int *)0x0;
        CCharString::~CCharString((CCharString *)&stack0xfffffd20);
        if ((piStack_1e4 != (int *)0x0) && (*piStack_1e4 = *piStack_1e4 + -1, *piStack_1e4 == 0)) {
          (*(code *)piStack_1e4[1])();
          operator_delete(piStack_1e4);
        }
        pCStack_1e8 = (CComponent *)0x0;
        piStack_1e4 = (int *)0x0;
        if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
          (*(code *)piVar27[1])();
          operator_delete(piVar27);
        }
        if ((pCStack_278 != (CComponent *)0x0) &&
           (*(int *)pCStack_278 = *(int *)pCStack_278 + -1, *(int *)pCStack_278 == 0)) {
          (**(code **)(pCStack_278 + 4))();
          operator_delete(pCStack_278);
        }
        local_27c = (CRegion *)0x0;
        pCStack_278 = (CComponent *)0x0;
      }
      else {
        CCharString::CCharString((CCharString *)aaStack_180,"UI_SUB_LIST",-1);
        pCStack_248 = NUISystem::CManager::CreateComponent
                                ((CManager *)this_07,(CCharString *)aaStack_180,false);
        ppVar35 = extraout_EDX_35;
        if ((pCStack_248 == (CComponent *)0x0) ||
           (piStack_244 = operator_new(0xc), ppVar35 = extraout_EDX_36, piStack_244 == (int *)0x0))
        {
          piStack_244 = (int *)0x0;
        }
        else {
          *piStack_244 = 1;
          piStack_244[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_244[2] = (int)pCStack_248;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_180,ppVar35,ppVar48);
        piStack_158 = (int *)0x0;
        pCVar17 = (CComponent *)0x0;
        if (piStack_244 != (int *)0x0) {
          piStack_158 = piStack_244;
          *piStack_244 = *piStack_244 + 1;
          pCVar17 = pCStack_248;
        }
        CCharString::CCharString(aCStack_19c,"UI_BACK_FOR_LIST",-1);
        pCStack_1dc = NUISystem::CManager::CreateComponent((CManager *)this_07,aCStack_19c,false);
        ppVar35 = extraout_EDX_37;
        if ((pCStack_1dc == (CComponent *)0x0) ||
           (puStack_1d8 = operator_new(0xc), ppVar35 = extraout_EDX_38,
           puStack_1d8 == (undefined4 *)0x0)) {
          puStack_1d8 = (undefined4 *)0x0;
        }
        else {
          *puStack_1d8 = 1;
          puStack_1d8[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
          puStack_1d8[2] = pCStack_1dc;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_19c,
                   ppVar35,ppVar48);
        pCVar47 = (CTCInventoryMap *)&pCStack_1dc;
        (**(code **)(*(int *)pCVar17 + 0x244))();
        ppVar46 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_24c;
        (**(code **)(*unaff_ESI + 0xec))();
        CCharString::CCharString((CCharString *)&piStack_1a8,"UI_TITLE_REGION_MAP",-1);
        pCStack_1f4 = NUISystem::CManager::CreateComponent
                                ((CManager *)this_07,(CCharString *)&piStack_1a8,false);
        ppVar35 = extraout_EDX_39;
        if ((pCStack_1f4 == (CComponent *)0x0) ||
           (piStack_1f0 = operator_new(0xc), ppVar35 = extraout_EDX_40,
           piStack_1f0 == (undefined4 *)0x0)) {
          piStack_1f0 = (undefined4 *)0x0;
        }
        else {
          *piStack_1f0 = 1;
          piStack_1f0[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_1f0[2] = (int)pCStack_1f4;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_1a8,
                   ppVar35,ppVar46);
        ppVar46 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_1f4;
        (**(code **)(*(int *)pCVar17 + 0x244))();
        CCharString::CCharString(aCStack_1a4,"TEXT_GUI_MENU_ZOOMED_REGION",-1);
        NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b86a0 + 0x14),aCStack_198);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1a4,
                   extraout_EDX_41,ppVar46);
        CCharString::CCharString((CCharString *)aaStack_184,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffd10,"PC_BUTTON_BASTARD",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffd08,"",-1);
        pCStack_234 = NUISystem::CManager::CreateMenuEntryComponent
                                ((CManager *)this_07,aCStack_198,0,0x3f800000,aaStack_184,0);
        ppVar35 = extraout_EDX_42;
        if ((pCStack_234 == (CComponent *)0x0) ||
           (puStack_230 = operator_new(0xc), ppVar35 = extraout_EDX_43,
           puStack_230 == (undefined4 *)0x0)) {
          puStack_230 = (undefined4 *)0x0;
        }
        else {
          *puStack_230 = 1;
          puStack_230[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
          puStack_230[2] = pCStack_234;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_184,ppVar35,ppVar46);
        CCharString::CCharString((CCharString *)&pCStack_194,"UI_MAP_MENU_MINI_MAP_TEMPLATE",-1);
        pCStack_28c = NUISystem::CManager::CreateComponent
                                ((CManager *)this_07,(CCharString *)&pCStack_194,false);
        ppVar35 = extraout_EDX_44;
        if ((pCStack_28c == (CComponent *)0x0) ||
           (pCStack_288 = operator_new(0xc), ppVar35 = extraout_EDX_45,
           pCStack_288 == (CCategory *)0x0)) {
          pCStack_288 = (CCategory *)0x0;
        }
        else {
          *(int *)pCStack_288 = 1;
          *(code **)(pCStack_288 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_288 + 8) = pCStack_28c;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_194,
                   ppVar35,ppVar46);
        pCVar42 = pCStack_25c;
        CCharString::CCharString(aCStack_1a0,"UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE",-1);
        pCStack_274 = NUISystem::CManager::CreateComponent((CManager *)this_07,aCStack_1a0,false);
        ppVar35 = extraout_EDX_46;
        if ((pCStack_274 == (CComponent *)0x0) ||
           (pCStack_270 = operator_new(0xc), ppVar35 = extraout_EDX_47,
           pCStack_270 == (CManager *)0x0)) {
          pCStack_270 = (CManager *)0x0;
        }
        else {
          *(int *)pCStack_270 = 1;
          *(code **)(pCStack_270 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_270 + 8) = pCStack_274;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1a0,
                   ppVar35,ppVar46);
        iVar33 = (**(code **)(*(int *)pCStack_274 + 0x104))();
        if (iVar33 == 0) {
          iVar33 = *(int *)pCStack_274;
          NInventory::CTCInventoryBase::GetMiniMapGraphic(pCVar10,aCStack_f8);
          (**(code **)(iVar33 + 0x244))();
        }
        pCVar39 = pCStack_164;
        (**(code **)(*(int *)pCStack_28c + 0xec))();
        (**(code **)(*(int *)pCVar42 + 0x238))();
        (**(code **)(*piStack_23c + 0xec))();
        (**(code **)(*(int *)pCVar17 + 0x240))(&pCStack_240);
        CTCInventoryMap::CreateMiniMapMarkers(pCVar47,unaff_EBX,aCStack_108);
        if ((local_280 != (CAIStateGroup_SummonerCharge *)0x0) &&
           (*(int *)local_280 = *(int *)local_280 + -1, *(int *)local_280 == 0)) {
          (**(code **)((int)local_280 + 4))();
          operator_delete(local_280);
        }
        local_284 = (CAIStateGroup_SummonerCharge *)0x0;
        local_280 = (CAIStateGroup_SummonerCharge *)0x0;
        if ((piStack_298 != (int *)0x0) && (*piStack_298 = *piStack_298 + -1, *piStack_298 == 0)) {
          (*(code *)piStack_298[1])();
          operator_delete(piStack_298);
        }
        unaff_EBX = (CComponent *)0x0;
        piStack_298 = (int *)0x0;
        if ((pCStack_240 != (CComponent *)0x0) &&
           (*(int *)pCStack_240 = *(int *)pCStack_240 + -1, *(int *)pCStack_240 == 0)) {
          (**(code **)(pCStack_240 + 4))();
          operator_delete(pCStack_240);
        }
        piStack_244 = (undefined4 *)0x0;
        pCStack_240 = (CComponent *)0x0;
        CCharString::~CCharString((CCharString *)&piStack_1a8);
        if ((piStack_204 != (int *)0x0) && (*piStack_204 = *piStack_204 + -1, *piStack_204 == 0)) {
          (*(code *)piStack_204[1])();
          operator_delete(piStack_204);
        }
        uStack_208 = 0;
        piStack_204 = (int *)0x0;
        if ((pCStack_1f4 != (CComponent *)0x0) &&
           (*(int *)pCStack_1f4 = *(int *)pCStack_1f4 + -1, *(int *)pCStack_1f4 == 0)) {
          (**(code **)(pCStack_1f4 + 4))();
          operator_delete(pCStack_1f4);
        }
        uStack_1f8 = 0;
        pCStack_1f4 = (CComponent *)0x0;
        if ((pCVar39 != (CRegion *)0x0) &&
           (*(int *)pCVar39 = *(int *)pCVar39 + -1, *(int *)pCVar39 == 0)) {
          (**(code **)(pCVar39 + 4))();
          operator_delete(pCVar39);
        }
        if ((pCStack_260 != (CComponent *)0x0) &&
           (*(int *)pCStack_260 = *(int *)pCStack_260 + -1, *(int *)pCStack_260 == 0)) {
          (**(code **)(pCStack_260 + 4))();
          operator_delete(pCStack_260);
        }
        uStack_264 = 0;
        pCStack_260 = (CComponent *)0x0;
      }
      NUISystem::CUIState::CUIState((CUIState *)&uStack_16c);
      this_06 = pCVar53;
      pCStack_b0 = pCVar53;
      CRegion::CRegion(aCStack_ac,aCStack_108);
      RBTree_Insert_Region(&stack0xfffffd54,&pCStack_b0);
      pfVar29 = (float *)CRegion::GetNameGraphicOffset(aCStack_108);
      pCVar15 = pCStack_294;
      uStack_110 = 0x3e4ccccd;
      pCStack_188 = (CRegion *)((float)ppCVar49[0x39] - pfVar29[1]);
      pCStack_18c = (CRegion *)((float)ppCVar49[0x38] - *pfVar29);
      uStack_168 = 3;
      uStack_128 = 3;
      uStack_16c = 0x67;
      uStack_12c = 0x67;
      pCStack_130 = pCVar53 + 8;
      uStack_112 = uStack_152;
      uStack_11c = uStack_15c;
      piStack_118 = piStack_158;
      uStack_111 = uStack_151;
      uStack_113 = uStack_153;
      uStack_150 = 0x3e4ccccd;
      uStack_114 = uStack_154;
      CStack_10c = aCStack_14c[0];
      pCStack_164 = pCStack_18c;
      pCStack_160 = pCStack_188;
      pCStack_124 = pCStack_18c;
      pCStack_120 = pCStack_188;
      (**(code **)(*(int *)pCStack_294 + 0x148))();
      (**(code **)(*(int *)pCVar15 + 0x148))();
      CMemoryAllocatorVariableSize_FindAllocationNode();
      CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(aCStack_74);
      if ((piStack_238 != (int *)0x0) && (*piStack_238 = *piStack_238 + -1, *piStack_238 == 0)) {
        (*(code *)piStack_238[1])();
        operator_delete(piStack_238);
      }
      piStack_23c = (int *)0x0;
      piStack_238 = (int *)0x0;
      if ((piStack_210 != (int *)0x0) && (*piStack_210 = *piStack_210 + -1, *piStack_210 == 0)) {
        (*(code *)piStack_210[1])();
        operator_delete(piStack_210);
      }
      puStack_214 = (undefined4 *)0x0;
      piStack_210 = (int *)0x0;
      if ((local_280 != (CAIStateGroup_SummonerCharge *)0x0) &&
         (*(int *)local_280 = *(int *)local_280 + -1, *(int *)local_280 == 0)) {
        (**(code **)(local_280 + 4))();
        operator_delete(local_280);
      }
      local_284 = (CAIStateGroup_SummonerCharge *)0x0;
      local_280 = (CAIStateGroup_SummonerCharge *)0x0;
      CCharString::~CCharString((CCharString *)&stack0xfffffd50);
      CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge
                ((CAIStateGroup_SummonerCharge *)aCStack_d0);
      pCStack_274 = (CComponent *)(pCVar53 + 1);
    } while (pCStack_274 < pCStack_26c);
  }
  CCharString::CCharString((CCharString *)&stack0xfffffd58,"UI_MAP_VIEWPORT",-1);
  pCStack_1f4 = NUISystem::CManager::CreateComponent
                          (pCStack_294,(CCharString *)&stack0xfffffd58,false);
  ppVar35 = extraout_EDX_59;
  if ((pCStack_1f4 == (CComponent *)0x0) ||
     (piStack_1f0 = operator_new(0xc), ppVar35 = extraout_EDX_60, piStack_1f0 == (int *)0x0)) {
    piStack_1f0 = (int *)0x0;
  }
  else {
    *piStack_1f0 = 1;
    piStack_1f0[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
    piStack_1f0[2] = (int)pCStack_1f4;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd58,
             ppVar35,ppVar31);
  pCVar41 = (CAIStateGroup_SummonerCharge *)&pCStack_25c;
  (**(code **)(*(int *)pCStack_1f4 + 0xec))();
  pCVar17 = pCStack_28c;
  pCVar40 = (CAIStateGroup_SummonerCharge *)&uStack_1f8;
  (**(code **)(*(int *)pCStack_28c + 0x244))();
  iVar33 = *(int *)pCVar17;
  piVar27 = extraout_ECX_00;
  CCharString::CCharString((CCharString *)&stack0xfffffd30,"UI_ASSIGN_TEXT",-1);
  piVar30 = (int *)(**(code **)(iVar33 + 0xc))();
  CCharString::CCharString((CCharString *)&stack0xfffffd4c,"TEXT_GUI_MENU_SELECT",-1);
  iVar33 = *piVar30;
  ppVar31 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
            NGameText::CDataBank::GetTextBySymbol
                      (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&stack0xfffffd64);
  (**(code **)(iVar33 + 0x240))();
  CCharString::~CCharString((CCharString *)&stack0xfffffd58);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd40,
             extraout_EDX_61,ppVar31);
  iVar33 = *(int *)pCVar17;
  CCharString::CCharString((CCharString *)&stack0xfffffd20,"UI_UNSELECT_TEXT",-1);
  piVar30 = (int *)(**(code **)(iVar33 + 0xc))();
  CCharString::CCharString((CCharString *)&stack0xfffffd3c,"TEXT_GUI_MENU_MAP_SCROLL",-1);
  iVar33 = *piVar30;
  ppVar31 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
            NGameText::CDataBank::GetTextBySymbol
                      (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&stack0xfffffd54);
  (**(code **)(iVar33 + 0x240))();
  CCharString::~CCharString((CCharString *)&stack0xfffffd48);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffd30,
             extraout_EDX_62,ppVar31);
  iVar33 = *(int *)pCVar17;
  ppVar31 = extraout_ECX_01;
  CCharString::CCharString
            ((CCharString *)&stack0xfffffd10,"UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST",-1);
  piVar30 = (int *)(**(code **)(iVar33 + 0xc))();
  (**(code **)(*piVar30 + 0x148))();
  uVar45 = CPersistTraits__Transfer();
  ppVar35 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar45 >> 0x20);
  *(undefined1 *)((int)uVar45 + 0x1b) = 0xff;
  if ((piStack_21c != (int *)0x0) && (*piStack_21c = *piStack_21c + -1, *piStack_21c == 0)) {
    (*(code *)piStack_21c[1])();
    operator_delete(piStack_21c);
    ppVar35 = extraout_EDX_63;
  }
  pCVar7 = pCStack_1b8;
  uStack_220 = 0;
  piStack_21c = (int *)0x0;
  for (pCVar38 = (CAIStateGroup_SummonerCharge *)pCStack_1bc; pCVar38 != pCVar7;
      pCVar38 = pCVar38 + 0x58) {
    CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(pCVar38);
    ppVar35 = extraout_EDX_64;
  }
  if (pCStack_1bc != (CComponent *)0x0) {
    free(pCStack_1bc);
    ppVar35 = extraout_EDX_65;
  }
  if (iStack_22c != 0) {
    pvVar25 = (void *)puStack_230[1];
    while (pvVar25 != (void *)0x0) {
      LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
                ((LTextTreeWalkThrough *)&puStack_230,*(LTextBinNode **)((int)pvVar25 + 0xc));
      pvVar4 = *(void **)((int)pvVar25 + 8);
      free(pvVar25);
      pvVar25 = pvVar4;
      ppVar35 = extraout_EDX_66;
    }
    puStack_230[2] = puStack_230;
    puStack_230[1] = 0;
    puStack_230[3] = puStack_230;
    iStack_22c = 0;
  }
  if (puStack_230 != (undefined4 *)0x0) {
    free(puStack_230);
    ppVar35 = extraout_EDX_67;
  }
  if ((piStack_1e0 != (int *)0x0) && (*piStack_1e0 = *piStack_1e0 + -1, *piStack_1e0 == 0)) {
    (*(code *)piStack_1e0[1])();
    operator_delete(piStack_1e0);
    ppVar35 = extraout_EDX_68;
  }
  piStack_1e4 = (int *)0x0;
  piStack_1e0 = (int *)0x0;
  if ((pCStack_25c != (CRegion *)0x0) &&
     (*(int *)pCStack_25c = *(int *)pCStack_25c + -1, *(int *)pCStack_25c == 0)) {
    (**(code **)(pCStack_25c + 4))();
    operator_delete(pCStack_25c);
    ppVar35 = extraout_EDX_69;
  }
  pCStack_260 = (CComponent *)0x0;
  pCStack_25c = (CRegion *)0x0;
  if ((piStack_1f0 != (int *)0x0) && (*piStack_1f0 = *piStack_1f0 + -1, *piStack_1f0 == 0)) {
    (*(code *)piStack_1f0[1])();
    operator_delete(piStack_1f0);
    ppVar35 = extraout_EDX_70;
  }
  pCStack_1f4 = (CComponent *)0x0;
  piStack_1f0 = (int *)0x0;
  if ((local_284 != (CAIStateGroup_SummonerCharge *)0x0) &&
     (*(int *)local_284 = *(int *)local_284 + -1, *(int *)local_284 == 0)) {
    (**(code **)(local_284 + 4))();
    operator_delete(local_284);
    ppVar35 = extraout_EDX_71;
  }
  pCStack_288 = (CCategory *)0x0;
  local_284 = (CAIStateGroup_SummonerCharge *)0x0;
  if (piVar27 != (int *)0x0) {
    piVar30 = piVar27 + 1;
    *piVar30 = *piVar30 + -1;
    if (*piVar30 == 0) {
      (**(code **)(*piVar27 + 4))();
      ppVar35 = extraout_EDX_72;
    }
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_294,ppVar35,
             ppVar31);
  pCVar38 = pCVar40;
  for (; pCVar40 != pCVar41; pCVar40 = pCVar40 + 0x58) {
    CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(pCVar40);
  }
  if (pCVar38 != (CAIStateGroup_SummonerCharge *)0x0) {
    free(pCVar38);
  }
  return pCVar17;
}


// CALLEES of CTCInventoryMap::BuildMapTeleportUI:
//   00411570  Compare
//   009c95e0  GetTextBySymbol
//   0041e5f2  GetInstance
//   00619d50  RBTree_Insert_Region
//   00519b80  operator=
//   0099efb0  operator=
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   006bbf20  GetDisplayName
//   005fd410  GetRegionsToDisplay
//   004a9cb0  BuildTreeArray
//   00686a80  GetDisplayEngine
//   00512da0  BinarySearchTree_Insert
//   0051d200  Vector_PushBack_Or_Reallocate
//   005beb57  GetMiniMapGraphic
//   004fb150  DrawGetEnvironment
//   005fa990  GetGuildSealRecallRegion
//   00603000  CreateMiniMapMarkers
//   005ff190  CreateMapMarkers
//   0099b510  ~CCharString
//   00bfea14  free
//   00686d20  GetDisplayEngine
//   0051cf10  VectorReallocateWithRange
//   0099e960  CCharString::NotEqual
//   0099e900  operator()
//   00686d30  GetDisplayEngine
//   0099ebf0  CCharString
//   00bfea0e  malloc
//   0044c6b0  GFGetPreMainMemoryUsed
//   00bfe9bc  operator_delete
//   0041db1d  CreateComponent
//   00605570  ConstructRegionDescription
//   0052e5d0  CUIState
//   0099bff0  CCharString::SetFromEnum
//   004fb470  DrawGetNoMaps
//   004fc190  GetRegionNumberMapIsIn
//   007021f0  CViewBase::GetMarkerRegionName
//   005fbea0  GetRegionTeleporter
//   0042b312  GetDef<CSpecialAbilitiesDrunkennessDef>
//   0099b8d0  operator+=
//   00a01b50  GetPItem
//   00428494  CreateMenuEntryComponent
//   009ad390  ActualToUniqueIndex
//   0051a900  CRegion
//   006196d0  BuildTreeArray
//   0051a110  ~CAIStateGroup_SummonerCharge
//   00618100  Campfire_InsertRegionNode
//   00bfea1a  operator_new
//   006bc410  CEngineInit
//   0040f020  LowerBound
//   004fc180  GetCategoryWithIndex
//   0042d5b1  CPersistTraits::Transfer
//   004fc210  GetRegionNumberFromName
//   006bbf40  GetNameGraphicOffset
//   0042d2c6  CMemoryAllocatorVariableSize_FindAllocationNode

