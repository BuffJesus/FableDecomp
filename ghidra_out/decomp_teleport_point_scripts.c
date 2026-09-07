//=== PopulateRegionWithVillagers @ 00502e90 (seed 005060a0) ===

/* WARNING: Type propagation algorithm not settling */
/* [bsim sim=0.5383389159293182 <- ego_r]
   public: void __thiscall CWorldMap::PopulateRegionWithVillagers(class CThing &) */

void __thiscall CWorldMap::PopulateRegionWithVillagers(CWorldMap *this,CThing *param_1)

{
  CBaseIntelligentPointer *this_00;
  undefined4 *puVar1;
  undefined *puVar2;
  int *piVar3;
  CPixelShader *pCVar4;
  CThingBuilding *this_01;
  CWorldMap *pCVar5;
  undefined **ppuVar6;
  bool bVar7;
  char extraout_AL;
  int *piVar8;
  CBaseObject *pCVar9;
  ulong uVar10;
  CBaseObject *pCVar11;
  int *piVar12;
  CGuiControlTreePane *pCVar13;
  int extraout_EAX;
  int extraout_EAX_00;
  C3DVector *pCVar14;
  CDisplayEngine *this_02;
  uint *puVar15;
  uint uVar16;
  CCharString *pCVar17;
  CDefinitionManager *pCVar18;
  CThing *pCVar19;
  CThingBuilding *pCVar20;
  CBaseObject *pCVar21;
  CBaseIntelligentPointer *this_03;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar22;
  CTCOwnedEntity *this_04;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  uint extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  uint extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  uint extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  uint extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  uint extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  uint extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  uint extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  uint extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  uint extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_55;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_56;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_57;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_58;
  uint extraout_EDX_59;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_73;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_74;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_75;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_76;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_77;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_78;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_79;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_80;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_81;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_82;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_83;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_84;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_85;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_86;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_87;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_88;
  uint extraout_EDX_89;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_90;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_91;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_92;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_93;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_94;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_95;
  uint extraout_EDX_96;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_97;
  uint extraout_EDX_98;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_99;
  int iVar24;
  uint extraout_EDX_x00100;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00101;
  uint extraout_EDX_x00102;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00103;
  uint extraout_EDX_x00104;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00105;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00106;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00107;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00108;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00109;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00110;
  uint extraout_EDX_x00111;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00112;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00113;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00114;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00115;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00116;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00117;
  uint extraout_EDX_x00118;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00119;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00120;
  uint extraout_EDX_x00121;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00122;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_x00123;
  int iVar25;
  undefined4 *puVar26;
  undefined4 **ppuVar27;
  CVillageTaskCarryCrate *this_05;
  CVillageTaskCarryCrate *pCVar28;
  CTCVillage *pCVar29;
  CThingAICreature *pCVar30;
  float fVar31;
  CTCDummyVillager *pCVar32;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  float fVar33;
  undefined **ppuVar34;
  undefined **_Memory;
  CVillageTaskCarryCrate *this_06;
  CVillageTaskCarryCrate *pCVar35;
  undefined8 uVar36;
  CThingAICreatureInit *pCVar37;
  vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *pvVar38;
  int iVar39;
  vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *pvVar40;
  long lVar41;
  undefined1 uVar42;
  CDefPointer<CThingPhysicalSwitchDef_const_> *pCVar43;
  byte local_199;
  CBaseObject *local_198;
  int *local_194;
  long local_190;
  CBaseObject *local_18c;
  long local_188;
  CThingAICreature *pCStack_184;
  CCharString aCStack_180 [4];
  undefined4 uStack_17c;
  undefined4 uStack_178;
  CCharString aCStack_174 [4];
  CCharString aCStack_170 [4];
  undefined1 uStack_16c;
  undefined1 uStack_16b;
  undefined1 uStack_16a;
  undefined1 uStack_169;
  undefined4 uStack_168;
  int iStack_164;
  CBaseObject *local_160;
  CBaseObject *local_15c;
  CBaseObject *local_158;
  CThingBuilding *local_154;
  int local_150;
  CBaseObject *pCStack_14c;
  CBaseObject *local_148;
  CBaseObject *pCStack_144;
  CTCVillage *local_140;
  int local_13c;
  CPixelShader *local_138;
  undefined4 *local_134;
  undefined4 *local_130;
  undefined4 *local_12c;
  undefined4 **local_128;
  CBaseObject *local_124;
  undefined4 *local_120;
  undefined4 *local_11c;
  undefined4 *local_118;
  CBaseObject *local_114 [2];
  int *piStack_10c;
  CBaseObject *local_108;
  float local_104;
  float local_100;
  CBaseObject *local_fc;
  CBaseObject *local_f8;
  void *local_f4;
  int iStack_f0;
  uint local_e8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_e4;
  int local_e0;
  CBaseObject *local_dc;
  int iStack_d8;
  int local_d0;
  int local_cc;
  float local_c8;
  CWorldMap *local_c4;
  CBaseObject *local_c0;
  CBaseObject *local_bc;
  void *local_b8;
  int local_b4;
  undefined4 local_b0;
  CBaseObject *local_ac;
  CBaseObject *local_a8;
  undefined4 local_a4;
  undefined **ppuStack_a0;
  undefined **ppuStack_9c;
  int iStack_94;
  CBaseObject *local_90;
  CBaseObject *local_8c;
  undefined4 local_88;
  CCharString aCStack_84 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_80 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_7c [4];
  CThingAICreatureInit aCStack_78 [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_70 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_6c [7];
  undefined1 uStack_65;
  undefined4 uStack_60;
  CThingAICreatureInit aCStack_5c [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_54 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_50 [7];
  undefined1 uStack_49;
  CBaseObject *pCStack_44;
  CThingAICreatureInit aCStack_40 [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_38 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_34 [7];
  undefined1 uStack_2d;
  CThingAICreatureInit aCStack_24 [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [7];
  undefined1 uStack_11;
  
  local_e8 = 0;
  local_194 = (int *)0x0;
  local_c4 = this;
  bVar7 = CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
                    (*(CDefClassBase **)(param_1 + 0x70),
                     (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)&local_194);
  piVar12 = local_194;
  if (bVar7) {
    local_199 = (byte)(*(uint *)(param_1 + 0x24) >> 2) & 1;
    if ((*(uint *)(param_1 + 0x24) >> 2 & 1) != 0) {
      local_154 = (CThingBuilding *)&DAT_00000022;
      piVar8 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(param_1 + 0x44),(ETCInterfaceType *)&local_154);
      if ((piVar8 == *(int **)(param_1 + 0x48)) || (0x22 < *piVar8)) {
        piVar8 = *(int **)(param_1 + 0x48);
      }
      local_140 = (CTCVillage *)piVar8[1];
    }
    bVar7 = CWorld::PopDontPopulateNextLoadedRegion(*(CWorld **)(local_c4 + 8));
    if ((bVar7) || (*(char *)((int)piVar12 + 0x25) == '\0')) {
      if (piVar12 == (int *)0x0) {
        return;
      }
      piVar8 = piVar12 + 1;
      *piVar8 = *piVar8 + -1;
      if (*piVar8 != 0) {
        return;
      }
      (**(code **)(*piVar12 + 4))();
      return;
    }
    DoublyLinkedList_Initialize();
    DoublyLinkedList_Initialize();
    local_108 = (CBaseObject *)piVar12[0x29];
    local_bc = (CBaseObject *)piVar12[0x28];
    local_134 = (undefined4 *)0x0;
    local_130 = (undefined4 *)0x0;
    local_12c = (undefined4 *)0x0;
    local_120 = (undefined4 *)0x0;
    local_11c = (undefined4 *)0x0;
    local_f8 = (CBaseObject *)
               (1.0 / ((float)piVar12[0xc] + (float)piVar12[0xb] + (float)piVar12[10]));
    local_118 = (undefined4 *)0x0;
    local_b8 = (void *)0x0;
    local_b4 = 0;
    local_b0 = 0;
    local_ac = (CBaseObject *)0x0;
    local_a8 = (CBaseObject *)0x0;
    local_a4 = 0;
    local_90 = (CBaseObject *)0x0;
    local_8c = (CBaseObject *)0x0;
    local_88 = 0;
    local_13c = 0;
    local_124 = (CBaseObject *)0x0;
    local_148 = (CBaseObject *)0x0;
    local_e4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
    local_fc = (CBaseObject *)0x0;
    local_150 = 0;
    local_158 = (CBaseObject *)0x0;
    local_128 = (undefined4 **)0x0;
    local_138 = (CPixelShader *)0x0;
    local_160 = (CBaseObject *)0x0;
    local_190 = 0;
    local_188 = 0;
    local_e0 = 0;
    local_d0 = 0;
    local_cc = 0;
    pCVar29 = local_140 + 0x2c;
    local_100 = (float)local_f8 * (float)piVar12[10];
    local_104 = (float)local_f8 * (float)piVar12[0xb];
    local_c8 = (float)local_f8 * (float)piVar12[0xc];
    DoublyLinkedList_CreateAndInit();
    puVar26 = *(undefined4 **)pCVar29;
    pCVar9 = local_15c;
    for (puVar1 = (undefined4 *)*puVar26; local_15c = pCVar9, puVar1 != puVar26;
        puVar1 = (undefined4 *)*puVar1) {
      CCharString::CCharString((CCharString *)&local_198,"PopulateRegionWithVillagers 1",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_198,(float)(extraout_EDX & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_198,
                 extraout_EDX_00,unaff_EDI);
      this_00 = (CBaseIntelligentPointer *)(puVar1 + 2);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_114);
      local_114[0] = (CBaseObject *)&PTR__vector_deleting_destructor__0123e7e0;
      pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_114,pCVar9);
      pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
      uVar10 = CPixelShader::CountReferences((CPixelShader *)pCVar9);
      if (uVar10 == 3) {
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
        if (pCVar9 != (CBaseObject *)0x0) {
          pCVar19 = (CThing *)0x0;
          pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
          CVillageTaskCarryCrate::AssignCreatureToTask((CVillageTaskCarryCrate *)pCVar9,pCVar19);
        }
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x170));
        if (pCVar9 != (CBaseObject *)0x0) {
          pCVar19 = (CThing *)0x0;
          pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
          CVillageTaskCarryCrate::AssignCreatureToTask((CVillageTaskCarryCrate *)pCVar9,pCVar19);
        }
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
        CThing::IsUnderScriptSupervision((CThing *)pCVar9);
      }
      else {
        pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x170));
        if (pCVar9 != (CBaseObject *)0x0) {
          local_154 = (CThingBuilding *)CBaseIntelligentPointer::GetPItem(this_00);
          pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
          local_18c = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x170))
          ;
          std__map__insert();
          LinkedList_AppendNode();
          pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
          pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x170));
          CThingBuilding::PeekPMaterial((CThingBuilding *)pCVar9);
          if (local_f8 != (CBaseObject *)0x0) {
            iVar24 = *(int *)((int)local_f8 + 4);
            *(int *)((int)local_f8 + 4) = iVar24 + -1;
            if (iVar24 + -1 == 0) {
              (**(code **)(*(int *)local_f8 + 4))();
            }
            local_f8 = (CBaseObject *)0x0;
          }
        }
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
        uVar10 = CPixelShader::CountReferences((CPixelShader *)pCVar9);
        if (uVar10 == 1) {
          pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
          pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
          if (pCVar9 != (CBaseObject *)0x0) {
            local_190 = local_190 + 1;
          }
          this_03 = (CBaseIntelligentPointer *)local_114;
        }
        else {
          pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
          uVar10 = CPixelShader::CountReferences((CPixelShader *)pCVar9);
          if (uVar10 == 2) {
            pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
            pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
            if (pCVar9 != (CBaseObject *)0x0) {
              local_188 = local_188 + 1;
            }
            this_03 = (CBaseIntelligentPointer *)local_114;
          }
          else {
            pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_114);
            pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
            this_03 = this_00;
            if (pCVar9 != (CBaseObject *)0x0) {
              local_e0 = local_e0 + 1;
            }
          }
        }
        pCVar9 = CBaseIntelligentPointer::GetPItem(this_03);
        CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
        pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168));
        if (pCVar9 != (CBaseObject *)0x0) {
          local_154 = (CThingBuilding *)CBaseIntelligentPointer::GetPItem(this_00);
          pCVar9 = CBaseIntelligentPointer::GetPItem(this_00);
          local_18c = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 0x168))
          ;
          std__map__insert();
          LinkedList_AppendNode();
        }
      }
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_114);
      pCVar9 = local_15c;
    }
    local_18c = *(CBaseObject **)(local_140 + 0x28);
    local_198 = *(CBaseObject **)local_18c;
    local_f8 = (CBaseObject *)0x0;
    if (local_198 != local_18c) {
      do {
        CCharString::CCharString(aCStack_180,"PopulateRegionWithVillagers 2",-1);
        NProgressDisplay::DisplayProgress
                  (aCStack_180,(float)(extraout_EDX_01 & 0xffffff00),false,false);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
                   extraout_EDX_02,unaff_EDI);
        pCVar11 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(local_198 + 8));
        pCStack_144 = pCVar11;
        if ((pCVar11 != (CBaseObject *)0x0) &&
           (bVar7 = CThingBuilding::GetIsAvailableForResidency((CThingBuilding *)pCVar11), bVar7)) {
          local_199 = (byte)(*(uint *)(pCVar11 + 0x38) >> 0x15) & 1;
          if ((*(uint *)(pCVar11 + 0x38) >> 0x15 & 1) == 0) {
LAB_00503417:
            bVar7 = false;
          }
          else {
            local_154 = (CThingBuilding *)0xd5;
            piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                             ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                           *)(pCVar11 + 0x44),(ETCInterfaceType *)&local_154);
            if ((piVar12 == *(int **)(pCVar11 + 0x48)) || (0xd5 < *piVar12)) {
              piVar12 = *(int **)(pCVar11 + 0x48);
            }
            pCVar9 = *(CBaseObject **)(piVar12[1] + 0xc);
            local_e8 = local_e8 | 1;
            if (pCVar9 != (CBaseObject *)0x0) {
              *(int *)(pCVar9 + 4) = *(int *)(pCVar9 + 4) + 1;
            }
            local_15c = pCVar9;
            if (pCVar9[0x26] == (CBaseObject)0x0) goto LAB_00503417;
            bVar7 = true;
          }
          if (((local_e8 & 1) != 0) &&
             (local_e8 = local_e8 & 0xfffffffe, pCVar9 != (CBaseObject *)0x0)) {
            pCVar21 = pCVar9 + 4;
            *(int *)pCVar21 = *(int *)pCVar21 + -1;
            if (*(int *)pCVar21 == 0) {
              (**(code **)(*(int *)pCVar9 + 4))();
            }
            pCVar9 = (CBaseObject *)0x0;
            local_15c = (CBaseObject *)0x0;
          }
          if (bVar7) {
            VectorPush_OrExpand();
          }
          else {
            CThingBuilding::PeekPMaterial((CThingBuilding *)pCVar11);
            if (pCStack_184 != (CThingAICreature *)0x0) {
              if (((byte)pCStack_184[0x3c] & 1) != 0) {
                pCVar13 = (CGuiControlTreePane *)std__map__insert();
                CGuiControlTreePane::SortTreeRecursively
                          (pCVar13,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *
                                   )unaff_EDI);
                iVar24 = *(int *)(pCStack_184 + 0x44);
                if (iVar24 - extraout_EAX < 1) {
                  PopulateRegionWithVillagers_FindTreeNode();
                  TreeNode_EraseRange();
                }
                else {
                  VectorPush_OrExpand();
                  local_13c = local_13c + (iVar24 - extraout_EAX);
                }
              }
              if (((byte)pCStack_184[0x3c] & 2) != 0) {
                pCVar13 = (CGuiControlTreePane *)std__map__insert();
                CGuiControlTreePane::SortTreeRecursively
                          (pCVar13,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *
                                   )unaff_EDI);
                pCStack_144 = (CBaseObject *)(*(int *)(pCStack_184 + 0x40) - extraout_EAX_00);
                if (0 < (int)pCStack_144) {
                  uVar16 = *(uint *)(pCStack_184 + 0x3c);
                  if ((uVar16 & 4) == 0) {
                    if ((uVar16 & 8) == 0) {
                      if ((uVar16 & 0x10) == 0) {
                        if ((uVar16 & 0x100) != 0) goto LAB_00503580;
                        if ((uVar16 & 0x20) == 0) {
                          if ((uVar16 & 0x800) == 0) {
                            if ((uVar16 & 0x200) == 0) {
                              if ((uVar16 & 0x400) != 0) {
                                local_160 = local_160 + (int)pCStack_144;
                              }
                            }
                            else {
                              local_138 = local_138 + (int)pCStack_144;
                            }
                          }
                          else {
                            local_199 = (byte)(*(uint *)(pCVar11 + 0x24) >> 1) & 1;
                            if ((*(uint *)(pCVar11 + 0x24) >> 1 & 1) != 0) {
                              local_154 = (CThingBuilding *)&DAT_00000021;
                              piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                               ::LowerBound((
                                                  CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                                  *)(pCVar11 + 0x44),(ETCInterfaceType *)&local_154)
                              ;
                              if ((piVar12 == *(int **)(pCVar11 + 0x48)) || (0x21 < *piVar12)) {
                                piVar12 = *(int **)(pCVar11 + 0x48);
                              }
                              puVar26 = *(undefined4 **)(piVar12[1] + 0xc);
                              for (puVar1 = (undefined4 *)*puVar26; puVar1 != puVar26;
                                  puVar1 = (undefined4 *)*puVar1) {
                                pCVar9 = CBaseIntelligentPointer::GetPItem
                                                   ((CBaseIntelligentPointer *)(puVar1 + 2));
                                local_154 = (CThingBuilding *)pCVar9;
                                bVar7 = IS_THING_ALIVE<CThingBuilding*>(&local_154);
                                if ((bVar7) &&
                                   (local_199 = (byte)(*(uint *)(pCVar9 + 0x38) >> 0xf) & 1,
                                   (*(uint *)(pCVar9 + 0x38) >> 0xf & 1) != 0)) {
                                  local_128 = (undefined4 **)((int)local_128 + 1);
                                }
                              }
                            }
                          }
                        }
                        else {
                          local_199 = (byte)(*(uint *)(pCVar11 + 0x24) >> 1) & 1;
                          if ((*(uint *)(pCVar11 + 0x24) >> 1 & 1) != 0) {
                            local_154 = (CThingBuilding *)&DAT_00000021;
                            piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                             ::LowerBound((
                                                  CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                                  *)(pCVar11 + 0x44),(ETCInterfaceType *)&local_154)
                            ;
                            if ((piVar12 == *(int **)(pCVar11 + 0x48)) || (0x21 < *piVar12)) {
                              piVar12 = *(int **)(pCVar11 + 0x48);
                            }
                            puVar26 = *(undefined4 **)(piVar12[1] + 0xc);
                            for (puVar1 = (undefined4 *)*puVar26; puVar1 != puVar26;
                                puVar1 = (undefined4 *)*puVar1) {
                              pCVar9 = CBaseIntelligentPointer::GetPItem
                                                 ((CBaseIntelligentPointer *)(puVar1 + 2));
                              local_154 = (CThingBuilding *)pCVar9;
                              bVar7 = IS_THING_ALIVE<CThingBuilding*>(&local_154);
                              if ((bVar7) &&
                                 (local_199 = (byte)(*(uint *)(pCVar9 + 0x38) >> 0xf) & 1,
                                 (*(uint *)(pCVar9 + 0x38) >> 0xf & 1) != 0)) {
                                local_128 = (undefined4 **)((int)local_128 + 1);
                                pCStack_144 = pCStack_144 + -1;
                              }
                            }
                          }
                          local_158 = pCStack_144 + -1;
                          local_150 = 1;
                        }
                      }
                      else if ((uVar16 & 0x100) == 0) {
                        local_e4 = local_e4 + (int)pCStack_144;
                      }
                      else {
LAB_00503580:
                        local_fc = local_fc + (int)pCStack_144;
                      }
                    }
                    else {
                      local_148 = local_148 + (int)pCStack_144;
                    }
                  }
                  else {
                    local_124 = local_124 + (int)pCStack_144;
                    local_f8 = pCVar11;
                  }
                }
              }
              pCVar9 = local_15c;
              if (pCStack_184 != (CThingAICreature *)0x0) {
                iVar24 = *(int *)(pCStack_184 + 4);
                *(int *)(pCStack_184 + 4) = iVar24 + -1;
                if (iVar24 + -1 == 0) {
                  (**(code **)(*(int *)pCStack_184 + 4))();
                }
                pCStack_184 = (CThingAICreature *)0x0;
                pCVar9 = local_15c;
              }
            }
          }
        }
        local_198 = *(CBaseObject **)local_198;
        puVar26 = local_130;
        pCVar11 = local_124;
      } while (local_198 != local_18c);
      while (0 < (int)pCVar11) {
        CCharString::CCharString(aCStack_180,"PopulateRegionWithVillagers 3",-1);
        pCVar30 = (CThingAICreature *)0x0;
        NProgressDisplay::DisplayProgress
                  (aCStack_180,(float)(extraout_EDX_03 & 0xffffff00),false,false);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
                   extraout_EDX_04,unaff_EDI);
        CCharString::CCharString((CCharString *)&local_15c,"",-1);
        CCharString::CCharString((CCharString *)&local_198,"",-1);
        uStack_17c = 0;
        uStack_178 = 0;
        CCharString::CCharString(aCStack_174,(CCharString *)&local_198);
        CCharString::CCharString(aCStack_170,(CCharString *)&local_15c);
        uStack_16c = 0;
        uStack_16b = 1;
        uStack_16a = 1;
        uStack_169 = 0;
        uStack_168 = 0;
        iStack_164 = 0;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_198,
                   extraout_EDX_05,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_15c,
                   extraout_EDX_06,unaff_EDI);
        uStack_169 = 1;
        uVar36 = CBuilding_FindMostSpaceForNPCs();
        iStack_164 = (int)uVar36;
        if (iStack_164 == 0) {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                     (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),
                     unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                     extraout_EDX_09,unaff_EDI);
          break;
        }
        pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
        iVar24 = local_194[0xf];
        iVar25 = local_194[0xe];
        if ((float)local_188 * local_100 <= (float)local_190 * local_104) {
          if (iVar24 == 0) {
            if (iVar25 == 0) goto LAB_0050392c;
            goto LAB_00503810;
          }
          pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
          if (local_11c == local_118) {
LAB_00503885:
            pCStack_184 = pCVar30;
            std__vector__insert(local_11c,&pCStack_184,&local_199);
            local_188 = local_188 + 1;
          }
          else {
            pCStack_184 = pCVar30;
            if (local_11c != (undefined4 *)0x0) {
              *local_11c = pCVar30;
            }
LAB_005038d4:
            local_11c = local_11c + 1;
            local_188 = local_188 + 1;
            pCVar30 = pCStack_184;
          }
LAB_00503910:
          if (pCVar30 != (CThingAICreature *)0x0) {
            *(CThing *)(pCVar30 + 0x1c9) = (CThing)((byte)*(CThing *)(pCVar30 + 0x1c9) | 1);
            *(CThing *)(pCVar30 + 0x15c) = (CThing)((byte)*(CThing *)(pCVar30 + 0x15c) | 0x40);
            CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
          }
        }
        else {
          if (iVar25 != 0) {
LAB_00503810:
            pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c)
            ;
            pCStack_184 = pCVar30;
            if (puVar26 == local_12c) {
              std__vector__insert(puVar26,&pCStack_184,&local_199);
            }
            else {
              if (puVar26 != (undefined4 *)0x0) {
                *puVar26 = pCVar30;
              }
              local_130 = puVar26 + 1;
            }
            local_190 = local_190 + 1;
            puVar26 = local_130;
            goto LAB_00503910;
          }
          if (iVar24 != 0) {
            pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c)
            ;
            if (local_11c == local_118) goto LAB_00503885;
            pCStack_184 = pCVar30;
            if (local_11c == (undefined4 *)0x0) goto LAB_005038d4;
            *local_11c = pCVar30;
            local_11c = local_11c + 1;
            local_188 = local_188 + 1;
            goto LAB_00503910;
          }
        }
