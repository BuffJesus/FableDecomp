 '0x507d21' '0x821a80' '0x521ae0' '0x4fb3e0' '0x4fb990' '0x4fd040' '0x4fbde0' '0x42f44d' '0x416a86' '0x494739' '0x4fcfc0' '0x4fd000'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== ?LoadFromFile@CWorldMap@@UAEXAAVCAFile@@J@Z @ 00507c30 ==================== 

void __thiscall
_LoadFromFile_CWorldMap__UAEXAAVCAFile__J_Z(CWorldMap *param_1,int *param_2,int param_3)

{
  C2DBoxI *pCVar1;
  undefined4 *puVar2;
  CBasicString<char> *this;
  CWorldMap *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar4;
  char cVar5;
  bool bVar6;
  CLevelLoader *this_00;
  CThingManager *pCVar7;
  CIPhysicsFacesSegmentBase *pCVar8;
  CGameJoystickManager *pCVar9;
  CCharString *pCVar10;
  CWideString *pCVar11;
  ulong uVar12;
  char *pcVar13;
  CCharString *pCVar14;
  undefined4 *puVar15;
  long lVar16;
  CCharString *pCVar17;
  CDiskFileWin32 *this_01;
  int iVar18;
  int *piVar19;
  int iVar20;
  char *pcVar21;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  uint extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar23;
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
  uint extraout_EDX_69;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_70;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_71;
  uint extraout_EDX_72;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_73;
  int unaff_EBX;
  CThingCreatureBase *this_02;
  uint uVar24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar25;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar26;
  float fVar27;
  int *unaff_retaddr;
  int *piVar28;
  undefined4 uVar29;
  undefined4 uStack_1ec;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_1e8;
  CCharString *pCVar30;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar31;
  vector<CThing*,std::allocator<CThing*>_> *pvVar32;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar33;
  CWorld *pCVar34;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar35;
  CBasicString<char> *pCStack_1bc;
  CWorldMap *pCStack_1b8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppStack_1b4;
  CCharString aCStack_1b0 [3];
  undefined1 uStack_1ad;
  int iStack_1ac;
  CCharString aCStack_1a8 [3];
  C2DBoxI CStack_1a5;
  undefined4 *puStack_1a4;
  CCharString aCStack_1a0 [4];
  CBasicString<char> *pCStack_19c;
  CCharString aCStack_198 [4];
  int iStack_194;
  CCharString aCStack_190 [4];
  long lStack_18c;
  CCharString aCStack_188 [4];
  CStringParser aCStack_184 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_178 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_174 [4];
  int *piStack_170;
  CCharString aCStack_15c [4];
  CCharString aCStack_158 [8];
  undefined4 *puStack_150;
  CCharString aCStack_14c [4];
  CCharString aCStack_148 [4];
  CCharString aCStack_144 [4];
  CCharString aCStack_140 [4];
  CCharString aCStack_13c [4];
  CCharString aCStack_138 [4];
  undefined4 uStack_134;
  CCharString aCStack_130 [4];
  int iStack_12c;
  undefined4 *puStack_128;
  undefined4 *puStack_124;
  CCharString aCStack_120 [4];
  undefined4 *puStack_11c;
  CCharString aCStack_118 [4];
  undefined4 *puStack_114;
  CCharString aCStack_110 [4];
  CCharString aCStack_10c [4];
  void *pvStack_108;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_104;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_100;
  CCharString aCStack_f8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_f4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_f0 [4];
  CUserProfileManager aCStack_ec [4];
  CUserProfileManager aCStack_e8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_e4 [4];
  CCharString aCStack_e0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_dc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_d8 [4];
  CCharString aCStack_d4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_d0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_cc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_c8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_c4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_c0 [4];
  CUserProfileManager aCStack_bc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b0 [4];
  CCharString aCStack_ac [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_a8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_a4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_a0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_9c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_98 [4];
  CCharString aCStack_94 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_90 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_8c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_88 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_84 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_80 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_7c [4];
  CCharString aCStack_78 [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_70 [4];
  CCharString aCStack_6c [16];
  CEngineInit aCStack_5c [92];
  
  ppStack_1b4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)param_1;
  if (*(int *)(param_1 + 0xbc) == 0) {
    this_00 = operator_new(0x24);
    if (this_00 == (CLevelLoader *)0x0) {
      pCVar8 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar34 = *(CWorld **)(param_1 + 8);
      ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)param_1;
      pCVar7 = (CThingManager *)(**(code **)(*(int *)pCVar34 + 0xc))();
      pCVar8 = (CIPhysicsFacesSegmentBase *)
               CLevelLoader::CLevelLoader(this_00,pCVar7,(CWorldMap *)ppVar23,pCVar34);
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(param_1 + 0xbc),pCVar8);
  }
  (**(code **)(*(int *)param_1 + 4))();
  (**(code **)(*(int *)param_1 + 8))();
  pCVar9 = CGameJoystickManager::Get();
  CEngineManager::Cleanup((CEngineManager *)pCVar9);
  CCharString::CCharString(aCStack_1a0,"Load .wld file",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1a0,
             extraout_EDX,unaff_EDI);
  if (DAT_013b8618 == '\0') {
    if (param_3 == -1) {
      (**(code **)(*param_2 + 0x24))();
    }
    (**(code **)(*param_2 + 0x24))();
    FUN_00410f30();
    ppVar33 = ppStack_100;
    ppVar35 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    ppVar31 = ppStack_100;
    (**(code **)(*param_2 + 0xc))();
    iVar18 = (**(code **)(*param_2 + 0x24))();
    ppVar33[iVar18] = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>)0x0;
    CCharString::CCharString(aCStack_78);
    ppStack_1e8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x507e21;
    CCharString::CCharString((CCharString *)&stack0xfffffe3c,"",-1);
    ppStack_1e8 = ppVar33;
    uStack_1ec = 0x507e2d;
    FUN_00414e30();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffe3c,
               extraout_EDX_00,ppVar31);
    ppStack_1b4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
    iStack_194 = -1;
    pCStack_19c = (CBasicString<char> *)0xffffffff;
    uStack_134 = 0;
    CCharString::CCharString(aCStack_1a0);
    CCharString::CCharString(aCStack_190);
    CCharString::CCharString((CCharString *)&puStack_1a4);
    *(int *)(param_1 + 0xac) = 1;
    uStack_1ad = 0;
    iVar18 = (**(code **)(**(int **)(param_1 + 8) + 0xc))();
    *(undefined4 *)(iVar18 + 0x68) = 1;
    *(undefined4 *)(iVar18 + 0x6c) = 0;
    bVar6 = CStringParser::NextItemExists(aCStack_184);
    ppVar23 = extraout_EDX_01;
    if (bVar6) {
      do {
        CStringParser::ReadAsString(aCStack_184);
        this = pCStack_1bc;
        if (pCStack_1bc == (CBasicString<char> *)0x0) {
          iVar18 = 0xc;
          bVar6 = true;
          pcVar21 = "MapUIDCount";
          pcVar13 = "";
          do {
            if (iVar18 == 0) break;
            iVar18 = iVar18 + -1;
            bVar6 = *pcVar21 == *pcVar13;
            pcVar21 = pcVar21 + 1;
            pcVar13 = pcVar13 + 1;
          } while (bVar6);
          if (bVar6) goto LAB_005081d6;
          iVar18 = 0x15;
          bVar6 = true;
          pcVar21 = "ThingManagerUIDCount";
          pcVar13 = "";
          do {
            if (iVar18 == 0) break;
            iVar18 = iVar18 + -1;
            bVar6 = *pcVar21 == *pcVar13;
            pcVar21 = pcVar21 + 1;
            pcVar13 = pcVar13 + 1;
          } while (bVar6);
          if (bVar6) {
LAB_005081fe:
            uVar26 = FUN_009ba5b0();
            iVar18 = (**(code **)(**(int **)(pCStack_1b8 + 8) + 0xc))();
            *(undefined8 *)(iVar18 + 0x68) = uVar26;
          }
          else {
            iVar18 = 0x10;
            bVar6 = true;
            pcVar21 = "LevelScriptName";
            pcVar13 = "";
            do {
              if (iVar18 == 0) break;
              iVar18 = iVar18 + -1;
              bVar6 = *pcVar21 == *pcVar13;
              pcVar21 = pcVar21 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar6);
            if (bVar6) {
LAB_00508236:
              puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
              if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                iVar18 = 2;
                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                bVar6 = true;
                pcVar21 = "\"";
                pcVar13 = "";
                do {
                  if (iVar18 == 0) break;
                  iVar18 = iVar18 + -1;
                  bVar6 = *pcVar21 == *pcVar13;
                  pcVar21 = pcVar21 + 1;
                  pcVar13 = pcVar13 + 1;
                } while (bVar6);
              }
              else {
                bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                ppVar23 = extraout_EDX_02;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_a0,ppVar23,ppVar35);
              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
              }
              CCharString::CCharString(aCStack_10c,"\"",-1);
              pCVar14 = (CCharString *)
                        CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_f8);
              CCharString::operator=((CCharString *)&pCStack_19c,pCVar14);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_f8
                         ,extraout_EDX_03,ppVar35);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_10c,extraout_EDX_04,ppVar35);
              uVar26 = CStringParser::ReadAsString(aCStack_184);
              ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar26 >> 0x20);
              if ((CBasicString<char> *)*(undefined4 *)uVar26 == (CBasicString<char> *)0x0) {
                bVar6 = true;
                iVar18 = 2;
                pcVar21 = "\"";
                pcVar13 = "";
                do {
                  if (iVar18 == 0) break;
                  iVar18 = iVar18 + -1;
                  bVar6 = *pcVar21 == *pcVar13;
                  pcVar21 = pcVar21 + 1;
                  pcVar13 = pcVar13 + 1;
                } while (bVar6);
              }
              else {
                bVar6 = CBasicString<char>::operator==
                                  ((CBasicString<char> *)*(undefined4 *)uVar26,"\"");
                ppVar23 = extraout_EDX_05;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_98,ppVar23,ppVar35);
              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
              }
            }
            else {
              iVar18 = 7;
              bVar6 = true;
              pcVar21 = "NewMap";
              pcVar13 = "";
              do {
                if (iVar18 == 0) break;
                iVar18 = iVar18 + -1;
                bVar6 = *pcVar21 == *pcVar13;
                pcVar21 = pcVar21 + 1;
                pcVar13 = pcVar13 + 1;
              } while (bVar6);
              if (bVar6) {
LAB_0050834f:
                CCharString::CCharString((CCharString *)&uStack_134,"Loading maps",-1);
                NProgressDisplay::DisplayProgress
                          ((CCharString *)&uStack_134,(float)(extraout_EDX_06 & 0xffffff00),false,
                           false);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &uStack_134,extraout_EDX_07,ppVar35);
                iStack_1ac = CStringParser::ReadAsInteger(aCStack_184);
              }
              else {
                iVar18 = 7;
                bVar6 = true;
                pcVar21 = "EndMap";
                pcVar13 = "";
                do {
                  if (iVar18 == 0) break;
                  iVar18 = iVar18 + -1;
                  bVar6 = *pcVar21 == *pcVar13;
                  pcVar21 = pcVar21 + 1;
                  pcVar13 = pcVar13 + 1;
                } while (bVar6);
                if (bVar6) {
LAB_005083a9:
                  iVar18 = iStack_1ac;
                  if (iStack_1ac != 0) {
                    this_02 = (CThingCreatureBase *)(pCStack_1b8 + 0x20);
                    if ((CCharString *)
                        ((*(int *)(pCStack_1b8 + 0x24) - *(int *)(pCStack_1b8 + 0x20)) / 0x48) <
                        (CCharString *)(iStack_1ac + 1)) {
                      CThingCreatureBase::GetAnimsWithNameStartingWith
                                (this_02,(CCharString *)(iStack_1ac + 1),
                                 (vector<CAnimationEntry_const*,std::allocator<CAnimationEntry_const*>_>
                                  *)ppVar35);
                    }
                    pCVar1 = (C2DBoxI *)(*(int *)this_02 + iVar18 * 0x48);
                    CCharString::operator=((CCharString *)(pCVar1 + 0x18),aCStack_188);
                    if (pCStack_19c == (CBasicString<char> *)0x0) {
                      iVar20 = 1;
                      bVar6 = true;
                      pcVar21 = "";
                      pcVar13 = "";
                      do {
                        if (iVar20 == 0) break;
                        iVar20 = iVar20 + -1;
                        bVar6 = *pcVar21 == *pcVar13;
                        pcVar21 = pcVar21 + 1;
                        pcVar13 = pcVar13 + 1;
                      } while (bVar6);
                      if (bVar6) goto LAB_0050850b;
LAB_0050841e:
                      pCVar14 = (CCharString *)&pCStack_19c;
                    }
                    else {
                      bVar6 = CBasicString<char>::operator==(pCStack_19c,"");
                      if (!bVar6) goto LAB_0050841e;
LAB_0050850b:
                      pCVar14 = aCStack_198;
                    }
                    CCharString::operator=((CCharString *)(pCVar1 + 0x1c),pCVar14);
                    pCVar3 = pCStack_1b8;
                    pCVar1[0x28] = SUB41((uint)unaff_EBX >> 0x18,0);
                    pCVar1[0x24] = (C2DBoxI)0x1;
                    pCVar1[0x40] = CStack_1a5;
                    puVar15 = (undefined4 *)CWorldMap::GetLevelDimensions(pCStack_1b8,aCStack_6c);
                    iVar20 = iStack_12c;
                    *(undefined4 *)pCVar1 = *puVar15;
                    *(undefined4 *)(pCVar1 + 4) = puVar15[1];
                    *(undefined4 *)(pCVar1 + 8) = puVar15[2];
                    *(undefined4 *)(pCVar1 + 0xc) = puVar15[3];
                    *(int *)(pCVar1 + 8) = *(int *)(pCVar1 + 8) + (lStack_18c - *(int *)pCVar1);
                    *(int *)(pCVar1 + 0xc) =
                         *(int *)(pCVar1 + 0xc) + (iStack_194 - *(int *)(pCVar1 + 4));
                    *(long *)pCVar1 = lStack_18c;
                    *(int *)(pCVar1 + 4) = iStack_194;
                    pCVar1[0x25] = (C2DBoxI)0x1;
                    *(int *)(pCVar1 + 0x20) = iStack_12c;
                    bVar6 = CWorldMap::IsMapPlacementPositionFree(pCVar3,pCVar1);
                    if (bVar6) {
                      if (iVar20 == 0) {
                        uVar12 = CWorldMap::AllocateMapUID(pCVar3,iVar18);
                        *(ulong *)(pCVar1 + 0x20) = uVar12;
                      }
                      CWorldMap::SetMapPlacement(pCVar3,pCVar1,iVar18);
                      pCVar9 = CGameJoystickManager::Get();
                      CEngineManager::AddMap((CEngineManager *)pCVar9,iVar18);
                      CCharString::operator=(aCStack_188,"");
                      iStack_194 = -1;
                      lStack_18c = -1;
                      iStack_1ac = 0;
                    }
                    else {
                      pCVar1[0x24] = (C2DBoxI)0x0;
                    }
                  }
                }
                else {
                  iVar18 = 7;
                  bVar6 = true;
                  pcVar21 = "MapUID";
                  pcVar13 = "";
                  do {
                    if (iVar18 == 0) break;
                    iVar18 = iVar18 + -1;
                    bVar6 = *pcVar21 == *pcVar13;
                    pcVar21 = pcVar21 + 1;
                    pcVar13 = pcVar13 + 1;
                  } while (bVar6);
                  if (bVar6) {
LAB_0050852e:
                    iStack_12c = CStringParser::ReadAsInteger(aCStack_184);
                  }
                  else {
                    iVar18 = 5;
                    bVar6 = true;
                    pcVar21 = "MapX";
                    pcVar13 = "";
                    do {
                      if (iVar18 == 0) break;
                      iVar18 = iVar18 + -1;
                      bVar6 = *pcVar21 == *pcVar13;
                      pcVar21 = pcVar21 + 1;
                      pcVar13 = pcVar13 + 1;
                    } while (bVar6);
                    if (bVar6) {
LAB_00508553:
                      lStack_18c = CStringParser::ReadAsInteger(aCStack_184);
                    }
                    else {
                      iVar18 = 5;
                      bVar6 = true;
                      pcVar21 = "MapY";
                      pcVar13 = "";
                      do {
                        if (iVar18 == 0) break;
                        iVar18 = iVar18 + -1;
                        bVar6 = *pcVar21 == *pcVar13;
                        pcVar21 = pcVar21 + 1;
                        pcVar13 = pcVar13 + 1;
                      } while (bVar6);
                      if (bVar6) {
LAB_00508575:
                        iStack_194 = CStringParser::ReadAsInteger(aCStack_184);
                      }
                      else {
                        iVar18 = 6;
                        bVar6 = true;
                        pcVar21 = "IsSea";
                        pcVar13 = "";
                        do {
                          if (iVar18 == 0) break;
                          iVar18 = iVar18 + -1;
                          bVar6 = *pcVar21 == *pcVar13;
                          pcVar21 = pcVar21 + 1;
                          pcVar13 = pcVar13 + 1;
                        } while (bVar6);
                        if (bVar6) {
LAB_00508597:
                          CStringParser::ReadAsString(aCStack_184);
                          CCharString::CCharString((CCharString *)&puStack_124,"TRUE",-1);
                          pcVar21 = "";
                          if (puStack_124 != (undefined4 *)0x0) {
                            pcVar21 = (char *)*puStack_124;
                          }
                          if (puStack_11c == (undefined4 *)0x0) {
                            pcVar13 = "";
                          }
                          else {
                            pcVar13 = (char *)*puStack_11c;
                          }
                          iVar18 = _stricmp(pcVar13,pcVar21);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                      *)&puStack_124,extraout_EDX_08,ppVar35);
                          paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *
                                    )&puStack_11c;
                          ppVar23 = extraout_EDX_09;
                          CStack_1a5 = (C2DBoxI)(iVar18 == 0);
                          goto LAB_00509336;
                        }
                        iVar18 = 0x18;
                        bVar6 = true;
                        pcVar21 = "LoadedOnPlayerProximity";
                        pcVar13 = "";
                        do {
                          if (iVar18 == 0) break;
                          iVar18 = iVar18 + -1;
                          bVar6 = *pcVar21 == *pcVar13;
                          pcVar21 = pcVar21 + 1;
                          pcVar13 = pcVar13 + 1;
                        } while (bVar6);
                        if (bVar6) {
LAB_00508622:
                          CStringParser::ReadAsString(aCStack_184);
                          CCharString::CCharString((CCharString *)&puStack_114,"TRUE",-1);
                          pcVar21 = "";
                          if (puStack_114 != (undefined4 *)0x0) {
                            pcVar21 = (char *)*puStack_114;
                          }
                          if (puStack_150 == (undefined4 *)0x0) {
                            pcVar13 = "";
                          }
                          else {
                            pcVar13 = (char *)*puStack_150;
                          }
                          iVar18 = _stricmp(pcVar13,pcVar21);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                      *)&puStack_114,extraout_EDX_10,ppVar35);
                          unaff_EBX = (uint)(iVar18 == 0) << 0x18;
                          paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *
                                    )&puStack_150;
                          ppVar23 = extraout_EDX_11;
                          goto LAB_00509336;
                        }
                        iVar18 = 10;
                        bVar6 = true;
                        pcVar21 = "LevelName";
                        pcVar13 = "";
                        do {
                          if (iVar18 == 0) break;
                          iVar18 = iVar18 + -1;
                          bVar6 = *pcVar21 == *pcVar13;
                          pcVar21 = pcVar21 + 1;
                          pcVar13 = pcVar13 + 1;
                        } while (bVar6);
                        if (bVar6) {
LAB_005086b1:
                          puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                          if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                            iVar18 = 2;
                            ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                            bVar6 = true;
                            pcVar21 = "\"";
                            pcVar13 = "";
                            do {
                              if (iVar18 == 0) break;
                              iVar18 = iVar18 + -1;
                              bVar6 = *pcVar21 == *pcVar13;
                              pcVar21 = pcVar21 + 1;
                              pcVar13 = pcVar13 + 1;
                            } while (bVar6);
                          }
                          else {
                            bVar6 = CBasicString<char>::operator==
                                              ((CBasicString<char> *)*puVar15,"\"");
                            ppVar23 = extraout_EDX_12;
                          }
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    (aaStack_90,ppVar23,ppVar35);
                          if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                            _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                          }
                          CCharString::CCharString(aCStack_148,"\"",-1);
                          CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_ac);
                          CUserProfileManager::GetAutoSaveFileName(aCStack_ec);
                          FUN_0099b2c0();
                          pCVar14 = (CCharString *)FUN_0099f570();
                          CCharString::operator=(aCStack_188,pCVar14);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    (aaStack_80,extraout_EDX_13,ppVar35);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    (aaStack_88,extraout_EDX_14,ppVar35);
                          CCharString::~CCharString((CCharString *)aCStack_ec);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                      *)aCStack_ac,extraout_EDX_15,ppVar35);
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                      *)aCStack_148,extraout_EDX_16,ppVar35);
                          CAFile::SplitPath(aCStack_188,(CCharString *)0x0,(CCharString *)0x0,
                                            aCStack_198,(CCharString *)0x0);
                          puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                          if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                            iVar18 = 2;
                            ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                            bVar6 = true;
                            pcVar21 = "\"";
                            pcVar13 = "";
                            do {
                              if (iVar18 == 0) break;
                              iVar18 = iVar18 + -1;
                              bVar6 = *pcVar21 == *pcVar13;
                              pcVar21 = pcVar21 + 1;
                              pcVar13 = pcVar13 + 1;
                            } while (bVar6);
                          }
                          else {
                            bVar6 = CBasicString<char>::operator==
                                              ((CBasicString<char> *)*puVar15,"\"");
                            ppVar23 = extraout_EDX_17;
                          }
                          std::
                          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                    (aaStack_e4,ppVar23,ppVar35);
                          if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                            _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                          }
                        }
                        else {
                          iVar18 = 10;
                          bVar6 = true;
                          pcVar21 = "NewRegion";
                          pcVar13 = "";
                          do {
                            if (iVar18 == 0) break;
                            iVar18 = iVar18 + -1;
                            bVar6 = *pcVar21 == *pcVar13;
                            pcVar21 = pcVar21 + 1;
                            pcVar13 = pcVar13 + 1;
                          } while (bVar6);
                          if (bVar6) {
LAB_0050882c:
                            CEngineInit::CEngineInit(aCStack_5c);
                            FUN_0051d200();
                            CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge
                                      ((CAIStateGroup_SummonerCharge *)aCStack_5c);
                            CStringParser::ReadAsInteger(aCStack_184);
                          }
                          else {
                            iVar18 = 10;
                            bVar6 = true;
                            pcVar21 = "EndRegion";
                            pcVar13 = "";
                            do {
                              if (iVar18 == 0) break;
                              iVar18 = iVar18 + -1;
                              bVar6 = *pcVar21 == *pcVar13;
                              pcVar21 = pcVar21 + 1;
                              pcVar13 = pcVar13 + 1;
                            } while (bVar6);
                            if (!bVar6) {
                              iVar18 = 10;
                              bVar6 = true;
                              pcVar21 = "RegionDef";
                              pcVar13 = "";
                              do {
                                if (iVar18 == 0) break;
                                iVar18 = iVar18 + -1;
                                bVar6 = *pcVar21 == *pcVar13;
                                pcVar21 = pcVar21 + 1;
                                pcVar13 = pcVar13 + 1;
                              } while (bVar6);
                              if (bVar6) {
LAB_00508887:
                                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                                  iVar18 = 2;
                                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                                  bVar6 = true;
                                  pcVar21 = "\"";
                                  pcVar13 = "";
                                  do {
                                    if (iVar18 == 0) break;
                                    iVar18 = iVar18 + -1;
                                    bVar6 = *pcVar21 == *pcVar13;
                                    pcVar21 = pcVar21 + 1;
                                    pcVar13 = pcVar13 + 1;
                                  } while (bVar6);
                                }
                                else {
                                  bVar6 = CBasicString<char>::operator==
                                                    ((CBasicString<char> *)*puVar15,"\"");
                                  ppVar23 = extraout_EDX_18;
                                }
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          (aaStack_8c,ppVar23,ppVar35);
                                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                                }
                                CCharString::CCharString((CCharString *)&pvStack_108,"\"",-1);
                                CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_140);
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                            *)&pvStack_108,extraout_EDX_19,ppVar35);
                                uVar26 = CStringParser::ReadAsString(aCStack_184);
                                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                          ((ulonglong)uVar26 >> 0x20);
                                if ((CBasicString<char> *)*(undefined4 *)uVar26 ==
                                    (CBasicString<char> *)0x0) {
                                  bVar6 = true;
                                  iVar18 = 2;
                                  pcVar21 = "\"";
                                  pcVar13 = "";
                                  do {
                                    if (iVar18 == 0) break;
                                    iVar18 = iVar18 + -1;
                                    bVar6 = *pcVar21 == *pcVar13;
                                    pcVar21 = pcVar21 + 1;
                                    pcVar13 = pcVar13 + 1;
                                  } while (bVar6);
                                }
                                else {
                                  bVar6 = CBasicString<char>::operator==
                                                    ((CBasicString<char> *)*(undefined4 *)uVar26,
                                                     "\"");
                                  ppVar23 = extraout_EDX_20;
                                }
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          (aaStack_dc,ppVar23,ppVar35);
                                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                                }
                                CRegion::SetRegionDefName
                                          ((CRegion *)(*(int *)(pCStack_1b8 + 0x30) + -0x58),
                                           aCStack_140);
                                paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                           *)aCStack_140;
                                ppVar23 = extraout_EDX_21;
                                goto LAB_00509336;
                              }
                              iVar18 = 0xf;
                              bVar6 = true;
                              pcVar21 = "EnvironmentDef";
                              pcVar13 = "";
                              do {
                                if (iVar18 == 0) break;
                                iVar18 = iVar18 + -1;
                                bVar6 = *pcVar21 == *pcVar13;
                                pcVar21 = pcVar21 + 1;
                                pcVar13 = pcVar13 + 1;
                              } while (bVar6);
                              if (!bVar6) {
                                iVar18 = 0xc;
                                bVar6 = true;
                                pcVar21 = "DisplayName";
                                pcVar13 = "";
                                do {
                                  if (iVar18 == 0) break;
                                  iVar18 = iVar18 + -1;
                                  bVar6 = *pcVar21 == *pcVar13;
                                  pcVar21 = pcVar21 + 1;
                                  pcVar13 = pcVar13 + 1;
                                } while (bVar6);
                                if (!bVar6) {
                                  iVar18 = 0xb;
                                  bVar6 = true;
                                  pcVar21 = "RegionName";
                                  pcVar13 = "";
                                  do {
                                    if (iVar18 == 0) break;
                                    iVar18 = iVar18 + -1;
                                    bVar6 = *pcVar21 == *pcVar13;
                                    pcVar21 = pcVar21 + 1;
                                    pcVar13 = pcVar13 + 1;
                                  } while (bVar6);
                                  if (!bVar6) {
                                    iVar18 = 0xf;
                                    bVar6 = true;
                                    pcVar21 = "NewDisplayName";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00508b03;
                                    iVar18 = 0xc;
                                    bVar6 = true;
                                    pcVar21 = "ContainsMap";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00508c16;
                                    iVar18 = 8;
                                    bVar6 = true;
                                    pcVar21 = "SeesMap";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00508df7;
                                    iVar18 = 0x11;
                                    bVar6 = true;
                                    pcVar21 = "AppearOnWorldMap";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00508fe5;
                                    iVar18 = 0xf;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapGraphic";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509005;
                                    iVar18 = 0xd;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapScale";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509049;
                                    iVar18 = 0xf;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapOffsetX";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509071;
                                    iVar18 = 0xf;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapOffsetY";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509099;
                                    iVar18 = 0x1d;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapRegionExitTextOffsetX";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_005090c5;
                                    iVar18 = 0x1d;
                                    bVar6 = true;
                                    pcVar21 = "MiniMapRegionExitTextOffsetY";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509159;
                                    iVar18 = 0x10;
                                    bVar6 = true;
                                    pcVar21 = "WorldMapOffsetX";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_005091e9;
                                    iVar18 = 0x10;
                                    bVar6 = true;
                                    pcVar21 = "WorldMapOffsetY";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509211;
                                    iVar18 = 0x13;
                                    bVar6 = true;
                                    pcVar21 = "NameGraphicOffsetX";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509239;
                                    iVar18 = 0x13;
                                    bVar6 = true;
                                    pcVar21 = "NameGraphicOffsetY";
                                    pcVar13 = "";
                                    do {
                                      if (iVar18 == 0) break;
                                      iVar18 = iVar18 + -1;
                                      bVar6 = *pcVar21 == *pcVar13;
                                      pcVar21 = pcVar21 + 1;
                                      pcVar13 = pcVar13 + 1;
                                    } while (bVar6);
                                    if (bVar6) goto LAB_00509265;
                                    goto LAB_0050933b;
                                  }
                                }
LAB_00508aa9:
                                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                                  iVar18 = 2;
                                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                                  bVar6 = true;
                                  pcVar21 = "\"";
                                  pcVar13 = "";
                                  do {
                                    if (iVar18 == 0) break;
                                    iVar18 = iVar18 + -1;
                                    bVar6 = *pcVar21 == *pcVar13;
                                    pcVar21 = pcVar21 + 1;
                                    pcVar13 = pcVar13 + 1;
                                  } while (bVar6);
                                }
                                else {
                                  bVar6 = CBasicString<char>::operator==
                                                    ((CBasicString<char> *)*puVar15,"\"");
                                  ppVar23 = extraout_EDX_49;
                                }
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          (aaStack_b0,ppVar23,ppVar35);
                                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                                }
                                CCharString::CCharString(aCStack_15c,"\"",-1);
                                CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_190);
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                            *)aCStack_15c,extraout_EDX_50,ppVar35);
                                uVar26 = CStringParser::ReadAsString(aCStack_184);
                                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                          ((ulonglong)uVar26 >> 0x20);
                                if ((CBasicString<char> *)*(undefined4 *)uVar26 ==
                                    (CBasicString<char> *)0x0) {
                                  bVar6 = true;
                                  iVar18 = 2;
                                  pcVar21 = "\"";
                                  pcVar13 = "";
                                  do {
                                    if (iVar18 == 0) break;
                                    iVar18 = iVar18 + -1;
                                    bVar6 = *pcVar21 == *pcVar13;
                                    pcVar21 = pcVar21 + 1;
                                    pcVar13 = pcVar13 + 1;
                                  } while (bVar6);
                                }
                                else {
                                  bVar6 = CBasicString<char>::operator==
                                                    ((CBasicString<char> *)*(undefined4 *)uVar26,
                                                     "\"");
                                  ppVar23 = extraout_EDX_51;
                                }
                                std::
                                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                          (aaStack_a8,ppVar23,ppVar35);
                                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                                }
                                CCharString::operator=
                                          ((CCharString *)(*(int *)(pCStack_1b8 + 0x30) + -0x40),
                                           aCStack_190);
                                paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                           *)aCStack_190;
                                ppVar23 = extraout_EDX_52;
                                goto LAB_00509336;
                              }
