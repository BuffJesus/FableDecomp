 '0x413c50' '0x4a1965' '0x49d584' '0x49d631' '0x49d794' '0x42f44d'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== InitialiseConsoleVariables @ 00413c50 ==================== 

/* WARNING: Removing unreachable block (ram,0x0041465f) */
/* [bsim sim=0.7679215751507659 <- ego_r]
   private: static void __fastcall CTCLook::InitialiseConsoleVariables(void) */

void __fastcall CTCLook::InitialiseConsoleVariables(void)

{
  void *pvVar1;
  CIPhysicsFacesSegmentBase *pCVar2;
  CConsoleInputBase *pCVar3;
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
  byte bVar4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  bool bVar5;
  undefined4 *puVar6;
  undefined4 local_4;
  
  local_4 = 0;
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::Initialise(DAT_013caa40,'a',0x1e,(CFontBank *)0x0);
  pCVar3 = ::operator_new(0x18);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"SetLevel",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__scalar_deleting_destructor__0122e638;
    FUN_009ebb30(5);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e65c;
    *(undefined1 **)(pCVar3 + 0x14) = &LAB_00413800;
  }
  bVar5 = !bVar5;
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX,unaff_ESI);
  }
  bVar4 = !bVar5 && bVar5;
  pCVar3 = ::operator_new(0x18);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"SetStartingHolySite",-1);
    bVar4 = bVar4 | 2;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__scalar_deleting_destructor__0122e638;
    FUN_009ebb30(5);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e65c;
    *(undefined1 **)(pCVar3 + 0x14) = &LAB_00413840;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 2) != 0) {
    bVar4 = bVar4 & 0xfd;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_00,unaff_ESI);
  }
  pCVar3 = ::operator_new(0x18);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"EnableCodeSectionLoading",-1);
    bVar4 = bVar4 | 4;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__scalar_deleting_destructor__0122e638;
    FUN_009ebb30(4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e65c;
    *(undefined1 **)(pCVar3 + 0x14) = &LAB_00413880;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 4) != 0) {
    bVar4 = bVar4 & 0xfb;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_01,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UsePhysicalDVD",-1);
    bVar4 = bVar4 | 8;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013b85f2;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 8) != 0) {
    bVar4 = bVar4 & 0xf7;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_02,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseRetailSaveGameSystem",-1);
    bVar4 = bVar4 | 0x10;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8646;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 0x10) != 0) {
    bVar4 = bVar4 & 0xef;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_03,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"RunFromDVD",-1);
    bVar4 = bVar4 | 0x20;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8615;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 0x20) != 0) {
    bVar4 = bVar4 & 0xdf;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_04,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseRetailBanks",-1);
    bVar4 = bVar4 | 0x40;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8616;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((bVar4 & 0x40) != 0) {
    bVar4 = bVar4 & 0xbf;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_05,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseCompiledAnimationEvents",-1);
    bVar4 = bVar4 | 0x80;
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b860a;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if ((char)bVar4 < '\0') {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_06,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseCompiledGlobalThings",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8609;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_07,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseCompiledDefs",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8617;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_08,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseCompiledWorldFiles",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8618;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_09,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"UseCompiledSoundSymbols",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_013b8619;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_10,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"AllowDataGeneration",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 4;
    *(undefined1 **)(pCVar3 + 0xc) = &DAT_01375459;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_11,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"InstallerBufferSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_01375494;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_12,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"NoInstallBuffers",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_01375498;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_13,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  if (pCVar3 == (CConsoleInputBase *)0x0) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"LandscapePoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined4 **)(pCVar3 + 0xc) = &DAT_0137549c;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"LandscapePhysicalMemoryRatio",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 3;
    *(undefined4 **)(pCVar3 + 0xc) = &DAT_013754a0;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_14,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"LandscapeVirtualMemoryMinSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754a4;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_15,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"MeshPoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754a8;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_16,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"MeshStatsPoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754ac;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_17,unaff_EDI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"MeshPhysicalMemoryRatio",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 3;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754b0;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_18,unaff_ESI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"HiresTextureMemory",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754b4;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_19,unaff_ESI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"PhysicsMeshPoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754b8;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_20,unaff_ESI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"AnimationPoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754bc;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_21,unaff_ESI);
  }
  pCVar3 = ::operator_new(0x10);
  bVar5 = pCVar3 == (CConsoleInputBase *)0x0;
  if (bVar5) {
    pCVar3 = (CConsoleInputBase *)0x0;
  }
  else {
    CCharString::CCharString((CCharString *)&local_4,"ClothPoolSize",-1);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5b0;
    CCharString::CCharString((CCharString *)(pCVar3 + 4),(CCharString *)&local_4);
    *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__0122e5c8;
    *(undefined4 *)(pCVar3 + 8) = 1;
    *(undefined **)(pCVar3 + 0xc) = &DAT_013754c0;
  }
  if (DAT_013caa40 == (CConsole *)0x0) {
    pvVar1 = ::operator_new(0x9c);
    if (pvVar1 == (void *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar2 = (CIPhysicsFacesSegmentBase *)FUN_009ecd80();
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&DAT_013caa40,pCVar2);
  }
  CConsole::AddInput(DAT_013caa40,pCVar3);
  if (!bVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
               extraout_EDX_22,unaff_EBX);
  }
  FUN_0099b6b0(L"default_userst.ini");
  bVar5 = CAFile::PathExists((CWideString *)&local_4);
  CCharString::~CCharString((CCharString *)&local_4);
  if (bVar5) {
    FUN_0099b6b0(L"default_userst.ini");
    puVar6 = &local_4;
    CTBaseSingleton<CCodeSectionManager>::Get();
    FUN_009ec890(puVar6);
    CCharString::~CCharString((CCharString *)&local_4);
  }
  puVar6 = &local_4;
  if (DAT_01375444 != '\0') {
    FUN_0099b6b0(L"userst.ini");
    CTBaseSingleton<CCodeSectionManager>::Get();
    FUN_009ec890(puVar6);
    CCharString::~CCharString((CCharString *)&local_4);
  }
  return;
}

 
==================== Load @ 004a1840 ==================== 