LAB_0050392c:
        *(CThing *)(pCVar30 + 0x1c9) = (CThing)((byte)*(CThing *)(pCVar30 + 0x1c9) & 0xfe);
        ListNode_InsertPointer();
        pCVar11 = local_124 + -1;
        local_13c = local_13c + -1;
        local_124 = pCVar11;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                   extraout_EDX_07,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                   extraout_EDX_08,unaff_EDI);
      }
      do {
        pCVar9 = local_fc;
        if ((int)local_e4 < 1) goto joined_r0x00503cad;
        CCharString::CCharString(aCStack_180,"PopulateRegionWithVillagers 4",-1);
        NProgressDisplay::DisplayProgress
                  (aCStack_180,(float)(extraout_EDX_10 & 0xffffff00),false,false);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
                   extraout_EDX_11,unaff_EDI);
        CCharString::CCharString((CCharString *)&local_124,"",-1);
        CCharString::CCharString((CCharString *)&local_15c,"",-1);
        uStack_17c = 0;
        uStack_178 = 0;
        CCharString::CCharString(aCStack_174,(CCharString *)&local_15c);
        CCharString::CCharString(aCStack_170,(CCharString *)&local_124);
        uStack_16c = 0;
        uStack_16b = 1;
        uStack_16a = 1;
        uStack_169 = 0;
        uStack_168 = 0;
        iStack_164 = 0;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_15c,
                   extraout_EDX_12,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_124,
                   extraout_EDX_13,unaff_EDI);
        uStack_169 = 1;
        uVar36 = CBuilding_FindMostSpaceForNPCs();
        iStack_164 = (int)uVar36;
        if (iStack_164 == 0) {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                     (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),
                     unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                     extraout_EDX_16,unaff_EDI);
          puVar26 = local_130;
          pCVar9 = local_fc;
          goto joined_r0x00503cad;
        }
        iVar24 = local_194[0x10];
        iVar25 = local_194[0x11];
        local_198 = (CBaseObject *)0x0;
        bVar7 = CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
                          (*(CDefClassBase **)(iStack_164 + 0x70),
                           (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)&local_198);
        pCVar9 = local_198;
        if ((bVar7) && (iVar39 = *(int *)(local_198 + 0x94), iVar39 != 0)) {
          pCVar43 = (CDefPointer<CThingPhysicalSwitchDef_const_> *)&pCStack_144;
          pCStack_144 = (CBaseObject *)0x0;
          this_02 = NDisplayView::CViewBase::GetDisplayEngine(*(CViewBase **)(local_140 + 4));
          CDefinitionManager::GetDef<CThingPhysicalSwitchDef>
                    ((CDefinitionManager *)this_02,iVar39,pCVar43);
          if (*(int *)(pCStack_144 + 0xc0) == 1) {
            iVar24 = *(int *)(pCVar9 + 0x94);
            iVar25 = 0;
          }
          else {
            iVar25 = *(int *)(pCVar9 + 0x94);
            iVar24 = 0;
          }
          pCVar9 = pCStack_144 + 4;
          *(int *)pCVar9 = *(int *)pCVar9 + -1;
          if (*(int *)pCVar9 == 0) {
            (**(code **)(*(int *)pCStack_144 + 4))();
          }
        }
        pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
        if ((float)local_188 * local_100 <= (float)local_190 * local_104) {
          if (iVar25 != 0) {
LAB_00503b8b:
            pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c)
            ;
            pCStack_184 = pCVar30;
            if (local_11c == local_118) {
              std__vector__insert(local_11c,&pCStack_184,&local_199);
              local_188 = local_188 + 1;
            }
            else {
              if (local_11c != (undefined4 *)0x0) {
                *local_11c = pCVar30;
              }
              local_11c = local_11c + 1;
              local_188 = local_188 + 1;
            }
            goto LAB_00503c0e;
          }
          if (iVar24 != 0) {
            pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c)
            ;
            if (local_130 == local_12c) goto LAB_00503bfd;
            goto LAB_00503b34;
          }
        }
        else if (iVar24 == 0) {
          if (iVar25 != 0) goto LAB_00503b8b;
        }
        else {
          pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
          if (local_130 == local_12c) {
LAB_00503bfd:
            pCStack_184 = pCVar30;
            std__vector__insert(local_130,&pCStack_184,&local_199);
          }
          else {
LAB_00503b34:
            if (local_130 != (undefined4 *)0x0) {
              *local_130 = pCVar30;
            }
            local_130 = local_130 + 1;
            pCStack_184 = pCVar30;
          }
          local_190 = local_190 + 1;
LAB_00503c0e:
          if (pCVar30 != (CThingAICreature *)0x0) {
            *(CThing *)(pCVar30 + 0x1c9) = (CThing)((byte)*(CThing *)(pCVar30 + 0x1c9) | 1);
            *(CThing *)(pCVar30 + 0x15c) = (CThing)((byte)*(CThing *)(pCVar30 + 0x15c) | 0x40);
            CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
          }
        }
        ListNode_InsertPointer();
        ppVar23 = local_e4 + -1;
        local_13c = local_13c + -1;
        local_e4 = ppVar23;
        if (local_198 != (CBaseObject *)0x0) {
          pCVar9 = local_198 + 4;
          *(int *)pCVar9 = *(int *)pCVar9 + -1;
          if (*(int *)pCVar9 == 0) {
            (**(code **)(*(int *)local_198 + 4))();
            ppVar23 = extraout_EDX_14;
          }
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                   ppVar23,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                   extraout_EDX_15,unaff_EDI);
        puVar26 = local_130;
      } while( true );
    }
    goto LAB_005048f4;
  }
  goto LAB_00506392;
