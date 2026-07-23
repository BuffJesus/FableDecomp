INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1034 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2386 ms) (AnalyzeHeadless)  
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
INFO  REPORT: Execute script: DecompFuncs.java '0xa910e0' '0xa91560' '0xa8eda0'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== CalculateVertexSize @ 00a910e0 ====================

/* [bsim sim=0.612966731118237 <- ego_r]
   public: static unsigned long __fastcall C3DPrimitive2::CalculateVertexSize(unsigned
   long,bool,bool) */

ulong __fastcall C3DPrimitive2::CalculateVertexSize(ulong param_1,bool param_2,bool param_3)

{
  ulong extraout_EAX;
  ulong extraout_EAX_00;
  undefined **local_1c;
  int local_18;
  
  if (param_3) {
    return 0x24;
  }
  if (param_2) {
    ::CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen
              ((CAIStateGroup_HoverHornetQueen *)&local_1c);
    local_1c = &PTR_ReleaseAll_0129dfb0;
    N3DPrimitive::CVertexShaderInputAnimated::Initialise
              ((CVertexShaderInputAnimated *)&local_1c,param_1,true);
    CPlayerManager::PrepareForLoad((CPlayerManager *)(local_18 + 4));
    CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip((CPatchTesselationEdgeStrip *)&local_1c)
    ;
    return extraout_EAX;
  }
  ::CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen
            ((CAIStateGroup_HoverHornetQueen *)&local_1c);
  local_1c = &PTR_ReleaseAll_0129dfb8;
  N3DPrimitive::CVertexShaderInputStatic::Initialise
            ((CVertexShaderInputStatic *)&local_1c,param_1,true);
  CPlayerManager::PrepareForLoad((CPlayerManager *)(local_18 + 4));
  CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip((CPatchTesselationEdgeStrip *)&local_1c);
  return extraout_EAX_00;
}


==================== LoadVertexBuffer @ 00a91560 ====================

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


==================== SetupVertexBuffer @ 00a8eda0 ====================

/* [bsim sim=1.0 <- ego_r]
   private: bool __thiscall C3DPrimitive2::SetupVertexBuffer(enum EVertexType,unsigned long,unsigned
   long,unsigned long) */

bool __thiscall
C3DPrimitive2::SetupVertexBuffer
          (C3DPrimitive2 *this,EVertexType param_1,ulong param_2,ulong param_3,ulong param_4)

{
  undefined4 *puVar1;
  bool bVar2;
  CBase *this_00;
  ulong uVar3;
  EVertexBufferPool EVar4;
  
  this_00 = ::operator_new(0x34);
  if (this_00 == (CBase *)0x0) {
    this_00 = (CBase *)0x0;
  }
  else {
    NHeroInformationScreens::CBase::CBase(this_00);
    *(undefined4 *)(this_00 + 8) = 0;
    *(undefined4 *)(this_00 + 0xc) = 0;
    *(undefined4 *)(this_00 + 0x14) = 0x3f800000;
    *(undefined4 *)(this_00 + 0x18) = 0x3f800000;
    *(undefined4 *)(this_00 + 0x1c) = 0x3f800000;
    *(undefined4 *)(this_00 + 0x20) = 0x3f800000;
    *(undefined4 *)(this_00 + 0x24) = 0;
    *(undefined4 *)(this_00 + 0x28) = 0;
    *(undefined4 *)(this_00 + 0x2c) = 0;
    *(undefined4 *)(this_00 + 0x30) = 0;
    *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0129ba20;
    *(undefined4 *)(this_00 + 4) = 0;
    this_00[0x10] = (CBase)0x0;
  }
  puVar1 = *(undefined4 **)(this + 0x30);
  *(CBase **)(this + 0x30) = this_00;
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)(1);
  }
  uVar3 = 8;
  if (param_3 == 0) {
    param_3 = 1;
  }
  EVar4 = 1;
  if ((param_4 & 1) != 0) {
    uVar3 = 0x18;
    EVar4 = 2;
  }
  bVar2 = CVertexBufferWin32::Initialise
                    (*(CVertexBufferWin32 **)(this + 0x30),param_1,param_3,param_2,uVar3,EVar4);
  return bVar2;
}


