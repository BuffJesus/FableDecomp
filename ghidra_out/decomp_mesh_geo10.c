INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1063 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2483 ms) (AnalyzeHeadless)  
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
INFO  REPORT: Execute script: DecompFuncs.java '0xab3870'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== GenerateD3DDeclaration @ 00ab3870 ====================

/* WARNING: Removing unreachable block (ram,0x00ab3904) */
/* WARNING: Removing unreachable block (ram,0x00ab3ab6) */
/* [bsim sim=0.7353525863294109 <- ego_r]
   private: void __thiscall CVertexShaderInput::GenerateD3DDeclaration(void) */

void __thiscall CVertexShaderInput::GenerateD3DDeclaration(CVertexShaderInput *this)

{
  int *piVar1;
  CVertexShaderInput *pCVar2;
  undefined1 *puVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  short sVar9;
  int *piVar10;
  uint uVar11;
  int iVar12;
  short sVar13;
  uint uVar14;
  uint local_1c;
  int local_18;
  uint local_10;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar12 = *(int *)(this + 8) - *(int *)(this + 4);
  iVar5 = iVar12 >> 0x1f;
  uVar11 = 0;
  uVar14 = 0;
  sVar13 = 0;
  if (iVar12 / 0x14 + iVar5 != iVar5) {
    piVar10 = (int *)(*(int *)(this + 4) + 8);
    do {
      piVar1 = piVar10 + 1;
      iVar5 = *piVar10;
      piVar10 = piVar10 + 5;
      uVar14 = uVar14 + 1;
      uVar11 = uVar11 + 1 + (*piVar1 - iVar5 >> 3);
    } while (uVar14 < (uint)((*(int *)(this + 8) - *(int *)(this + 4)) / 0x14));
  }
  pCVar2 = this + 0x10;
  local_8 = 0;
  local_4 = 0;
  uVar14 = *(int *)(this + 0x14) - *(int *)(this + 0x10) >> 3;
  if (uVar11 < uVar14) {
    *(uint *)(this + 0x14) = *(int *)(this + 0x10) + uVar11 * 8;
  }
  else {
    FUN_00ab4260(*(int *)(this + 0x14),uVar11 - uVar14,&local_8);
  }
  iVar5 = *(int *)(this + 8) - *(int *)(this + 4) >> 0x1f;
  iVar12 = 0;
  local_1c = 0;
  if ((*(int *)(this + 8) - *(int *)(this + 4)) / 0x14 + iVar5 != iVar5) {
    local_18 = 0;
    do {
      iVar5 = *(int *)(this + 4);
      uVar11 = *(int *)(iVar5 + 0xc + local_18) - *(int *)(iVar5 + 8 + local_18) >> 3;
      local_10 = 0;
      if (uVar11 != 0) {
        do {
          puVar3 = (undefined1 *)(*(int *)(iVar5 + local_18 + 8) + local_10 * 8);
          iVar6 = *(int *)(puVar3 + 4);
          iVar7 = *(int *)(&DAT_0129e9cc + iVar6 * 4);
          if (iVar7 == -1) {
            *(int *)(this + 0x14) = *(int *)pCVar2;
            return;
          }
          uVar4 = *puVar3;
          iVar8 = *(int *)pCVar2;
          *(uint *)(iVar8 + iVar12 * 8) = CONCAT22(sVar13,(short)local_1c);
          local_4._0_3_ = CONCAT12(5,(ushort)(byte)iVar7);
          *(uint *)(iVar8 + 4 + iVar12 * 8) = CONCAT13(uVar4,(undefined3)local_4);
          iVar12 = iVar12 + 1;
          switch(iVar6) {
          case 0:
          case 4:
          case 5:
          case 7:
          case 9:
          case 0xc:
          case 0xd:
            sVar9 = 4;
            break;
          case 1:
          case 6:
          case 0xb:
            sVar9 = 8;
            break;
          case 2:
            sVar9 = 0xc;
            break;
          case 3:
            sVar9 = 0x10;
            break;
          case 8:
            sVar9 = 2;
            break;
          case 10:
            sVar9 = 6;
            break;
          default:
            sVar9 = 0;
          }
          sVar13 = sVar13 + sVar9;
          local_10 = local_10 + 1;
        } while (local_10 < uVar11);
      }
      local_18 = local_18 + 0x14;
      local_1c = local_1c + 1;
    } while (local_1c < (uint)((*(int *)(this + 8) - *(int *)(this + 4)) / 0x14));
  }
  iVar5 = *(int *)pCVar2;
  *(undefined4 *)(iVar5 + iVar12 * 8) = 0xff;
  *(undefined4 *)(iVar5 + 4 + iVar12 * 8) = 0x11;
  return;
}