LAB_0050899a:
                              puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                              if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                                iVar18 = 2;
                                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                                bVar6 = true;
                                pcVar21 = "\"";
                                pcVar13 = "";
                                do {
                                  if (iVar18 == 0) break;
                                  iVar18 = iVar18 + -1;
                                  bVar6 = *pcVar21 == *pcVar13;
                                  pcVar21 = pcVar21 + 1;
                                  pcVar13 = pcVar13 + 1;
                                } while (bVar6);
                              }
                              else {
                                bVar6 = CBasicString<char>::operator==
                                                  ((CBasicString<char> *)*puVar15,"\"");
                                ppVar23 = extraout_EDX_22;
                              }
                              std::
                              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                        (aaStack_a4,ppVar23,ppVar35);
                              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                              }
                              CCharString::CCharString(aCStack_120,"\"",-1);
                              CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_d4);
                              std::
                              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                          *)aCStack_d4,extraout_EDX_23,ppVar35);
                              std::
                              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                          *)aCStack_120,extraout_EDX_24,ppVar35);
                              uVar26 = CStringParser::ReadAsString(aCStack_184);
                              ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                        ((ulonglong)uVar26 >> 0x20);
                              if ((CBasicString<char> *)*(undefined4 *)uVar26 ==
                                  (CBasicString<char> *)0x0) {
                                bVar6 = true;
                                iVar18 = 2;
                                pcVar21 = "\"";
                                pcVar13 = "";
                                do {
                                  if (iVar18 == 0) break;
                                  iVar18 = iVar18 + -1;
                                  bVar6 = *pcVar21 == *pcVar13;
                                  pcVar21 = pcVar21 + 1;
                                  pcVar13 = pcVar13 + 1;
                                } while (bVar6);
                              }
                              else {
                                bVar6 = CBasicString<char>::operator==
                                                  ((CBasicString<char> *)*(undefined4 *)uVar26,"\"")
                                ;
                                ppVar23 = extraout_EDX_25;
                              }
                              std::
                              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                        (aaStack_84,ppVar23,ppVar35);
                              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else {
          bVar6 = CBasicString<char>::operator==(pCStack_1bc,"MapUIDCount");
          if (!bVar6) {
            bVar6 = CBasicString<char>::operator==(this,"ThingManagerUIDCount");
            if (bVar6) goto LAB_005081fe;
            bVar6 = CBasicString<char>::operator==(this,"LevelScriptName");
            if (bVar6) goto LAB_00508236;
            bVar6 = CBasicString<char>::operator==(this,"NewMap");
            if (bVar6) goto LAB_0050834f;
            bVar6 = CBasicString<char>::operator==(this,"EndMap");
            if (bVar6) goto LAB_005083a9;
            bVar6 = CBasicString<char>::operator==(this,"MapUID");
            if (bVar6) goto LAB_0050852e;
            bVar6 = CBasicString<char>::operator==(this,"MapX");
            if (bVar6) goto LAB_00508553;
            bVar6 = CBasicString<char>::operator==(this,"MapY");
            if (bVar6) goto LAB_00508575;
            bVar6 = CBasicString<char>::operator==(this,"IsSea");
            if (bVar6) goto LAB_00508597;
            bVar6 = CBasicString<char>::operator==(this,"LoadedOnPlayerProximity");
            if (bVar6) goto LAB_00508622;
            bVar6 = CBasicString<char>::operator==(this,"LevelName");
            if (bVar6) goto LAB_005086b1;
            bVar6 = CBasicString<char>::operator==(this,"NewRegion");
            if (bVar6) goto LAB_0050882c;
            bVar6 = CBasicString<char>::operator==(this,"EndRegion");
            if (bVar6) goto LAB_0050933b;
            bVar6 = CBasicString<char>::operator==(this,"RegionDef");
            if (bVar6) goto LAB_00508887;
            bVar6 = CBasicString<char>::operator==(this,"EnvironmentDef");
            if (bVar6) goto LAB_0050899a;
            bVar6 = CBasicString<char>::operator==(this,"DisplayName");
            if ((bVar6) || (bVar6 = CBasicString<char>::operator==(this,"RegionName"), bVar6))
            goto LAB_00508aa9;
            bVar6 = CBasicString<char>::operator==(this,"NewDisplayName");
            if (bVar6) {
LAB_00508b03:
              puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
              if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                iVar18 = 2;
                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                bVar6 = true;
                pcVar21 = "\"";
                pcVar13 = "";
                do {
                  if (iVar18 == 0) break;
                  iVar18 = iVar18 + -1;
                  bVar6 = *pcVar21 == *pcVar13;
                  pcVar21 = pcVar21 + 1;
                  pcVar13 = pcVar13 + 1;
                } while (bVar6);
              }
              else {
                bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                ppVar23 = extraout_EDX_26;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_cc,ppVar23,ppVar35);
              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
              }
              CCharString::CCharString(aCStack_138,"\"",-1);
              CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_110);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_138,extraout_EDX_27,ppVar35);
              uVar26 = CStringParser::ReadAsString(aCStack_184);
              ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar26 >> 0x20);
              if ((CBasicString<char> *)*(undefined4 *)uVar26 == (CBasicString<char> *)0x0) {
                bVar6 = true;
                iVar18 = 2;
                pcVar21 = "\"";
                pcVar13 = "";
                do {
                  if (iVar18 == 0) break;
                  iVar18 = iVar18 + -1;
                  bVar6 = *pcVar21 == *pcVar13;
                  pcVar21 = pcVar21 + 1;
                  pcVar13 = pcVar13 + 1;
                } while (bVar6);
              }
              else {
                bVar6 = CBasicString<char>::operator==
                                  ((CBasicString<char> *)*(undefined4 *)uVar26,"\"");
                ppVar23 = extraout_EDX_28;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_9c,ppVar23,ppVar35);
              if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
              }
              CCharString::operator=
                        ((CCharString *)(*(int *)(pCStack_1b8 + 0x30) + -0x3c),aCStack_110);
              paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_110
              ;
              ppVar23 = extraout_EDX_29;
            }
            else {
              bVar6 = CBasicString<char>::operator==(this,"ContainsMap");
              if (bVar6) {
LAB_00508c16:
                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                  iVar18 = 2;
                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                  bVar6 = true;
                  pcVar21 = "\"";
                  pcVar13 = "";
                  do {
                    if (iVar18 == 0) break;
                    iVar18 = iVar18 + -1;
                    bVar6 = *pcVar21 == *pcVar13;
                    pcVar21 = pcVar21 + 1;
                    pcVar13 = pcVar13 + 1;
                  } while (bVar6);
                }
                else {
                  bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                  ppVar23 = extraout_EDX_30;
                }
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_c4,ppVar23,ppVar35);
                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                }
                CCharString::CCharString(aCStack_130,"\"",-1);
                CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_94);
                CUserProfileManager::GetAutoSaveFileName(aCStack_bc);
                FUN_0099b2c0();
                FUN_0099f570();
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_7c,extraout_EDX_31,ppVar35);
                CCharString::~CCharString((CCharString *)aCStack_bc);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_94,extraout_EDX_32,ppVar35);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_130,extraout_EDX_33,ppVar35);
                CAFile::SplitPath((CCharString *)&puStack_1a4,(CCharString *)0x0,(CCharString *)0x0,
                                  aCStack_198,(CCharString *)0x0);
                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                  iVar18 = 2;
                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                  bVar6 = true;
                  pcVar21 = "\"";
                  pcVar13 = "";
                  do {
                    if (iVar18 == 0) break;
                    iVar18 = iVar18 + -1;
                    bVar6 = *pcVar21 == *pcVar13;
                    pcVar21 = pcVar21 + 1;
                    pcVar13 = pcVar13 + 1;
                  } while (bVar6);
                }
                else {
                  bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                  ppVar23 = extraout_EDX_34;
                }
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_b4,ppVar23,ppVar35);
                puVar15 = puStack_1a4;
                pCVar3 = pCStack_1b8;
                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                }
                uVar24 = 1;
                if (1 < (uint)((*(int *)(pCStack_1b8 + 0x24) - *(int *)(pCStack_1b8 + 0x20)) / 0x48)
                   ) {
                  iVar18 = 0x48;
                  do {
                    puVar2 = *(undefined4 **)(*(int *)(pCVar3 + 0x20) + 0x18 + iVar18);
                    if ((puVar2 == puVar15) ||
                       ((((puVar2 != (undefined4 *)0x0 && (puVar15 != (undefined4 *)0x0)) &&
                         (puVar2[1] == puVar15[1])) &&
                        (lVar16 = CBasicString<char>::Compare((char *)*puVar2,(char *)*puVar15),
                        lVar16 == 0)))) break;
                    uVar24 = uVar24 + 1;
                    iVar18 = iVar18 + 0x48;
                  } while (uVar24 < (uint)((*(int *)(pCVar3 + 0x24) - *(int *)(pCVar3 + 0x20)) /
                                          0x48));
                }
                CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera
                          ((CTCCameraPointScriptedSpline *)(*(int *)(pCVar3 + 0x30) + -0x58));
                paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                          &puStack_1a4;
                ppVar23 = extraout_EDX_35;
              }
              else {
                bVar6 = CBasicString<char>::operator==(this,"SeesMap");
                if (!bVar6) {
                  bVar6 = CBasicString<char>::operator==(this,"AppearOnWorldMap");
                  if (bVar6) {
LAB_00508fe5:
                    *(undefined1 *)(*(int *)(pCStack_1b8 + 0x30) + -4) = 1;
                  }
                  else {
                    bVar6 = CBasicString<char>::operator==(this,"MiniMapGraphic");
                    if (bVar6) {
LAB_00509005:
                      CStringParser::ReadAsString(aCStack_184);
                      CCharString::operator=
                                ((CCharString *)(*(int *)(pCStack_1b8 + 0x30) + -0x30),aCStack_14c);
                      paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                                aCStack_14c;
                      ppVar23 = extraout_EDX_42;
                      goto LAB_00509336;
                    }
                    bVar6 = CBasicString<char>::operator==(this,"MiniMapScale");
                    if (bVar6) {
LAB_00509049:
                      fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                      *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0x2c) = fVar27;
                    }
                    else {
                      bVar6 = CBasicString<char>::operator==(this,"MiniMapOffsetX");
                      if (bVar6) {
LAB_00509071:
                        fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                        *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0x28) = fVar27;
                      }
                      else {
                        bVar6 = CBasicString<char>::operator==(this,"MiniMapOffsetY");
                        if (!bVar6) {
                          bVar6 = CBasicString<char>::operator==
                                            (this,"MiniMapRegionExitTextOffsetX");
                          if (bVar6) {
LAB_005090c5:
                            CStringParser::ReadAsString(aCStack_184);
                            std::
                            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                      (aaStack_d0,extraout_EDX_43,ppVar35);
                            CStringParser::ReadAsString(aCStack_184);
                            CStringParser::ReadAsString(aCStack_184);
                            std::
                            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                      (aaStack_c8,extraout_EDX_44,ppVar35);
                            ppStack_1b4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                          CStringParser::ReadAsFloat(aCStack_184);
                            CRegion::SetMiniMapRegionExitTextOffsetX
                                      ((CRegion *)(*(int *)(pCStack_1b8 + 0x30) + -0x58),aCStack_144
                                       ,(float)ppStack_1b4);
                            paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                       *)aCStack_144;
                            ppVar23 = extraout_EDX_45;
                          }
                          else {
                            bVar6 = CBasicString<char>::operator==
                                              (this,"MiniMapRegionExitTextOffsetY");
                            if (!bVar6) {
                              bVar6 = CBasicString<char>::operator==(this,"WorldMapOffsetX");
                              if (bVar6) {
LAB_005091e9:
                                fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                                *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0x14) = fVar27;
                              }
                              else {
                                bVar6 = CBasicString<char>::operator==(this,"WorldMapOffsetY");
                                if (bVar6) {
LAB_00509211:
                                  fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                                  *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0x10) = fVar27;
                                }
                                else {
                                  bVar6 = CBasicString<char>::operator==(this,"NameGraphicOffsetX");
                                  if (bVar6) {
LAB_00509239:
                                    fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                                    *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0xc) = fVar27;
                                  }
                                  else {
                                    bVar6 = CBasicString<char>::operator==
                                                      (this,"NameGraphicOffsetY");
                                    if (bVar6) {
LAB_00509265:
                                      fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                                      *(float *)(*(int *)(pCStack_1b8 + 0x30) + -8) = fVar27;
                                    }
                                  }
                                }
                              }
                              goto LAB_0050933b;
                            }
LAB_00509159:
                            CStringParser::ReadAsString(aCStack_184);
                            std::
                            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                      (aaStack_c0,extraout_EDX_46,ppVar35);
                            CStringParser::ReadAsString(aCStack_184);
                            CStringParser::ReadAsString(aCStack_184);
                            std::
                            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                                      (aaStack_b8,extraout_EDX_47,ppVar35);
                            ppStack_1b4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                                          CStringParser::ReadAsFloat(aCStack_184);
                            CRegion::SetMiniMapRegionExitTextOffsetY
                                      ((CRegion *)(*(int *)(pCStack_1b8 + 0x30) + -0x58),aCStack_13c
                                       ,(float)ppStack_1b4);
                            paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>
                                       *)aCStack_13c;
                            ppVar23 = extraout_EDX_48;
                          }
                          goto LAB_00509336;
                        }