/* [bsim sim=0.8749307306590621 <- ego_r]
   public: void __thiscall CWorld::Load(class CWideString const &) */

void __thiscall CWorld::Load(CWorld *this,CWideString *param_1)

{
  CTCUserActionPointBase *this_00;
  int *piVar1;
  int *piVar2;
  bool bVar3;
  CCharString *pCVar4;
  undefined4 uVar5;
  CTCTrophy *this_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  uint extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  uint extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  uint extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  uint extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar7;
  undefined **local_b0 [6];
  CCharString local_98 [4];
  undefined **local_94 [6];
  CCharString local_7c [4];
  undefined **local_78 [5];
  undefined1 local_64;
  CCharString local_60 [4];
  CCharString local_5c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_58 [4];
  CCharString local_54 [4];
  CCharString local_50 [4];
  CCharString local_4c [4];
  CCharString local_48 [4];
  CCharString local_44 [4];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> local_40 [4];
  CWideString local_3c [4];
  CCharString local_38 [4];
  CCharString local_34 [4];
  CCharString local_30 [4];
  CCharString local_2c [4];
  CCharString local_28 [8];
  CWideString local_20 [4];
  CWideString local_1c [4];
  CCharString local_18 [4];
  undefined1 *local_14;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_012109b0;
  pvStack_10 = ExceptionList;
  local_14 = &stack0xffffff40;
  ExceptionList = &pvStack_10;
  CCharString::CCharString(local_18);
  CAFile::SplitPath((CCharString *)param_1,(CCharString *)0x0,(CCharString *)0x0,local_18,
                    (CCharString *)0x0);
  pCVar4 = (CCharString *)FUN_0099b2c0(local_58);
  CCharString::operator=((CCharString *)(this + 0x84),pCVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_58,extraout_EDX,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)local_78);
  local_78[0] = &PTR__vector_deleting_destructor__0122d06c;
  CCharString::CCharString(local_60);
  local_64 = 0;
  local_8 = 0;
  CDiskFileWin32::_Open_CDiskFileWin32__UAEXABVCWideString__W4FileAccessType__K_Z
            ((CDiskFileWin32 *)local_78,param_1,1,2);
  local_8 = 0xffffffff;
  CCharString::CCharString(local_44,"Load Quests",-1);
  ::_E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_44,
             extraout_EDX_00,unaff_EDI);
  FUN_0049ddd0(local_40,param_1);
  CPersistContext::GetDefaultVal<CFireEffectCreationSet>();
  bVar3 = CAFile::PathExists(local_20);
  if (bVar3) {
    CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_94,local_20,1,2);
    LoadQuestsFromFile(this,(CAFile *)local_94,true);
    local_94[0] = &PTR__vector_deleting_destructor__0122d06c;
    CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_94);
    CCharString::~CCharString(local_7c);
    NHeroInformationScreens::CBase::CBase((CBase *)local_94);
  }
  FUN_0099b6b0(L"Data\\Levels\\GlobalQuests.qst");
  bVar3 = CAFile::PathExists(local_1c);
  if (bVar3) {
    CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_b0,local_1c,1,2);
    LoadQuestsFromFile(this,(CAFile *)local_b0,false);
    local_b0[0] = &PTR__vector_deleting_destructor__0122d06c;
    CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_b0);
    CCharString::~CCharString(local_98);
    NHeroInformationScreens::CBase::CBase((CBase *)local_b0);
  }
  FUN_0099b6b0(&DAT_0122d70c);
  FUN_004fdab0(local_50);
  CCharString::~CCharString(local_50);
  CCharString::CCharString(local_34,"Startup Wad",-1);
  ::_E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_34,
             extraout_EDX_01,unaff_EDI);
  CCharString::CCharString(local_48,"Startup WAD",-1);
  NProgressDisplay::DisplayProgress(local_48,(float)(extraout_EDX_02 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_48,
             extraout_EDX_03,unaff_EDI);
  if (DAT_01375456 != '\0') {
    CPersistContext::GetDefaultVal<CFireEffectCreationSet>();
    CWorldMap::CreateWorldLevelWad(*(CWorldMap **)(this + 0x14),param_1,local_3c);
    uVar5 = CPersistContext::GetDefaultVal<CFireEffectCreationSet>();
    FUN_004fdab0(uVar5);
    CCharString::~CCharString(local_5c);
    CCharString::~CCharString((CCharString *)local_3c);
  }
  ::_E2();
  ppVar7 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_78;
  (**(code **)(*(int *)this + 8))(ppVar7,0xffffffff);
  while( true ) {
    this_00 = *(CTCUserActionPointBase **)(*(int *)(this + 0x14) + 0xbc);
    piVar1 = *(int **)(*(int *)(this + 0x14) + 0xc0);
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + 1;
    }
    bVar3 = CTCUserActionPointBase::IsActionPointFree(this_00);
    if ((piVar1 != (int *)0x0) && (*piVar1 = *piVar1 + -1, *piVar1 == 0)) {
      (*(code *)piVar1[1])();
      operator_delete(piVar1);
    }
    if (!bVar3) break;
    piVar1 = *(int **)(*(int *)(this + 0x14) + 0xbc);
    piVar2 = *(int **)(*(int *)(this + 0x14) + 0xc0);
    if (piVar2 != (int *)0x0) {
      *piVar2 = *piVar2 + 1;
    }
    (**(code **)(*piVar1 + 4))();
    if ((piVar2 != (int *)0x0) && (*piVar2 = *piVar2 + -1, *piVar2 == 0)) {
      (*(code *)piVar2[1])();
      operator_delete(piVar2);
    }
  }
  CCharString::CCharString(local_4c,"Generate Offline Data",-1);
  ::_E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4c,
             extraout_EDX_04,ppVar7);
  CCharString::CCharString(local_28,"Generate Offline Data",-1);
  NProgressDisplay::DisplayProgress(local_28,(float)(extraout_EDX_05 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_28,
             extraout_EDX_06,ppVar7);
  if (DAT_01375446 != '\0') {
    FUN_0049ddd0(local_54,param_1);
    uVar5 = *(undefined4 *)(*(int *)(*(int *)(this + 8) + 0x28) + 0x2c);
    pCVar4 = local_54;
    (**(code **)(*(int *)this + 0x30))(pCVar4,uVar5);
    ::_E2(pCVar4,uVar5);
    CCharString::~CCharString(local_54);
  }
  CCharString::CCharString(local_2c,"Set Static Map for Engine",-1);
  ::_E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_2c,
             extraout_EDX_07,ppVar7);
  CCharString::CCharString(local_30,"Set Static Map for Engine",-1);
  NProgressDisplay::DisplayProgress(local_30,(float)(extraout_EDX_08 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_30,
             extraout_EDX_09,ppVar7);
  ppVar6 = local_40;
  (**(code **)(**(int **)(*(int *)(*(int *)(this + 8) + 0x28) + 0x2c) + 0xd0))();
  CCharString::CCharString(local_38,"Set Static Map for Engine: End",-1);
  NProgressDisplay::DisplayProgress(local_38,(float)(extraout_EDX_10 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_38,
             extraout_EDX_11,ppVar6);
  ::_E2();
  CCharString::~CCharString((CCharString *)local_1c);
  CCharString::~CCharString((CCharString *)local_20);
  CCharString::~CCharString((CCharString *)local_40);
  local_78[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_78);
  CCharString::~CCharString(local_60);
  NHeroInformationScreens::CBase::CBase((CBase *)local_78);
  CTCTrophy::OnShowTrophyInitialise(this_01,(CThing *)ppVar7);
  return;
}

 
==================== ConvertWorldToISMThingyPath @ 0049d550 ==================== 

/* [bsim sim=0.6666349988789577 <- ego_r]
   public: class CWideString __thiscall CWorld::ConvertWorldToISMThingyPath(class CWideString const
   &,bool,bool)const  */

CWideString * __thiscall
CWorld::ConvertWorldToISMThingyPath(CWorld *this,CWideString *param_1,bool param_2,bool param_3)

{
  CWideString *pCVar1;
  undefined3 in_stack_00000009;
  CCharString in_stack_00000010;
  CWorld **ppCVar2;
  wchar_t *pwVar3;
  CWorld *local_4;
  
  local_4 = this;
  CCharString::CCharString((CCharString *)&local_4);
  CAFile::SplitPath(_param_2,(CCharString *)0x0,(CCharString *)0x0,(CCharString *)&local_4,
                    (CCharString *)0x0);
  if (in_stack_00000010 == (CCharString)0x0) {
    if (param_3 == false) {
      pwVar3 = L".stb";
    }
    else {
      pwVar3 = L"_XB.stb";
    }
  }
  else if (param_3 == false) {
    pwVar3 = L"_RT.stb";
  }
  else {
    pwVar3 = L"_RT_XB.stb";
  }
  ppCVar2 = &local_4;
  CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&param_2);
  FUN_0099be70(ppCVar2);
  pCVar1 = (CWideString *)FUN_0099bf30(pwVar3);
  CWideString::operator=((CWideString *)&local_4,pCVar1);
  CCharString::~CCharString((CCharString *)&param_3);
  CCharString::~CCharString(&stack0x00000010);
  CCharString::~CCharString((CCharString *)&param_2);
  CWideString::CWideString(param_1,(CWideString *)&local_4);
  CCharString::~CCharString((CCharString *)&local_4);
  return param_1;
}

 
==================== GetDefaultVal<CFireEffectCreationSet> @ 0049d610 ==================== 

