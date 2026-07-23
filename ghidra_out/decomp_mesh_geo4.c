INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1129 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2636 ms) (AnalyzeHeadless)  
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
INFO  REPORT: Execute script: DecompFuncs.java '0xa8ad40' '0xa92900' '0xa932c0' '0xa92870' '0xa63240' '0xa8e8a0' '0xa8eb10'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== LoadBinary @ 00a8ad40 ====================

/* [bsim sim=0.7468157842343001 <- ego_r]
   public: bool __thiscall C3DMesh2::LoadBinary(class CMemoryDataInputStream &,class
   CMovableResourceMemoryPool *,class CGraphicDataBank const *,bool) */

bool __thiscall
C3DMesh2::LoadBinary
          (C3DMesh2 *this,CMemoryDataInputStream *param_1,CMovableResourceMemoryPool *param_2,
          CGraphicDataBank *param_3,bool param_4)

{
  CMemoryDataInputStream *this_00;
  bool bVar1;
  char *pcVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined4 unaff_ESI;
  uchar *puVar6;
  undefined4 *puVar7;
  CGraphicDataBank *unaff_EDI;
  uint uVar8;
  uchar *puVar9;
  C3DMesh2 *pCVar10;
  CHandleBasedMemoryPool *this_01;
  char cVar11;
  
  this_00 = param_1;
  this[0xe5] = (C3DMesh2)0x1;
  *(CMovableResourceMemoryPool **)(this + 4) = param_2;
  bVar1 = C3DMeshStats::LoadBinary((C3DMeshStats *)this,param_1);
  if (!bVar1) {
    this[0xe5] = (C3DMesh2)0x0;
    return false;
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x88) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x90) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x98) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0xac) = param_1;
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0xe4] = (C3DMesh2)((char)param_1 != '\0');
  if (*(int *)(this_00 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,2);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT22(param_1._2_2_,**(undefined2 **)(this_00 + 0xc));
      *(undefined2 **)(this_00 + 0xc) = *(undefined2 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -2;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 2U;
    }
  }
  uVar8 = (uint)param_1 & 0xffff;
  if (*(int *)(this_00 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,2);
    }
    else {
      param_1 = (CMemoryDataInputStream *)(uint)**(ushort **)(this_00 + 0xc);
      *(ushort **)(this_00 + 0xc) = *(ushort **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -2;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 2U;
    }
  }
  uVar5 = (uint)param_1 & 0xffff;
  if (param_4 != false) {
    *(undefined4 *)(this + 0x90) = 0;
  }
  param_1 = (CMemoryDataInputStream *)
            (*(int *)(this + 0x98) * 0xac + *(int *)(this + 0x88) * 0x30 +
            *(int *)(this + 0x90) * 0xa0);
  if (((((param_1 < (CMemoryDataInputStream *)0x80000000) &&
        (uVar8 = uVar8 * 0x20 + uVar5 * 0x30, uVar8 < 0x80000000)) &&
       (uVar5 = *(int *)(this + 0xac) + *(int *)(this + 0x98) * 2, uVar5 < 0x80000000)) &&
      ((bVar1 = CMovableResourceMemoryPool::AllocateResource
                          (*(CMovableResourceMemoryPool **)(this + 4),(long *)(this + 0x7c),
                           (CMovableResource *)this,(ulong)param_1,0), bVar1 &&
       (bVar1 = CMovableResourceMemoryPool::AllocateResource
                          (*(CMovableResourceMemoryPool **)(this + 4),(long *)(this + 0x80),
                           (CMovableResource *)this,uVar8,0), bVar1)))) &&
     ((uVar5 == 0 ||
      (bVar1 = CMovableResourceMemoryPool::AllocateResource
                         (*(CMovableResourceMemoryPool **)(this + 4),(long *)(this + 0x84),
                          (CMovableResource *)this,uVar5,0), bVar1)))) {
    InitialisePointers(this,*(ulong *)(this + 0x98),(ulong)param_1);
    InitialiseBoneNamePointers(this,uVar5);
    uVar8 = *(uint *)(this + 0x98);
    uVar5 = uVar8 * 2;
    if ((0 < (int)uVar5) && (*(int *)(this_00 + 4) + uVar5 < 0x80000000)) {
      if (uVar5 - *(int *)(this_00 + 0x14) == 0 || (int)uVar5 < *(int *)(this_00 + 0x14)) {
        puVar6 = *(uchar **)(this_00 + 0xc);
        puVar9 = *(uchar **)(this + 0xa4);
        for (uVar3 = (uVar8 & 0x7fffffff) >> 1; uVar3 != 0; uVar3 = uVar3 - 1) {
          *(undefined4 *)puVar9 = *(undefined4 *)puVar6;
          puVar6 = puVar6 + 4;
          puVar9 = puVar9 + 4;
        }
        for (uVar3 = uVar5 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
          *puVar9 = *puVar6;
          puVar6 = puVar6 + 1;
          puVar9 = puVar9 + 1;
        }
        *(uint *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + uVar5;
        *(uint *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + uVar8 * -2;
        *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + uVar5;
      }
      else {
        CDataInputStream::ReadWithSrcChunkOverflow
                  ((CDataInputStream *)this_00,*(uchar **)(this + 0xa4),uVar5);
      }
    }
    this_01 = *(CHandleBasedMemoryPool **)(this + 0xac);
    (**(code **)(*(int *)this_00 + 0x10))();
    pcVar2 = *(char **)(this + 0xa8);
    uVar8 = 0;
    if (*(int *)(this + 0x98) != 0) {
      do {
        *(short *)(*(int *)(this + 0xa4) + uVar8 * 2) = (short)pcVar2 - *(short *)(this + 0xa8);
        do {
          cVar11 = *pcVar2;
          pcVar2 = pcVar2 + 1;
        } while (cVar11 != '\0');
        uVar8 = uVar8 + 1;
      } while (uVar8 < *(uint *)(this + 0x98));
    }
    (**(code **)(*(int *)this_00 + 0x10))();
    (**(code **)(*(int *)this_00 + 0x10))(*(undefined4 *)(this + 0xa0),*(int *)(this + 0x98) * 0x30)
    ;
    (**(code **)(*(int *)this_00 + 0x10))(*(undefined4 *)(this + 0xe0),*(int *)(this + 0x98) << 6);
    if (*(int *)(this_00 + 4) + 0x30U < 0x80000000) {
      if (*(int *)(this_00 + 0x14) < 0x30) {
        CDataInputStream::ReadWithSrcChunkOverflow
                  ((CDataInputStream *)this_00,(uchar *)(this + 0xb0),0x30);
      }
      else {
        puVar7 = *(undefined4 **)(this_00 + 0xc);
        pCVar10 = this + 0xb0;
        for (iVar4 = 0xc; iVar4 != 0; iVar4 = iVar4 + -1) {
          *(undefined4 *)pCVar10 = *puVar7;
          puVar7 = puVar7 + 1;
          pCVar10 = pCVar10 + 4;
        }
        *(int *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + 0x30;
        *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -0x30;
        *(int *)(this_00 + 4) = *(int *)(this_00 + 4) + 0x30;
      }
    }
    cVar11 = (char)unaff_ESI;
    uVar8 = 0;
    if (*(int *)(this + 0x88) != 0) {
      iVar4 = 0;
      do {
        C3DMeshMaterial::LoadBinary
                  ((C3DMeshMaterial *)(*(int *)(this + 0x8c) + iVar4),this_00,unaff_EDI);
        cVar11 = (char)unaff_ESI;
        uVar8 = uVar8 + 1;
        iVar4 = iVar4 + 0x30;
      } while (uVar8 < *(uint *)(this + 0x88));
    }
    if (cVar11 == '\0') {
      CHandleBasedMemoryPool::GetPointerFromHandle(this_01,*(long *)(this + 0x80));
      CHandleBasedMemoryPool::Lock(*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x7c));
      CHandleBasedMemoryPool::Lock(*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x80));
      uVar8 = 0;
      if (*(int *)(this + 0x90) != 0) {
        iVar4 = 0;
        do {
          bVar1 = C3DPrimitive2::LoadBinary
                            ((C3DPrimitive2 *)(*(int *)(this + 0x94) + iVar4),this_00,0,
                             (uchar **)&stack0xffffffdc,*(CMovableResourceMemoryPool **)(this + 4));
          if (!bVar1) {
            CHandleBasedMemoryPool::Unlock
                      (*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x80));
            CHandleBasedMemoryPool::Unlock
                      (*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x7c));
            goto LAB_00a8b385;
          }
          uVar8 = uVar8 + 1;
          iVar4 = iVar4 + 0xa0;
        } while (uVar8 < *(uint *)(this + 0x90));
      }
      CHandleBasedMemoryPool::Unlock(*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x80));
      CHandleBasedMemoryPool::Unlock(*(CHandleBasedMemoryPool **)(this + 4),*(long *)(this + 0x7c));
    }
    this[0xe5] = (C3DMesh2)0x0;
    return true;
  }