LAB_00509099:
                        fVar27 = CStringParser::ReadAsFloat(aCStack_184);
                        *(float *)(*(int *)(pCStack_1b8 + 0x30) + -0x24) = fVar27;
                      }
                    }
                  }
                  goto LAB_0050933b;
                }
LAB_00508df7:
                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                  iVar18 = 2;
                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                  bVar6 = true;
                  pcVar21 = "\"";
                  pcVar13 = "";
                  do {
                    if (iVar18 == 0) break;
                    iVar18 = iVar18 + -1;
                    bVar6 = *pcVar21 == *pcVar13;
                    pcVar21 = pcVar21 + 1;
                    pcVar13 = pcVar13 + 1;
                  } while (bVar6);
                }
                else {
                  bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                  ppVar23 = extraout_EDX_36;
                }
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_f4,ppVar23,ppVar35);
                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                }
                CCharString::CCharString(aCStack_118,"\"",-1);
                CStringParser::ReadAsStringUntilString(aCStack_184,aCStack_e0);
                CUserProfileManager::GetAutoSaveFileName(aCStack_e8);
                FUN_0099b2c0();
                FUN_0099f570();
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_f0,extraout_EDX_37,ppVar35);
                CCharString::~CCharString((CCharString *)aCStack_e8);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_e0,extraout_EDX_38,ppVar35);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_118,extraout_EDX_39,ppVar35);
                puVar15 = (undefined4 *)CStringParser::ReadAsString(aCStack_184);
                if ((CBasicString<char> *)*puVar15 == (CBasicString<char> *)0x0) {
                  iVar18 = 2;
                  ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
                  bVar6 = true;
                  pcVar21 = "\"";
                  pcVar13 = "";
                  do {
                    if (iVar18 == 0) break;
                    iVar18 = iVar18 + -1;
                    bVar6 = *pcVar21 == *pcVar13;
                    pcVar21 = pcVar21 + 1;
                    pcVar13 = pcVar13 + 1;
                  } while (bVar6);
                }
                else {
                  bVar6 = CBasicString<char>::operator==((CBasicString<char> *)*puVar15,"\"");
                  ppVar23 = extraout_EDX_40;
                }
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (aaStack_d8,ppVar23,ppVar35);
                puVar15 = puStack_128;
                pCVar3 = pCStack_1b8;
                if (bVar6 == false) {
                    /* WARNING: Subroutine does not return */
                  _CxxThrowException(&stack0xfffffe43,(ThrowInfo *)&DAT_013692f8);
                }
                iVar18 = *(int *)(pCStack_1b8 + 0x24) - *(int *)(pCStack_1b8 + 0x20) >> 0x1f;
                ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                          ((*(int *)(pCStack_1b8 + 0x24) - *(int *)(pCStack_1b8 + 0x20)) / 0x48 +
                          iVar18);
                ppVar25 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x1;
                ppStack_1b4 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x1;
                ppVar4 = ppStack_1b4;
                if (1 < (uint)((int)ppVar23 - iVar18)) {
                  iVar18 = 0x48;
                  do {
                    puVar2 = *(undefined4 **)(*(int *)(pCVar3 + 0x20) + 0x18 + iVar18);
                    if ((puVar2 == puVar15) ||
                       (((puVar2 != (undefined4 *)0x0 && (puVar15 != (undefined4 *)0x0)) &&
                        ((puVar2[1] == puVar15[1] &&
                         (lVar16 = CBasicString<char>::Compare((char *)*puVar2,(char *)*puVar15),
                         lVar16 == 0)))))) {
                      ppStack_1b4 = ppVar25;
                      FUN_00485ff3();
                      paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                                &puStack_128;
                      ppVar23 = extraout_EDX_41;
                      goto LAB_00509336;
                    }
                    iVar20 = *(int *)(pCVar3 + 0x24) - *(int *)(pCVar3 + 0x20) >> 0x1f;
                    ppVar23 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                              ((*(int *)(pCVar3 + 0x24) - *(int *)(pCVar3 + 0x20)) / 0x48 + iVar20);
                    ppVar25 = ppVar25 + 1;
                    iVar18 = iVar18 + 0x48;
                    ppVar4 = ppVar25;
                  } while (ppVar25 < ppVar23 + -iVar20);
                }
                ppStack_1b4 = ppVar4;
                paVar22 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                          &puStack_128;
              }
            }
LAB_00509336:
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (paVar22,ppVar23,ppVar35);
            goto LAB_0050933b;
          }
LAB_005081d6:
          lVar16 = CStringParser::ReadAsInteger(aCStack_184);
          *(long *)(pCStack_1b8 + 0xac) = lVar16;
        }