/* [bsim sim=0.5470594528291263 <- ego_r]
   protected: static class CFireEffectCreationSet __fastcall CPersistContext::GetDefaultVal<class
   CFireEffectCreationSet>(void) */

CWideString * __fastcall CPersistContext::GetDefaultVal<CFireEffectCreationSet>(void)

{
  CWideString *pCVar1;
  CWideString *in_stack_00000004;
  CCharString *in_stack_00000008;
  CCharString *pCVar2;
  wchar_t *pwVar3;
  CCharString local_c [4];
  CCharString local_8 [4];
  CUserProfileManager local_4 [4];
  
  CCharString::CCharString(local_c);
  CAFile::SplitPath(in_stack_00000008,(CCharString *)0x0,(CCharString *)0x0,local_c,
                    (CCharString *)0x0);
  pwVar3 = L".wad";
  pCVar2 = local_c;
  CUserProfileManager::GetAutoSaveFileName(local_4);
  FUN_0099be70(pCVar2);
  pCVar1 = (CWideString *)FUN_0099bf30(pwVar3);
  CWideString::operator=((CWideString *)local_c,pCVar1);
  CCharString::~CCharString((CCharString *)&stack0x00000008);
  CCharString::~CCharString(local_8);
  CCharString::~CCharString((CCharString *)local_4);
  CWideString::CWideString(in_stack_00000004,(CWideString *)local_c);
  CCharString::~CCharString(local_c);
  return in_stack_00000004;
}

 
==================== GetDefaultVal<CFireEffectCreationSet> @ 0049d770 ==================== 