LAB_00a8b385:
  this[0xe5] = (C3DMesh2)0x0;
  return false;
}


==================== LoadBinary @ 00a92900 ====================

/* [bsim sim=0.7836406936547198 <- ego_r]
   public: bool __thiscall C3DPrimitive2::LoadBinary(class CMemoryDataInputStream &,unsigned
   long,unsigned char * *,class CMovableResourceMemoryPool *) */

bool __thiscall
C3DPrimitive2::LoadBinary
          (C3DPrimitive2 *this,CMemoryDataInputStream *param_1,ulong param_2,uchar **param_3,
          CMovableResourceMemoryPool *param_4)

{
  CStaticBlock *pCVar1;
  undefined4 *puVar2;
  int *piVar3;
  CMemoryDataInputStream *this_00;
  bool bVar4;
  undefined4 *extraout_EAX;
  uchar *puVar5;
  ulong uVar6;
  CIndexBuffer *this_01;
  undefined4 uVar7;
  ulong uVar8;
  CMemoryDataInputStream *pCVar9;
  CCountedPointer<CGuiVarTransferBase> *pCVar10;
  CBase *this_02;
  undefined4 *puVar11;
  int iVar12;
  int iVar13;
  EIndexBufferPool EVar14;
  uint uVar15;
  CBrainUpdateZone *unaff_EDI;
  uchar **ppuVar16;
  CMemoryDataInputStream **ppCVar17;
  uint local_34;
  EVertexType local_30;
  ulong local_2c [3];
  CMemoryDataInputStream *local_20 [8];
  
  this_00 = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&param_1,4);
    }
    else {
      pCVar9 = (CMemoryDataInputStream *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
      param_1 = pCVar9;
    }
  }
  *(CMemoryDataInputStream **)(this + 4) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 8) = param_1;
  CPersistTraits<CBrainUpdateZone>::TransferBinaryIn
            ((CPersistTraits<CBrainUpdateZone> *)this_00,(CMemoryDataInputStream *)local_2c,
             unaff_EDI);
  *(undefined4 *)(this + 0xc) = *extraout_EAX;
  *(undefined4 *)(this + 0x10) = extraout_EAX[1];
  *(undefined4 *)(this + 0x14) = extraout_EAX[2];
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x18) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x1c) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x40) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x48) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x58) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x5c) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x60) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&local_34,4);
    }
    else {
      local_34 = **(uint **)(this_00 + 0xc);
      *(uint **)(this_00 + 0xc) = *(uint **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(uint *)(this + 0x68) = local_34 & 0xfffffffb;
  this[0x65] = (C3DPrimitive2)0x0;
  if (*(int *)(this + 0x48) != 0) {
    this[0x65] = (C3DPrimitive2)0x1;
  }
  if ((local_34 & 2) != 0) {
    this[0x65] = (C3DPrimitive2)((byte)this[0x65] | 2);
  }
  if ((local_34 & 0x10) != 0) {
    this[0x65] = (C3DPrimitive2)((byte)this[0x65] | 4);
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x40) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  ppuVar16 = param_3;
  *(CMemoryDataInputStream **)(this + 0x48) = param_1;
  pCVar1 = (CStaticBlock *)*param_3;
  *(CStaticBlock **)(this + 0x44) = pCVar1;
  GFExplicitNew<N3DPrimitive::CStaticBlock>(pCVar1,*(ulong *)(this + 0x40));
  *ppuVar16 = *ppuVar16 + *(int *)(this + 0x40) * 0x20;
  uVar15 = 0;
  if (*(int *)(this + 0x40) != 0) {
    param_1 = (CMemoryDataInputStream *)0x0;
    do {
      N3DPrimitive::CStaticBlock::LoadBinary
                ((CStaticBlock *)(param_1 + *(int *)(this + 0x44)),this_00);
      uVar15 = uVar15 + 1;
      param_1 = param_1 + 0x20;
    } while (uVar15 < *(uint *)(this + 0x40));
  }
  iVar12 = *(int *)(this + 0x48);
  puVar5 = *ppuVar16;
  uVar15 = 0;
  *(uchar **)(this + 0x4c) = puVar5;
  if (iVar12 != 0) {
    puVar5 = puVar5 + 8;
    do {
      if (puVar5 != &DAT_00000008) {
        puVar5[-0xffffffff00000008] = '\0';
        puVar5[-0xffffffff00000007] = '\0';
        puVar5[-0xffffffff00000006] = '\0';
        puVar5[-0xffffffff00000005] = '\0';
        puVar5[-0xffffffff00000004] = '\0';
        puVar5[-0xffffffff00000003] = '\0';
        puVar5[-0xffffffff00000002] = '\0';
        puVar5[-0xffffffff00000001] = '\0';
        *puVar5 = '\0';
        puVar5[1] = '\0';
        puVar5[2] = '\0';
        puVar5[0x24] = '\0';
        puVar5[0x25] = '\0';
        puVar5[0x26] = '\0';
        puVar5[0x27] = '\0';
      }
      puVar5 = puVar5 + 0x30;
      iVar12 = iVar12 + -1;
    } while (iVar12 != 0);
  }
  *ppuVar16 = *ppuVar16 + *(int *)(this + 0x48) * 0x30;
  if (*(int *)(this + 0x48) != 0) {
    iVar12 = 0;
    do {
      N3DPrimitive::CAnimatedBlock::LoadBinary
                ((CAnimatedBlock *)(*(int *)(this + 0x4c) + iVar12),this_00);
      uVar15 = uVar15 + 1;
      iVar12 = iVar12 + 0x30;
    } while (uVar15 < *(uint *)(this + 0x48));
  }
  ppuVar16 = *(uchar ***)(this + 8);
  param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,*(int *)(this + 0x48) != 0);
  if ((int)ppuVar16 < 2) {
    ppuVar16 = (uchar **)0x1;
  }
  iVar12 = *(int *)(this_00 + 4);
  local_20[0] = (CMemoryDataInputStream *)0x3f800000;
  local_20[1] = (CMemoryDataInputStream *)0x3f800000;
  local_20[2] = (CMemoryDataInputStream *)0x3f800000;
  local_20[3] = (CMemoryDataInputStream *)0x3f800000;
  local_20[4] = (CMemoryDataInputStream *)0x0;
  local_20[5] = (CMemoryDataInputStream *)0x0;
  local_20[6] = (CMemoryDataInputStream *)0x0;
  local_20[7] = (CMemoryDataInputStream *)0x0;
  param_3 = ppuVar16;
  if (iVar12 + 0x20U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 0x20) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)local_20,0x20)
      ;
    }
    else {
      puVar2 = *(undefined4 **)(this_00 + 0xc);
      puVar11 = puVar2;
      ppCVar17 = local_20;
      for (iVar13 = 8; iVar13 != 0; iVar13 = iVar13 + -1) {
        *ppCVar17 = (CMemoryDataInputStream *)*puVar11;
        puVar11 = puVar11 + 1;
        ppCVar17 = ppCVar17 + 1;
      }
      *(undefined4 **)(this_00 + 0xc) = puVar2 + 8;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -0x20;
      *(uint *)(this_00 + 4) = iVar12 + 0x20U;
    }
  }
  uVar15 = (uint)param_3 >> 8;
  param_3 = (uchar **)((uint)param_3 & 0xffffff00);
  if (0 < *(int *)(this + 8)) {
    param_3 = (uchar **)CONCAT31((int3)uVar15,1);
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)local_2c,4);
    }
    else {
      local_2c[0] = **(ulong **)(this_00 + 0xc);
      *(ulong **)(this_00 + 0xc) = *(ulong **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&local_30,4);
    }
    else {
      local_30 = **(EVertexType **)(this_00 + 0xc);
      *(EVertexType **)(this_00 + 0xc) = *(EVertexType **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  uVar6 = CalculateVertexSize(*(ulong *)(this + 0x68),param_1._0_1_,SUB41(param_3,0));
  bVar4 = SetupVertexBuffer(this,local_30,uVar6,*(int *)(this + 0x58) * (int)ppuVar16,
                            *(ulong *)(this + 0x68));
  if (!bVar4) {
    return false;
  }
  LoadVertexBuffer(this,this_00,uVar6,local_2c[0],local_34,(CVertexCompressionParams *)local_20);
  uVar6 = *(int *)(this + 0x60) * (int)ppuVar16 * 2;
  this_01 = ::operator_new(0x14);
  if (this_01 == (CIndexBuffer *)0x0) {
    uVar7 = 0;
  }
  else {
    uVar7 = ::CIndexBuffer::CIndexBuffer(this_01);
  }
  puVar2 = *(undefined4 **)(this + 0x34);
  *(undefined4 *)(this + 0x34) = uVar7;
  if (puVar2 != (undefined4 *)0x0) {
    (**(code **)*puVar2)(1);
  }
  EVar14 = 1;
  uVar8 = 8;
  if ((param_2 & 1) != 0) {
    uVar8 = 0x18;
    EVar14 = 2;
  }
  ::CIndexBuffer::Initialise(*(CIndexBuffer **)(this + 0x34),uVar6,uVar8,0x65,EVar14,4);
  puVar5 = ::CIndexBuffer::Lock(*(CIndexBuffer **)(this + 0x34),0,0,0);
  (**(code **)(*(int *)this_00 + 0x10))(puVar5,uVar6);
  CVertexBufferWin32::DoUnlock(*(CVertexBufferWin32 **)(this + 0x34));
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow
                ((CDataInputStream *)this_00,(uchar *)(local_20 + 7),4);
    }
    else {
      local_20[7] = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  iVar12 = *(int *)(this + 0x24);
  local_34 = 0;
  local_30 = 0;
  pCVar9 = (CMemoryDataInputStream *)(iVar12 - *(int *)(this + 0x20) >> 3);
  param_1 = local_20[7];
  if (local_20[7] < pCVar9) {
    pCVar10 = std::
              _Move<CCountedPointer<CGuiVarTransferBase>*,CCountedPointer<class_CGuiVarTransferBase>*>
                        (iVar12,iVar12,*(int *)(this + 0x20) + (int)local_20[7] * 8,local_20 + 7,0);
    std::
    _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
              (pCVar10,*(undefined4 *)(this + 0x24),local_20 + 7);
    *(CCountedPointer<CGuiVarTransferBase> **)(this + 0x24) = pCVar10;
  }
  else {
    FUN_00a97e40(iVar12,(int)local_20[7] - (int)pCVar9,&local_34);
  }
  local_20[7] = (CMemoryDataInputStream *)0x0;
  if (param_1 != (CMemoryDataInputStream *)0x0) {
    do {
      puVar2 = (undefined4 *)(*(int *)(this + 0x20) + (int)local_20[7] * 8);
      this_02 = ::operator_new(0x14);
      if (this_02 == (CBase *)0x0) {
        this_02 = (CBase *)0x0;
      }
      else {
        NHeroInformationScreens::CBase::CBase(this_02);
        *(undefined ***)this_02 = &PTR__vector_deleting_destructor__0129df74;
        *(undefined4 *)(this_02 + 4) = 0;
        *(undefined4 *)(this_02 + 8) = 0;
      }
      piVar3 = (int *)puVar2[1];
      if (piVar3 != (int *)0x0) {
        *piVar3 = *piVar3 + -1;
        if (*(int *)puVar2[1] == 0) {
          (*(code *)((int *)puVar2[1])[1])();
          operator_delete((void *)puVar2[1]);
        }
      }
      puVar2[1] = 0;
      *puVar2 = this_02;
      if (this_02 != (CBase *)0x0) {
        puVar11 = ::operator_new(0xc);
        if (puVar11 == (undefined4 *)0x0) {
          puVar11 = (undefined4 *)0x0;
        }
        else {
          uVar7 = *puVar2;
          *puVar11 = 1;
          puVar11[1] = &LAB_00a93a60;
          puVar11[2] = uVar7;
        }
        puVar2[1] = puVar11;
      }
      bVar4 = C3DClothPrimitive::LoadBinary
                        ((C3DClothPrimitive *)*puVar2,this_00,(CMovableResourceMemoryPool *)param_2)
      ;
      if (!bVar4) {
        pCVar10 = std::
                  _Move<CCountedPointer<CGuiVarTransferBase>*,CCountedPointer<class_CGuiVarTransferBase>*>
                            (*(undefined4 *)(this + 0x24),*(undefined4 *)(this + 0x24),
                             *(undefined4 *)(this + 0x20),local_20 + 7,0);
        std::
        _Destroy_range<std::allocator<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>_>_>
                  (pCVar10,*(undefined4 *)(this + 0x24),local_20 + 7);
        *(CCountedPointer<CGuiVarTransferBase> **)(this + 0x24) = pCVar10;
        break;
      }
      local_20[7] = local_20[7] + 1;
    } while (local_20[7] < param_1);
  }
  *(undefined4 *)(this + 0x38) = 1;
  *(undefined4 *)(this + 0x3c) = 1;
  *(undefined4 *)(this + 0x50) = 1;
  *(undefined4 *)(this + 0x54) = 1;
  this[100] = (C3DPrimitive2)0x1;
  return true;
}


==================== UploadRepeatedBuffers @ 00a932c0 ====================

/* [bsim sim=0.8262740446397677 <- ego_r]
   public: bool __thiscall C3DPrimitive2::UploadRepeatedBuffers(unsigned long,unsigned long) */

bool __thiscall
C3DPrimitive2::UploadRepeatedBuffers(C3DPrimitive2 *this,ulong param_1,ulong param_2)

{
  undefined4 *puVar1;
  bool bVar2;
  
  if ((((byte)this[0x38] & 1) == 0) && (bVar2 = UploadToVertexBufferStatic(this,param_1), !bVar2)) {
    return false;
  }
  if ((((byte)this[0x3c] & 1) == 0) && (bVar2 = UploadToIndexBuffer(this,param_1), !bVar2)) {
    return false;
  }
  if ((param_1 & 8) != 0) {
    puVar1 = *(undefined4 **)(this + 0x2c);
    *(undefined4 *)(this + 0x2c) = 0;
    if (puVar1 != (undefined4 *)0x0) {
      (**(code **)*puVar1)(1);
    }
  }
  this[100] = (C3DPrimitive2)0x1;
  return true;
}


==================== UploadAnimatedBuffers @ 00a92870 ====================

/* [bsim sim=0.8959834392975466 <- ego_r]
   public: bool __thiscall C3DPrimitive2::UploadAnimatedBuffers(unsigned long) */

bool __thiscall C3DPrimitive2::UploadAnimatedBuffers(C3DPrimitive2 *this,ulong param_1)

{
  int *piVar1;
  int *piVar2;
  undefined4 *puVar3;
  int *piVar4;
  bool bVar5;
  uint uVar6;
  int iVar7;
  
  if (((byte)this[0x3c] & 1) == 0) {
    UploadToIndexBuffer(this,param_1);
    *(uint *)(this + 0x3c) = *(uint *)(this + 0x3c) | 1;
  }
  if ((((byte)this[0x38] & 1) == 0) && (bVar5 = UploadToVertexBufferAnimated(this,param_1), !bVar5))
  {
    return false;
  }
  if ((param_1 & 8) != 0) {
    puVar3 = *(undefined4 **)(this + 0x2c);
    iVar7 = 0;
    *(undefined4 *)(this + 0x2c) = 0;
    if (puVar3 != (undefined4 *)0x0) {
      (**(code **)*puVar3)(1);
    }
    uVar6 = 0;
    if (*(int *)(this + 0x48) != 0) {
      do {
        piVar2 = (int *)(*(int *)(this + 0x4c) + 0x2c + iVar7);
        piVar4 = (int *)*piVar2;
        if (piVar4 != (int *)0x0) {
          piVar1 = piVar4 + 1;
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*piVar4 + 4))();
          }
          *piVar2 = 0;
        }
        *piVar2 = 0;
        uVar6 = uVar6 + 1;
        iVar7 = iVar7 + 0x30;
      } while (uVar6 < *(uint *)(this + 0x48));
    }
  }
  this[100] = (C3DPrimitive2)0x1;
  return true;
}