LAB_0050933b:
        CStringParser::ReadAsString(aCStack_184);
        cVar5 = FUN_0099e960();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1a0,
                   extraout_EDX_53,ppVar35);
        ppVar23 = extraout_EDX_54;
        if (cVar5 != '\0') {
          CStringParser::ReadLineAsString(aCStack_184);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1b0,
                     extraout_EDX_55,ppVar35);
          ppVar23 = extraout_EDX_56;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1bc,
                   ppVar23,ppVar35);
        bVar6 = CStringParser::NextItemExists(aCStack_184);
        ppVar23 = extraout_EDX_57;
        param_1 = pCStack_1b8;
        param_2 = unaff_retaddr;
        ppVar33 = ppStack_104;
      } while (bVar6);
    }
    if (DAT_01375459 != '\0') {
      CCharString::CCharString((CCharString *)&pCStack_1b8);
      CCharString::CCharString((CCharString *)&iStack_1ac);
      CCharString::CCharString((CCharString *)&ppStack_1b4);
      pCVar10 = (CCharString *)0x0;
      pCVar14 = (CCharString *)&ppStack_1b4;
      pCVar30 = (CCharString *)&pCStack_1b8;
      pCVar17 = (CCharString *)(**(code **)(*param_2 + 0x2c))();
      CAFile::SplitPath(pCVar17,(CCharString *)&iStack_1ac,pCVar30,pCVar14,pCVar10);
      CCharString::~CCharString(aCStack_1b0);
      FUN_0099be70();
      FUN_0099be70();
      FUN_0099bf30();
      CCharString::~CCharString(aCStack_1b0);
      CCharString::~CCharString(aCStack_1a0);
      CWorldMap::SaveWorldToBinaryFile(param_1,(CWideString *)aCStack_190);
      FUN_0099be70();
      FUN_0099be70();
      FUN_0099bf30();
      CCharString::~CCharString(aCStack_1b0);
      CCharString::~CCharString(aCStack_1a0);
      CWorldMap::CompileLoadableGlobalThingsToSingleFile(param_1,(CWideString *)&pCStack_1bc);
      CCharString::~CCharString((CCharString *)&pCStack_1bc);
      CCharString::~CCharString(aCStack_190);
      CCharString::~CCharString((CCharString *)&ppStack_1b4);
      CCharString::~CCharString((CCharString *)&iStack_1ac);
      CCharString::~CCharString((CCharString *)&pCStack_1b8);
      ppVar23 = extraout_EDX_58;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_19c,ppVar23
               ,ppVar35);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_188,
               extraout_EDX_59,ppVar35);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_198,
               extraout_EDX_60,ppVar35);
    piVar19 = (int *)*piStack_170;
    ppVar23 = extraout_EDX_61;
    if (piVar19 != piStack_170) {
      do {
        piVar28 = (int *)*piVar19;
        free(piVar19);
        ppVar23 = extraout_EDX_62;
        piVar19 = piVar28;
      } while (piVar28 != piStack_170);
    }
    *piStack_170 = (int)piStack_170;
    piStack_170[1] = (int)piStack_170;
    if (piStack_170 != (int *)0x0) {
      free(piStack_170);
      ppVar23 = extraout_EDX_63;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_174,ppVar23,ppVar35);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_178,extraout_EDX_64,ppVar35);
    NHeroInformationScreens::CBase::CBase((CBase *)aCStack_184);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_70,extraout_EDX_65,ppVar35);
    if (ppVar33 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
      free(ppVar33);
    }
  }
  else {
    CCharString::CCharString(aCStack_1a8);
    CCharString::CCharString(aCStack_1b0);
    CCharString::CCharString(aCStack_198);
    CCharString::CCharString((CCharString *)&iStack_194);
    ppVar35 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    pCVar14 = (CCharString *)&iStack_194;
    pCVar30 = aCStack_1b0;
    pCVar17 = (CCharString *)&pCStack_1b8;
    pCVar10 = (CCharString *)(**(code **)(*param_2 + 0x2c))();
    CAFile::SplitPath(pCVar10,(CCharString *)&pCStack_19c,pCVar17,pCVar30,pCVar14);
    CCharString::~CCharString((CCharString *)&pCStack_1bc);
    FUN_0099be70();
    FUN_0099be70();
    pCVar11 = (CWideString *)FUN_0099bf30();
    CWideString::operator=((CWideString *)&iStack_1ac,pCVar11);
    CCharString::~CCharString((CCharString *)&pCStack_1bc);
    CCharString::~CCharString(aCStack_190);
    CCharString::~CCharString(aCStack_15c);
    FUN_00507650();
    CCharString::~CCharString(aCStack_198);
    CCharString::~CCharString((CCharString *)&pCStack_19c);
    CCharString::~CCharString((CCharString *)&ppStack_1b4);
    CCharString::~CCharString((CCharString *)&iStack_1ac);
  }
  CCharString::CCharString((CCharString *)&pCStack_1bc,"Init thing maps",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1bc,
             extraout_EDX_66,ppVar35);
  NThingMap::CThingMapManager::Init
            (*(CThingMapManager **)(param_1 + 0x10),*(CWorld **)(param_1 + 8),
             (*(int *)(param_1 + 0x24) - *(int *)(param_1 + 0x20)) / 0x48);
  CCharString::CCharString((CCharString *)&pCStack_1bc,"Load GTNG",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_1bc,
             extraout_EDX_67,ppVar35);
  CCharString::CCharString(aCStack_158);
  ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  pCVar14 = aCStack_158;
  pCVar10 = (CCharString *)0x0;
  pCVar30 = aCStack_1b0;
  pCVar17 = (CCharString *)(**(code **)(*param_2 + 0x2c))();
  CAFile::SplitPath(pCVar17,(CCharString *)0x0,pCVar30,pCVar10,pCVar14);
  CCharString::~CCharString((CCharString *)&ppStack_1b4);
  CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&puStack_1a4);
  FUN_0099be70();
  FUN_0099bf30();
  CCharString::~CCharString((CCharString *)&ppStack_1b4);
  CCharString::~CCharString((CCharString *)&puStack_1a4);
  bVar6 = CAFile::PathExists((CWideString *)aCStack_158);
  if (!bVar6) goto LAB_00509857;
  this_01 = operator_new(0x1c);
  if (this_01 == (CDiskFileWin32 *)0x0) {
    iVar18 = 0;
LAB_005097c6:
    piVar19 = (int *)0x0;
  }
  else {
    iVar18 = CDiskFileWin32::CDiskFileWin32(this_01,(CWideString *)aCStack_158,1,2);
    if ((iVar18 == 0) || (piVar19 = operator_new(0xc), piVar19 == (int *)0x0)) goto LAB_005097c6;
    *piVar19 = 1;
    piVar19[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
    piVar19[2] = iVar18;
  }
  pvVar32 = (vector<CThing*,std::allocator<CThing*>_> *)0x0;
  pvStack_108 = (void *)0x0;
  ppStack_104 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  ppStack_100 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  puVar15 = &uStack_1ec;
  (**(code **)(*(int *)param_1 + 0x68))(&uStack_1ec);
  uVar29 = 0;
  if (piVar19 != (int *)0x0) {
    *piVar19 = *piVar19 + 1;
  }
  piVar28 = piVar19;
  pCVar7 = (CThingManager *)(**(code **)(**(int **)(param_1 + 8) + 0xc))(iVar18,piVar19,0);
  CThingManager::LoadFromFile(pCVar7,iVar18,piVar28,uVar29,puVar15);
  pCVar7 = (CThingManager *)(**(code **)(**(int **)(param_1 + 8) + 0xc))();
  CThingManager::ActivateThings(pCVar7,pvVar32);
  if (pvStack_108 != (void *)0x0) {
    free(pvStack_108);
  }
  if ((piVar19 != (int *)0x0) && (*piVar19 = *piVar19 + -1, *piVar19 == 0)) {
    (*(code *)piVar19[1])();
    operator_delete(piVar19);
  }
LAB_00509857:
  CCharString::CCharString((CCharString *)&stack0xfffffe40,"Load global things",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffe40,
             extraout_EDX_68,ppVar33);
  if (DAT_013b8609 == '\0') {
    CWorldMap::LoadLoadableGlobalThings(param_1);
  }
  else {
    CCharString::CCharString(aCStack_190);
    CCharString::CCharString(aCStack_198);
    CCharString::CCharString(aCStack_1a8);
    pCVar10 = (CCharString *)0x0;
    pCVar14 = aCStack_1a8;
    pCVar30 = aCStack_190;
    ppStack_1e8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x5098bc;
    pCVar17 = (CCharString *)(**(code **)(*param_2 + 0x2c))();
    CAFile::SplitPath(pCVar17,aCStack_198,pCVar30,pCVar14,pCVar10);
    CCharString::~CCharString((CCharString *)&ppStack_1b4);
    FUN_0099be70();
    FUN_0099be70();
    FUN_0099bf30();
    CCharString::~CCharString((CCharString *)&ppStack_1b4);
    CCharString::~CCharString((CCharString *)&puStack_1a4);
    CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(param_1,(CWideString *)&stack0xfffffe40);
    CCharString::~CCharString((CCharString *)&stack0xfffffe40);
    CCharString::~CCharString(aCStack_1a8);
    CCharString::~CCharString(aCStack_198);
    CCharString::~CCharString(aCStack_190);
  }
  CCharString::CCharString((CCharString *)&stack0xfffffe40,"Load global things end",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&stack0xfffffe40,(float)(extraout_EDX_69 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffe40,
             extraout_EDX_70,ppVar33);
  CArray<std::pair<unsigned_long,CDefClassBase::CSubDefInfo>_>::DeallocateUnusedMemory
            ((CArray<std::pair<unsigned_long,CDefClassBase::CSubDefInfo>_> *)(param_1 + 0x20));
  CCharString::CCharString((CCharString *)&stack0xfffffe40,"Load region graph",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffe40,
             extraout_EDX_71,ppVar33);
  CCharString::CCharString((CCharString *)&stack0xfffffe40,"Load region graph",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&stack0xfffffe40,(float)(extraout_EDX_72 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffe40,
             extraout_EDX_73,ppVar33);
  CPlayerGui::PeekGuiDef();
  pCVar11 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
  CWorldMap::LoadRegionGraph(param_1,pCVar11);
  CCharString::~CCharString((CCharString *)&ppStack_1b4);
  _E2();
  CCharString::~CCharString(aCStack_158);
  CCharString::~CCharString(aCStack_15c);
  return;
}

 
==================== LoadFromFile @ 00821a80 ==================== 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=0.716674083551181 <- ego_r]
   public: void __thiscall CMap::LoadFromFile(class CAFile &,class CASuspendableProcess *) */

void __thiscall CMap::LoadFromFile(CMap *this,CAFile *param_1,CASuspendableProcess *param_2)

{
  int *piVar1;
  float *pfVar2;
  CDefinitionManager *this_00;
  byte bVar3;
  CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *pCVar4;
  int iVar5;
  long lVar6;
  long *_Memory;
  int *piVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  uint uVar11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar12;
  int iVar13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *_Str1;
  uint uVar14;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  long *plVar15;
  undefined4 *puVar16;
  undefined1 uStack_8d;
  uint uStack_8c;
  long local_88;
  int *local_84;
  int *local_80;
  CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *local_7c;
  CDefinitionManager *local_78;
  void *pvStack_74;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppStack_70;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppStack_6c;
  uint uStack_68;
  undefined **local_64 [3];
  void *pvStack_58;
  CCharString aCStack_4c [4];
  undefined4 uStack_48;
  int *piStack_44;
  uint local_3c;
  int local_38;
  undefined1 uStack_34;
  long local_33;
  CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6> aCStack_1c [4];
  undefined1 uStack_18;
  undefined4 uStack_17;
  CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> CStack_13;
  byte bStack_12;
  byte bStack_11;
  byte bStack_10;
  undefined1 uStack_f;
  undefined1 uStack_e;
  char cStack_d;
  char cStack_c;
  char cStack_b;
  char cStack_9;
  char cStack_8;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  local_78 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  pCVar4 = ::operator_new(0x10822);
  if (pCVar4 == (CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *)0x0) {
    pCVar4 = (CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *)0x0;
  }
  else {
    CEngineLandscapeMeshBuilder::CLayer::SetAllPolys((CLayer *)(pCVar4 + 0x16));
    CEngineLandscapeMeshBuilder::CLayer::SetAllPolys((CLayer *)(pCVar4 + 0x841e));
  }
  local_7c = pCVar4;
  C3DMeshObject::C3DMeshObject((C3DMeshObject *)local_64);
  CCachedFile::OpenFromFile((CCachedFile *)local_64,param_1,param_2);
  CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader>::ReadFromFile
            ((CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader> *)
             &local_38,(CAFile *)local_64,param_2);
  CHeightMap::Clear(*(CHeightMap **)(this + 0x1a7c));
  *(undefined4 *)(this + 0x1d00) = 0;
  *(undefined4 *)(this + 0x1d04) = 0;
  *(undefined4 *)(this + 0x1d08) = 0;
  CCachedFile::_SetPosition_CCachedFile__UAEXJ_Z((CCachedFile *)local_64,local_33);
  GFGetTime();
  CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ReadFromFile
            (pCVar4,(CAFile *)local_64,(CASuspendableProcess *)0x0);
  *(undefined4 *)(this + 0x1d30) = *(undefined4 *)(pCVar4 + 5);
  _Str1 = pCVar4 + 0x16;
  *(undefined4 *)(this + 0x1d34) = *(undefined4 *)(pCVar4 + 9);
  local_3c = *(uint *)(pCVar4 + 0x8416);
  if (local_3c == 0) {
    this[0x1d38] = (CMap)0x1;
  }
  if (DAT_013b8636 != '\0') {
    *(undefined4 *)(this + 0x1d30) = 1;
    *(undefined4 *)(this + 0x1d34) = 0;
  }
  iVar5 = _stricmp((char *)_Str1,"UNINITIALISED");
  if (iVar5 == 0) {
    CCharString::CCharString((CCharString *)&local_84,"ENGINE_THEME",-1);
    lVar6 = LTextSystem::GetIDFromSymbol(*(LTextSystem **)(this + 0x1c),(char *)&local_84);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_84,
               extraout_EDX,unaff_EDI);
    iVar5 = 0;
    do {
      if (iVar5 < lVar6) {
        *(int *)(*(int *)(this + 0x1a6c) + iVar5 * 8) = iVar5;
        *(undefined1 *)(*(int *)(this + 0x1a6c) + 4 + iVar5 * 8) = 1;
      }
      else {
        *(undefined4 *)(*(int *)(this + 0x1a6c) + iVar5 * 8) = 0;
        *(undefined1 *)(*(int *)(this + 0x1a6c) + 4 + iVar5 * 8) = 0;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < 0x100);
  }
  else {
    iVar5 = 0;
    do {
      if (*(int *)(_Str1 + 0x80) != 0) {
        CCharString::CCharString((CCharString *)&local_88,(char *)_Str1,-1);
        lVar6 = CDefinitionManager::GetDefGlobalIndexFromName
                          (*(CDefinitionManager **)(this + 0x1c),(CCharString *)&local_88);
        if (lVar6 == 0) {
LAB_00821ca7:
          *(undefined4 *)(*(int *)(this + 0x1a6c) + iVar5) = 0;
          ppVar12 = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this + 0x1a6c);
          ppVar12[iVar5 + 4] = (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0;
        }
        else {
          local_80 = (int *)0x0;
          CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>
                    (*(CDefinitionManager **)(this + 0x1c),lVar6,
                     (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_80);
          if (local_80 == (int *)0x0) goto LAB_00821ca7;
          *(long *)(*(int *)(this + 0x1a6c) + iVar5) = lVar6;
          local_84 = (int *)0x0;
          CDefinitionManager::GetDef<CThingPhysicalSwitchDef>
                    (*(CDefinitionManager **)(this + 0x1c),lVar6,
                     (CDefPointer<CThingPhysicalSwitchDef_const_> *)&local_84);
          *(undefined1 *)(*(int *)(this + 0x1a6c) + 4 + iVar5) = 1;
          ppVar12 = extraout_EDX_00;
          if (local_84 != (int *)0x0) {
            piVar1 = local_84 + 1;
            *piVar1 = *piVar1 + -1;
            if (*piVar1 == 0) {
              (**(code **)(*local_84 + 4))();
              ppVar12 = extraout_EDX_01;
            }
          }
          piVar1 = local_80 + 1;
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*local_80 + 4))();
            ppVar12 = extraout_EDX_02;
          }
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_88,
                   ppVar12,unaff_EDI);
      }
      iVar5 = iVar5 + 8;
      _Str1 = _Str1 + 0x84;
    } while (iVar5 < 0x800);
  }
  if (param_2 != (CASuspendableProcess *)0x0) {
    GFGetTime();
    GFGetTime();
    (**(code **)(*(int *)param_2 + 8))();
  }
  if (*(int *)(*(int *)(this + 0x1a6c) + 8) == 0) {
    *(undefined4 *)(*(int *)(this + 0x1a6c) + 8) =
         *(undefined4 *)(*(int *)(*(int *)(this + 0x1c) + 0xd0) + 0x50);
    *(undefined1 *)(*(int *)(this + 0x1a6c) + 0xc) = 1;
    iVar5 = 0x10;
    do {
      if (*(int *)(*(int *)(this + 0x1a6c) + iVar5) == 1) {
        *(undefined4 *)(*(int *)(this + 0x1a6c) + iVar5) = 0;
        *(undefined1 *)(*(int *)(this + 0x1a6c) + 4 + iVar5) = 0;
      }
      if (*(int *)(*(int *)(this + 0x1a6c) + 8 + iVar5) == 1) {
        *(undefined4 *)(*(int *)(this + 0x1a6c) + 8 + iVar5) = 0;
        *(undefined1 *)(*(int *)(this + 0x1a6c) + 0xc + iVar5) = 0;
      }
      iVar5 = iVar5 + 0x10;
    } while (iVar5 < 0x800);
  }
  if (param_2 != (CASuspendableProcess *)0x0) {
    GFGetTime();
    GFGetTime();
    (**(code **)(*(int *)param_2 + 8))();
  }
  pvStack_74 = (void *)0x0;
  ppStack_70 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
  ppStack_6c = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
  uStack_8c = 0;
  FUN_00440990(0,&uStack_8c,&uStack_8d,1,1);
  uStack_8c = 1;
  if (1 < *(uint *)(local_7c + 0x841a)) {
    do {
      iVar5 = (**(code **)((int)local_64[0] + 0x34))();
      if (iVar5 < 2) {
        (**(code **)((int)local_64[0] + 0xc))(&local_88,4,0);
      }
      else {
        uVar14 = (**(code **)((int)local_64[0] + 0x34))();
        uVar14 = ((uVar14 + 3) / uVar14) * uVar14;
        if (uVar14 == 0) {
          _Memory = (long *)0x0;
        }
        else {
          _Memory = malloc(uVar14);
        }
        plVar15 = _Memory;
        for (uVar11 = uVar14 >> 2; uVar11 != 0; uVar11 = uVar11 - 1) {
          *plVar15 = 0;
          plVar15 = plVar15 + 1;
        }
        for (uVar14 = uVar14 & 3; uVar14 != 0; uVar14 = uVar14 - 1) {
          *(undefined1 *)plVar15 = 0;
          plVar15 = (long *)((int)plVar15 + 1);
        }
        (**(code **)((int)local_64[0] + 0xc))(_Memory,4,0);
        local_88 = *_Memory;
        free(_Memory);
      }
      CCharString::CCharString((CCharString *)&local_80);
      CCharString::LoadFromFile
                ((CCharString *)&local_80,(CAFile *)local_64,(CASuspendableProcess *)0x0,local_88);
      this_00 = local_78;
      CDefinitionManager::GetPDefFromInstantiationName(local_78,(CCharString *)&local_84);
      piVar1 = local_84;
      if (local_84 == (int *)0x0) {
        uStack_68 = 0;
        if (ppStack_70 == ppStack_6c) {
          FUN_00440990(ppStack_70,&uStack_68,&uStack_8d,1,1);
          ppVar12 = extraout_EDX_04;
        }
        else {
          if (ppStack_70 != (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0) {
            *(long *)ppStack_70 = 0;
          }
          ppStack_70 = ppStack_70 + 4;
          ppVar12 = extraout_EDX_03;
        }
      }
      else {
        piVar7 = local_84 + 1;
        *piVar7 = local_84[1] + 1;
        iVar5 = local_84[1];
        local_84[1] = iVar5 + -1;
        if (iVar5 + -1 == 0) {
          (**(code **)(*local_84 + 4))();
        }
        local_84 = (int *)0x0;
        lVar6 = CDefinitionManager::GetDefGlobalIndexFromName(this_00,(CCharString *)&local_80);
        uStack_68 = CDefinitionManager::GetDefClassIndexFromGlobalIndex(this_00,lVar6);
        if (ppStack_70 == ppStack_6c) {
          FUN_00440990(ppStack_70,&uStack_68,&uStack_8d,1,1);
          ppVar12 = extraout_EDX_05;
        }
        else {
          if (ppStack_70 != (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0) {
            *(uint *)ppStack_70 = uStack_68;
          }
          ppVar12 = ppStack_70;
          ppStack_70 = ppStack_70 + 4;
        }
        if ((piVar1 != (int *)0x0) && (*piVar7 = *piVar7 + -1, *piVar7 == 0)) {
          (**(code **)(*piVar1 + 4))();
          ppVar12 = extraout_EDX_06;
        }
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_80,ppVar12,
                 unaff_EDI);
      uStack_8c = uStack_8c + 1;
    } while (uStack_8c < *(uint *)(local_7c + 0x841a));
  }
  uStack_8c = *(uint *)(this + 0x1d14);
  iVar5 = *(int *)(this + 0x1d18);
  if (local_7c[0x15] != (CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>)0x0) {
    uStack_8c = uStack_8c + 1;
    iVar5 = iVar5 + 1;
  }
  local_80 = (int *)(iVar5 * uStack_8c);
  local_84 = DAT_01375718;
  local_88 = 0;
  if (0 < (int)local_80) {
    do {
      lVar6 = local_88;
      CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ReadFromFile
                (aCStack_1c,(CAFile *)local_64,(CASuspendableProcess *)0x0);
      iVar8 = lVar6 / (int)uStack_8c;
      iVar13 = lVar6 % (int)uStack_8c;
      iVar5 = *(int *)(this + 0x1a7c);
      pfVar2 = (float *)(*(int *)(iVar5 + 0x10) +
                        ((*(int *)(iVar5 + 0x208) - iVar8) * (*(int *)(iVar5 + 0x204) + 1) + iVar13)
                        * 0xc);
      *pfVar2 = uStack_17 * _DAT_012724c0;
      bVar3 = *(byte *)((int)pfVar2 + 9) ^ (cStack_d << 1 ^ *(byte *)((int)pfVar2 + 9)) & 2;
      *(byte *)((int)pfVar2 + 9) = bVar3;
      bVar3 = (cStack_c << 3 ^ bVar3) & 8 ^ bVar3;
      *(byte *)((int)pfVar2 + 9) = bVar3;
      if (((bVar3 & 8) == 0) && ((bVar3 & 2) != 0)) {
        *(byte *)((int)pfVar2 + 9) = bVar3 | 8;
      }
      bVar3 = *(byte *)((int)pfVar2 + 9) ^ (cStack_9 << 2 ^ *(byte *)((int)pfVar2 + 9)) & 4;
      *(byte *)((int)pfVar2 + 9) = bVar3;
      *(byte *)((int)pfVar2 + 9) = (cStack_8 << 4 ^ bVar3) & 0x10 ^ bVar3;
      if (local_3c < 3) {
        iVar5 = *(int *)(this + 0x1a78);
        iVar13 = (int)(iVar13 + (iVar13 >> 0x1f & 3U)) >> 2;
        iVar8 = (int)(iVar8 + (iVar8 >> 0x1f & 3U)) >> 2;
        if ((iVar13 < *(int *)(iVar5 + 0x1c)) && (iVar8 < *(int *)(iVar5 + 0x20))) {
          uStack_68 = (uint)cStack_b;
          if (((int)uStack_68 < 0) ||
             (uVar14 = uStack_68, (uint)((int)ppStack_70 - (int)pvStack_74 >> 2) < uStack_68)) {
            uVar14 = 0;
          }
          *(undefined1 *)
           ((((*(int *)(iVar5 + 0x20) - iVar8) + -1) * *(int *)(iVar5 + 0x1c) + iVar13) * 7 + 5 +
           *(int *)(iVar5 + 4)) = *(undefined1 *)((int)pvStack_74 + uVar14 * 4);
          lVar6 = local_88;
        }
      }
      *(undefined1 *)((int)pfVar2 + 7) = uStack_f;
      *(undefined1 *)(pfVar2 + 2) = uStack_e;
      bVar3 = bStack_12;
      if ((bStack_12 != 0) && (*(int *)(*(int *)(this + 0x1a6c) + (uint)bStack_12 * 8) == 0)) {
        bVar3 = 1;
      }
      *(byte *)(pfVar2 + 1) = bVar3;
      bVar3 = bStack_11;
      if ((bStack_11 != 0) && (*(int *)(*(int *)(this + 0x1a6c) + (uint)bStack_11 * 8) == 0)) {
        bVar3 = 1;
      }
      *(byte *)((int)pfVar2 + 5) = bVar3;
      bVar3 = bStack_10;
      if ((bStack_10 != 0) && (*(int *)(*(int *)(this + 0x1a6c) + (uint)bStack_10 * 8) == 0)) {
        bVar3 = 1;
      }
      *(byte *)((int)pfVar2 + 6) = bVar3;
      if ((param_2 != (CASuspendableProcess *)0x0) && (lVar6 % (int)local_84 == 0)) {
        GFGetTime();
        (**(code **)(*(int *)param_2 + 8))();
        GFGetTime();
      }
      local_88 = lVar6 + 1;
    } while (local_88 < (int)local_80);
  }
  if (1 < local_3c) {
    pCVar4 = local_7c + 0x849e;
    iVar5 = 0x100;
    do {
      if (*(int *)pCVar4 != 0) {
        CCharString::CCharString((CCharString *)&uStack_8c,(char *)(pCVar4 + -0x80),-1);
        lVar6 = CDefinitionManager::GetDefGlobalIndexFromName(local_78,(CCharString *)&uStack_8c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_8c,
                   extraout_EDX_07,unaff_EDI);
        if (lVar6 == 0) {
          *(int *)pCVar4 = 1;
        }
        else {
          lVar6 = CDefinitionManager::GetDefClassIndexFromGlobalIndex(local_78,lVar6);
          *(long *)pCVar4 = lVar6;
        }
      }
      pCVar4 = pCVar4 + 0x84;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    uStack_18 = 1;
    if (*(int *)(this + 0x1d20) * *(int *)(this + 0x1d1c) != 0) {
      local_78 = (CDefinitionManager *)0x0;
      local_88 = *(int *)(this + 0x1d20) * *(int *)(this + 0x1d1c);
      do {
        iVar5 = (**(code **)((int)local_64[0] + 0x34))();
        if (iVar5 < 2) {
          iVar5 = 4;
          (**(code **)((int)local_64[0] + 0xc))(&uStack_8c,4,0);
        }
        else {
          uVar14 = (**(code **)((int)local_64[0] + 0x34))();
          uVar14 = ((uVar14 + 3) / uVar14) * uVar14;
          if (uVar14 == 0) {
            puVar10 = (undefined4 *)0x0;
          }
          else {
            puVar10 = malloc(uVar14);
          }
          puVar16 = puVar10;
          for (uVar11 = uVar14 >> 2; uVar11 != 0; uVar11 = uVar11 - 1) {
            *puVar16 = 0;
            puVar16 = puVar16 + 1;
          }
          for (uVar14 = uVar14 & 3; uVar14 != 0; uVar14 = uVar14 - 1) {
            *(undefined1 *)puVar16 = 0;
            puVar16 = (undefined4 *)((int)puVar16 + 1);
          }
          iVar5 = 4;
          (**(code **)((int)local_64[0] + 0xc))(puVar10,4,0);
          free(puVar10);
        }
        uVar9 = (**(code **)(ppStack_70 + 0x1c))();
        iVar8 = (**(code **)(ppStack_70 + 0x34))();
        if (iVar8 < 2) {
          (**(code **)(ppStack_70 + 0xc))(&stack0xffffff67,1,0);
        }
        else {
          uVar14 = (**(code **)(ppStack_70 + 0x34))();
          uVar14 = (uVar14 / uVar14) * uVar14;
          if (uVar14 == 0) {
            puVar10 = (undefined4 *)0x0;
          }
          else {
            puVar10 = malloc(uVar14);
          }
          puVar16 = puVar10;
          for (uVar11 = uVar14 >> 2; uVar11 != 0; uVar11 = uVar11 - 1) {
            *puVar16 = 0;
            puVar16 = puVar16 + 1;
          }
          for (uVar14 = uVar14 & 3; uVar14 != 0; uVar14 = uVar14 - 1) {
            *(undefined1 *)puVar16 = 0;
            puVar16 = (undefined4 *)((int)puVar16 + 1);
          }
          (**(code **)(ppStack_70 + 0xc))(puVar10,1,0);
          iVar5 = CONCAT13(*(undefined1 *)puVar10,(int3)iVar5);
          free(puVar10);
        }
        (**(code **)(local_7c + 0x14))(uVar9);
        local_38 = iVar5;
        (**(code **)((int)local_80 + 0xc))(&uStack_34,iVar5 + -4,0);
        pCVar4 = (CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *)
                 (local_78 + *(int *)(*(int *)(this + 0x1a78) + 4));
        pCVar4[3] = uStack_17._3_1_;
        pCVar4[4] = CStack_13;
        *pCVar4 = local_7c[((uint)uStack_17 & 0xff) * 0x84 + 0x849e];
        pCVar4[1] = local_7c[((uint)uStack_17 >> 8 & 0xff) * 0x84 + 0x849e];
        pCVar4[2] = local_7c[((uint)uStack_17 >> 0x10 & 0xff) * 0x84 + 0x849e];
        if (2 < local_3c) {
          uVar14 = (uint)(char)bStack_12;
          if (((int)uVar14 < 0) || ((uint)((int)ppStack_70 - (int)pvStack_74 >> 2) < uVar14)) {
            uVar14 = 0;
          }
          pCVar4[5] = *(CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8> *)
                       ((int)pvStack_74 + uVar14 * 4);
        }
        local_78 = local_78 + 7;
        local_88 = local_88 + -1;
      } while (local_88 != 0);
    }
  }
  GFGetTime();
  if (pvStack_74 != (void *)0x0) {
    free(pvStack_74);
  }
  local_64[0] = &PTR__vector_deleting_destructor__012724d4;
  NScript::CQ_GuildTrainingScript::CMeleeApprentice::_Close_CCachedFile__UAEXXZ
            ((CMeleeApprentice *)local_64);
  if ((piStack_44 != (int *)0x0) && (*piStack_44 = *piStack_44 + -1, *piStack_44 == 0)) {
    (*(code *)piStack_44[1])();
    operator_delete(piStack_44);
  }
  uStack_48 = 0;
  piStack_44 = (int *)0x0;
  CCharString::~CCharString(aCStack_4c);
  if (pvStack_58 != (void *)0x0) {
    free(pvStack_58);
  }
  NHeroInformationScreens::CBase::CBase((CBase *)local_64);
  operator_delete(local_7c);
  return;
}

 
==================== LoadFromFile @ 00521ae0 ==================== 

/* [bsim sim=0.7585740799243835 <- ego_r]
   public: void __thiscall CThingManager::LoadFromFile(class CCountedPointer<class
   CAFile>,long,class std::set<class CCharString,struct std::less<class CCharString>,class
   std::allocator<class CCharString> >,class std::vector<class CThing *,class std::allocator<class
   CThing *> > &,class CASuspendableProcess *,int) */

void __thiscall
CThingManager::LoadFromFile
          (CThingManager *this,CAFile *param_2,int *param_3,long param_4,void *param_5,int param_6,
          undefined4 param_7,vector<CThing*,std::allocator<CThing*>_> *param_8,
          CASuspendableProcess *param_9,int param_10)

{
  CASuspendableProcess *pCVar1;
  bool bVar2;
  CCharString *pCVar3;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  uint extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBP;
  long lVar4;
  long local_30;
  CCharString local_2c [4];
  CStringParser local_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_18 [4];
  CGuiControlTreePane local_14 [20];
  
  this[0x9b] = (CThingManager)0x1;
  this[0x9a] = (CThingManager)0x0;
  local_30 = param_10;
  if (param_10 == 3) {
    if (*(int *)(this + 0x80) == 1) {
      local_30 = 1;
    }
    else if (*(int *)(this + 0x80) == 2) {
      local_30 = 0;
    }
  }
  CCharString::CCharString(local_2c,"Thing Manager: Load From File",-1);
  NProgressDisplay::DisplayProgress(local_2c,(float)(extraout_EDX & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_2c,
             extraout_EDX_00,unaff_EBP);
  CCharString::CCharString((CCharString *)&param_10);
  pCVar1 = param_9;
  CCharString::LoadFromFile((CCharString *)&param_10,param_2,param_9,-1);
  lVar4 = 0;
  pCVar3 = (CCharString *)CCharString::CCharString((CCharString *)&param_9,"",-1);
  CStringParser::CStringParser
            (local_28,(CCharString *)&param_10,(_func_void_CCharString_ptr *)0x0,pCVar3,lVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_9,
             extraout_EDX_01,unaff_EBP);
  CCharString::CCharString((CCharString *)&param_9,"Thing Manager: Load From File End",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_9,(float)(extraout_EDX_02 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_9,
             extraout_EDX_03,unaff_EBP);
  lVar4 = 0;
  CCharString::CCharString((CCharString *)&param_9,"Version",-1);
  bVar2 = CStringParser::SkipPastNextItemIfIs(local_28,(CCharString *)&param_9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_9,
             extraout_EDX_04,unaff_EBP);
  if (bVar2) {
    CStringParser::ReadAsFloat(local_28);
    lVar4 = __ftol2();
    CCharString::CCharString((CCharString *)&param_9,";",-1);
    CStringParser::SkipPastNextItemIfIs(local_28,(CCharString *)&param_9);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_9,
               extraout_EDX_05,unaff_EBP);
  }
  LoadEntitiesFromScript
            (this,(CCharString *)&param_10,lVar4,param_4,
             (set<CCharString,std::less<CCharString>,std::allocator<CCharString>_> *)&param_5,pCVar1
             ,local_30,param_8);
  this[0x9b] = (CThingManager)0x0;
  CGuiControlTreePane::SortTreeRecursively
            (local_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EBP);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_18,extraout_EDX_06,unaff_EBP);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_1c,extraout_EDX_07,unaff_EBP);
  NHeroInformationScreens::CBase::CBase((CBase *)local_28);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_10,
             extraout_EDX_08,unaff_EBP);
  if ((param_3 != (int *)0x0) && (*param_3 = *param_3 + -1, *param_3 == 0)) {
    (*(code *)param_3[1])();
    operator_delete(param_3);
  }
  if (param_6 != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              ((LTextTreeWalkThrough *)&param_5,*(LTextBinNode **)((int)param_5 + 4));
    *(void **)((int)param_5 + 8) = param_5;
    *(undefined4 *)((int)param_5 + 4) = 0;
    *(void **)((int)param_5 + 0xc) = param_5;
    param_6 = 0;
  }
  if (param_5 != (void *)0x0) {
    free(param_5);
  }
  return;
}

 
==================== SaveBinary @ 004fb3e0 ==================== 

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CMapInfo::SaveBinary(class CDataOutputStream &) */