joined_r0x00503cad:
  iVar24 = local_150;
  if ((int)pCVar9 < 1) goto joined_r0x00503f30;
  CCharString::CCharString(aCStack_180,"PopulateRegionWithVillagers 5",-1);
  NProgressDisplay::DisplayProgress(aCStack_180,(float)(extraout_EDX_17 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_18,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_15c,"",-1);
  CCharString::CCharString((CCharString *)&local_198,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,(CCharString *)&local_198);
  CCharString::CCharString(aCStack_170,(CCharString *)&local_15c);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_198,
             extraout_EDX_19,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_15c,
             extraout_EDX_20,unaff_EDI);
  uStack_169 = 1;
  uVar36 = CBuilding_FindMostSpaceForNPCs();
  iStack_164 = (int)uVar36;
  if (iStack_164 == 0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),unaff_EDI
              );
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
               extraout_EDX_23,unaff_EDI);
    iVar24 = local_150;
    goto joined_r0x00503f30;
  }
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  iVar24 = local_194[0x13];
  iVar25 = local_194[0x12];
  if ((float)local_188 * local_100 <= (float)local_190 * local_104) {
    if (iVar24 == 0) {
      if (iVar25 == 0) goto LAB_00503ecd;
      goto LAB_00503db1;
    }
    pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
    if (local_11c == local_118) {
LAB_00503e26:
      pCStack_184 = pCVar30;
      std__vector__insert(local_11c,&pCStack_184,&local_199);
      local_188 = local_188 + 1;
    }
    else {
      pCStack_184 = pCVar30;
      if (local_11c != (undefined4 *)0x0) {
        *local_11c = pCVar30;
      }
LAB_00503e75:
      local_11c = local_11c + 1;
      local_188 = local_188 + 1;
      pCVar30 = pCStack_184;
    }
LAB_00503eb1:
    if (pCVar30 != (CThingAICreature *)0x0) {
      *(CThing *)(pCVar30 + 0x1c9) = (CThing)((byte)*(CThing *)(pCVar30 + 0x1c9) | 1);
      *(CThing *)(pCVar30 + 0x15c) = (CThing)((byte)*(CThing *)(pCVar30 + 0x15c) | 0x40);
      CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
    }
  }
  else {
    if (iVar25 != 0) {
LAB_00503db1:
      pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      pCStack_184 = pCVar30;
      if (puVar26 == local_12c) {
        std__vector__insert(puVar26,&pCStack_184,&local_199);
      }
      else {
        if (puVar26 != (undefined4 *)0x0) {
          *puVar26 = pCVar30;
        }
        local_130 = puVar26 + 1;
      }
      local_190 = local_190 + 1;
      puVar26 = local_130;
      goto LAB_00503eb1;
    }
    if (iVar24 != 0) {
      pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      if (local_11c == local_118) goto LAB_00503e26;
      pCStack_184 = pCVar30;
      if (local_11c == (undefined4 *)0x0) goto LAB_00503e75;
      *local_11c = pCVar30;
      local_11c = local_11c + 1;
      local_188 = local_188 + 1;
      goto LAB_00503eb1;
    }
  }
LAB_00503ecd:
  ListNode_InsertPointer();
  pCVar9 = local_fc + -1;
  local_13c = local_13c + -1;
  local_fc = pCVar9;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_21,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_22,unaff_EDI);
  goto joined_r0x00503cad;
joined_r0x00503f30:
  pCVar9 = local_158;
  if (iVar24 < 1) goto joined_r0x0050413c;
  CCharString::CCharString((CCharString *)&local_150,"PopulateRegionWithVillagers 6",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&local_150,(float)(extraout_EDX_24 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
             extraout_EDX_25,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_198,"",-1);
  CCharString::CCharString(aCStack_180,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,aCStack_180);
  CCharString::CCharString(aCStack_170,(CCharString *)&local_198);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_26,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_198,
             extraout_EDX_27,unaff_EDI);
  uStack_169 = 1;
  uVar36 = CBuilding_FindMostSpaceForNPCs();
  iStack_164 = (int)uVar36;
  if (iStack_164 == 0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),unaff_EDI
              );
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
               extraout_EDX_30,unaff_EDI);
    pCVar9 = local_158;
    goto joined_r0x0050413c;
  }
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  iVar25 = local_194[0x15];
  iVar39 = local_194[0x14];
  if ((float)local_188 * local_100 <= (float)local_190 * local_104) {
    if (iVar25 == 0) {
      if (iVar39 == 0) goto LAB_005040f0;
      goto LAB_0050407e;
    }
    pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
    local_188 = local_188 + 1;
LAB_0050408d:
    pCVar29 = local_140;
    if (pCVar30 != (CThingAICreature *)0x0) {
      pCVar30[0x1c9] = (CThingAICreature)((byte)pCVar30[0x1c9] | 1);
      pCVar30[0x15c] = (CThingAICreature)((byte)pCVar30[0x15c] | 0x40);
      CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
      local_15c = (CBaseObject *)0x0;
      bVar7 = CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
                        (*(CDefClassBase **)(*(int *)(pCVar29 + 4) + 0x70),
                         (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)&local_15c);
      pCVar9 = local_15c;
      if (bVar7) {
        CThingAICreature::AutoSetupSexuality(pCVar30,*(float *)(local_15c + 0x34));
      }
      if (pCVar9 != (CBaseObject *)0x0) {
        pCVar11 = pCVar9 + 4;
        *(int *)pCVar11 = *(int *)pCVar11 + -1;
        if (*(int *)pCVar11 == 0) {
          (**(code **)(*(int *)pCVar9 + 4))();
        }
      }
    }
  }
  else {
    if (iVar39 != 0) {
LAB_0050407e:
      pCVar30 = CThingAICreature::Create(iVar39,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      local_190 = local_190 + 1;
      goto LAB_0050408d;
    }
    if (iVar25 != 0) {
      pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      local_188 = local_188 + 1;
      goto LAB_0050408d;
    }
  }
LAB_005040f0:
  ListNode_InsertPointer();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_28,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_29,unaff_EDI);
  iVar24 = iVar24 + -1;
  goto joined_r0x00503f30;
