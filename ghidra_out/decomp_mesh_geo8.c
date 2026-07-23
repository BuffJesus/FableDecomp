INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1027 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2359 ms) (AnalyzeHeadless)  
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
INFO  REPORT: Execute script: DecompFuncs.java '0xa90560' '0xa90630' '0xa8ea70'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== Initialise @ 00a90560 ====================

/* [bsim sim=0.8502292064956719 <- ego_r]
   public: void __thiscall N3DPrimitive::CVertexShaderInputStatic::Initialise(unsigned long,bool) */

void __thiscall
N3DPrimitive::CVertexShaderInputStatic::Initialise
          (CVertexShaderInputStatic *this,ulong param_1,bool param_2)

{
  undefined4 uVar1;
  CAIStateGroup_WhisperPotionCombat local_10 [4];
  void *local_c;
  
  ::CAIStateGroup_WhisperPotionCombat::CAIStateGroup_WhisperPotionCombat(local_10);
  if ((param_1 & 4) == 0) {
    FUN_00ab3810(0,2);
    FUN_00ab3810(1,2);
    uVar1 = 1;
  }
  else {
    uVar1 = 0xd;
    if ((param_1 & 0x10) != 0) {
      uVar1 = 2;
    }
    FUN_00ab3810(0,uVar1);
    FUN_00ab3810(1,0xc);
    uVar1 = 7;
  }
  FUN_00ab3810(2,uVar1);
  if ((param_1 & 2) != 0) {
    if ((param_1 & 4) == 0) {
      uVar1 = 3;
    }
    else {
      uVar1 = 0xb;
    }
    FUN_00ab3810(3,uVar1);
  }
  FUN_00ab3b40(0,local_10,!param_2);
  if (local_c != (void *)0x0) {
    free(local_c);
  }
  NHeroInformationScreens::CBase::CBase((CBase *)local_10);
  return;
}


==================== Initialise @ 00a90630 ====================

/* [bsim sim=0.8717016169380799 <- ego_r]
   public: void __thiscall N3DPrimitive::CVertexShaderInputAnimated::Initialise(unsigned long,bool)
    */

void __thiscall
N3DPrimitive::CVertexShaderInputAnimated::Initialise
          (CVertexShaderInputAnimated *this,ulong param_1,bool param_2)

{
  uint uVar1;
  undefined4 uVar2;
  CAIStateGroup_WhisperPotionCombat local_10 [4];
  void *local_c;
  
  ::CAIStateGroup_WhisperPotionCombat::CAIStateGroup_WhisperPotionCombat(local_10);
  uVar1 = param_1 & 4;
  if ((uVar1 == 0) || ((param_1 & 0x10) != 0)) {
    uVar2 = 2;
  }
  else {
    uVar2 = 0xd;
  }
  FUN_00ab3810(0,uVar2);
  FUN_00ab3810(1,4);
  FUN_00ab3810(2,4);
  if (uVar1 == 0) {
    uVar2 = 2;
  }
  else {
    uVar2 = 0xc;
  }
  FUN_00ab3810(3,uVar2);
  if (uVar1 == 0) {
    uVar2 = 1;
  }
  else {
    uVar2 = 7;
  }
  FUN_00ab3810(4,uVar2);
  if ((param_1 & 2) != 0) {
    if (uVar1 == 0) {
      uVar2 = 3;
    }
    else {
      uVar2 = 0xb;
    }
    FUN_00ab3810(5,uVar2);
  }
  FUN_00ab3b40(0,local_10,!param_2);
  if (local_c != (void *)0x0) {
    free(local_c);
  }
  NHeroInformationScreens::CBase::CBase((CBase *)local_10);
  return;
}


==================== LoadBinary @ 00a8ea70 ====================

/* WARNING: Removing unreachable block (ram,0x00a8eaf5) */
/* WARNING: Removing unreachable block (ram,0x00a8eae9) */
/* [bsim sim=0.9999999999999998 <- ego_r]
   public: void __thiscall N3DPrimitive::CStaticBlock::LoadBinary(class CMemoryDataInputStream &) */

void __thiscall
N3DPrimitive::CStaticBlock::LoadBinary(CStaticBlock *this,CMemoryDataInputStream *param_1)

{
  undefined4 uVar1;
  CMemoryDataInputStream *this_00;
  
  this_00 = param_1;
  CBaseBlock::LoadBinary((CBaseBlock *)this,param_1);
  if (0x7fffffff < *(int *)(this_00 + 4) + 4U) {
    *(CMemoryDataInputStream **)(this + 0x1c) = param_1;
    return;
  }
  if (*(int *)(this_00 + 0x14) < 4) {
    CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    *(CMemoryDataInputStream **)(this + 0x1c) = param_1;
    return;
  }
  uVar1 = **(undefined4 **)(this_00 + 0xc);
  *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
  *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
  *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
  *(undefined4 *)(this + 0x1c) = uVar1;
  return;
}