void __thiscall CMapInfo::SaveBinary(CMapInfo *this,CDataOutputStream *param_1)

{
  CDataOutputStream::WritePresizedString(param_1,(CCharString *)(this + 0x18));
  CDataOutputStream::WritePresizedString(param_1,(CCharString *)(this + 0x1c));
  CDataOutputStream::WriteEBOOL(param_1,(bool)this[0x24]);
  CDataOutputStream::WriteEBOOL(param_1,(bool)this[0x28]);
  CDataOutputStream::WriteEBOOL(param_1,(bool)this[0x40]);
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)this);
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 8));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 4));
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0xc));
  CDataOutputStream::WriteEBOOL(param_1,(bool)this[0x25]);
  CDataOutputStream::WriteCBYTE(param_1,(char)*(undefined4 *)(this + 0x20));
  return;
}

 
==================== SaveMapsToString @ 004fb990 ==================== 

/* [bsim sim=0.8741076216179348 <- ego_r]
   protected: void __thiscall CWorldMap::SaveMapsToString(class CCharString &) */

void __thiscall CWorldMap::SaveMapsToString(CWorldMap *this,CCharString *param_1)

{
  CCharString *this_00;
  undefined4 uVar1;
  CCharString *pCVar2;
  int iVar3;
  char *pcVar4;
  char *_Str;
  long lVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
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
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBP;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar6;
  undefined *puVar7;
  CCharString aCStack_68 [4];
  CCharString aCStack_64 [4];
  CCharString aCStack_60 [4];
  int local_5c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_58 [4];
  uint local_54;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_50 [4];
  CUserProfileManager aCStack_4c [4];
  CUserProfileManager aCStack_48 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_44 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_40 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_3c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_38 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_34 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_30 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_2c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_28 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_24 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_20 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_14 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_10 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_4 [4];
  
  puVar7 = &DAT_012353b8;
  uVar1 = GFIntToCharString_API(&DAT_012353b8);
  CCharString_OperatorPlus_API(uVar1);
  pCVar2 = (CCharString *)FUN_0099f600(puVar7);
  this_00 = param_1;
  CCharString::operator+=(param_1,pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_54,extraout_EDX
             ,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_58,extraout_EDX_00,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_5c,
             extraout_EDX_01,unaff_EDI);
  iVar3 = (**(code **)(**(int **)(this + 8) + 0xc))();
  puVar7 = &DAT_012353b8;
  uVar1 = FUN_0099f930(*(undefined4 *)(iVar3 + 0x68),*(undefined4 *)(iVar3 + 0x6c));
  CCharString_OperatorPlus_API(uVar1);
  pCVar2 = (CCharString *)FUN_0099f600(puVar7);
  CCharString::operator+=(this_00,pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_02,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_54,
             extraout_EDX_03,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_58,extraout_EDX_04,unaff_EDI);
  local_54 = 1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    local_5c = 0x48;
    do {
      iVar3 = *(int *)(this + 0x20) + local_5c;
      if (*(char *)(*(int *)(this + 0x20) + 0x24 + local_5c) != '\0') {
        CCharString::CCharString(aCStack_60);
        CCharString::CCharString(aCStack_64);
        CCharString::CCharString(aCStack_68);
        CAFile::SplitPath((CCharString *)(iVar3 + 0x18),(CCharString *)0x0,aCStack_68,aCStack_60,
                          aCStack_64);
        paVar6 = aaStack_50;
        CUserProfileManager::GetAutoSaveFileName(aCStack_4c);
        pCVar2 = (CCharString *)FUN_0099b2c0(paVar6);
        pcVar4 = CCharString::operator_char_const_(pCVar2);
        _Str = CCharString::operator_char_const_(aCStack_68);
        pcVar4 = strstr(_Str,pcVar4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_50,extraout_EDX_05,unaff_EBP);
        CCharString::~CCharString((CCharString *)aCStack_4c);
        CCharString::CCharString((CCharString *)&param_1);
        if (pcVar4 != (char *)0x0) {
          pCVar2 = (CCharString *)CUserProfileManager::GetAutoSaveFileName(aCStack_48);
          lVar5 = CCharString::GetLength(pCVar2);
          CCharString::operator=((CCharString *)&param_1,pcVar4 + lVar5);
          CCharString::~CCharString((CCharString *)aCStack_48);
        }
        pCVar2 = aCStack_64;
        FUN_0099f570(aCStack_60);
        FUN_0099f570(pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_44,extraout_EDX_06,unaff_EBP);
        puVar7 = &DAT_012353b8;
        uVar1 = GFIntToCharString_API(&DAT_012353b8);
        CCharString_OperatorPlus_API(uVar1);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_40,extraout_EDX_07,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_3c,extraout_EDX_08,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_38,extraout_EDX_09,unaff_EBP);
        puVar7 = &DAT_012353b8;
        uVar1 = GFIntToCharString_API(&DAT_012353b8);
        CCharString_OperatorPlus_API(uVar1);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_34,extraout_EDX_10,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_30,extraout_EDX_11,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_2c,extraout_EDX_12,unaff_EBP);
        puVar7 = &DAT_012353b8;
        uVar1 = GFIntToCharString_API(&DAT_012353b8);
        CCharString_OperatorPlus_API(uVar1);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_28,extraout_EDX_13,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_24,extraout_EDX_14,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_20,extraout_EDX_15,unaff_EBP);
        puVar7 = &DAT_0123990c;
        CCharString_OperatorPlus_API(local_58);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_1c,extraout_EDX_16,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_18,extraout_EDX_17,unaff_EBP);
        puVar7 = &DAT_0123990c;
        CCharString_OperatorPlus_API(iVar3 + 0x1c);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_14,extraout_EDX_18,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_10,extraout_EDX_19,unaff_EBP);
        puVar7 = &DAT_012353b8;
        uVar1 = GFIntToCharString_API(&DAT_012353b8);
        CCharString_OperatorPlus_API(uVar1);
        pCVar2 = (CCharString *)FUN_0099f600(puVar7);
        CCharString::operator+=(this_00,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_c,extraout_EDX_20,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_8,extraout_EDX_21,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_4,extraout_EDX_22,unaff_EBP);
        if (*(char *)(iVar3 + 0x40) == '\0') {
          pcVar4 = "IsSea FALSE;\r\n";
        }
        else {
          pcVar4 = "IsSea TRUE;\r\n";
        }
        CCharString::operator+=(this_00,pcVar4);
        if (*(char *)(iVar3 + 0x28) == '\0') {
          pcVar4 = "LoadedOnPlayerProximity FALSE;\r\n";
        }
        else {
          pcVar4 = "LoadedOnPlayerProximity TRUE;\r\n";
        }
        CCharString::operator+=(this_00,pcVar4);
        CCharString::operator+=(this_00,"EndMap;\r\n\r\n");
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_58,extraout_EDX_23,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                   extraout_EDX_24,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_68,
                   extraout_EDX_25,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_64,
                   extraout_EDX_26,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_60,
                   extraout_EDX_27,unaff_EBP);
      }
      local_54 = local_54 + 1;
      local_5c = local_5c + 0x48;
    } while (local_54 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  }
  return;
}

 
==================== SaveRegionsToString @ 004fd040 ==================== 

