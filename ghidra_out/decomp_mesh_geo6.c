INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1043 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2452 ms) (AnalyzeHeadless)  
INFO  Class searcher loaded 60 extension points (20 false positives) (ClassSearcher)  
INFO  HEADLESS Script Paths:
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SwiftDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\GnuDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BytePatterns\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\JVM\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SystemEmulation\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\8051\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FileFormats\ghidra_scripts
    D:\Documents\FableTLC\tools\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BSim\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\VersionTracking\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PDB\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\WildcardAssembler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\PIC\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Base\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Extensions\XEXLoaderWV\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\Cornelio\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\DATA\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Decompiler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\Atmel\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PyGhidra\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FunctionID\ghidra_scripts (HeadlessAnalyzer)  
INFO  HEADLESS: execution starts (HeadlessAnalyzer)  
INFO  Opening existing project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessAnalyzer)  
INFO  Opening project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessProject)  
INFO  REPORT: Processing read-only project file: /Fable.exe (HeadlessAnalyzer)  
INFO  REPORT: Execute script: DecompFuncs.java '0xa2f2d0' '0xabf7b0'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== LoadBinary @ 00a2f2d0 ====================

/* [bsim sim=0.7578873937894068 <- ego_r]
   public: bool __thiscall C3DMeshStats::LoadBinary(class CMemoryDataInputStream &) */

bool __thiscall C3DMeshStats::LoadBinary(C3DMeshStats *this,CMemoryDataInputStream *param_1)