==================== ?DoLock@CVertexBufferWin32@@EAEPAEKKK@Z @ 00a63240 ====================

CEnginePrimitive * __thiscall
_DoLock_CVertexBufferWin32__EAEPAEKKK_Z
          (CEngineInternalPrimitiveBase *param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  ulong unaff_ESI;
  CEnginePrimitive *unaff_EDI;
  int iVar1;
  
  iVar1 = param_4;
  if (param_4 == 0) {
    iVar1 = *(int *)(param_1 + 8);
  }
  CEngineInternalPrimitiveBase::AddChildPrimitive(param_1,unaff_EDI,unaff_ESI);
  _E2(*(undefined4 *)(param_1 + 4),0x3f800000);
  param_4 = 0;
  (**(code **)(**(int **)(param_1 + 4) + 0x2c))
            (*(int **)(param_1 + 4),param_3,iVar1,&param_4,param_2);
  return unaff_EDI;
}


==================== LoadBinary @ 00a8e8a0 ====================

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall N3DPrimitive::CBaseBlock::LoadBinary(class CMemoryDataInputStream &) */

void __thiscall
N3DPrimitive::CBaseBlock::LoadBinary(CBaseBlock *this,CMemoryDataInputStream *param_1)

{
  char cVar1;
  CMemoryDataInputStream *pCVar2;
  CMemoryDataInputStream *this_00;
  
  this_00 = param_1;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&param_1,4);
    }
    else {
      pCVar2 = (CMemoryDataInputStream *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
      param_1 = pCVar2;
    }
  }
  *(CMemoryDataInputStream **)this = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 4) = param_1;
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[8] = (CBaseBlock)(param_1._0_1_ != (CBaseBlock)0x0);
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[10] = param_1._0_1_;
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (0 < *(int *)(this_00 + 0x14)) {
      cVar1 = **(char **)(this_00 + 0xc);
      *(char **)(this_00 + 0xc) = *(char **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
      this[9] = (CBaseBlock)(cVar1 != '\0');
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
  }
  this[9] = (CBaseBlock)(param_1._0_1_ != (CBaseBlock)0x0);
  return;
}