/* [bsim sim=0.8299264038154608 <- ego_r]
   protected: void __thiscall CWorldMap::SaveRegionsToString(class CCharString &) */

void __thiscall CWorldMap::SaveRegionsToString(CWorldMap *this,CCharString *param_1)

{
  int iVar1;
  char cVar2;
  CCharString *pCVar3;
  float *pfVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  long lVar8;
  char *_Str;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
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
  int *piVar9;
  CRegion *this_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined *puVar10;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar11;
  char *local_138;
  CCharString local_130 [4];
  CCharString local_12c [4];
  CCharString local_128 [4];
  uint local_124;
  CCharString local_120 [4];
  int local_11c;
  CCharString local_118 [4];
  undefined4 *local_114;
  CCharString local_110 [4];
  CCharString local_10c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_108 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_104 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_100 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_fc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_f8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_f4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_f0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_ec [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_e8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_e4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_e0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_dc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_d8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_d4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_d0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_cc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_bc [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_b8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_b4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_b0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_ac [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_a8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_a4 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_a0 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_9c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_98 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_94 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_90 [4];
  CUserProfileManager local_8c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_88 [4];
  CUserProfileManager local_84 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_80 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_7c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_78 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_74 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_70 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_6c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_68 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_64 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_60 [4];
  CUserProfileManager local_5c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_58 [4];
  CUserProfileManager local_54 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_50 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_4c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_48 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_44 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_40 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_3c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_38 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_34 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_30 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_2c [40];
  
  local_124 = 1;
  if (1 < (uint)((*(int *)(this + 0x30) - *(int *)(this + 0x2c)) / 0x58)) {
    local_11c = 0x58;
    do {
      puVar10 = &DAT_012353b8;
      this_00 = (CRegion *)(*(int *)(this + 0x2c) + local_11c);
      GFIntToCharString_API(&DAT_012353b8);
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_a8,extraout_EDX,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_f8,extraout_EDX_00,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_88,extraout_EDX_01,unaff_EDI);
      puVar10 = &DAT_0123990c;
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_f0,extraout_EDX_02,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_50,extraout_EDX_03,unaff_EDI);
      puVar10 = &DAT_0123990c;
      CRegion::GetDisplayName(this_00);
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_e8,extraout_EDX_04,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_80,extraout_EDX_05,unaff_EDI);
      puVar10 = &DAT_0123990c;
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_e0,extraout_EDX_06,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_2c,extraout_EDX_07,unaff_EDI);
      if (this_00[0x54] != (CRegion)0x0) {
        CCharString::operator+=(param_1,"AppearOnWorldMap;\r\n");
      }
      cVar2 = FUN_0099e960(&DAT_0122d70e);
      if (cVar2 != '\0') {
        puVar10 = &DAT_012353b8;
        CCharString_OperatorPlus_API();
        pCVar3 = (CCharString *)FUN_0099f600(puVar10);
        CCharString::operator+=(param_1,pCVar3);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_d8,extraout_EDX_08,unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (local_78,extraout_EDX_09,unaff_EDI);
      }
      puVar10 = &DAT_012353b8;
      FUN_009a08b0((double)*(float *)(this_00 + 0x2c));
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_d0,extraout_EDX_10,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_48,extraout_EDX_11,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_c8,extraout_EDX_12,unaff_EDI);
      puVar10 = &DAT_012353b8;
      FUN_009a08b0((double)*(float *)(this_00 + 0x30));
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_70,extraout_EDX_13,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_c0,extraout_EDX_14,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_30,extraout_EDX_15,unaff_EDI);
      puVar10 = &DAT_012353b8;
      FUN_009a08b0((double)*(float *)(this_00 + 0x34));
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_b8,extraout_EDX_16,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_68,extraout_EDX_17,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_b0,extraout_EDX_18,unaff_EDI);
      pfVar4 = (float *)CRegion::GetNameGraphicOffset(this_00);
      puVar10 = &DAT_012353b8;
      FUN_009a08b0((double)*pfVar4);
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_40,extraout_EDX_19,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_100,extraout_EDX_20,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_60,extraout_EDX_21,unaff_EDI);
      puVar10 = &DAT_012353b8;
      iVar5 = CRegion::GetNameGraphicOffset(this_00);
      FUN_009a08b0((double)*(float *)(iVar5 + 4));
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_a0,extraout_EDX_22,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_34,extraout_EDX_23,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_98,extraout_EDX_24,unaff_EDI);
      pfVar4 = (float *)CRegion::GetNameGraphicOffset(this_00);
      puVar10 = &DAT_012353b8;
      FUN_009a08b0((double)*pfVar4);
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_58,extraout_EDX_25,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_90,extraout_EDX_26,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_38,extraout_EDX_27,unaff_EDI);
      puVar10 = &DAT_012353b8;
      iVar5 = CRegion::GetNameGraphicOffset(this_00);
      FUN_009a08b0((double)*(float *)(iVar5 + 4));
      CCharString_OperatorPlus_API();
      pCVar3 = (CCharString *)FUN_0099f600(puVar10);
      CCharString::operator+=(param_1,pCVar3);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_fc,extraout_EDX_28,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_f4,extraout_EDX_29,unaff_EDI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_ec,extraout_EDX_30,unaff_EDI);
      iVar5 = *(int *)(*(int *)(this_00 + 0x38) + 8);
      if (iVar5 != *(int *)(this_00 + 0x38)) {
        do {
          puVar10 = &DAT_012353b8;
          FUN_009a08b0((double)*(float *)(iVar5 + 0x14));
          CCharString_OperatorPlus_API(iVar5 + 0x10);
          FUN_0099f600();
          FUN_0099f570();
          pCVar3 = (CCharString *)FUN_0099f600(puVar10);
          CCharString::operator+=(param_1,pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_e4,extraout_EDX_31,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_dc,extraout_EDX_32,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_d4,extraout_EDX_33,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_cc,extraout_EDX_34,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_c4,extraout_EDX_35,unaff_EDI);
          puVar10 = &DAT_012353b8;
          FUN_009a08b0((double)*(float *)(iVar5 + 0x18));
          CCharString_OperatorPlus_API(iVar5 + 0x10);
          FUN_0099f600();
          FUN_0099f570();
          pCVar3 = (CCharString *)FUN_0099f600(puVar10);
          CCharString::operator+=(param_1,pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_bc,extraout_EDX_36,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_b4,extraout_EDX_37,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_ac,extraout_EDX_38,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_a4,extraout_EDX_39,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_9c,extraout_EDX_40,unaff_EDI);
          iVar6 = *(int *)(iVar5 + 0xc);
          if (iVar6 == 0) {
            iVar6 = *(int *)(iVar5 + 4);
            if (iVar5 == *(int *)(iVar6 + 0xc)) {
              do {
                iVar5 = iVar6;
                iVar6 = *(int *)(iVar5 + 4);
              } while (iVar5 == *(int *)(iVar6 + 0xc));
            }
            if (*(int *)(iVar5 + 0xc) != iVar6) {
              iVar5 = iVar6;
            }
          }
          else {
            for (iVar1 = *(int *)(iVar6 + 8); iVar5 = iVar6, iVar1 != 0; iVar1 = *(int *)(iVar1 + 8)
                ) {
              iVar6 = iVar1;
            }
          }
        } while (iVar5 != *(int *)(this_00 + 0x38));
      }
      piVar9 = *(int **)this_00;
      if (piVar9 != *(int **)(this_00 + 4)) {
        do {
          iVar5 = *piVar9;
          iVar6 = *(int *)(this + 0x20);
          CCharString::CCharString(local_10c);
          CCharString::CCharString(local_128);
          CCharString::CCharString((CCharString *)&local_114);
          CAFile::SplitPath((CCharString *)(iVar6 + iVar5 * 0x48 + 0x18),(CCharString *)0x0,
                            (CCharString *)&local_114,local_10c,local_128);
          if (local_114 == (undefined4 *)0x0) {
            local_138 = "";
          }
          else {
            local_138 = (char *)*local_114;
          }
          paVar11 = local_94;
          CUserProfileManager::GetAutoSaveFileName(local_8c);
          pCVar3 = (CCharString *)FUN_0099b2c0(paVar11);
          pcVar7 = CCharString::operator_char_const_(pCVar3);
          pcVar7 = strstr(local_138,pcVar7);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_94,extraout_EDX_41,unaff_EDI);
          CCharString::~CCharString((CCharString *)local_8c);
          CCharString::CCharString(local_110);
          if (pcVar7 != (char *)0x0) {
            pCVar3 = (CCharString *)CUserProfileManager::GetAutoSaveFileName(local_84);
            lVar8 = CCharString::GetLength(pCVar3);
            CCharString::operator=(local_110,pcVar7 + lVar8);
            CCharString::~CCharString((CCharString *)local_84);
          }
          pCVar3 = local_128;
          FUN_0099f570();
          FUN_0099f570(pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_7c,extraout_EDX_42,unaff_EDI);
          puVar10 = &DAT_0123990c;
          CCharString_OperatorPlus_API();
          pCVar3 = (CCharString *)FUN_0099f600(puVar10);
          CCharString::operator+=(param_1,pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_74,extraout_EDX_43,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_6c,extraout_EDX_44,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_108,extraout_EDX_45,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_110,
                     extraout_EDX_46,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_114,
                     extraout_EDX_47,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_128,
                     extraout_EDX_48,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10c,
                     extraout_EDX_49,unaff_EDI);
          piVar9 = piVar9 + 1;
        } while (piVar9 != *(int **)(this_00 + 4));
      }
      piVar9 = *(int **)(this_00 + 0xc);
      if (piVar9 != *(int **)(this_00 + 0x10)) {
        do {
          iVar5 = *piVar9;
          iVar6 = *(int *)(this + 0x20);
          CCharString::CCharString(local_12c);
          CCharString::CCharString(local_118);
          CCharString::CCharString(local_130);
          CAFile::SplitPath((CCharString *)(iVar6 + iVar5 * 0x48 + 0x18),(CCharString *)0x0,
                            local_130,local_12c,local_118);
          paVar11 = local_64;
          CUserProfileManager::GetAutoSaveFileName(local_5c);
          pCVar3 = (CCharString *)FUN_0099b2c0(paVar11);
          pcVar7 = CCharString::operator_char_const_(pCVar3);
          _Str = CCharString::operator_char_const_(local_130);
          pcVar7 = strstr(_Str,pcVar7);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_64,extraout_EDX_50,unaff_EDI);
          CCharString::~CCharString((CCharString *)local_5c);
          CCharString::CCharString(local_120);
          if (pcVar7 != (char *)0x0) {
            pCVar3 = (CCharString *)CUserProfileManager::GetAutoSaveFileName(local_54);
            lVar8 = CCharString::GetLength(pCVar3);
            CCharString::operator=(local_120,pcVar7 + lVar8);
            CCharString::~CCharString((CCharString *)local_54);
          }
          pCVar3 = local_118;
          FUN_0099f570();
          FUN_0099f570(pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_4c,extraout_EDX_51,unaff_EDI);
          puVar10 = &DAT_0123990c;
          CCharString_OperatorPlus_API();
          pCVar3 = (CCharString *)FUN_0099f600(puVar10);
          CCharString::operator+=(param_1,pCVar3);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_44,extraout_EDX_52,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_3c,extraout_EDX_53,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (local_104,extraout_EDX_54,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_120,
                     extraout_EDX_55,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_130,
                     extraout_EDX_56,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_118,
                     extraout_EDX_57,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_12c,
                     extraout_EDX_58,unaff_EDI);
          piVar9 = piVar9 + 1;
        } while (piVar9 != *(int **)(this_00 + 0x10));
      }
      CCharString::operator+=(param_1,"EndRegion;\r\n\r\n");
      local_124 = local_124 + 1;
      local_11c = local_11c + 0x58;
    } while (local_124 < (uint)((*(int *)(this + 0x30) - *(int *)(this + 0x2c)) / 0x58));
  }
  return;
}

 
==================== SaveWorldToBinaryFile @ 004fbde0 ==================== 

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: void __thiscall CWorldMap::SaveWorldToBinaryFile(class CWideString &) */