joined_r0x0050413c:
  ppuVar27 = local_128;
  if ((int)pCVar9 < 1) goto joined_r0x00504341;
  CCharString::CCharString((CCharString *)&local_158,"PopulateRegionWithVillagers 7",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&local_158,(float)(extraout_EDX_31 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_158,
             extraout_EDX_32,unaff_EDI);
  CCharString::CCharString(aCStack_180,"",-1);
  CCharString::CCharString((CCharString *)&local_150,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,(CCharString *)&local_150);
  CCharString::CCharString(aCStack_170,aCStack_180);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
             extraout_EDX_33,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_34,unaff_EDI);
  uStack_169 = 1;
  uVar36 = CBuilding_FindMostSpaceForNPCs();
  iStack_164 = (int)uVar36;
  if (iStack_164 == 0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),unaff_EDI
              );
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
               extraout_EDX_37,unaff_EDI);
    ppuVar27 = local_128;
    goto joined_r0x00504341;
  }
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  iVar24 = local_194[0x17];
  iVar25 = local_194[0x16];
  if ((float)local_188 * local_100 <= (float)local_190 * local_104) {
    if (iVar24 == 0) {
      if (iVar25 == 0) goto LAB_005042f2;
      goto LAB_00504280;
    }
    pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
    local_188 = local_188 + 1;
LAB_0050428f:
    pCVar29 = local_140;
    if (pCVar30 != (CThingAICreature *)0x0) {
      pCVar30[0x1c9] = (CThingAICreature)((byte)pCVar30[0x1c9] | 1);
      pCVar30[0x15c] = (CThingAICreature)((byte)pCVar30[0x15c] | 0x40);
      CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
      local_198 = (CBaseObject *)0x0;
      bVar7 = CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
                        (*(CDefClassBase **)(*(int *)(pCVar29 + 4) + 0x70),
                         (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)&local_198);
      pCVar11 = local_198;
      if (bVar7) {
        CThingAICreature::AutoSetupSexuality(pCVar30,*(float *)(local_198 + 0x34));
      }
      if (pCVar11 != (CBaseObject *)0x0) {
        pCVar21 = pCVar11 + 4;
        *(int *)pCVar21 = *(int *)pCVar21 + -1;
        if (*(int *)pCVar21 == 0) {
          (**(code **)(*(int *)pCVar11 + 4))();
        }
      }
    }
  }
  else {
    if (iVar25 != 0) {
LAB_00504280:
      pCVar30 = CThingAICreature::Create(iVar25,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      local_190 = local_190 + 1;
      goto LAB_0050428f;
    }
    if (iVar24 != 0) {
      pCVar30 = CThingAICreature::Create(iVar24,pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
      local_188 = local_188 + 1;
      goto LAB_0050428f;
    }
  }
LAB_005042f2:
  ListNode_InsertPointer();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_35,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_36,unaff_EDI);
  pCVar9 = pCVar9 + -1;
  goto joined_r0x0050413c;
joined_r0x00504341:
  piVar12 = local_194;
  pCVar4 = local_138;
  if ((int)ppuVar27 < 1) goto joined_r0x00504530;
  CCharString::CCharString((CCharString *)&local_158,"PopulateRegionWithVillagers 8",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&local_158,(float)(extraout_EDX_38 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_158,
             extraout_EDX_39,unaff_EDI);
  CCharString::CCharString(aCStack_180,"",-1);
  CCharString::CCharString((CCharString *)&local_150,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,(CCharString *)&local_150);
  CCharString::CCharString(aCStack_170,aCStack_180);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
             extraout_EDX_40,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_41,unaff_EDI);
  uStack_169 = 1;
  iStack_164 = CBuilding_FindMostSpaceForNPCs();
  if (iStack_164 == 0) {
    uVar36 = CBuilding_FindMostSpaceForNPCs();
    iStack_164 = (int)uVar36;
    if (iStack_164 == 0) {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                 (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),
                 unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                 extraout_EDX_44,unaff_EDI);
      piVar12 = local_194;
      pCVar4 = local_138;
      goto joined_r0x00504530;
    }
  }
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  if (local_194[0x24] != 0) {
    pCVar30 = CThingAICreature::Create
                        (local_194[0x24],pCVar14,4,(CThingAICreatureInit *)&uStack_17c);
    pCVar29 = local_140;
    local_190 = local_190 + 1;
    if (pCVar30 != (CThingAICreature *)0x0) {
      pCVar30[0x1c9] = (CThingAICreature)((byte)pCVar30[0x1c9] | 1);
      pCVar30[0x15c] = (CThingAICreature)((byte)pCVar30[0x15c] | 0x40);
      CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
      local_198 = (CBaseObject *)0x0;
      bVar7 = CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
                        (*(CDefClassBase **)(*(int *)(pCVar29 + 4) + 0x70),
                         (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)&local_198);
      pCVar9 = local_198;
      if (bVar7) {
        CThingAICreature::AutoSetupSexuality(pCVar30,*(float *)(local_198 + 0x34));
      }
      if (pCVar9 != (CBaseObject *)0x0) {
        pCVar11 = pCVar9 + 4;
        *(int *)pCVar11 = *(int *)pCVar11 + -1;
        if (*(int *)pCVar11 == 0) {
          (**(code **)(*(int *)pCVar9 + 4))();
        }
      }
    }
  }
  ListNode_InsertPointer();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_42,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_43,unaff_EDI);
  ppuVar27 = (undefined4 **)((int)ppuVar27 + -1);
  goto joined_r0x00504341;