{
  CThingCreatureBase *this_00;
  CMemoryDataInputStream *this_01;
  bool bVar1;
  CCharString *pCVar2;
  undefined4 *extraout_EAX;
  uint uVar3;
  uint uVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  C3DMeshStats *pCVar5;
  uint unaff_retaddr;
  vector<CAnimationEntry_const*,std::allocator<CAnimationEntry_const*>_> *pvVar6;
  int iVar7;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar8;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  CMemoryDataInputStream aCStack_10 [16];
  
  this_01 = param_1;
  ppVar8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&param_1;
  pCVar2 = (CCharString *)(**(code **)(*(int *)param_1 + 0x18))();
  CCharString::operator=((CCharString *)(this + 0xc),pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000000,
             extraout_EDX,ppVar8);
  if (*(int *)(this_01 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,1);
    }
    else {
      unaff_retaddr = CONCAT31((int3)(unaff_retaddr >> 8),**(undefined1 **)(this_01 + 0xc));
      *(undefined1 **)(this_01 + 0xc) = *(undefined1 **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -1;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 1U;
    }
  }
  this[0x7a] = (C3DMeshStats)
               (SUB41(unaff_retaddr,0) !=
               (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0);
  CPersistTraits<CBrainUpdateZone>::TransferBinaryIn
            ((CPersistTraits<CBrainUpdateZone> *)this_01,aCStack_10,(CBrainUpdateZone *)ppVar8);
  *(undefined4 *)(this + 0x10) = *extraout_EAX;
  *(undefined4 *)(this + 0x14) = extraout_EAX[1];
  *(undefined4 *)(this + 0x18) = extraout_EAX[2];
  if (*(int *)(this_01 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,4);
    }
    else {
      unaff_retaddr = **(uint **)(this_01 + 0xc);
      *(uint **)(this_01 + 0xc) = *(uint **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -4;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 4U;
    }
  }
  *(uint *)(this + 0x1c) = unaff_retaddr;
  CPersistTraits<CBrainUpdateZone>::TransferBinaryIn
            ((CPersistTraits<CBrainUpdateZone> *)this_01,(CMemoryDataInputStream *)&uStack_28,
             (CBrainUpdateZone *)ppVar8);
  CPersistTraits<CBrainUpdateZone>::TransferBinaryIn
            ((CPersistTraits<CBrainUpdateZone> *)this_01,(CMemoryDataInputStream *)&uStack_1c,
             (CBrainUpdateZone *)ppVar8);
  *(undefined4 *)(this + 0x20) = uStack_28;
  *(undefined4 *)(this + 0x24) = uStack_24;
  *(undefined4 *)(this + 0x28) = uStack_20;
  *(undefined4 *)(this + 0x2c) = uStack_1c;
  *(undefined4 *)(this + 0x30) = uStack_18;
  *(undefined4 *)(this + 0x34) = uStack_14;
  if (*(int *)(this_01 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,2);
    }
    else {
      unaff_retaddr = (uint)**(ushort **)(this_01 + 0xc);
      *(ushort **)(this_01 + 0xc) = *(ushort **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -2;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 2U;
    }
  }
  uVar4 = unaff_retaddr & 0xffff;
  if (*(int *)(this_01 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,2);
    }
    else {
      unaff_retaddr = (uint)**(ushort **)(this_01 + 0xc);
      *(ushort **)(this_01 + 0xc) = *(ushort **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -2;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 2U;
    }
  }
  uVar3 = unaff_retaddr & 0xffff;
  if (*(int *)(this_01 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,2);
    }
    else {
      unaff_retaddr = (uint)**(ushort **)(this_01 + 0xc);
      *(ushort **)(this_01 + 0xc) = *(ushort **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -2;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 2U;
    }
  }
  *(short *)(this + 0x78) = (short)unaff_retaddr;
  if (*(int *)(this_01 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,2);
    }
    else {
      unaff_retaddr = (uint)**(ushort **)(this_01 + 0xc);
      *(ushort **)(this_01 + 0xc) = *(ushort **)(this_01 + 0xc) + 1;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -2;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 2U;
    }
  }
  if (*(int *)(this_01 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_01 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_01,&stack0x00000000,2);
    }
    else {
      *(int *)(this_01 + 0xc) = *(int *)(this_01 + 0xc) + 2;
      *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -2;
      *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 2U;
    }
  }
  bVar1 = AllocateBuffers(this,uVar4,uVar3,(uint)*(ushort *)(this + 0x78));
  if (bVar1) {
    (**(code **)(*(int *)this_01 + 0x10))(*(undefined4 *)(this + 0x3c),*(int *)(this + 0x40) * 0x14)
    ;
    this[0x44] = (C3DMeshStats)0x0;
    (**(code **)(*(int *)this_01 + 0x10))(*(undefined4 *)(this + 0x4c),*(int *)(this + 0x50) * 0x38)
    ;
    pvVar6 = *(vector<CAnimationEntry_const*,std::allocator<CAnimationEntry_const*>_> **)
              (this + 0x74);
    this[0x44] = (C3DMeshStats)0x0;
    (**(code **)(*(int *)this_01 + 0x10))(pvVar6,*(undefined2 *)(this + 0x78));
    this_00 = (CThingCreatureBase *)(this + 0x58);
    CThingCreatureBase::GetAnimsWithNameStartingWith
              (this_00,(CCharString *)(unaff_retaddr & 0xffff),pvVar6);
    uVar4 = 0;
    if (*(int *)(this + 0x5c) - *(int *)this_00 >> 4 != 0) {
      iVar7 = 0;
      do {
        if (*(int *)(this_01 + 4) + 4U < 0x80000000) {
          if (*(int *)(this_01 + 0x14) < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow
                      ((CDataInputStream *)this_01,(uchar *)&uStack_18,4);
          }
          else {
            uStack_18 = **(undefined4 **)(this_01 + 0xc);
            *(undefined4 **)(this_01 + 0xc) = *(undefined4 **)(this_01 + 0xc) + 1;
            *(int *)(this_01 + 0x14) = *(int *)(this_01 + 0x14) + -4;
            *(uint *)(this_01 + 4) = *(int *)(this_01 + 4) + 4U;
          }
        }
        *(undefined4 *)(iVar7 + *(int *)this_00) = uStack_18;
        bVar1 = C3DMeshVolume::LoadBinary
                          ((C3DMeshVolume *)(iVar7 + 4 + *(int *)this_00),this_01,
                           *(CMovableResourceMemoryPool **)(this + 4));
        if (!bVar1) {
          return false;
        }
        uVar4 = uVar4 + 1;
        iVar7 = iVar7 + 0x10;
      } while (uVar4 < (uint)(*(int *)(this + 0x5c) - *(int *)this_00 >> 4));
    }
    this[0x65] = (C3DMeshStats)0x0;
    pCVar5 = this + 0x68;
    FUN_00a34df0(ppVar8);
    uVar4 = 0;
    if (*(int *)(this + 0x6c) - *(int *)pCVar5 >> 6 != 0) {
      iVar7 = 0;
      do {
        C3DGeneratorObject::LoadBinary((C3DGeneratorObject *)(*(int *)pCVar5 + iVar7),this_01);
        uVar4 = uVar4 + 1;
        iVar7 = iVar7 + 0x40;
      } while (uVar4 < (uint)(*(int *)(this + 0x6c) - *(int *)pCVar5 >> 6));
    }
    return true;
  }
  return false;
}


==================== C3DMeshMaterial @ 00abf7b0 ====================

/* [bsim sim=0.8321016752854721 <- ego_r]
   public: __thiscall C3DMeshMaterial::C3DMeshMaterial(void) */

C3DMeshMaterial * __thiscall C3DMeshMaterial::C3DMeshMaterial(C3DMeshMaterial *this)

{
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129edc0;
  *(undefined4 *)(this + 4) = 0xffffffff;
  CCharString::CCharString((CCharString *)(this + 8));
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  this[0x28] = (C3DMeshMaterial)0x0;
  this[0x29] = (C3DMeshMaterial)0x0;
  this[0x2a] = (C3DMeshMaterial)0x0;
  this[0x2b] = (C3DMeshMaterial)0x0;
  *(undefined4 *)(this + 0x2c) = 0;
  return this;
}