/* [bsim sim=0.5470594528291263 <- ego_r]
   protected: static class CFireEffectCreationSet __fastcall CPersistContext::GetDefaultVal<class
   CFireEffectCreationSet>(void) */

void __fastcall CPersistContext::GetDefaultVal<CFireEffectCreationSet>(void)

{
  CWideString *pCVar1;
  CWideString *in_ECX;
  CCharString *in_EDX;
  CCharString *pCVar2;
  wchar_t *pwVar3;
  CCharString local_10 [4];
  CCharString local_c [4];
  CCharString local_8 [4];
  CUserProfileManager local_4 [4];
  
  CCharString::CCharString(local_10);
  CAFile::SplitPath(in_EDX,(CCharString *)0x0,(CCharString *)0x0,local_10,(CCharString *)0x0);
  pwVar3 = L".qst";
  pCVar2 = local_10;
  CUserProfileManager::GetAutoSaveFileName(local_4);
  FUN_0099be70(pCVar2);
  pCVar1 = (CWideString *)FUN_0099bf30(pwVar3);
  CWideString::operator=((CWideString *)local_10,pCVar1);
  CCharString::~CCharString(local_c);
  CCharString::~CCharString(local_8);
  CCharString::~CCharString((CCharString *)local_4);
  CWideString::CWideString(in_ECX,(CWideString *)local_10);
  CCharString::~CCharString(local_10);
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
  double dVar14;
  float fVar15;
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
  
  dVar14 = GFGetTime();
  *(float *)(param_1 + 0xb8) = (float)dVar14;
  dVar14 = GFGetTime();
  *(float *)(param_1 + 0xbc) = (float)dVar14;
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
       (fVar15 = CStopWatch::GetElapsedSeconds((CStopWatch *)(param_1 + 0x120)),
       _DAT_012305a0 <= fVar15)) {
      CStopWatch::Stop((CStopWatch *)(param_1 + 0x120));
    }
    CNewFrontendGameComponent::Input(param_1);
    CNewFrontendGameComponent::Update(param_1);
    CEngineInterpolationInfo::CEngineInterpolationInfo(aCStack_8c);
    CNewFrontendGameComponent::UpdateInterpolation(param_1,aCStack_8c);
    CNewFrontendGameComponent::Draw(param_1,aCStack_8c);
    fVar15 = CStopWatch::GetElapsedSeconds((CStopWatch *)(param_1 + 0x100));
    this_04 = extraout_ECX_00;
    if ((_DAT_01230d74 <= fVar15) &&
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

 