joined_r0x00504530:
  piVar8 = local_194;
  local_194 = piVar12;
  pCVar9 = local_160;
  if ((int)pCVar4 < 1) goto joined_r0x0050469d;
  CCharString::CCharString((CCharString *)&local_158,"PopulateRegionWithVillagers 9",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&local_158,(float)(extraout_EDX_45 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_158,
             extraout_EDX_46,unaff_EDI);
  CCharString::CCharString(aCStack_180,"",-1);
  CCharString::CCharString((CCharString *)&local_150,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,(CCharString *)&local_150);
  CCharString::CCharString(aCStack_170,aCStack_180);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
             extraout_EDX_47,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_48,unaff_EDI);
  uStack_169 = 1;
  uVar36 = CBuilding_FindMostSpaceForNPCs();
  iStack_164 = (int)uVar36;
  if (iStack_164 == 0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),unaff_EDI
              );
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
               extraout_EDX_51,unaff_EDI);
    pCVar9 = local_160;
    goto joined_r0x0050469d;
  }
  bVar7 = true;
  pvVar40 = (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&local_120;
  pvVar38 = (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&local_134;
  pCVar37 = (CThingAICreatureInit *)&uStack_17c;
  pCVar29 = local_140;
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  CreateRelevantPersonAndUpdateStats
            (*(float *)((int)piVar8 + 0x68),*(float *)((int)piVar8 + 0x6c),(long)local_100,
             (long)local_104,&local_190,&local_188,pCVar14,pCVar37,pCVar29,pvVar38,pvVar40,bVar7);
  ListNode_InsertPointer();
  local_13c = local_13c + -1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_49,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_50,unaff_EDI);
  piVar12 = local_194;
  pCVar4 = pCVar4 + -1;
  local_194 = piVar8;
  goto joined_r0x00504530;
joined_r0x0050469d:
  if ((int)pCVar9 < 1) goto LAB_00504846;
  CCharString::CCharString((CCharString *)&local_158,"PopulateRegionWithVillagers 10",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&local_158,(float)(extraout_EDX_52 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_158,
             extraout_EDX_53,unaff_EDI);
  CCharString::CCharString(aCStack_180,"",-1);
  CCharString::CCharString((CCharString *)&local_150,"",-1);
  uStack_17c = 0;
  uStack_178 = 0;
  CCharString::CCharString(aCStack_174,(CCharString *)&local_150);
  CCharString::CCharString(aCStack_170,aCStack_180);
  uStack_16c = 0;
  uStack_16b = 1;
  uStack_16a = 1;
  uStack_169 = 0;
  uStack_168 = 0;
  iStack_164 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
             extraout_EDX_54,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
             extraout_EDX_55,unaff_EDI);
  uStack_169 = 1;
  uVar36 = CBuilding_FindMostSpaceForNPCs();
  iStack_164 = (int)uVar36;
  if (iStack_164 == 0) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar36 >> 0x20),unaff_EDI
              );
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
               extraout_EDX_58,unaff_EDI);
    goto LAB_00504846;
  }
  puVar15 = (uint *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(local_c4 + 8));
  uVar10 = *puVar15 * 0x24a1 + 0x24df;
  *puVar15 = uVar10;
  uVar16 = GFROR13(uVar10);
  *puVar15 = uVar16;
  local_199 = (byte)((ulonglong)uVar16 % 0xffffffff) & 1;
  if (((ulonglong)uVar16 % 0xffffffff & 1) == 0) {
    fVar33 = (float)local_194[0x1d];
    fVar31 = (float)local_194[0x1c];
  }
  else {
    fVar33 = (float)local_194[0x1f];
    fVar31 = (float)local_194[0x1e];
  }
  bVar7 = true;
  pvVar40 = (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&local_120;
  pvVar38 = (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&local_134;
  pCVar37 = (CThingAICreatureInit *)&uStack_17c;
  pCVar29 = local_140;
  pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
  CreateRelevantPersonAndUpdateStats
            (fVar31,fVar33,(long)local_100,(long)local_104,&local_190,&local_188,pCVar14,pCVar37,
             pCVar29,pvVar38,pvVar40,bVar7);
  ListNode_InsertPointer();
  local_13c = local_13c + -1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
             extraout_EDX_56,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
             extraout_EDX_57,unaff_EDI);
  pCVar9 = pCVar9 + -1;
  goto joined_r0x0050469d;
LAB_00504846:
  piVar12 = local_194;
  if (local_13c < 0) {
    CGuiControlTreePane::SortTreeRecursively
              ((CGuiControlTreePane *)&local_c0,
               (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
    if (local_c0 != (CBaseObject *)0x0) {
      free(local_c0);
    }
    if (local_90 != (CBaseObject *)0x0) {
      free(local_90);
    }
    if (local_ac != (CBaseObject *)0x0) {
      free(local_ac);
    }
    if (local_120 != (undefined4 *)0x0) {
      free(local_120);
    }
    if (local_134 != (undefined4 *)0x0) {
      free(local_134);
    }
    DoublyLinkedList_DestroyRecursive();
    DoublyLinkedList_DestroyRecursive();
    piVar8 = piVar12 + 1;
    *piVar8 = *piVar8 + -1;
    if (*piVar8 != 0) {
      return;
    }
    (**(code **)(*piVar12 + 4))();
    return;
  }
LAB_005048f4:
  piVar12 = local_194;
  local_154 = (CThingBuilding *)(local_e0 + local_188 + local_190 + local_13c);
  if (local_13c != 0) {
    fVar33 = (float)(int)local_154;
    local_18c = (CBaseObject *)local_13c;
    local_138 = (CPixelShader *)(local_100 * fVar33);
    local_15c = (CBaseObject *)(local_104 * fVar33);
    local_128 = (undefined4 **)(fVar33 * local_c8);
    pCVar9 = local_148;
    do {
      CCharString::CCharString((CCharString *)&local_158,"PopulateRegionWithVillagers 11",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_158,(float)(extraout_EDX_59 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_158,
                 extraout_EDX_60,unaff_EDI);
      fVar33 = (float)local_138 - (float)local_190;
      local_198 = (CBaseObject *)((float)local_15c - (float)local_188);
      local_160 = (CBaseObject *)((float)local_128 - (float)local_e0);
      if (local_100 == 0.0) {
        fVar33 = -1000.0;
      }
      if (local_104 == 0.0) {
        local_198 = (CBaseObject *)0xc47a0000;
      }
      if (local_c8 == 0.0) {
        local_160 = (CBaseObject *)0xc47a0000;
      }
      if (((fVar33 <= (float)local_198) || (fVar33 <= (float)local_160)) ||
         ((piVar12[0x18] == 0 && (piVar12[0x20] == 0)))) {
        if (((float)local_160 < (float)local_198) && ((piVar12[0x19] != 0 || (piVar12[0x21] != 0))))
        {
          if (((int)pCVar9 < 1) || (piVar12[0x19] == 0)) {
            CCharString::CCharString((CCharString *)&local_13c,"",-1);
            CCharString::CCharString((CCharString *)&pCStack_184,"",-1);
            CThingAICreatureInit::CThingAICreatureInit
                      (aCStack_24,(CThingPhysical *)0x0,(CThingPhysical *)0x0,false,
                       (CCharString *)&pCStack_184,(CCharString *)&local_13c,true);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_184
                       ,extraout_EDX_73,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_13c,
                       extraout_EDX_74,unaff_EDI);
            pCVar37 = aCStack_24;
            lVar41 = 4;
            uStack_11 = 1;
            pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            pCVar30 = CThingAICreature::Create(piVar12[0x21],pCVar14,lVar41,pCVar37);
            local_148 = (CBaseObject *)pCVar30;
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_18,extraout_EDX_75,unaff_EDI);
            paVar22 = aaStack_1c;
            ppVar23 = extraout_EDX_76;
          }
          else {
            CCharString::CCharString((CCharString *)&local_e4,"",-1);
            CCharString::CCharString((CCharString *)&pCStack_144,"",-1);
            CThingAICreatureInit::CThingAICreatureInit
                      (aCStack_78,(CThingPhysical *)0x0,(CThingPhysical *)0x0,false,
                       (CCharString *)&pCStack_144,(CCharString *)&local_e4,true);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_144
                       ,extraout_EDX_69,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_e4,
                       extraout_EDX_70,unaff_EDI);
            uStack_65 = 1;
            uStack_60 = CBuilding_FindMostSpaceForNPCs();
            pCVar37 = aCStack_78;
            lVar41 = 4;
            pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            pCVar30 = CThingAICreature::Create(piVar12[0x19],pCVar14,lVar41,pCVar37);
            local_148 = (CBaseObject *)pCVar30;
            ListNode_InsertPointer();
            pCVar9 = pCVar9 + -1;
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_6c,extraout_EDX_71,unaff_EDI);
            paVar22 = aaStack_70;
            ppVar23 = extraout_EDX_72;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar22,ppVar23,unaff_EDI);
          if (pCVar30 != (CThingAICreature *)0x0) {
            if (local_11c != local_118) {
              if (local_11c != (undefined4 *)0x0) {
                *local_11c = pCVar30;
              }
              local_11c = local_11c + 1;
              local_188 = local_188 + 1;
              goto LAB_00504fd4;
            }
            std__vector__insert(local_11c,&local_148,&local_199);
          }
          local_188 = local_188 + 1;
          goto LAB_00504fd4;
        }
        if ((piVar12[0x23] != 0) && (piVar12[0x22] != 0)) {
          CCharString::CCharString((CCharString *)&pCStack_14c,"",-1);
          CCharString::CCharString((CCharString *)&piStack_10c,"",-1);
          CThingAICreatureInit::CThingAICreatureInit
                    (aCStack_5c,(CThingPhysical *)0x0,(CThingPhysical *)0x0,false,
                     (CCharString *)&piStack_10c,(CCharString *)&pCStack_14c,true);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_10c,
                     extraout_EDX_77,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_14c,
                     extraout_EDX_78,unaff_EDI);
          pCStack_44 = local_f8;
          uStack_49 = 1;
          puVar15 = (uint *)(DAT_013b86a0 + 0x1613c);
          uVar10 = *(int *)(DAT_013b86a0 + 0x1613c) * 0x24a1 + 0x24df;
          *puVar15 = uVar10;
          uVar16 = GFROR13(uVar10);
          *puVar15 = uVar16;
          local_198 = (CBaseObject *)(uVar16 & 1);
          if (((local_198 == (CBaseObject *)0x0) || (local_cc == 0)) && (0 < local_d0)) {
            pCVar37 = aCStack_5c;
            lVar41 = 4;
            pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            pCVar30 = CThingAICreature::Create(piVar12[0x23],pCVar14,lVar41,pCVar37);
            local_148 = (CBaseObject *)pCVar30;
            GFIntToCharString_API();
            CCharString::CCharString(aCStack_84,"girl",-1);
            pCVar17 = (CCharString *)CCharString__AppendData();
            CCharString::operator=((CCharString *)(pCVar30 + 0x74),pCVar17);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_80,extraout_EDX_79,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_84,
                       extraout_EDX_80,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_7c,extraout_EDX_81,unaff_EDI);
            local_cc = local_cc + 1;
            ppVar23 = extraout_EDX_82;
          }
          else {
            pCVar37 = aCStack_5c;
            lVar41 = 4;
            pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            pCVar30 = CThingAICreature::Create(piVar12[0x22],pCVar14,lVar41,pCVar37);
            local_148 = (CBaseObject *)pCVar30;
            GFIntToCharString_API();
            CCharString::CCharString((CCharString *)&ppuStack_a0,"boy",-1);
            pCVar17 = (CCharString *)CCharString__AppendData();
            CCharString::operator=((CCharString *)(pCVar30 + 0x74),pCVar17);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&iStack_94,
                       extraout_EDX_83,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppuStack_a0
                       ,extraout_EDX_84,unaff_EDI);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_114,
                       extraout_EDX_85,unaff_EDI);
            local_d0 = local_d0 + 1;
            ppVar23 = extraout_EDX_86;
          }
          if (pCVar30 != (CThingAICreature *)0x0) {
            CRegister_push_back_or_allocate();
            ppVar23 = extraout_EDX_87;
          }
          local_e0 = local_e0 + 1;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_50,ppVar23,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_54,extraout_EDX_88,unaff_EDI);
          goto LAB_00504fd4;
        }
      }
      else {
        if (((int)pCVar9 < 1) || (piVar12[0x18] == 0)) {
          CCharString::CCharString((CCharString *)&local_124,"",-1);
          CCharString::CCharString((CCharString *)&local_fc,"",-1);
          CThingAICreatureInit::CThingAICreatureInit
                    (aCStack_40,(CThingPhysical *)0x0,(CThingPhysical *)0x0,false,
                     (CCharString *)&local_fc,(CCharString *)&local_124,true);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_fc,
                     extraout_EDX_65,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_124,
                     extraout_EDX_66,unaff_EDI);
          pCVar37 = aCStack_40;
          lVar41 = 4;
          uStack_2d = 1;
          pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
          pCVar30 = CThingAICreature::Create(piVar12[0x20],pCVar14,lVar41,pCVar37);
          local_148 = (CBaseObject *)pCVar30;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_34,extraout_EDX_67,unaff_EDI);
          paVar22 = aaStack_38;
          ppVar23 = extraout_EDX_68;
        }
        else {
          CCharString::CCharString(aCStack_180,"",-1);
          CCharString::CCharString((CCharString *)&local_150,"",-1);
          CThingAICreatureInit::CThingAICreatureInit
                    ((CThingAICreatureInit *)&uStack_17c,(CThingPhysical *)0x0,(CThingPhysical *)0x0
                     ,false,(CCharString *)&local_150,aCStack_180,true);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_150,
                     extraout_EDX_61,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_180,
                     extraout_EDX_62,unaff_EDI);
          uStack_169 = 1;
          iStack_164 = CBuilding_FindMostSpaceForNPCs();
          pCVar37 = (CThingAICreatureInit *)&uStack_17c;
          lVar41 = 4;
          pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
          pCVar30 = CThingAICreature::Create(piVar12[0x18],pCVar14,lVar41,pCVar37);
          local_148 = (CBaseObject *)pCVar30;
          ListNode_InsertPointer();
          pCVar9 = pCVar9 + -1;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                     extraout_EDX_63,unaff_EDI);
          paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174;
          ppVar23 = extraout_EDX_64;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar22,ppVar23,unaff_EDI);
        if (pCVar30 != (CThingAICreature *)0x0) {
          CRegister_push_back_or_allocate();
        }
        local_190 = local_190 + 1;
LAB_00504fd4:
        if (pCVar30 != (CThingAICreature *)0x0) {
          *(CThing *)(pCVar30 + 0x1c9) = (CThing)((byte)*(CThing *)(pCVar30 + 0x1c9) | 1);
          *(CThing *)(pCVar30 + 0x15c) = (CThing)((byte)*(CThing *)(pCVar30 + 0x15c) | 0x40);
          CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
        }
      }
      local_18c = (CBaseObject *)((int)local_18c + -1);
    } while (local_18c != (CBaseObject *)0x0);
    local_18c = (CBaseObject *)0x0;
    local_148 = pCVar9;
  }
  ppuVar27 = &local_134;
  while( true ) {
    CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 12",-1);
    NProgressDisplay::DisplayProgress
              ((CCharString *)&local_18c,(float)(extraout_EDX_89 & 0xffffff00),false,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
               extraout_EDX_90,unaff_EDI);
    puVar1 = ppuVar27[1];
    for (puVar26 = *ppuVar27; puVar26 != puVar1; puVar26 = puVar26 + 1) {
      pCVar19 = (CThing *)*puVar26;
      if (((pCVar19 != (CThing *)0x0) && (((byte)pCVar19[0x91] & 1) == 0)) &&
         (bVar7 = CThing::IsUnderScriptSupervision(pCVar19), !bVar7)) {
        CThingAICreature::AutoSetupSexuality((CThingAICreature *)*puVar26,(float)local_194[0xd]);
      }
    }
    if (ppuVar27 != &local_134) break;
    ppuVar27 = &local_120;
  }
  if ((0.0 < (float)local_194[0xd]) &&
     (lVar41 = CTCVillage::GetNumberOfGayVillagers(local_140), lVar41 < 1)) {
    PopulateVillagers_ArrayResize();
    ppuVar6 = ppuStack_9c;
    ppuVar34 = ppuStack_a0;
    PopulateRegionWithVillagers_RandChance();
    ppVar23 = extraout_EDX_91;
    _Memory = ppuVar34;
    if (ppuVar34 != ppuVar6) {
      do {
        puVar2 = *ppuVar34;
        if (((puVar2 == (undefined *)0x0) || ((puVar2[0x91] & 1) != 0)) ||
           (local_199 = (byte)(*(uint *)(puVar2 + 0x20) >> 0xf) & 1,
           (*(uint *)(puVar2 + 0x20) >> 0xf & 1) == 0)) {
LAB_00505165:
          bVar7 = false;
        }
        else {
          local_114[0] = (CBaseObject *)&DAT_0000000f;
          piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                           ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                         *)(puVar2 + 0x44),(ETCInterfaceType *)local_114);
          if ((piVar12 == *(int **)(puVar2 + 0x48)) || (0xf < *piVar12)) {
            piVar12 = *(int **)(puVar2 + 0x48);
          }
          local_e8 = local_e8 | 2;
          CCharString::CCharString((CCharString *)&local_18c,(CCharString *)(piVar12[1] + 0x38));
          ppVar23 = extraout_EDX_92;
          if ((local_18c != (CBaseObject *)DAT_013bab2c) &&
             (((local_18c == (CBaseObject *)0x0 || (DAT_013bab2c == (CTCParticleAttacher *)0x0)) ||
              (CTCParticleAttacher::OnInitialActivate((CTCParticleAttacher *)local_18c),
              ppVar23 = extraout_EDX_93, extraout_AL == '\0')))) goto LAB_00505165;
          bVar7 = true;
        }
        if ((local_e8 & 2) != 0) {
          local_e8 = local_e8 & 0xfffffffd;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                     ppVar23,unaff_EDI);
          ppVar23 = extraout_EDX_94;
        }
        if ((bVar7) &&
           (bVar7 = CThing::IsUnderScriptSupervision((CThing *)*ppuVar34), ppVar23 = extraout_EDX_95
           , !bVar7)) {
          CThingAICreature::AutoSetupSexuality((CThingAICreature *)*ppuVar34,1.0);
          _Memory = ppuStack_a0;
          break;
        }
        ppuVar34 = ppuVar34 + 1;
        _Memory = ppuStack_a0;
      } while (ppuVar34 != ppuStack_9c);
    }
    if (_Memory != (undefined **)0x0) {
      free(_Memory);
    }
  }
  ppuVar27 = &local_134;
  do {
    local_128 = ppuVar27;
    CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 13",-1);
    NProgressDisplay::DisplayProgress
              ((CCharString *)&local_18c,(float)(extraout_EDX_96 & 0xffffff00),false,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
               extraout_EDX_97,unaff_EDI);
    piVar12 = *ppuVar27;
    if (piVar12 != ppuVar27[1]) {
      do {
        pCVar9 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(*piVar12 + 0x170));
        local_198 = pCVar9;
        RBTree__FindPredecessorNode();
        if ((local_114[0] != local_dc) && (local_160 = local_ac, local_ac != local_a8)) {
LAB_00505264:
          if (*(CBaseObject **)local_160 != pCVar9) goto code_r0x00505268;
          iVar24 = *piVar12;
          CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_a0);
          ppuStack_a0 = &PTR__vector_deleting_destructor__01244344;
          CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)&ppuStack_a0,pCVar9);
          pCVar11 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)&ppuStack_a0);
          CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)(iVar24 + 0x168),pCVar11);
          CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_a0)
          ;
          piVar8 = (int *)std__map__insert();
          iVar24 = *piVar8;
          piVar8 = malloc(0xc);
          if (piVar8 + 2 != (int *)0x0) {
            piVar8[2] = *piVar12;
          }
          puVar26 = *(undefined4 **)(iVar24 + 4);
          piVar8[1] = (int)puVar26;
          *piVar8 = iVar24;
          *puVar26 = piVar8;
          *(int **)(iVar24 + 4) = piVar8;
          CThingBuilding::PeekPMaterial((CThingBuilding *)pCVar9);
          piVar8 = (int *)std__map__insert();
          pCVar9 = local_a8;
          iVar24 = 0;
          for (puVar26 = *(undefined4 **)*piVar8; puVar26 != (undefined4 *)*piVar8;
              puVar26 = (undefined4 *)*puVar26) {
            iVar24 = iVar24 + 1;
          }
          if (iVar24 == *(int *)(local_138 + 0x44)) {
            pCVar11 = local_160 + 4;
            if (pCVar11 != local_a8) {
              memmove(local_160,pCVar11,(int)local_a8 - (int)pCVar11);
            }
            local_a8 = pCVar9 + 0xfffffffc;
          }
          if (local_138 != (CPixelShader *)0x0) {
            iVar24 = *(int *)(local_138 + 4);
            *(int *)(local_138 + 4) = iVar24 + -1;
            if (iVar24 + -1 == 0) {
              (**(code **)(*(int *)local_138 + 4))();
            }
            local_138 = (CPixelShader *)0x0;
          }
          ppuVar27 = local_128;
          piVar3 = local_128[1];
          piVar8 = piVar12 + 1;
          if (piVar3 != piVar8) {
            memmove(piVar12,piVar8,(int)piVar3 - (int)piVar8);
          }
          ppuVar27[1] = piVar3 + -1;
          goto LAB_0050527d;
        }