==================== LoadBinary @ 00a8eb10 ====================

/* [bsim sim=0.8508247150383175 <- ego_r]
   public: void __thiscall N3DPrimitive::CAnimatedBlock::LoadBinary(class CMemoryDataInputStream &)
    */

void __thiscall
N3DPrimitive::CAnimatedBlock::LoadBinary(CAnimatedBlock *this,CMemoryDataInputStream *param_1)

{
  CMemoryDataInputStream *this_00;
  CAnimatedBlock CVar1;
  uint uVar2;
  uint uVar3;
  CAnimatedBlock *pCVar4;
  CAnimatedBlock *pCVar5;
  
  this_00 = param_1;
  CBaseBlock::LoadBinary((CBaseBlock *)this,param_1);
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CMemoryDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CMemoryDataInputStream **)(this + 0x10) = param_1;
  if (*(int *)(this_00 + 4) + 2U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 2) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,2);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT22(param_1._2_2_,**(undefined2 **)(this_00 + 0xc));
      *(undefined2 **)(this_00 + 0xc) = *(undefined2 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -2;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 2U;
    }
  }
  *(undefined2 *)(this + 0x14) = param_1._0_2_;
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CMemoryDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0x16] = (CAnimatedBlock)(param_1._0_1_ != (CAnimatedBlock)0x0);
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (0 < *(int *)(this_00 + 0x14)) {
      CVar1 = **(CAnimatedBlock **)(this_00 + 0xc);
      *(CAnimatedBlock **)(this_00 + 0xc) = *(CAnimatedBlock **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
      goto LAB_00a8ec6e;
    }
    CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)this_00,(uchar *)&param_1,1);
  }
  CVar1 = param_1._0_1_;