void __thiscall CWorldMap::SaveWorldToBinaryFile(CWorldMap *this,CWideString *param_1)

{
  long lVar1;
  void *pvVar2;
  int iVar3;
  uint uVar4;
  CASuspendableProcess *pCVar5;
  CMemoryDataOutputStream local_34 [24];
  undefined **local_1c [6];
  CCharString local_4 [4];
  
  CMemoryDataOutputStream::CMemoryDataOutputStream(local_34);
  CDataOutputStream::WriteCBYTE
            ((CDataOutputStream *)local_34,
             (char)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  uVar4 = 1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    iVar3 = 0x48;
    do {
      CMapInfo::SaveBinary
                ((CMapInfo *)(*(int *)(this + 0x20) + iVar3),(CDataOutputStream *)local_34);
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x48;
    } while (uVar4 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  }
  CDataOutputStream::WriteCBYTE
            ((CDataOutputStream *)local_34,
             (char)((*(int *)(this + 0x30) - *(int *)(this + 0x2c)) / 0x58));
  uVar4 = 1;
  if (1 < (uint)((*(int *)(this + 0x30) - *(int *)(this + 0x2c)) / 0x58)) {
    iVar3 = 0x58;
    do {
      CRegion::SaveBinary((CRegion *)(*(int *)(this + 0x2c) + iVar3),(CDataOutputStream *)local_34);
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x58;
    } while (uVar4 < (uint)((*(int *)(this + 0x30) - *(int *)(this + 0x2c)) / 0x58));
  }
  CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_1c,param_1,4,2);
  pCVar5 = (CASuspendableProcess *)0x0;
  lVar1 = CCreatureActionBase::GetFramesRemaining((CCreatureActionBase *)local_34);
  pvVar2 = CMemoryDataOutputStream::PeekData(local_34);
  CDiskFileWin32::_Write_CDiskFileWin32__UAEXPBXJPAVCASuspendableProcess___Z
            ((CDiskFileWin32 *)local_1c,pvVar2,lVar1,pCVar5);
  local_1c[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_1c);
  CCharString::~CCharString(local_4);
  NHeroInformationScreens::CBase::CBase((CBase *)local_1c);
  CCompressedMemoryDataInputStream::~CCompressedMemoryDataInputStream
            ((CCompressedMemoryDataInputStream *)local_34);
  return;
}

 
==================== ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z @ 0042ec7c ==================== 

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall
_Run_CNewFrontendGameComponent__UAE_NPAPAVCGameComponent___Z
          (CNewFrontendGameComponent *param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  CGame *pCVar3;
  bool bVar4;
  CFrontEndManager *pCVar5;
  CSystemManager *pCVar6;
  ESystemReturn EVar7;
  CWideString *pCVar8;
  CXMVPlayerBuffers *this;
  ulong uVar9;
  int iVar10;
  CFrontendGameComponent *this_00;
  undefined4 uVar11;
  CMainGameComponent *this_01;
  CEngineInternalPrimitiveBase *this_02;
  CEngineInternalPrimitiveBase *this_03;
  CEngineInternalPrimitiveBase *extraout_ECX;
  CEngineInternalPrimitiveBase *extraout_ECX_00;
  CEngineInternalPrimitiveBase *extraout_ECX_01;
  CEngineInternalPrimitiveBase *extraout_ECX_02;
  CEngineInternalPrimitiveBase *this_04;
  CEngineInternalPrimitiveBase *this_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  CMacro *extraout_EDX_06;
  int *piVar12;
  ulong unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined1 *puVar13;
  float fVar14;
  double dVar15;
  CCharString *pCVar16;
  CXMVPlayerBuffers **ppCVar17;
  char *pcVar18;
  wchar_t *pwVar19;
  CEnginePrimitive *pCVar20;
  undefined1 uVar21;
  ulong uVar22;
  CMainGameComponentInit aCStack_9c [12];
  CWideString aCStack_90 [4];
  CEngineInterpolationInfo aCStack_8c [16];
  char *pcStack_7c;
  ulong auStack_78 [2];
  undefined1 auStack_70 [4];
  DWORD DStack_6c;
  ulong uStack_68;
  float fStack_64;
  char *apcStack_60 [4];
  undefined1 uStack_50;
  undefined1 uStack_4f;
  undefined1 uStack_4e;
  undefined1 uStack_4d;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined1 *puStack_40;
  char *pcStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined1 uStack_30;
  undefined1 uStack_2f;
  undefined1 uStack_2e;
  undefined1 uStack_2d;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  char cStack_19;
  undefined4 uStack_18;
  undefined4 uStack_14;
  CCharString CStack_10;
  undefined1 uStack_f;
  undefined1 uStack_e;
  undefined1 uStack_d;
  CXMVPlayerBuffers *pCStack_c;
  CEngineInternalPrimitiveBase *pCStack_8;
  
  dVar15 = GFGetTime();
  *(float *)(param_1 + 0xb8) = (float)dVar15;
  dVar15 = GFGetTime();
  *(float *)(param_1 + 0xbc) = (float)dVar15;
  piVar12 = (int *)0x0;
  fStack_64 = (float)_DAT_0122f160;
  uStack_44 = _DAT_0122f160;
  pcStack_7c = "Data\\Video\\lionhead_logo.xmv";
  auStack_78[0] = 0x280;
  auStack_78[1] = 400;
  auStack_70[2] = 0xff;
  auStack_70[1] = 0xff;
  auStack_70[0] = 0xff;
  auStack_70[3] = 0xff;
  DStack_6c = 0;
  uStack_68 = 0x1a0;
  apcStack_60[0] = &LAB_0042e3ce;
  apcStack_60[1] = "Data\\Video\\Microsoft_Logo.xmv";
  apcStack_60[2] = (char *)0x280;
  apcStack_60[3] = (char *)0x1e0;
  uStack_4e = 0;
  uStack_4f = 0;
  uStack_50 = 0;
  uStack_4d = 0xff;
  uStack_4c = 0;
  uStack_48 = 0x1a0;
  puStack_40 = &LAB_0042e3ce;
  pcStack_3c = "Data\\Video\\intro_comp.xmv";
  uStack_38 = 0x280;
  uStack_34 = 0x168;
  uStack_2e = 0;
  uStack_2f = 0;
  uStack_30 = 0;
  uStack_2d = 0;
  uStack_2c = 0;
  uStack_28 = 0x1a0;
  uStack_24 = 0x41200000;
  puStack_20 = &LAB_0042e3ce;
  if ((DAT_01375448 != '\0') && (DAT_0137544a != '\0')) {
    pCStack_c = (CXMVPlayerBuffers *)0x0;
    uStack_18 = 0xff000000;
    puVar13 = auStack_70 + 1;
    do {
      uVar11 = uStack_14;
      uStack_14._0_3_ = CONCAT12(puVar13[1],(undefined2)uStack_14);
      uStack_14._0_2_ = CONCAT11(*puVar13,SUB41(uVar11,0));
      uStack_14 = CONCAT31(uStack_14._1_3_,puVar13[-1]);
      uStack_14 = CONCAT13(puVar13[2],(undefined3)uStack_14);
      _DAT_013961e0 = uStack_14;
      DAT_013ca7e9 = 1;
      CCharString::CCharString(&CStack_10);
      CCharString::CCharString((CCharString *)&pCStack_8,*(char **)(puVar13 + -0xd),-1);
      cStack_19 = GFPlayFullScreenXBoxMovie
                            ((CCharString *)&pCStack_8,*(ulong *)(puVar13 + 7),
                             *(ulong *)(puVar13 + -9),*(ulong *)(puVar13 + -5),0xa00000,
                             *(CDataBank **)(param_1 + 0x60),*(CIEngine **)(param_1 + 0x58),true,
                             *(float *)(puVar13 + 0xb),*(_func_bool **)(puVar13 + 0xf),
                             (CXMVPlayerBuffers *)0x0,(CWideString *)&CStack_10);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
                 extraout_EDX,unaff_EDI);
      if (cStack_19 == '\0') {
        param_1[0x144] = (CNewFrontendGameComponent)0x0;
        pCVar5 = NUISystem::CFrontEndManager::GetInstance();
        CWideString::operator=((CWideString *)(pCVar5 + 0x14),(CWideString *)&CStack_10);
      }
      DAT_013ca7e9 = 0;
      _DAT_013961e0 = uStack_18;
      Sleep(*(DWORD *)(puVar13 + 3));
      pCVar6 = (CSystemManager *)
               CEngineInternalPrimitiveBase::AddChildPrimitive
                         (this_02,(CEnginePrimitive *)unaff_EDI,unaff_ESI);
      EVar7 = CSystemManager::Update(pCVar6);
      if (EVar7 == 2) {
        CCharString::~CCharString(&CStack_10);
        return 0;
      }
      CCharString::~CCharString(&CStack_10);
      pCStack_c = pCStack_c + 1;
      puVar13 = puVar13 + 0x20;
    } while (pCStack_c < (CXMVPlayerBuffers *)0x3);
  }
  if (DAT_013b8616 != '\0') {
    FUN_0099b6b0(&DAT_0122daa4);
    uVar21 = 1;
    pwVar19 = L"frontend.big";
    ppCVar17 = &pCStack_c;
    CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&uStack_18);
    FUN_0099be70(ppCVar17);
    pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&pCStack_8);
    CCharString::~CCharString(&CStack_10);
    CCharString::~CCharString((CCharString *)&uStack_18);
    uVar21 = 0;
    pwVar19 = L"text.big";
    NResourceDirectoryNames::GetDVDDialogueDir();
    pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&pCStack_8);
    CCharString::~CCharString(&CStack_10);
    uVar21 = 0;
    pwVar19 = L"shaders.big";
    ppCVar17 = &pCStack_c;
    CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&uStack_18);
    FUN_0099be70(ppCVar17);
    pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&pCStack_8);
    CCharString::~CCharString(&CStack_10);
    CCharString::~CCharString((CCharString *)&uStack_18);
    CCharString::~CCharString((CCharString *)&pCStack_c);
  }
  param_1[9] = (CNewFrontendGameComponent)0x1;
  CNewFrontendGameComponent::Init2(param_1);
  CCharString::CCharString((CCharString *)&pCStack_8,"Init Engine",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
             extraout_EDX_00,unaff_EDI);
  CNewFrontendGameComponent::InitialiseEngine(param_1);
  _E2();
  CCharString::CCharString((CCharString *)&pCStack_8,"Init frontend",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
             extraout_EDX_01,unaff_EDI);
  this = operator_new(0x10);
  if (this == (CXMVPlayerBuffers *)0x0) {
    pCStack_c = (CXMVPlayerBuffers *)0x0;
  }
  else {
    pCStack_c = (CXMVPlayerBuffers *)CXMVPlayerBuffers::CXMVPlayerBuffers(this);
  }
  *(undefined1 *)(DAT_013b838c + 0x38) = 0;
  FUN_0099b6b0(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()");
  uVar22 = 1;
  pCVar20 = (CEnginePrimitive *)&pCStack_8;
  (**(code **)(**(int **)(param_1 + 0x10) + 0x10))();
  CCharString::~CCharString((CCharString *)&pCStack_8);
  uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_03,pCVar20,uVar22);
  pCStack_8 = *(CEngineInternalPrimitiveBase **)(uVar9 + 0x60);
  uStack_e = 0;
  uStack_f = 0;
  CStack_10 = (CCharString)0x0;
  uStack_d = 0xff;
  CRenderManager2D::ClearRenderTarget(DAT_013b8384,(CRGBColour *)&CStack_10,0);
  CDisplayManager::SwapScreens((CDisplayManager *)pCStack_8);
  FUN_0042ded5(0);
  NUISystem::CFrontEndManager::ChangeStateFirstTime(*(CFrontEndManager **)(param_1 + 0xb4));
  CStopWatch::StartZero((CStopWatch *)(param_1 + 0x100));
  CStopWatch::Reset((CStopWatch *)(param_1 + 0x100));
  _E2();
  NUISystem::CUIScreenUnitConverter::ResetDimensions();
  this_04 = extraout_ECX;
  while( true ) {
    pCVar6 = (CSystemManager *)
             CEngineInternalPrimitiveBase::AddChildPrimitive(this_04,pCVar20,uVar22);
    EVar7 = CSystemManager::Update(pCVar6);
    if ((EVar7 == 2) || (param_1[8] != (CNewFrontendGameComponent)0x0)) break;
    uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_05,pCVar20,uVar22);
    pCStack_8 = *(CEngineInternalPrimitiveBase **)(uVar9 + 0x170);
    if (*(char *)(uVar9 + 0x16c) != '\0') {
      uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(pCStack_8,pCVar20,uVar22);
      *(undefined4 *)(uVar9 + 0x170) = 0;
      *(undefined1 *)(uVar9 + 0x16c) = 0;
      FUN_0042ded5(pCStack_8);
    }
    if ((param_1[0x138] != (CNewFrontendGameComponent)0x0) &&
       (fVar14 = CStopWatch::GetElapsedSeconds((CStopWatch *)(param_1 + 0x120)),
       _DAT_012305a0 <= fVar14)) {
      CStopWatch::Stop((CStopWatch *)(param_1 + 0x120));
    }
    CNewFrontendGameComponent::Input(param_1);
    CNewFrontendGameComponent::Update(param_1);
    CEngineInterpolationInfo::CEngineInterpolationInfo(aCStack_8c);
    CNewFrontendGameComponent::UpdateInterpolation(param_1,aCStack_8c);
    CNewFrontendGameComponent::Draw(param_1,aCStack_8c);
    fVar14 = CStopWatch::GetElapsedSeconds((CStopWatch *)(param_1 + 0x100));
    this_04 = extraout_ECX_00;
    if ((_DAT_01230d74 <= fVar14) &&
       (bVar4 = CThingFilter_IsUnconscious::operator()
                          (*(CThingFilter_IsUnconscious **)(param_1 + 0xb4),(CThing *)pCVar20),
       this_04 = extraout_ECX_01, bVar4)) {
      if (DAT_013b8394 != (int *)0x0) {
        (**(code **)(*DAT_013b8394 + 0x48))(0);
      }
      iVar1 = *(int *)(*(int *)(param_1 + 0x3c) + 0x40);
      iVar2 = *(int *)(*(int *)(param_1 + 0x3c) + 0x3c);
      iVar10 = rand();
      CCharString::CCharString
                ((CCharString *)&uStack_14,
                 (CCharString *)
                 (*(int *)(*(int *)(param_1 + 0x3c) + 0x3c) + (iVar10 % (iVar1 - iVar2 >> 2)) * 4));
      uVar9 = NLocalisation::GetLanguage();
      if (uVar9 == 4) {
        pcVar18 = "English";
      }
      else if (uVar9 == 7) {
        pcVar18 = "German";
      }
      else if (uVar9 == 10) {
        pcVar18 = "Spanish";
      }
      else if (uVar9 == 0xc) {
        pcVar18 = "French";
      }
      else if (uVar9 == 0x10) {
        pcVar18 = "Italian";
      }
      else {
        pcVar18 = "English";
      }
      CCharString::operator+=((CCharString *)&uStack_14,pcVar18);
      CCharString::operator+=((CCharString *)&uStack_14,".wmv");
      DAT_013ca7e9 = 1;
      GFPlayFullScreenXBoxMovie
                ((CCharString *)&uStack_14,0x221,0,0,0x100000,*(CDataBank **)(param_1 + 0x60),
                 *(CIEngine **)(param_1 + 0x58),true,_DAT_0122dee0,(_func_bool *)0x0,pCStack_c,
                 (CWideString *)0x0);
      DAT_013ca7e9 = 0;
      CStopWatch::Reset((CStopWatch *)(param_1 + 0x100));
      CInputManager::ClearEvents(DAT_013b8388);
      CStopWatch::StartZero((CStopWatch *)(param_1 + 0x120));
      FUN_0042ded5(0);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_14,
                 extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      this_04 = extraout_ECX_02;
    }
  }
  param_1[8] = (CNewFrontendGameComponent)0x1;
  param_1[9] = (CNewFrontendGameComponent)0x0;
  if (pCStack_c != (CXMVPlayerBuffers *)0x0) {
    (*(code *)**(undefined4 **)pCStack_c)(1);
  }
  CNewFrontendGameComponent::ReleaseXMVCode(param_1);
  DAT_013b871c = 0;
  CEngineResourceManager::ChangeScene((CEngineResourceManager *)param_1);
  if (param_1[0x2a] == (CNewFrontendGameComponent)0x0) {
    if (param_1[0x29] == (CNewFrontendGameComponent)0x0) {
      DAT_013b871c = 0;
      CNewFrontendGameComponent::Shutdown(param_1);
      uVar11 = 0;
      DAT_013b7d58 = param_1;
    }
    else {
      DAT_01375448 = '\0';
      CCharString::CCharString((CCharString *)&pCStack_8,"Leave frontend",-1);
      _E2();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
                 extraout_EDX_03,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      if (DAT_013b8394 != (int *)0x0) {
        (**(code **)(*DAT_013b8394 + 0x48))(500);
      }
      if (DAT_013b8616 != '\0') {
        FUN_0099b6b0(&DAT_0122daa4);
        pwVar19 = L"frontend.big";
        pCVar16 = &CStack_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&uStack_18);
        FUN_0099be70(pCVar16);
        pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
        CBankFileManager::CloseRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8);
        CCharString::~CCharString((CCharString *)&pCStack_8);
        CCharString::~CCharString((CCharString *)&pCStack_c);
        CCharString::~CCharString((CCharString *)&uStack_18);
        uVar21 = 1;
        pwVar19 = L"graphics.big";
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&pCStack_c);
        pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&pCStack_8);
        CCharString::~CCharString((CCharString *)&pCStack_c);
        uVar21 = 1;
        pwVar19 = L"textures.big";
        pCVar16 = &CStack_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&uStack_18);
        FUN_0099be70(pCVar16);
        pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&pCStack_8);
        CCharString::~CCharString((CCharString *)&pCStack_c);
        CCharString::~CCharString((CCharString *)&uStack_18);
        uVar21 = 0;
        pwVar19 = L"dialogue.big";
        NResourceDirectoryNames::GetDVDDialogueDir();
        pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&pCStack_8);
        CCharString::~CCharString((CCharString *)&pCStack_c);
        uVar21 = 0;
        pwVar19 = L"effects.big";
        pCVar16 = &CStack_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&uStack_18);
        FUN_0099be70(pCVar16);
        pCVar8 = (CWideString *)FUN_0099bf30(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&pCStack_8);
        CCharString::~CCharString((CCharString *)&pCStack_c);
        CCharString::~CCharString((CCharString *)&uStack_18);
        CCharString::~CCharString(&CStack_10);
      }
      CTBaseSingleton<CFileInstaller>::Release();
      CMainGameComponentInit::CMainGameComponentInit(aCStack_9c);
      CWideString::operator=(aCStack_90,(CWideString *)(param_1 + 0x84));
      CCharString::CCharString((CCharString *)&pCStack_8,"FinalAlbion.wld",-1);
      pCVar8 = (CWideString *)
               NScript::
               operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                         ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                           *)&pCStack_c,(CIsThingAlive *)&pCStack_8);
      CWideString::operator=((CWideString *)aCStack_9c,pCVar8);
      CCharString::~CCharString((CCharString *)&pCStack_c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
                 extraout_EDX_04,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      _E2();
      pCVar3 = *(CGame **)(param_1 + 0xc);
      CNewFrontendGameComponent::Shutdown(param_1);
      CCharString::CCharString((CCharString *)&pCStack_8,"Init Game",-1);
      _E2();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_8,
                 extraout_EDX_05,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      this_01 = operator_new(0x161e8);
      if (this_01 != (CMainGameComponent *)0x0) {
        piVar12 = (int *)CMainGameComponent::CMainGameComponent(this_01,pCVar3,aCStack_9c);
      }
      (**(code **)(*piVar12 + 4))();
      *param_2 = piVar12;
      DAT_013b7d58 = param_1;
      std::_Dest_val<std::allocator<CMacro>,CMacro>((allocator<CMacro> *)aCStack_9c,extraout_EDX_06)
      ;
      uVar11 = 1;
    }
  }
  else {
    CNewFrontendGameComponent::Shutdown(param_1);
    this_00 = operator_new(0x1e60);
    if (this_00 == (CFrontendGameComponent *)0x0) {
      piVar12 = (int *)0x0;
    }
    else {
      pCStack_8 = (CEngineInternalPrimitiveBase *)0x0;
      piVar12 = (int *)CFrontendGameComponent::CFrontendGameComponent
                                 (this_00,*(CGame **)(param_1 + 0xc),
                                  (CFrontendGameComponentInit *)&pCStack_8);
    }
    (**(code **)(*piVar12 + 4))();
    *param_2 = piVar12;
    DAT_013b871c = 0;
    uVar11 = 1;
    DAT_013b7d58 = param_1;
  }
  return uVar11;
}

 
==================== ?PostInit@CMainGameComponent@@MAEXXZ @ 00416953 ==================== 

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=1.0 <- ego_r]
   protected: virtual void __thiscall CMainGameComponent::PostInit(void) */