LAB_00505273:
        piVar12 = piVar12 + 1;
        ppuVar27 = local_128;
LAB_0050527d:
      } while (piVar12 != ppuVar27[1]);
    }
    pCVar11 = local_a8;
    pCVar9 = local_ac;
    if (local_128 != &local_134) goto LAB_005053e0;
    ppuVar27 = &local_120;
  } while( true );
code_r0x00505268:
  local_160 = local_160 + 4;
  if (local_160 == local_a8) goto LAB_00505273;
  goto LAB_00505264;
LAB_005053e0:
  local_160 = (CBaseObject *)(DAT_013b86a0 + 0x1613c);
  PopulateRegionWithVillagers_FloatRandom();
  std_random_shuffle_Fisher_Yates();
  std_random_shuffle_Fisher_Yates();
  std_random_shuffle_Fisher_Yates();
  Introsort_TripletPartition();
  if (pCVar9 != pCVar11) {
    do {
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 14",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_98 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_99,unaff_EDI);
      iVar25 = 0;
      piVar12 = (int *)CThingBuilding::PeekPMaterial(*(CThingBuilding **)pCVar9);
      iVar24 = *piVar12;
      if (pCStack_14c != (CBaseObject *)0x0) {
        iVar39 = *(int *)(pCStack_14c + 4);
        *(int *)(pCStack_14c + 4) = iVar39 + -1;
        if (iVar39 + -1 == 0) {
          (**(code **)(*(int *)pCStack_14c + 4))();
        }
        pCStack_14c = (CBaseObject *)0x0;
      }
      if (iVar24 != 0) {
        piVar12 = (int *)CThingBuilding::PeekPMaterial(*(CThingBuilding **)pCVar9);
        iVar25 = *(int *)(*piVar12 + 0x44);
        if (piStack_10c != (int *)0x0) {
          iVar24 = piStack_10c[1];
          piStack_10c[1] = iVar24 + -1;
          if (iVar24 + -1 == 0) {
            (**(code **)(*piStack_10c + 4))();
          }
          piStack_10c = (int *)0x0;
        }
      }
      piVar12 = (int *)std__map__insert();
      iVar24 = 0;
      for (puVar26 = *(undefined4 **)*piVar12; puVar26 != (undefined4 *)*piVar12;
          puVar26 = (undefined4 *)*puVar26) {
        iVar24 = iVar24 + 1;
      }
      iVar25 = iVar25 - iVar24;
      while ((iVar25 != 0 &&
             ((((iVar24 = (int)local_11c - (int)local_120 >> 2, iVar24 != 0 ||
                (((int)local_130 - (int)local_134 & 0xfffffffcU) != 0)) ||
               ((local_b4 - (int)local_b8 & 0xfffffffcU) != 0)) && (iVar25 != 0))))) {
        if (iVar24 != 0) {
          CVillageTaskCarryCrate::AssignCreatureToTask
                    ((CVillageTaskCarryCrate *)local_11c[-1],*(CThing **)pCVar9);
          piVar12 = (int *)std__map__insert();
          iVar24 = *piVar12;
          piVar12 = malloc(0xc);
          if (piVar12 + 2 != (int *)0x0) {
            piVar12[2] = local_11c[-1];
          }
          puVar26 = *(undefined4 **)(iVar24 + 4);
          *piVar12 = iVar24;
          piVar12[1] = (int)puVar26;
          *puVar26 = piVar12;
          *(int **)(iVar24 + 4) = piVar12;
          local_11c = local_11c + -1;
          iVar25 = iVar25 + -1;
        }
        if (iVar25 == 0) break;
        if (((int)local_130 - (int)local_134 & 0xfffffffcU) != 0) {
          CVillageTaskCarryCrate::AssignCreatureToTask
                    ((CVillageTaskCarryCrate *)local_130[-1],*(CThing **)pCVar9);
          piVar12 = (int *)std__map__insert();
          iVar24 = *piVar12;
          piVar12 = malloc(0xc);
          if (piVar12 + 2 != (int *)0x0) {
            piVar12[2] = local_130[-1];
          }
          puVar26 = *(undefined4 **)(iVar24 + 4);
          *piVar12 = iVar24;
          piVar12[1] = (int)puVar26;
          *puVar26 = piVar12;
          *(int **)(iVar24 + 4) = piVar12;
          local_130 = local_130 + -1;
          iVar25 = iVar25 + -1;
        }
        if (iVar25 == 0) break;
        if ((local_b4 - (int)local_b8 & 0xfffffffcU) != 0) {
          CVillageTaskCarryCrate::AssignCreatureToTask
                    (*(CVillageTaskCarryCrate **)(local_b4 + -4),*(CThing **)pCVar9);
          piVar12 = (int *)std__map__insert();
          iVar24 = *piVar12;
          piVar12 = malloc(0xc);
          if (piVar12 + 2 != (int *)0x0) {
            piVar12[2] = *(int *)(local_b4 + -4);
          }
          puVar26 = *(undefined4 **)(iVar24 + 4);
          *piVar12 = iVar24;
          piVar12[1] = (int)puVar26;
          *puVar26 = piVar12;
          *(int **)(iVar24 + 4) = piVar12;
          local_b4 = local_b4 + -4;
          iVar25 = iVar25 + -1;
        }
      }
      pCVar9 = pCVar9 + 4;
    } while (pCVar9 != local_a8);
  }
  pCVar5 = local_c4;
  pCStack_14c = (CBaseObject *)((int)local_130 - (int)local_134 >> 2);
  pCVar9 = (CBaseObject *)0x0;
  if (pCStack_14c != (CBaseObject *)0x0) {
    local_114[0] = (CBaseObject *)((int)local_11c - (int)local_120 >> 2);
    ppuStack_a0 = (undefined **)((int)local_120 - (int)local_134);
    iStack_94 = (int)local_b8 - (int)local_134;
    puVar26 = local_134;
    do {
      if ((local_114[0] <= pCVar9) || ((CBaseObject *)(local_b4 - (int)local_b8 >> 2) <= pCVar9))
      break;
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 15",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_x00100 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_x00101,unaff_EDI);
      pCVar18 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
      AddToRelevantBuilding
                (*(CThingAICreature **)((int)ppuStack_a0 + (int)puVar26),
                 (map<CThingBuilding*,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>,std::less<CThingBuilding*>,std::allocator<std::pair<CThingBuilding*_const,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>_>_>_>
                  *)&local_dc,(vector<CThingBuilding*,std::allocator<CThingBuilding*>_> *)&local_ac,
                 pCVar18,*(CWorld **)(pCVar5 + 8));
      if (pCVar9 < pCStack_14c) {
        pCVar18 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
        AddToRelevantBuilding
                  ((CThingAICreature *)*puVar26,
                   (map<CThingBuilding*,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>,std::less<CThingBuilding*>,std::allocator<std::pair<CThingBuilding*_const,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>_>_>_>
                    *)&local_dc,
                   (vector<CThingBuilding*,std::allocator<CThingBuilding*>_> *)&local_ac,pCVar18,
                   *(CWorld **)(pCVar5 + 8));
      }
      pCVar18 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
      AddToRelevantBuilding
                (*(CThingAICreature **)(iStack_94 + (int)puVar26),
                 (map<CThingBuilding*,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>,std::less<CThingBuilding*>,std::allocator<std::pair<CThingBuilding*_const,std::list<CThingAICreature*,std::allocator<CThingAICreature*>_>_>_>_>
                  *)&local_dc,(vector<CThingBuilding*,std::allocator<CThingBuilding*>_> *)&local_ac,
                 pCVar18,*(CWorld **)(pCVar5 + 8));
      pCVar9 = pCVar9 + 1;
      puVar26 = puVar26 + 1;
    } while (pCVar9 < pCStack_14c);
  }
  local_15c = *(CBaseObject **)(local_dc + 8);
  if (local_15c != local_dc) {
    do {
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 16",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_x00102 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_x00103,unaff_EDI);
      local_114[0] = *(CBaseObject **)(local_15c + 0x14);
      local_198 = *(CBaseObject **)local_114[0];
      local_160 = (CBaseObject *)0x0;
      local_138 = (CPixelShader *)0x0;
      this_05 = (CVillageTaskCarryCrate *)0x0;
      this_06 = (CVillageTaskCarryCrate *)0x0;
      if (local_198 != local_114[0]) {
        do {
          pCVar4 = *(CPixelShader **)(local_198 + 8);
          pCVar28 = this_05;
          pCVar35 = this_06;
          if ((pCVar4 != (CPixelShader *)0x0) &&
             (uVar10 = CPixelShader::CountReferences(pCVar4), uVar10 == 2)) {
            uVar10 = CPixelShader::CountReferences(pCVar4);
            if ((uVar10 == 1) &&
               (pCVar9 = CBaseIntelligentPointer::GetPItem
                                   ((CBaseIntelligentPointer *)(pCVar4 + 0x188)),
               pCVar9 == (CBaseObject *)0x0)) {
              pCVar28 = (CVillageTaskCarryCrate *)pCVar4;
              if (this_06 != (CVillageTaskCarryCrate *)0x0) {
                CVillageTaskCarryCrate::AssignCreatureToTask(this_06,(CThing *)pCVar4);
                CVillageTaskCarryCrate::AssignCreatureToTask
                          ((CVillageTaskCarryCrate *)pCVar4,(CThing *)this_06);
                pCVar28 = this_05;
                pCVar35 = (CVillageTaskCarryCrate *)(CPixelShader *)0x0;
                local_160 = (CBaseObject *)pCVar4;
                local_138 = (CPixelShader *)this_06;
              }
            }
            else {
              uVar10 = CPixelShader::CountReferences(pCVar4);
              if (((uVar10 == 2) &&
                  (pCVar9 = CBaseIntelligentPointer::GetPItem
                                      ((CBaseIntelligentPointer *)(pCVar4 + 0x188)),
                  pCVar9 == (CBaseObject *)0x0)) &&
                 (pCVar35 = (CVillageTaskCarryCrate *)pCVar4,
                 this_05 != (CVillageTaskCarryCrate *)0x0)) {
                CVillageTaskCarryCrate::AssignCreatureToTask(this_05,(CThing *)pCVar4);
                CVillageTaskCarryCrate::AssignCreatureToTask
                          ((CVillageTaskCarryCrate *)pCVar4,(CThing *)this_05);
                pCVar28 = (CVillageTaskCarryCrate *)(CPixelShader *)0x0;
                pCVar35 = this_06;
                local_160 = (CBaseObject *)this_05;
                local_138 = pCVar4;
              }
            }
          }
          local_198 = *(CBaseObject **)local_198;
          this_05 = pCVar28;
          this_06 = pCVar35;
        } while (local_198 != local_114[0]);
        if ((local_138 != (CPixelShader *)0x0) || (local_160 != (CBaseObject *)0x0)) {
          puVar26 = *(undefined4 **)(local_15c + 0x14);
          for (puVar1 = (undefined4 *)*puVar26; puVar1 != puVar26; puVar1 = (undefined4 *)*puVar1) {
            pCVar4 = (CPixelShader *)puVar1[2];
            if (((pCVar4 != (CPixelShader *)0x0) &&
                (uVar10 = CPixelShader::CountReferences(pCVar4), uVar10 == 1)) &&
               ((pCVar9 = CBaseIntelligentPointer::GetPItem
                                    ((CBaseIntelligentPointer *)(pCVar4 + 0x178)),
                pCVar9 == (CBaseObject *)0x0 &&
                (pCVar9 = CBaseIntelligentPointer::GetPItem
                                    ((CBaseIntelligentPointer *)(pCVar4 + 0x180)),
                pCVar9 == (CBaseObject *)0x0)))) {
              if (local_160 != (CBaseObject *)0x0) {
                CVillageTaskCarryCrate::AssignCreatureToTask
                          ((CVillageTaskCarryCrate *)pCVar4,(CThing *)local_160);
              }
              if (local_138 != (CPixelShader *)0x0) {
                CVillageTaskCarryCrate::AssignCreatureToTask
                          ((CVillageTaskCarryCrate *)pCVar4,(CThing *)local_138);
              }
            }
          }
        }
      }
      pCVar9 = *(CBaseObject **)(local_15c + 0xc);
      if (pCVar9 == (CBaseObject *)0x0) {
        pCVar9 = *(CBaseObject **)(local_15c + 4);
        if (local_15c == *(CBaseObject **)(pCVar9 + 0xc)) {
          do {
            local_15c = pCVar9;
            pCVar9 = *(CBaseObject **)(local_15c + 4);
          } while (local_15c == *(CBaseObject **)(pCVar9 + 0xc));
        }
        if (*(CBaseObject **)(local_15c + 0xc) != pCVar9) {
          local_15c = pCVar9;
        }
      }
      else {
        for (pCVar11 = *(CBaseObject **)(pCVar9 + 8); local_15c = pCVar9,
            pCVar11 != (CBaseObject *)0x0; pCVar11 = *(CBaseObject **)(pCVar11 + 8)) {
          pCVar9 = pCVar11;
        }
      }
    } while (local_15c != local_dc);
  }
  piVar12 = local_194;
  if ((local_194[0x25] != 0) && (0 < (int)local_bc)) {
    local_158 = local_bc;
    do {
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 17",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_x00104 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_x00105,unaff_EDI);
      CCharString::CCharString((CCharString *)&piStack_10c,"",-1);
      CCharString::CCharString((CCharString *)&pCStack_14c,"",-1);
      uStack_17c = 0;
      uStack_178 = 0;
      CCharString::CCharString(aCStack_174,(CCharString *)&pCStack_14c);
      CCharString::CCharString(aCStack_170,(CCharString *)&piStack_10c);
      uStack_16c = 0;
      uStack_16b = 1;
      uStack_16a = 1;
      uStack_169 = 0;
      uStack_168 = 0;
      iStack_164 = 0;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_14c,
                 extraout_EDX_x00106,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_10c,
                 extraout_EDX_x00107,unaff_EDI);
      pCVar37 = (CThingAICreatureInit *)&uStack_17c;
      lVar41 = 4;
      uStack_169 = 1;
      pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
      pCVar30 = CThingAICreature::Create(piVar12[0x25],pCVar14,lVar41,pCVar37);
      ppVar23 = extraout_EDX_x00108;
      if ((pCVar30 != (CThingAICreature *)0x0) && (((byte)pCVar30[0x91] & 1) == 0)) {
        pCVar30[0x1c9] = (CThingAICreature)((byte)pCVar30[0x1c9] | 1);
        pCVar30[0x15c] = (CThingAICreature)((byte)pCVar30[0x15c] | 0x40);
        CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
        ppVar23 = extraout_EDX_x00109;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                 ppVar23,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                 extraout_EDX_x00110,unaff_EDI);
      local_158 = local_158 + -1;
    } while (local_158 != (CBaseObject *)0x0);
  }
  if (((piVar12[0x26] != 0) || (piVar12[0x27] != 0)) && (0 < (int)local_108)) {
    local_158 = local_108;
    do {
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 17.5",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_x00111 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_x00112,unaff_EDI);
      CCharString::CCharString((CCharString *)&piStack_10c,"",-1);
      CCharString::CCharString((CCharString *)&pCStack_14c,"",-1);
      uStack_17c = 0;
      uStack_178 = 0;
      CCharString::CCharString(aCStack_174,(CCharString *)&pCStack_14c);
      CCharString::CCharString(aCStack_170,(CCharString *)&piStack_10c);
      uStack_16c = 0;
      uStack_16b = 1;
      uStack_16a = 1;
      uStack_169 = 0;
      uStack_168 = 0;
      iStack_164 = 0;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_14c,
                 extraout_EDX_x00113,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_10c,
                 extraout_EDX_x00114,unaff_EDI);
      uStack_169 = 1;
      iVar24 = piVar12[0x26];
      if (iVar24 == 0) {
LAB_00505c6b:
        iVar24 = piVar12[0x27];
      }
      else if (piVar12[0x27] != 0) {
        puVar15 = (uint *)(DAT_013b86a0 + 0x1613c);
        uVar10 = *(int *)(DAT_013b86a0 + 0x1613c) * 0x24a1 + 0x24df;
        *puVar15 = uVar10;
        uVar16 = GFROR13(uVar10);
        *puVar15 = uVar16;
        local_108 = (CBaseObject *)(uVar16 & 1);
        if (local_108 != (CBaseObject *)0x0) goto LAB_00505c6b;
        iVar24 = piVar12[0x26];
      }
      pCVar37 = (CThingAICreatureInit *)&uStack_17c;
      lVar41 = 4;
      pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
      pCVar30 = CThingAICreature::Create(iVar24,pCVar14,lVar41,pCVar37);
      ppVar23 = extraout_EDX_x00115;
      if ((pCVar30 != (CThingAICreature *)0x0) && (((byte)pCVar30[0x91] & 1) == 0)) {
        pCVar30[0x1c9] = (CThingAICreature)((byte)pCVar30[0x1c9] | 1);
        pCVar30[0x15c] = (CThingAICreature)((byte)pCVar30[0x15c] | 0x40);
        CTCVillage::AddThingToVillage(local_140,(CThing *)pCVar30);
        ppVar23 = extraout_EDX_x00116;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_170,
                 ppVar23,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_174,
                 extraout_EDX_x00117,unaff_EDI);
      local_158 = local_158 + -1;
    } while (local_158 != (CBaseObject *)0x0);
  }
  pCStack_144 = local_90;
  if (local_90 != local_8c) {
    do {
      CCharString::CCharString((CCharString *)&local_18c,"PopulateRegionWithVillagers 18",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_18c,(float)(extraout_EDX_x00118 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                 extraout_EDX_x00119,unaff_EDI);
      this_01 = *(CThingBuilding **)pCStack_144;
      CThingBuilding::PeekPMaterial(this_01);
      if (local_124 != (CBaseObject *)0x0) {
        if ((((byte)local_124[0x3c] & 1) != 0) && (*(int *)(local_124 + 0x44) != 0)) {
          fVar33 = (float)(int)local_154;
          local_138 = (CPixelShader *)(local_100 * fVar33);
          local_15c = (CBaseObject *)(local_104 * fVar33);
          local_128 = (undefined4 **)(fVar33 * local_c8);
          piStack_10c = (int *)*(int *)(local_124 + 0x44);
          do {
            local_198 = (CBaseObject *)((float)local_138 - (float)local_190);
            local_160 = (CBaseObject *)((float)local_15c - (float)local_188);
            local_fc = (CBaseObject *)((float)local_128 - (float)local_e0);
            if (local_100 == 0.0) {
              local_198 = (CBaseObject *)0xc47a0000;
            }
            if (local_104 == 0.0) {
              local_160 = (CBaseObject *)0xc47a0000;
            }
            if (local_c8 == 0.0) {
              local_fc = (CBaseObject *)0xc47a0000;
            }
            CCharString::CCharString((CCharString *)&pCStack_14c,"DUMMY_VILLAGER",-1);
            pCVar18 = *(CDefinitionManager **)(*(int *)(local_c4 + 8) + 0x10);
            uVar42 = 0;
            pCVar14 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            lVar41 = CDefinitionManager::GetDefGlobalIndexFromName
                               (pCVar18,(CCharString *)&pCStack_14c);
            pCVar19 = CTCDNone::Create(lVar41,pCVar14,(bool)uVar42);
            local_bc = (CBaseObject *)pCVar19;
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_14c
                       ,extraout_EDX_x00120,unaff_EDI);
            pCVar32 = (CTCDummyVillager *)0x0;
            local_199 = (byte)(*(uint *)(pCVar19 + 0x38) >> 0x16) & 1;
            if ((*(uint *)(pCVar19 + 0x38) >> 0x16 & 1) != 0) {
              local_108 = (CBaseObject *)0xd6;
              piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                               ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                             *)(pCVar19 + 0x44),(ETCInterfaceType *)&local_108);
              if ((piVar12 == *(int **)(pCVar19 + 0x48)) || (0xd6 < *piVar12)) {
                piVar12 = *(int **)(pCVar19 + 0x48);
              }
              pCVar32 = (CTCDummyVillager *)piVar12[1];
            }
            if (((float)local_198 <= (float)local_160) || ((float)local_198 <= (float)local_fc)) {
              if ((float)local_160 <= (float)local_fc) {
                puVar15 = (uint *)(DAT_013b86a0 + 0x1613c);
                uVar10 = *(int *)(DAT_013b86a0 + 0x1613c) * 0x24a1 + 0x24df;
                *puVar15 = uVar10;
                uVar16 = GFROR13(uVar10);
                *puVar15 = uVar16;
                local_108 = (CBaseObject *)(uVar16 & 1);
                if (((local_108 == (CBaseObject *)0x0) || (local_cc == 0)) && (0 < local_d0)) {
                  CTCDummyVillager::SetupFromData
                            (pCVar32,local_194[0x23],this_01,(CThingBuilding *)local_f8,false);
                  local_cc = local_cc + 1;
                }
                else {
                  CTCDummyVillager::SetupFromData
                            (pCVar32,local_194[0x22],this_01,(CThingBuilding *)local_f8,false);
                  local_d0 = local_d0 + 1;
                }
                local_e0 = local_e0 + 1;
                pCVar19 = (CThing *)local_bc;
              }
              else if (((int)local_148 < 1) || (local_194[0x19] == 0)) {
                CTCDummyVillager::SetupFromData
                          (pCVar32,local_194[0x21],this_01,(CThingBuilding *)0x0,false);
                local_188 = local_188 + 1;
              }
              else {
                pCVar20 = (CThingBuilding *)CBuilding_FindMostSpaceForNPCs();
                CTCDummyVillager::SetupFromData(pCVar32,local_194[0x19],this_01,pCVar20,false);
                local_148 = local_148 + -1;
                local_188 = local_188 + 1;
              }
            }
            else if (((int)local_148 < 1) || (local_194[0x18] == 0)) {
              CTCDummyVillager::SetupFromData
                        (pCVar32,local_194[0x20],this_01,(CThingBuilding *)0x0,false);
              local_190 = local_190 + 1;
            }
            else {
              pCVar20 = (CThingBuilding *)CBuilding_FindMostSpaceForNPCs();
              CTCDummyVillager::SetupFromData(pCVar32,local_194[0x18],this_01,pCVar20,false);
              local_148 = local_148 + -1;
              local_190 = local_190 + 1;
            }
            this_04 = (CTCOwnedEntity *)0x0;
            local_199 = (byte)(*(uint *)(pCVar19 + 0x24) >> 0x13) & 1;
            if ((*(uint *)(pCVar19 + 0x24) >> 0x13 & 1) != 0) {
              local_108 = (CBaseObject *)0x33;
              piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                               ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                             *)(pCVar19 + 0x44),(ETCInterfaceType *)&local_108);
              if ((piVar12 == *(int **)(pCVar19 + 0x48)) || (0x33 < *piVar12)) {
                piVar12 = *(int **)(pCVar19 + 0x48);
              }
              this_04 = (CTCOwnedEntity *)piVar12[1];
            }
            CTCOwnedEntity::SetPOwner(this_04,(CThing *)this_01);
            CTCVillage::AddThingToVillage(local_140,pCVar19);
            piStack_10c = (int *)((int)piStack_10c + -1);
          } while (piStack_10c != (int *)0x0);
          piStack_10c = (int *)0x0;
        }
        if (local_124 != (CBaseObject *)0x0) {
          iVar24 = *(int *)(local_124 + 4);
          *(int *)(local_124 + 4) = iVar24 + -1;
          if (iVar24 + -1 == 0) {
            (**(code **)(*(int *)local_124 + 4))();
          }
          local_124 = (CBaseObject *)0x0;
        }
      }
      pCStack_144 = pCStack_144 + 4;
    } while (pCStack_144 != local_8c);
  }
  local_114[0] = *(CBaseObject **)(local_140 + 0x2c);
  pCVar9 = *(CBaseObject **)local_114[0];
  pCVar11 = local_c0;
  pCStack_14c = pCVar9;
  if (pCVar9 != local_114[0]) {
    do {
      pCStack_14c = pCVar9;
      CCharString::CCharString((CCharString *)&local_154,"PopulateRegionWithVillagers 19",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&local_154,(float)(extraout_EDX_x00121 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_154,
                 extraout_EDX_x00122,unaff_EDI);
      pCVar21 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(pCVar9 + 8));
      local_108 = pCVar21;
      if (((pCVar21 != (CBaseObject *)0x0) && (((byte)pCVar21[0x91] & 1) == 0)) &&
         (std::
          _Find<std::_List_unchecked_iterator<std::_List_val<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>_>,CThing*>
                    (&local_bc,&local_108,*(undefined4 *)pCVar11), local_bc == pCVar11)) {
        CCharString::CCharString((CCharString *)&local_18c,"DUMMY_VILLAGER",-1);
        pCVar18 = *(CDefinitionManager **)(*(int *)(local_c4 + 8) + 0x10);
        uVar42 = 0;
        local_108 = (CBaseObject *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
        lVar41 = CDefinitionManager::GetDefGlobalIndexFromName(pCVar18,(CCharString *)&local_18c);
        pCVar19 = CTCDNone::Create(lVar41,(C3DVector *)local_108,(bool)uVar42);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_18c,
                   extraout_EDX_x00123,unaff_EDI);
        pCVar32 = (CTCDummyVillager *)0x0;
        if ((*(uint *)(pCVar19 + 0x38) & 0x400000) != 0) {
          local_108 = (CBaseObject *)0xd6;
          piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                           ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                         *)(pCVar19 + 0x44),(ETCInterfaceType *)&local_108);
          if ((piVar12 == *(int **)(pCVar19 + 0x48)) || (0xd6 < *piVar12)) {
            piVar12 = *(int **)(pCVar19 + 0x48);
          }
          pCVar32 = (CTCDummyVillager *)piVar12[1];
          pCVar9 = pCStack_14c;
        }
        CTCDummyVillager::SetupFromVillager(pCVar32,(CThingAICreature *)pCVar21,false);
        CTCVillage::AddThingToVillage(local_140,pCVar19);
        pCVar11 = local_c0;
      }
      pCVar9 = *(CBaseObject **)pCVar9;
      pCStack_14c = pCVar9;
    } while (pCVar9 != local_114[0]);
  }
  CGuiControlTreePane::SortTreeRecursively
            ((CGuiControlTreePane *)&local_c0,
             (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
  if (pCVar11 != (CBaseObject *)0x0) {
    free(pCVar11);
  }
  if (local_90 != (CBaseObject *)0x0) {
    free(local_90);
  }
  if (local_ac != (CBaseObject *)0x0) {
    free(local_ac);
  }
  if (local_b8 != (void *)0x0) {
    free(local_b8);
  }
  if (local_120 != (undefined4 *)0x0) {
    free(local_120);
  }
  if (local_134 != (undefined4 *)0x0) {
    free(local_134);
  }
  if (iStack_f0 != 0) {
    TreeNode_DestroyRecursive();
    *(void **)((int)local_f4 + 8) = local_f4;
    *(undefined4 *)((int)local_f4 + 4) = 0;
    *(void **)((int)local_f4 + 0xc) = local_f4;
    iStack_f0 = 0;
  }
  if (local_f4 != (void *)0x0) {
    free(local_f4);
  }
  if (iStack_d8 != 0) {
    TreeNode_DestroyRecursive();
    *(CBaseObject **)(local_dc + 8) = local_dc;
    *(int *)(local_dc + 4) = 0;
    *(CBaseObject **)(local_dc + 0xc) = local_dc;
    iStack_d8 = 0;
  }
  if (local_dc != (CBaseObject *)0x0) {
    free(local_dc);
  }
LAB_00506392:
  if (local_194 != (int *)0x0) {
    piVar12 = local_194 + 1;
    *piVar12 = *piVar12 + -1;
    if (*piVar12 == 0) {
      (**(code **)(*local_194 + 4))();
    }
  }
  return;
}


// CALLEES of PopulateRegionWithVillagers:
//   00661ff0  CountReferences
//   0065b854  AddThingToVillage
//   00514730  PopulateRegionWithVillagers_FindTreeNode
//   00bfea14  free
//   00833800  Create
//   0082d2d0  PeekPMaterial
//   005131d0  PopulateRegionWithVillagers_RandChance
//   0051bf90  std::map::insert
//   00515cc0  std::vector::insert
//   00513270  std_random_shuffle_Fisher_Yates
//   0099f830  GFIntToCharString_API
//   00658f41  GetNumberOfGayVillagers
//   00835a20  Create
//   0040f020  LowerBound
//   0099ec30  CCharString
//   009e9f40  DisplayProgress
//   0049d810  DoSizeof
//   0099ebf0  CCharString
//   004c7e90  IsUnderScriptSupervision
//   00497890  GFROR13
//   0099f570  CCharString::AppendData
//   005029a0  CBuilding_FindMostSpaceForNPCs
//   004c7990  GetDisplayEngine
//   0049dbd0  PopDontPopulateNextLoadedRegion
//   00512210  AssignCreatureToTask
//   004365b0  LowerBound
//   00662010  CountReferences
//   00512260  AssignCreatureToTask
//   00512350  AssignCreatureToTask
//   00512fe0  GetSubDef<CAICreatureWillPowerIndicatorDef>
//   0072f170  SetupFromData
//   0082e0e0  GetIsAvailableForResidency
//   00514350  PopulateVillagers_ArrayResize
//   008330d0  AutoSetupSexuality
//   0051af80  TreeNode_EraseRange
//   00bfea0e  malloc
//   00519970  TreeNode_DestroyRecursive
//   00512300  AssignCreatureToTask
//   004ff750  CreateRelevantPersonAndUpdateStats
//   0051c400  Introsort_TripletPartition
//   00510d40  SortTreeRecursively
//   005003e0  ListNode_InsertPointer
//   0051b6e0  DoublyLinkedList_Initialize
//   00bfeae6  memmove
//   00511bf0  _Find<std::_List_unchecked_iterator<std::_List_val<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>_>,CThing*>
//   00a01b90  SetPItem
//   00519420  DoublyLinkedList_CreateAndInit
//   005143e0  SortTreeRecursively
//   005130a0  GetSubDef<CAICreatureWillPowerIndicatorDef>
//   00a01b10  CBaseIntelligentPointer
//   004a93c0  OnInitialActivate
//   00513160  GetDef<CThingPhysicalSwitchDef>
//   004ad220  VectorPush_OrExpand
//   00513210  PopulateRegionWithVillagers_FloatRandom
//   00662000  CountReferences
//   007e8590  SetPOwner
//   00a01c10  ~CBaseIntelligentPointer
//   00415fbc  GFGetPreMainMemoryUsed
//   009ad410  GetDefGlobalIndexFromName
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   005122b0  AssignCreatureToTask
//   0040dce0  IS_THING_ALIVE<CThingBuilding*>
//   004c73d0  DoSizeof
//   0099efb0  operator=
//   00a01b50  GetPItem
//   00517a10  CRegister_push_back_or_allocate
//   004ac4b0  LinkedList_AppendNode
//   0072f2f0  SetupFromVillager
//   0051b720  DoublyLinkedList_DestroyRecursive
//   00513420  RBTree::FindPredecessorNode
//   00502b00  AddToRelevantBuilding
//   004facd0  CThingAICreatureInit

//=== CCreatureAction_BriarRoseShotFarInto @ 008c5410 (seed 008c5470) ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall
   CCreatureAction_BriarRoseShotFarInto::CCreatureAction_BriarRoseShotFarInto(class
   CThingCreatureBase &,class CThingCreatureBase &) */

CCreatureAction_BriarRoseShotFarInto * __thiscall
CCreatureAction_BriarRoseShotFarInto::CCreatureAction_BriarRoseShotFarInto
          (CCreatureAction_BriarRoseShotFarInto *this,CThingCreatureBase *param_1,
          CThingCreatureBase *param_2)

{
  CThingCreatureBase *pCVar1;
  CSoundPair *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString local_c [4];
  CThingCreatureBase local_8 [8];
  
  CCharString::CCharString(local_c,(char *)0x0,-1);
  pCVar1 = param_1;
  pCVar2 = (CSoundPair *)CCombatActionBase::GetSoundPair(local_8,(CCharString *)param_1);
  CCombatActionBase::CCombatActionBase
            ((CCombatActionBase *)this,pCVar1,(CThing *)param_2,0x96,pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,extraout_EDX,
             unaff_EDI);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01281304;
  CCharString::CCharString((CCharString *)&param_1,"DIVE_ATTACK",-1);
  CTCInGameMenu::SetPDef((CTCInGameMenu *)this,(long)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_00,unaff_EDI);
  return this;
}


// CALLEES of CCreatureAction_BriarRoseShotFarInto:
//   008574d0  SetPDef
//   008561a0  GetSoundPair
//   00857f30  CCombatActionBase
//   0099ebf0  CCharString
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>