LAB_00a8ec6e:
  this[0x17] = CVar1;
  uVar2 = (uint)(byte)CVar1;
  if ((uVar2 != 0) && (*(int *)(this_00 + 4) + uVar2 < 0x80000000)) {
    if ((int)uVar2 <= *(int *)(this_00 + 0x14)) {
      pCVar4 = *(CAnimatedBlock **)(this_00 + 0xc);
      pCVar5 = this + 0x18;
      for (uVar3 = (uint)((byte)CVar1 >> 2); uVar3 != 0; uVar3 = uVar3 - 1) {
        *(undefined4 *)pCVar5 = *(undefined4 *)pCVar4;
        pCVar4 = pCVar4 + 4;
        pCVar5 = pCVar5 + 4;
      }
      for (uVar3 = uVar2 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pCVar5 = *pCVar4;
        pCVar4 = pCVar4 + 1;
        pCVar5 = pCVar5 + 1;
      }
      *(uint *)(this_00 + 0xc) = *(int *)(this_00 + 0xc) + uVar2;
      *(uint *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) - uVar2;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + uVar2;
      return;
    }
    CDataInputStream::ReadWithSrcChunkOverflow
              ((CDataInputStream *)this_00,(uchar *)(this + 0x18),uVar2);
  }
  return;
}