void __thiscall CMainGameComponent::_PostInit_CMainGameComponent__MAEXXZ(CMainGameComponent *this)

{
  CCharString *this_00;
  bool bVar1;
  char cVar2;
  long lVar3;
  CWideString *pCVar4;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  uint extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  CCharString *pCVar5;
  undefined *puVar6;
  wchar_t *pwVar7;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar8;
  CDiskFileWin32 local_88 [28];
  CDiskFileWin32 local_6c [28];
  CDiskFileWin32 local_50 [28];
  CDiskFileWin32 local_34 [28];
  CWideString local_18 [4];
  CCharString local_14 [4];
  CUserProfileManager local_10 [4];
  CCharString local_c [4];
  CCharString local_8 [4];
  
  ppVar8 = *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 0x28);
  (**(code **)(**(int **)(this + 0x24) + 0x1c))();
  lVar3 = CCharString::GetLength((CCharString *)(this + 0x161dc));
  if (lVar3 < 1) {
    CCharString::CCharString(local_14,"Loading world",-1);
    NProgressDisplay::DisplayProgress
              (local_14,(float)(extraout_EDX_01 & 0xffffff00),SUB41(_DAT_0122dee0,0),false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_14,
               extraout_EDX_02,ppVar8);
    CCharString::CCharString(local_8);
    this_00 = (CCharString *)(this + 0x161d0);
    lVar3 = CCharString::GetLength(this_00);
    if (lVar3 < 1) {
      lVar3 = CCharString::GetLength((CCharString *)&DAT_013b8668);
      if (lVar3 < 1) {
        pwVar7 = L"updatedscenic.wld";
        CUserProfileManager::GetAutoSaveFileName(local_10);
        pCVar4 = (CWideString *)FUN_0099bf30(pwVar7);
      }
      else {
        puVar6 = &DAT_013b8668;
        CUserProfileManager::GetAutoSaveFileName(local_10);
        pCVar4 = (CWideString *)FUN_0099be70(puVar6);
      }
      CWideString::operator=((CWideString *)local_8,pCVar4);
      CCharString::~CCharString(local_c);
      CCharString::~CCharString((CCharString *)local_10);
    }
    else {
      pCVar5 = this_00;
      CUserProfileManager::GetAutoSaveFileName(local_10);
      pCVar4 = (CWideString *)FUN_0099be70(pCVar5);
      CWideString::operator=((CWideString *)local_8,pCVar4);
      CCharString::~CCharString(local_c);
      CCharString::~CCharString((CCharString *)local_10);
      CWideString::operator=((CWideString *)&DAT_013b8668,(CWideString *)this_00);
      lVar3 = CCharString::GetLength((CCharString *)(this + 0x161d4));
      if (0 < lVar3) {
        CWideString::operator=((CWideString *)&DAT_013b866c,(CWideString *)(this + 0x161d4));
      }
    }
    CWorld::Load(*(CWorld **)(this + 0x24),(CWideString *)local_8);
    if (DAT_013b8648 != '\0') {
      FUN_0049ddd0(local_c,local_8);
      CDiskFileWin32::CDiskFileWin32(local_88,(CWideString *)local_c,1,2);
      CWorld::ConvertWorldToISMThingyPath
                (*(CWorld **)(this + 0x24),(CWideString *)local_10,SUB41(local_8,0),true);
      CDiskFileWin32::CDiskFileWin32(local_50,(CWideString *)local_10,4,2);
      CAFile::CopyFileW((CAFile *)local_88,(CAFile *)local_50);
      FUN_0049dec0(local_18,local_8);
      CDiskFileWin32::CDiskFileWin32(local_6c,local_18,1,2);
      CWorld::ConvertWorldToISMThingyPath
                (*(CWorld **)(this + 0x24),(CWideString *)local_14,SUB41(local_8,0),true);
      CDiskFileWin32::CDiskFileWin32(local_34,(CWideString *)local_14,4,2);
      CAFile::CopyFileW((CAFile *)local_6c,(CAFile *)local_34);
      CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                ((CAIStateGroup_CreatureUseCoverBase *)local_34);
      CCharString::~CCharString(local_14);
      CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                ((CAIStateGroup_CreatureUseCoverBase *)local_6c);
      CCharString::~CCharString((CCharString *)local_18);
      CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                ((CAIStateGroup_CreatureUseCoverBase *)local_50);
      CCharString::~CCharString((CCharString *)local_10);
      CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                ((CAIStateGroup_CreatureUseCoverBase *)local_88);
      CCharString::~CCharString(local_c);
      CCharString::~CCharString(local_8);
      return;
    }
    CWorld::ActivateWorld(*(CWorld **)(this + 0x24),false);
    CCharString::CCharString(local_c,"Activate Initial Quests",-1);
    ::_E2();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,
               extraout_EDX_03,ppVar8);
    cVar2 = FUN_0099e960(&DAT_0122d70e);
    if (cVar2 != '\0') {
      FUN_004b4a10(this + 0x161d8,0,1);
    }
    ::_E2();
    CCharString::~CCharString(local_8);
  }
  else {
    CCharString::CCharString(local_8,"Loading save",-1);
    NProgressDisplay::DisplayProgress
              (local_8,(float)(extraout_EDX & 0xffffff00),SUB41(_DAT_0122dee0,0),false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
               extraout_EDX_00,ppVar8);
    bVar1 = CWorld::LoadGameState(*(CWorld **)(this + 0x24),(CWideString *)(this + 0x161dc),true);
    if (!bVar1) {
      DAT_013b89c0 = 1;
    }
  }
  ::_E2(&DAT_013b8674);
  return;
}

 
==================== ChooseWorldNumber @ 00494710 ==================== 

/* [bsim sim=0.9999999999999999 <- ego_r]
   private: void __thiscall CFrontendGameComponent::ChooseWorldNumber(long) */

void __thiscall CFrontendGameComponent::ChooseWorldNumber(CFrontendGameComponent *this,long param_1)

{
  CWideString *pCVar1;
  
  if ((uint)(*(int *)(this + 0x1e3c) + -1 + param_1) <
      (uint)(*(int *)(this + 0x1e34) - *(int *)(this + 0x1e30) >> 2)) {
    pCVar1 = (CWideString *)FUN_0099bf30(L".wld");
    CWideString::operator=((CWideString *)(this + 0x1e40),pCVar1);
    CCharString::~CCharString((CCharString *)&param_1);
    (**(code **)(*(int *)this + 0x10))();
  }
  return;
}

 
==================== GetLevelScriptName @ 004fcfc0 ==================== 

/* [bsim sim=1.0 <- ego_r]
   public: class CCharString const & __thiscall CWorldMap::GetLevelScriptName(long)const  */

CCharString * __thiscall CWorldMap::GetLevelScriptName(CWorldMap *this,long param_1)

{
  return (CCharString *)(*(int *)(this + 0x20) + 0x1c + param_1 * 0x48);
}

 
==================== IsMapLoadedOnPlayerProximity @ 004fd000 ==================== 

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: bool __thiscall CWorldMap::IsMapLoadedOnPlayerProximity(long)const  */

bool __thiscall CWorldMap::IsMapLoadedOnPlayerProximity(CWorldMap *this,long param_1)

{
  return (bool)*(undefined1 *)(*(int *)(this + 0x20) + 0x27 + param_1 * 0x48);
}

 