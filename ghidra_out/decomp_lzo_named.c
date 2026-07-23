//=== ?Decompress@CACompressor@@UAEXPAXK0KAAVCCompressionInfo@@@Z @ 0052adf0 ===

void _Decompress_CACompressor__UAEXPAXK0KAAVCCompressionInfo___Z(void)

{
  return;
}



//=== DecompressDefinitionData @ 009ad5f0 ===

/* [bsim sim=0.6935245321459852 <- ego_r]
   protected: void __thiscall CDefinitionManager::DecompressDefinitionData(class
   CMemoryDataInputStream &,unsigned long)const  */

void __thiscall
CDefinitionManager::DecompressDefinitionData
          (CDefinitionManager *this,CMemoryDataInputStream *param_1,ulong param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  CEngineInternalPrimitiveBase *this_00;
  ulong unaff_ESI;
  CEnginePrimitive *unaff_EDI;
  undefined **local_58 [22];
  
  piVar4 = (int *)CVectorMap<unsigned_long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned_long,CTCCreatureModeManager::CRegisteredModeInfo>_>
                  ::UpperBound((CVectorMap<unsigned_long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned_long,CTCCreatureModeManager::CRegisteredModeInfo>_>
                                *)(this + 0x94),&param_2);
  iVar1 = piVar4[-2];
  iVar5 = piVar4[-1];
  iVar2 = piVar4[1];
  iVar3 = *piVar4;
  if ((DAT_0138e190 != this) || (DAT_0138e194 != iVar5)) {
    GFGetTime();
    CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,unaff_EDI,unaff_ESI);
    CPreallocTexturePool::CPreallocTexturePool((CPreallocTexturePool *)local_58);
    local_58[0] = &PTR__vector_deleting_destructor__01245974;
    NScript::CV_OracleScript::CVO_OracleMarker::Init((CVO_OracleMarker *)local_58);
    DAT_0138e198 = CCompressorZlib::InternalDecompress
                             ((CCompressorZlib *)local_58,(void *)(*(int *)(this + 0x88) + iVar5),
                              iVar2 - iVar5,&DAT_0138e19c,0x8000);
    DAT_0138e190 = this;
    DAT_0138e194 = iVar5;
    GFGetTime();
    CCompressorZlib::~CCompressorZlib((CCompressorZlib *)local_58);
  }
  iVar5 = param_2 - iVar1;
  uVar6 = DAT_0138e198;
  if (iVar5 + 1U < (uint)(iVar3 - iVar1)) {
    uVar6 = (uint)*(ushort *)(&DAT_0138e19e + iVar5 * 2);
  }
  CMemoryDataInputStream::OpenMemorySource
            (param_1,&DAT_0138e19c + *(ushort *)(&DAT_0138e19c + iVar5 * 2),
             uVar6 - *(ushort *)(&DAT_0138e19c + iVar5 * 2));
  return;
}



//=== ProcessActiveFileForDecompression @ 009c3c20 ===

/* [bsim sim=0.8213884203781718 <- ego_r]
   protected: void __thiscall CFileInstaller::ProcessActiveFileForDecompression(class
   CFileInstaller::CActiveFile &) */

void __thiscall
CFileInstaller::ProcessActiveFileForDecompression(CFileInstaller *this,CActiveFile *param_1)

{
  ulong *puVar1;
  int iVar2;
  int *piVar3;
  CActiveFile *pCVar4;
  bool bVar5;
  bool bVar6;
  CCodeSectionManager *pCVar7;
  CBufferChunk *pCVar8;
  uint uVar9;
  ulong uVar10;
  int *piVar11;
  void *pvVar12;
  int *_Memory;
  uint uVar13;
  CCriticalSectionLock local_14 [8];
  undefined4 local_c;
  undefined4 local_8;
  int local_4;
  
  pCVar4 = param_1;
  bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)(param_1 + 0x38));
  if (bVar5) {
    if (*(int *)(pCVar4 + 0xa4) == 0) {
      CCriticalSectionLock::CCriticalSectionLock(local_14,(_RTL_CRITICAL_SECTION *)(this + 4),false)
      ;
      bVar5 = false;
      pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
      pCVar8 = AllocateBuffer((CFileInstaller *)pCVar7,bVar5);
      *(CBufferChunk **)(pCVar4 + 0xa4) = pCVar8;
      *(undefined4 *)(pCVar4 + 0xa8) = 0;
      CCriticalSectionLock::~CCriticalSectionLock(local_14);
    }
    if (*(int **)(pCVar4 + 0xa4) == (int *)0x0) {
      return;
    }
    pvVar12 = (void *)(**(int **)(pCVar4 + 0xa4) + *(int *)(pCVar4 + 0xa8));
    uVar13 = *(int *)(this + 100) - *(int *)(pCVar4 + 0xa8);
    param_1 = (CActiveFile *)((uint)param_1 & 0xffffff00);
    if (uVar13 != 0) {
      do {
        if ((char)param_1 != '\0') goto LAB_009c3d1a;
        bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)(pCVar4 + 0x38));
        if (!bVar5) break;
        uVar9 = uVar13;
        if (0x3ffff < uVar13) {
          uVar9 = 0x40000;
        }
        uVar10 = CCompressorZlib::DecompressStream
                           ((CCompressorZlib *)(pCVar4 + 0x38),pvVar12,uVar9,(bool *)&param_1);
        if (pCVar4[0x8e] != (CActiveFile)0x0) {
          bVar5 = false;
          pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
          CCriticalSectionLock::CCriticalSectionLock
                    (local_14,(_RTL_CRITICAL_SECTION *)(pCVar7 + 4),bVar5);
          DAT_013ca814 = 1;
          CTBaseSingleton<CCodeSectionManager>::Get();
          pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
          pCVar7[0x7a] = (CCodeSectionManager)0x1;
          CCriticalSectionLock::~CCriticalSectionLock(local_14);
          return;
        }
        *(ulong *)(pCVar4 + 0xa8) = *(int *)(pCVar4 + 0xa8) + uVar10;
        pvVar12 = (void *)((int)pvVar12 + uVar10);
        uVar13 = uVar13 - uVar10;
        Sleep(1);
      } while (uVar13 != 0);
      if ((char)param_1 != '\0') {
LAB_009c3d1a:
        if (pCVar4[0xac] != (CActiveFile)0x0) {
          bVar5 = true;
          CCompressorZlib::FinishDecompressStream((CCompressorZlib *)(pCVar4 + 0x38));
          goto LAB_009c3d35;
        }
      }
    }
    bVar5 = false;
LAB_009c3d35:
    bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)(pCVar4 + 0x38));
    if (!bVar6) {
      CCriticalSectionLock::CCriticalSectionLock(local_14,(_RTL_CRITICAL_SECTION *)(this + 4),false)
      ;
      pCVar8 = *(CBufferChunk **)(pCVar4 + 0xa0);
      pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
      FreeBuffer((CFileInstaller *)pCVar7,pCVar8);
      *(undefined4 *)(pCVar4 + 0xa0) = 0;
      CCriticalSectionLock::~CCriticalSectionLock(local_14);
    }
    iVar2 = *(int *)(pCVar4 + 0xa8);
    if ((iVar2 != *(int *)(this + 100)) && (!bVar5)) {
      return;
    }
    CCriticalSectionLock::CCriticalSectionLock(local_14,(_RTL_CRITICAL_SECTION *)(this + 4),false);
    local_8 = *(undefined4 *)(pCVar4 + 0x9c);
    local_c = *(undefined4 *)(pCVar4 + 0xa4);
    local_4 = iVar2;
    FUN_009c6af0(&local_c);
    *(undefined4 *)(pCVar4 + 0xa4) = 0;
    *(int *)(pCVar4 + 0x9c) = *(int *)(pCVar4 + 0x9c) + iVar2;
    SetEvent(*(HANDLE *)(this + 0x84));
    CCriticalSectionLock::~CCriticalSectionLock(local_14);
    return;
  }
  if (*(int *)(pCVar4 + 0xa0) != 0) {
    CCriticalSectionLock::CCriticalSectionLock
              ((CCriticalSectionLock *)&local_c,(_RTL_CRITICAL_SECTION *)(this + 4),false);
    pCVar8 = *(CBufferChunk **)(pCVar4 + 0xa0);
    pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
    FreeBuffer((CFileInstaller *)pCVar7,pCVar8);
    *(undefined4 *)(pCVar4 + 0xa0) = 0;
    CCriticalSectionLock::~CCriticalSectionLock((CCriticalSectionLock *)&local_c);
  }
  CCriticalSectionLock::CCriticalSectionLock(local_14,(_RTL_CRITICAL_SECTION *)(this + 4),false);
  _Memory = (int *)**(int **)(pCVar4 + 0x94);
  if (_Memory == *(int **)(pCVar4 + 0x94)) goto LAB_009c3f9a;
  while (pvVar12 = (void *)_Memory[2], *(int *)((int)pvVar12 + 4) != *(int *)(pCVar4 + 0x98)) {
    _Memory = (int *)*_Memory;
    if (_Memory == (int *)*(int *)(pCVar4 + 0x94)) {
      CCriticalSectionLock::~CCriticalSectionLock(local_14);
      return;
    }
  }
  piVar11 = (int *)**(int **)((int)pvVar12 + 0xc);
  param_1 = *(CActiveFile **)((int)pvVar12 + 8);
  if (*(int *)((int)pvVar12 + 4) == 0) {
    if (*piVar11 == 0x217a6962) {
      puVar1 = (ulong *)(piVar11 + 1);
      param_1 = param_1 + -8;
      piVar11 = piVar11 + 2;
      StartWritingFile(this,pCVar4,*puVar1);
      goto LAB_009c3f47;
    }
    bVar5 = false;
    pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
    CCriticalSectionLock::CCriticalSectionLock
              ((CCriticalSectionLock *)&local_c,(_RTL_CRITICAL_SECTION *)(pCVar7 + 4),bVar5);
    DAT_013ca814 = 1;
    CTBaseSingleton<CCodeSectionManager>::Get();
    pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
    pCVar7[0x7a] = (CCodeSectionManager)0x1;
    CCriticalSectionLock::~CCriticalSectionLock((CCriticalSectionLock *)&local_c);
  }
  else {
LAB_009c3f47:
    *(int *)(pCVar4 + 0x98) = *(int *)(pCVar4 + 0x98) + *(int *)((int)pvVar12 + 8);
    iVar2 = *_Memory;
    piVar3 = (int *)_Memory[1];
    *piVar3 = iVar2;
    *(int **)(iVar2 + 4) = piVar3;
    free(_Memory);
    *(undefined4 *)(pCVar4 + 0xa0) = *(undefined4 *)((int)pvVar12 + 0xc);
    CCompressorZlib::SetStreamInput((CCompressorZlib *)(pCVar4 + 0x38),piVar11,(ulong)param_1);
    if (*(uint *)(pCVar4 + 0x14) <= (uint)(*(int *)((int)pvVar12 + 8) + *(int *)((int)pvVar12 + 4)))
    {
      pCVar4[0xac] = (CActiveFile)0x1;
    }
  }
  operator_delete(pvVar12);
LAB_009c3f9a:
  CCriticalSectionLock::~CCriticalSectionLock(local_14);
  return;
}



//=== DecompressStream @ 00a3a840 ===

/* [bsim sim=0.9475426480133842 <- ego_r]
   public: unsigned long __thiscall CCompressorZlib::DecompressStream(void *,unsigned long,bool &)
    */

ulong __thiscall
CCompressorZlib::DecompressStream(CCompressorZlib *this,void *param_1,ulong param_2,bool *param_3)

{
  int iVar1;
  long in_EDX;
  
  if (this[0x54] == (CCompressorZlib)0x0) {
    NGameText::ConstructString((CWideString *)this,in_EDX,(long)(this + 0x1c));
    this[0x54] = (CCompressorZlib)0x1;
    this[0x55] = (CCompressorZlib)0x0;
  }
  *(void **)(this + 0x28) = param_1;
  *(ulong *)(this + 0x2c) = param_2;
  iVar1 = _z_inflate(this + 0x1c,2);
  if (iVar1 == 1) {
    *param_3 = true;
    return param_2 - *(int *)(this + 0x2c);
  }
  if (iVar1 != 0) {
    this[0x56] = (CCompressorZlib)0x1;
  }
  *param_3 = false;
  return param_2 - *(int *)(this + 0x2c);
}



//=== Decompress @ 00a3a950 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CCompressorZlib::Decompress(unsigned char const *,long,long,void *) */

void __thiscall
CCompressorZlib::Decompress
          (CCompressorZlib *this,uchar *param_1,long param_2,long param_3,void *param_4)

{
  undefined4 local_c;
  undefined4 local_8;
  uchar local_4;
  
  local_4 = param_1[0xc];
  local_8 = *(undefined4 *)(param_1 + 8);
  local_c = *(undefined4 *)(param_1 + 4);
  (**(code **)(*(int *)this + 0x1c))(param_1,local_8,param_4,local_c,&local_c);
  return;
}



//=== InternalDecompress @ 00a3ab00 ===

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=0.9999999999999999 <- ego_r]
   public: unsigned long __thiscall CCompressorZlib::InternalDecompress(void const *,unsigned
   long,void *,unsigned long) */

ulong __thiscall
CCompressorZlib::InternalDecompress
          (CCompressorZlib *this,void *param_1,ulong param_2,void *param_3,ulong param_4)

{
  if (param_2 == 0) {
    return 0;
  }
  _DAT_013d27f0 = param_2;
  _DAT_013d27fc = param_4;
  _DAT_013d27ec = param_1;
  _DAT_013d27f8 = param_3;
  if (DAT_013d2824 == '\0') {
    _DAT_013d280c = &LAB_00a3a9c0;
    _DAT_013d2810 = &LAB_00a3aa00;
    NGameText::ConstructString(param_1,(long)param_3,0x13d27ec);
    DAT_013d2824 = '\x01';
  }
  else {
    _z_inflateReset(&DAT_013d27ec);
  }
  _z_inflate(&DAT_013d27ec,4);
  return DAT_013d2800;
}



//=== FinishDecompressStream @ 00a3abb0 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CCompressorZlib::FinishDecompressStream(void) */

void __thiscall CCompressorZlib::FinishDecompressStream(CCompressorZlib *this)

{
  if (this[0x54] != (CCompressorZlib)0x0) {
    _z_inflateEnd(this + 0x1c);
    *(undefined4 *)(this + 0x1c) = 0;
    *(undefined4 *)(this + 0x28) = 0;
    *(undefined4 *)(this + 0x20) = 0;
    *(undefined4 *)(this + 0x2c) = 0;
    this[0x54] = (CCompressorZlib)0x0;
  }
  return;
}



//=== ?Decompress@CCompressorZlib@@UAEXPBXKPAXKAAVCCompressionInfo@@@Z @ 00a3abe0 ===

void _Decompress_CCompressorZlib__UAEXPBXKPAXKAAVCCompressionInfo___Z
               (int param_1,int param_2,void *param_3,ulong param_4,undefined4 *param_5)

{
  CCompressorZlib *in_ECX;
  
  if (param_2 - 0xdU == 0) {
    param_5[1] = *(undefined4 *)(param_1 + 8);
    *param_5 = *(undefined4 *)(param_1 + 4);
    *(undefined1 *)(param_5 + 2) = *(undefined1 *)(param_1 + 0xc);
    return;
  }
  CCompressorZlib::InternalDecompress(in_ECX,(void *)(param_1 + 0xd),param_2 - 0xdU,param_3,param_4)
  ;
  param_5[1] = *(undefined4 *)(param_1 + 8);
  *param_5 = *(undefined4 *)(param_1 + 4);
  *(undefined1 *)(param_5 + 2) = *(undefined1 *)(param_1 + 0xc);
  return;
}



//=== _z_inflateReset @ 00c021f0 ===

/* [bsim sim=1.0000000000000002 <- ego_r] */

undefined4 _z_inflateReset(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  if ((param_1 != 0) && (puVar2 = *(undefined4 **)(param_1 + 0x1c), puVar2 != (undefined4 *)0x0)) {
    puVar2[6] = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2[3] = 0;
    puVar2[8] = 0;
    puVar2[9] = 0;
    puVar2[0xc] = 0;
    puVar2[0xd] = 0;
    puVar1 = puVar2 + 0x14a;
    puVar2[0x19] = puVar1;
    puVar2[0x12] = puVar1;
    puVar2[0x11] = puVar1;
    return 0;
  }
  return 0xfffffffe;
}



//=== _z_inflateInit2_ @ 00c02240 ===

/* [bsim sim=0.9999999999999999 <- ego_r] */

undefined4 _z_inflateInit2_(int param_1,uint param_2,char *param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  if (((param_3 == (char *)0x0) || (*param_3 != '1')) || (param_4 != 0x38)) {
    return 0xfffffffa;
  }
  if (param_1 != 0) {
    *(undefined4 *)(param_1 + 0x18) = 0;
    if (*(int *)(param_1 + 0x20) == 0) {
      *(code **)(param_1 + 0x20) = _zcalloc;
      *(undefined4 *)(param_1 + 0x28) = 0;
    }
    if (*(int *)(param_1 + 0x24) == 0) {
      *(code **)(param_1 + 0x24) = CEngineSubPrimitiveBase::operator_new;
    }
    iVar1 = (**(code **)(param_1 + 0x20))(*(undefined4 *)(param_1 + 0x28),1,0x1ba8);
    if (iVar1 == 0) {
      return 0xfffffffc;
    }
    *(int *)(param_1 + 0x1c) = iVar1;
    if ((int)param_2 < 0) {
      *(undefined4 *)(iVar1 + 8) = 0;
      param_2 = -param_2;
    }
    else {
      *(int *)(iVar1 + 8) = ((int)param_2 >> 4) + 1;
      if ((int)param_2 < 0x30) {
        param_2 = param_2 & 0xf;
      }
    }
    if ((7 < (int)param_2) && ((int)param_2 < 0x10)) {
      *(uint *)(iVar1 + 0x1c) = param_2;
      *(undefined4 *)(iVar1 + 0x2c) = 0;
      uVar2 = _z_inflateReset(param_1);
      return uVar2;
    }
    (**(code **)(param_1 + 0x24))(*(undefined4 *)(param_1 + 0x28),iVar1);
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  return 0xfffffffe;
}



//=== _z_inflate @ 00c02470 ===

/* [bsim sim=1.0 <- ego_r] */

int _z_inflate(int *param_1,int param_2)

{
  uint *puVar1;
  char cVar2;
  ushort uVar3;
  byte *pbVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  byte bVar8;
  byte bVar9;
  uint uVar10;
  byte *pbVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  uint local_38;
  uint local_34;
  byte *local_30;
  uint local_2c;
  uint local_28;
  char local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  byte *local_20;
  uint local_1c;
  byte *local_18;
  int local_14;
  uint local_10;
  uint local_c;
  byte *local_8;
  
  if ((((param_1 == (int *)0x0) || (puVar1 = (uint *)param_1[7], puVar1 == (uint *)0x0)) ||
      (param_1[3] == 0)) || ((*param_1 == 0 && (param_1[1] != 0)))) {
    return -2;
  }
  if (*puVar1 == 0xb) {
    *puVar1 = 0xc;
  }
  local_20 = (byte *)param_1[3];
  uVar7 = puVar1[0xc];
  uVar10 = puVar1[0xd];
  pbVar11 = (byte *)*param_1;
  uVar13 = param_1[1];
  local_28 = param_1[4];
  uVar5 = *puVar1;
  local_14 = 0;
  local_38 = uVar7;
  local_30 = pbVar11;
  local_34 = uVar13;
  local_c = uVar13;
  local_1c = local_28;
  do {
    uVar12 = uVar13;
    switch(uVar5) {
    case 0:
      if (puVar1[2] == 0) {
        *puVar1 = 0xc;
      }
      else {
        for (; uVar10 < 0x10; uVar10 = uVar10 + 8) {
          uVar12 = 0;
          if (uVar13 == 0) goto LAB_00c03717;
          uVar13 = uVar13 - 1;
          uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
          pbVar11 = pbVar11 + 1;
          local_38 = uVar7;
          local_34 = uVar13;
          local_30 = pbVar11;
        }
        if (((puVar1[2] & 2) == 0) || (uVar7 != 0x8b1f)) {
          puVar1[4] = 0;
          uVar5 = uVar7;
          if (((puVar1[2] & 1) == 0) ||
             (uVar5 = local_38, ((uVar7 & 0xff) * 0x100 + (uVar7 >> 8)) % 0x1f != 0)) {
            param_1[6] = (int)"incorrect header check";
          }
          else if (((byte)local_38 & 0xf) == 8) {
            uVar5 = local_38 >> 4;
            uVar10 = uVar10 - 4;
            if ((uVar5 & 0xf) + 8 <= puVar1[7]) {
              uVar10 = 0;
              uVar7 = _z_adler32(0,0,0);
              puVar1[5] = uVar7;
              param_1[0xc] = uVar7;
              *puVar1 = ~(local_38 >> 0xc) & 2 | 9;
              local_38 = 0;
              uVar7 = 0;
              goto LAB_00c036b5;
            }
            param_1[6] = (int)"invalid window size";
            local_38 = uVar5;
          }
          else {
            param_1[6] = (int)"unknown compression method";
          }
          break;
        }
        uVar10 = 0;
        uVar7 = _z_crc32(0,0,0);
        puVar1[5] = uVar7;
        local_24 = '\x1f';
        local_23 = 0x8b;
        uVar7 = _z_crc32(puVar1[5],&local_24,2);
        puVar1[5] = uVar7;
        local_38 = 0;
        *puVar1 = 1;
        uVar7 = 0;
      }
      goto LAB_00c036b5;
    case 1:
      for (; uVar10 < 0x10; uVar10 = uVar10 + 8) {
        uVar12 = 0;
        if (uVar13 == 0) goto LAB_00c03717;
        uVar13 = uVar13 - 1;
        uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
        pbVar11 = pbVar11 + 1;
        local_38 = uVar7;
        local_30 = pbVar11;
        local_34 = uVar13;
      }
      puVar1[4] = uVar7;
      uVar5 = uVar7;
      if ((char)uVar7 == '\b') {
        if ((uVar7 & 0xe000) == 0) {
          if ((uVar7 & 0x200) != 0) {
            local_24 = (char)uVar7;
            local_23 = (char)(uVar7 >> 8);
            uVar7 = _z_crc32(puVar1[5],&local_24,2);
            puVar1[5] = uVar7;
          }
          uVar10 = 0;
          *puVar1 = 2;
          uVar7 = 0;
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            uVar10 = uVar10 + 8;
            uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
            pbVar11 = pbVar11 + 1;
joined_r0x00c026dd:
          } while (uVar10 < 0x20);
          if ((puVar1[4] & 0x200) != 0) {
            local_24 = (char)uVar7;
            local_23 = (undefined1)(uVar7 >> 8);
            local_22 = (undefined1)(uVar7 >> 0x10);
            local_21 = (undefined1)(uVar7 >> 0x18);
            uVar7 = _z_crc32(puVar1[5],&local_24,4);
            puVar1[5] = uVar7;
          }
          uVar10 = 0;
          *puVar1 = 3;
          uVar7 = 0;
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            uVar10 = uVar10 + 8;
            uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
            pbVar11 = pbVar11 + 1;
            local_30 = pbVar11;
            local_34 = uVar13;
joined_r0x00c02749:
          } while (uVar10 < 0x10);
          if ((puVar1[4] & 0x200) != 0) {
            local_24 = (char)uVar7;
            local_23 = (undefined1)(uVar7 >> 8);
            uVar7 = _z_crc32(puVar1[5],&local_24,2);
            puVar1[5] = uVar7;
          }
          uVar10 = 0;
          local_38 = 0;
          *puVar1 = 4;
          uVar7 = 0;
          goto switchD_00c024f4_caseD_4;
        }
        param_1[6] = (int)"unknown header flags set";
      }
      else {
        param_1[6] = (int)"unknown compression method";
      }
      break;
    case 2:
      goto joined_r0x00c026dd;
    case 3:
      goto joined_r0x00c02749;
    case 4:
switchD_00c024f4_caseD_4:
      if ((puVar1[4] & 0x400) != 0) {
        for (; uVar10 < 0x10; uVar10 = uVar10 + 8) {
          uVar12 = 0;
          if (uVar13 == 0) goto LAB_00c03717;
          uVar13 = uVar13 - 1;
          uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
          pbVar11 = pbVar11 + 1;
          local_34 = uVar13;
          local_30 = pbVar11;
        }
        puVar1[0xe] = uVar7;
        if ((puVar1[4] & 0x200) != 0) {
          local_24 = (char)uVar7;
          local_23 = (undefined1)(uVar7 >> 8);
          uVar7 = _z_crc32(puVar1[5],&local_24,2);
          puVar1[5] = uVar7;
        }
        uVar10 = 0;
        local_38 = 0;
        uVar7 = 0;
      }
      *puVar1 = 5;
      uVar12 = uVar13;
    case 5:
      if ((puVar1[4] & 0x400) == 0) {
LAB_00c02872:
        *puVar1 = 6;
switchD_00c024f4_caseD_6:
        if ((puVar1[4] & 0x800) == 0) {
LAB_00c028e0:
          *puVar1 = 7;
switchD_00c024f4_caseD_7:
          if ((puVar1[4] & 0x1000) == 0) {
LAB_00c02948:
            *puVar1 = 8;
            uVar13 = uVar12;
switchD_00c024f4_caseD_8:
            if ((puVar1[4] & 0x200) != 0) {
              for (; uVar10 < 0x10; uVar10 = uVar10 + 8) {
                uVar12 = 0;
                if (uVar13 == 0) goto LAB_00c03717;
                uVar13 = uVar13 - 1;
                uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
                pbVar11 = pbVar11 + 1;
                local_38 = uVar7;
                local_34 = uVar13;
                local_30 = pbVar11;
              }
              if (uVar7 != (puVar1[5] & 0xffff)) {
                param_1[6] = (int)"header crc mismatch";
                uVar5 = uVar7;
                break;
              }
              uVar10 = 0;
              local_38 = 0;
            }
            uVar7 = _z_crc32(0,0,0);
            puVar1[5] = uVar7;
            param_1[0xc] = uVar7;
            *puVar1 = 0xb;
            uVar7 = local_38;
            goto LAB_00c036b5;
          }
          if (uVar12 != 0) {
            uVar5 = 0;
            do {
              local_18 = (byte *)(uint)pbVar11[uVar5];
              uVar5 = uVar5 + 1;
              if (local_18 == (byte *)0x0) break;
            } while (uVar5 < uVar12);
            if ((puVar1[4] & 0x2000) != 0) {
              uVar7 = _z_crc32(puVar1[5],pbVar11,uVar5);
              puVar1[5] = uVar7;
              uVar7 = local_38;
            }
            uVar12 = uVar12 - uVar5;
            pbVar11 = pbVar11 + uVar5;
            local_34 = uVar12;
            local_30 = pbVar11;
            if (local_18 == (byte *)0x0) goto LAB_00c02948;
          }
        }
        else if (uVar12 != 0) {
          uVar5 = 0;
          do {
            local_18 = (byte *)(uint)pbVar11[uVar5];
            uVar5 = uVar5 + 1;
            if (local_18 == (byte *)0x0) break;
          } while (uVar5 < uVar12);
          if ((puVar1[4] & 0x2000) != 0) {
            uVar7 = _z_crc32(puVar1[5],pbVar11,uVar5);
            puVar1[5] = uVar7;
            uVar7 = local_38;
          }
          uVar12 = uVar12 - uVar5;
          pbVar11 = pbVar11 + uVar5;
          local_34 = uVar12;
          local_30 = pbVar11;
          if (local_18 == (byte *)0x0) goto LAB_00c028e0;
        }
      }
      else {
        uVar5 = puVar1[0xe];
        if (uVar12 < puVar1[0xe]) {
          uVar5 = uVar12;
        }
        if (uVar5 != 0) {
          if ((puVar1[4] & 0x200) != 0) {
            uVar7 = _z_crc32(puVar1[5],pbVar11,uVar5);
            puVar1[5] = uVar7;
            uVar7 = local_38;
          }
          uVar12 = uVar12 - uVar5;
          pbVar11 = pbVar11 + uVar5;
          puVar1[0xe] = puVar1[0xe] - uVar5;
          local_34 = uVar12;
          local_30 = pbVar11;
        }
        if (puVar1[0xe] == 0) goto LAB_00c02872;
      }
LAB_00c03717:
      param_1[3] = (int)local_20;
      param_1[4] = local_28;
      *param_1 = (int)pbVar11;
      param_1[1] = uVar12;
      puVar1[0xc] = uVar7;
      puVar1[0xd] = uVar10;
      if (((puVar1[8] == 0) && ((0x17 < (int)*puVar1 || (local_1c == param_1[4])))) ||
         (iVar6 = updatewindow(param_1), iVar6 == 0)) {
        iVar6 = local_c - param_1[1];
        iVar14 = local_1c - param_1[4];
        param_1[2] = param_1[2] + iVar6;
        param_1[5] = param_1[5] + iVar14;
        puVar1[6] = puVar1[6] + iVar14;
        if ((puVar1[2] != 0) && (iVar14 != 0)) {
          if (puVar1[4] == 0) {
            uVar7 = _z_adler32(puVar1[5],param_1[3] - iVar14,iVar14);
          }
          else {
            uVar7 = _z_crc32(puVar1[5],param_1[3] - iVar14);
          }
          puVar1[5] = uVar7;
          param_1[0xc] = uVar7;
        }
        param_1[0xb] = (-(uint)(puVar1[1] != 0) & 0x40) + ((*puVar1 != 0xb) - 1 & 0x80) +
                       puVar1[0xd];
        if (((iVar6 != 0) || (iVar14 != 0)) && (param_2 != 4)) {
          return local_14;
        }
        if (local_14 != 0) {
          return local_14;
        }
        return -5;
      }
      *puVar1 = 0x1c;
switchD_00c024f4_caseD_1c:
      return -4;
    case 6:
      goto switchD_00c024f4_caseD_6;
    case 7:
      goto switchD_00c024f4_caseD_7;
    case 8:
      goto switchD_00c024f4_caseD_8;
    case 9:
      for (; uVar10 < 0x20; uVar10 = uVar10 + 8) {
        uVar12 = 0;
        if (uVar13 == 0) goto LAB_00c03717;
        uVar13 = uVar13 - 1;
        uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
        pbVar11 = pbVar11 + 1;
        local_38 = uVar7;
        local_34 = uVar13;
        local_30 = pbVar11;
      }
      uVar7 = ((uVar7 & 0xff00) + uVar7 * 0x10000) * 0x100 + (local_38 >> 0x10 & 0xff) * 0x100 +
              (uVar7 >> 0x18);
      uVar10 = 0;
      puVar1[5] = uVar7;
      param_1[0xc] = uVar7;
      local_38 = 0;
      *puVar1 = 10;
      uVar7 = 0;
    case 10:
      if (puVar1[3] == 0) {
        param_1[3] = (int)local_20;
        param_1[1] = uVar13;
        *param_1 = (int)pbVar11;
        param_1[4] = local_28;
        puVar1[0xd] = uVar10;
        puVar1[0xc] = uVar7;
        return 2;
      }
      uVar7 = _z_adler32(0,0,0);
      puVar1[5] = uVar7;
      param_1[0xc] = uVar7;
      *puVar1 = 0xb;
      uVar7 = local_38;
      uVar12 = uVar13;
switchD_00c024f4_caseD_b:
      uVar13 = uVar12;
      if (param_2 != 5) {
switchD_00c024f4_caseD_c:
        if (puVar1[1] == 0) {
          for (; uVar10 < 3; uVar10 = uVar10 + 8) {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            uVar13 = uVar13 - 1;
            uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
            pbVar11 = pbVar11 + 1;
            local_34 = uVar13;
            local_30 = pbVar11;
          }
          puVar1[1] = uVar7 & 1;
          switch(uVar7 >> 1 & 3) {
          case 0:
            uVar7 = uVar7 >> 3;
            *puVar1 = 0xd;
            uVar10 = uVar10 - 3;
            local_38 = uVar7;
            break;
          case 1:
            uVar7 = uVar7 >> 3;
            puVar1[0x11] = (uint)&DAT_012ae2e0;
            puVar1[0x13] = 9;
            puVar1[0x12] = (uint)&DAT_012aeae0;
            puVar1[0x14] = 5;
            *puVar1 = 0x12;
            uVar10 = uVar10 - 3;
            local_38 = uVar7;
            break;
          case 2:
            uVar7 = uVar7 >> 3;
            *puVar1 = 0xf;
            uVar10 = uVar10 - 3;
            local_38 = uVar7;
            break;
          case 3:
            param_1[6] = (int)"invalid block type";
            *puVar1 = 0x1b;
          default:
            uVar7 = uVar7 >> 3;
            uVar10 = uVar10 - 3;
            local_38 = uVar7;
          }
        }
        else {
          uVar7 = uVar7 >> (sbyte)(uVar10 & 7);
          uVar10 = uVar10 - (uVar10 & 7);
          *puVar1 = 0x18;
          local_38 = uVar7;
        }
        goto LAB_00c036b5;
      }
      goto LAB_00c03717;
    case 0xb:
      goto switchD_00c024f4_caseD_b;
    case 0xc:
      goto switchD_00c024f4_caseD_c;
    case 0xd:
      uVar7 = uVar7 >> (sbyte)(uVar10 & 7);
      for (uVar10 = uVar10 - (uVar10 & 7); uVar10 < 0x20; uVar10 = uVar10 + 8) {
        uVar12 = 0;
        if (uVar13 == 0) goto LAB_00c03717;
        uVar13 = uVar13 - 1;
        uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
        pbVar11 = pbVar11 + 1;
        local_30 = pbVar11;
        local_34 = uVar13;
      }
      if ((uVar7 & 0xffff) == ~uVar7 >> 0x10) {
        uVar10 = 0;
        puVar1[0xe] = uVar7 & 0xffff;
        local_38 = 0;
        *puVar1 = 0xe;
        uVar7 = 0;
        uVar12 = uVar13;
        goto switchD_00c024f4_caseD_e;
      }
      param_1[6] = (int)"invalid stored block lengths";
      uVar5 = uVar7;
      local_38 = uVar7;
      break;
    case 0xe:
switchD_00c024f4_caseD_e:
      local_2c = puVar1[0xe];
      uVar13 = uVar12;
      if (local_2c == 0) {
LAB_00c0324c:
        *puVar1 = 0xb;
      }
      else {
        if (uVar12 < local_2c) {
          local_2c = uVar12;
        }
        if (local_28 < local_2c) {
          local_2c = local_28;
        }
        if (local_2c == 0) goto LAB_00c03717;
        pbVar11 = local_30;
        pbVar4 = local_20;
        for (uVar5 = local_2c >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
          *(undefined4 *)pbVar4 = *(undefined4 *)pbVar11;
          pbVar11 = pbVar11 + 4;
          pbVar4 = pbVar4 + 4;
        }
        for (uVar5 = local_2c & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pbVar4 = *pbVar11;
          pbVar11 = pbVar11 + 1;
          pbVar4 = pbVar4 + 1;
        }
        uVar13 = local_34 - local_2c;
        pbVar11 = local_30 + local_2c;
        local_28 = local_28 - local_2c;
        local_20 = local_20 + local_2c;
        puVar1[0xe] = puVar1[0xe] - local_2c;
        local_34 = uVar13;
        local_30 = pbVar11;
      }
      goto LAB_00c036b5;
    case 0xf:
      for (; uVar10 < 0xe; uVar10 = uVar10 + 8) {
        uVar12 = 0;
        if (uVar13 == 0) goto LAB_00c03717;
        uVar13 = uVar13 - 1;
        uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
        pbVar11 = pbVar11 + 1;
        local_34 = uVar13;
        local_30 = pbVar11;
      }
      puVar1[0x16] = (uVar7 & 0x1f) + 0x101;
      uVar5 = uVar7 >> 10;
      puVar1[0x17] = (uVar7 >> 5 & 0x1f) + 1;
      uVar7 = uVar7 >> 0xe;
      uVar10 = uVar10 - 0xe;
      puVar1[0x15] = (uVar5 & 0xf) + 4;
      local_38 = uVar7;
      if ((puVar1[0x16] < 0x11f) && (puVar1[0x17] < 0x1f)) {
        puVar1[0x18] = 0;
        *puVar1 = 0x10;
        goto switchD_00c024f4_caseD_10;
      }
      param_1[6] = (int)"too many length or distance symbols";
      uVar5 = uVar7;
      break;
    case 0x10:
switchD_00c024f4_caseD_10:
      if (puVar1[0x18] < puVar1[0x15]) {
        do {
          for (; uVar10 < 3; uVar10 = uVar10 + 8) {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = *pbVar11;
            bVar8 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            pbVar11 = pbVar11 + 1;
            uVar7 = uVar7 + ((uint)bVar9 << (bVar8 & 0x1f));
            local_34 = uVar13;
            local_30 = pbVar11;
          }
          local_38 = uVar7 >> 3;
          uVar10 = uVar10 - 3;
          *(ushort *)((int)puVar1 + (uint)*(ushort *)(&DAT_012aeb60 + puVar1[0x18] * 2) * 2 + 0x68)
               = (ushort)uVar7 & 7;
          uVar5 = puVar1[0x18];
          puVar1[0x18] = uVar5 + 1;
          uVar7 = local_38;
        } while (uVar5 + 1 < puVar1[0x15]);
      }
      uVar7 = puVar1[0x18];
      while (uVar7 < 0x13) {
        *(undefined2 *)
         ((int)puVar1 + (uint)*(ushort *)(&DAT_012aeb60 + puVar1[0x18] * 2) * 2 + 0x68) = 0;
        uVar7 = puVar1[0x18] + 1;
        puVar1[0x18] = uVar7;
      }
      puVar1[0x19] = (uint)(puVar1 + 0x14a);
      puVar1[0x11] = (uint)(puVar1 + 0x14a);
      puVar1[0x13] = 7;
      local_14 = _inflate_table(0,puVar1 + 0x1a,0x13,puVar1 + 0x19,puVar1 + 0x13,puVar1 + 0xba);
      if (local_14 == 0) {
        puVar1[0x18] = 0;
        *puVar1 = 0x11;
        uVar7 = local_38;
        goto switchD_00c024f4_caseD_11;
      }
      param_1[6] = (int)"invalid code lengths set";
      uVar5 = local_38;
      break;
    case 0x11:
switchD_00c024f4_caseD_11:
      if (puVar1[0x18] < puVar1[0x17] + puVar1[0x16]) {
        do {
          local_8 = *(byte **)(puVar1[0x11] + ((1 << ((byte)puVar1[0x13] & 0x1f)) - 1U & uVar7) * 4)
          ;
          if (uVar10 < ((uint)local_8 >> 8 & 0xff)) {
            do {
              uVar12 = 0;
              if (uVar13 == 0) goto LAB_00c03717;
              bVar9 = (byte)uVar10;
              uVar13 = uVar13 - 1;
              uVar10 = uVar10 + 8;
              uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
              pbVar11 = pbVar11 + 1;
              local_8 = *(byte **)(puVar1[0x11] +
                                  ((1 << ((byte)puVar1[0x13] & 0x1f)) - 1U & uVar7) * 4);
              local_34 = uVar13;
              local_30 = pbVar11;
            } while (uVar10 < ((uint)local_8 >> 8 & 0xff));
          }
          uVar3 = (ushort)((uint)local_8 >> 0x10);
          bVar9 = (byte)((uint)local_8 >> 8);
          if (0xf < uVar3) {
            if (uVar3 == 0x10) {
              uVar5 = (uint)local_8 >> 8 & 0xff;
              if (uVar10 < uVar5 + 2) {
                do {
                  uVar12 = 0;
                  if (uVar13 == 0) goto LAB_00c03717;
                  bVar8 = (byte)uVar10;
                  uVar13 = uVar13 - 1;
                  uVar10 = uVar10 + 8;
                  uVar7 = uVar7 + ((uint)*pbVar11 << (bVar8 & 0x1f));
                  pbVar11 = pbVar11 + 1;
                  local_34 = uVar13;
                  local_30 = pbVar11;
                } while (uVar10 < uVar5 + 2);
              }
              local_38 = uVar7 >> (bVar9 & 0x1f);
              uVar10 = uVar10 - uVar5;
              if (puVar1[0x18] != 0) {
                local_18 = (byte *)(uint)*(ushort *)((int)puVar1 + puVar1[0x18] * 2 + 0x66);
                iVar6 = (local_38 & 3) + 3;
                uVar7 = local_38 >> 2;
                uVar10 = uVar10 - 2;
                goto LAB_00c02f93;
              }
              param_1[6] = (int)"invalid bit length repeat";
            }
            else {
              uVar5 = (uint)local_8 >> 8 & 0xff;
              if (uVar3 == 0x11) {
                if (uVar10 < uVar5 + 3) {
                  do {
                    uVar12 = 0;
                    if (uVar13 == 0) goto LAB_00c03717;
                    bVar8 = (byte)uVar10;
                    uVar13 = uVar13 - 1;
                    uVar10 = uVar10 + 8;
                    uVar7 = uVar7 + ((uint)*pbVar11 << (bVar8 & 0x1f));
                    pbVar11 = pbVar11 + 1;
                    local_34 = uVar13;
                    local_30 = pbVar11;
                  } while (uVar10 < uVar5 + 3);
                }
                uVar7 = uVar7 >> (bVar9 & 0x1f);
                iVar14 = -3;
                iVar6 = (uVar7 & 7) + 3;
                uVar7 = uVar7 >> 3;
              }
              else {
                if (uVar10 < uVar5 + 7) {
                  do {
                    uVar12 = 0;
                    if (uVar13 == 0) goto LAB_00c03717;
                    bVar8 = (byte)uVar10;
                    uVar13 = uVar13 - 1;
                    uVar10 = uVar10 + 8;
                    uVar7 = uVar7 + ((uint)*pbVar11 << (bVar8 & 0x1f));
                    pbVar11 = pbVar11 + 1;
                    local_34 = uVar13;
                    local_30 = pbVar11;
                  } while (uVar10 < uVar5 + 7);
                }
                uVar7 = uVar7 >> (bVar9 & 0x1f);
                iVar14 = -7;
                iVar6 = (uVar7 & 0x7f) + 0xb;
                uVar7 = uVar7 >> 7;
              }
              local_18 = (byte *)0x0;
              uVar10 = uVar10 + (iVar14 - uVar5);
LAB_00c02f93:
              uVar13 = local_34;
              if (puVar1[0x18] + iVar6 <= puVar1[0x17] + puVar1[0x16]) {
                for (; iVar6 != 0; iVar6 = iVar6 + -1) {
                  *(short *)((int)puVar1 + puVar1[0x18] * 2 + 0x68) = (short)local_18;
                  puVar1[0x18] = puVar1[0x18] + 1;
                }
                goto LAB_00c02fc6;
              }
              param_1[6] = (int)"invalid bit length repeat";
              local_38 = uVar7;
            }
            *puVar1 = 0x1b;
            break;
          }
          uVar5 = (uint)local_8 >> 8 & 0xff;
          for (; uVar10 < uVar5; uVar10 = uVar10 + 8) {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            uVar13 = uVar13 - 1;
            uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
            pbVar11 = pbVar11 + 1;
            local_34 = uVar13;
            local_30 = pbVar11;
          }
          uVar7 = uVar7 >> (bVar9 & 0x1f);
          uVar10 = uVar10 - uVar5;
          *(ushort *)((int)puVar1 + puVar1[0x18] * 2 + 0x68) = uVar3;
          puVar1[0x18] = puVar1[0x18] + 1;
LAB_00c02fc6:
          local_38 = uVar7;
        } while (puVar1[0x18] < puVar1[0x17] + puVar1[0x16]);
      }
      puVar1[0x19] = (uint)(puVar1 + 0x14a);
      puVar1[0x11] = (uint)(puVar1 + 0x14a);
      puVar1[0x13] = 9;
      local_14 = _inflate_table(1,puVar1 + 0x1a,puVar1[0x16],puVar1 + 0x19,puVar1 + 0x13,
                                puVar1 + 0xba);
      uVar5 = local_38;
      if (local_14 == 0) {
        puVar1[0x12] = puVar1[0x19];
        puVar1[0x14] = 6;
        local_14 = _inflate_table(2,(int)puVar1 + puVar1[0x16] * 2 + 0x68,puVar1[0x17],puVar1 + 0x19
                                  ,puVar1 + 0x14,puVar1 + 0xba);
        if (local_14 == 0) {
          *puVar1 = 0x12;
          uVar7 = local_38;
          goto switchD_00c024f4_caseD_12;
        }
        param_1[6] = (int)"invalid distances set";
      }
      else {
        param_1[6] = (int)"invalid literal/lengths set";
      }
      break;
    case 0x12:
switchD_00c024f4_caseD_12:
      if ((uVar13 < 6) || (local_28 < 0x102)) {
        pbVar4 = *(byte **)(puVar1[0x11] + ((1 << ((byte)puVar1[0x13] & 0x1f)) - 1U & uVar7) * 4);
        local_8 = pbVar4;
        if (uVar10 < ((uint)pbVar4 >> 8 & 0xff)) {
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            uVar10 = uVar10 + 8;
            uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
            pbVar11 = pbVar11 + 1;
            pbVar4 = *(byte **)(puVar1[0x11] + ((1 << ((byte)puVar1[0x13] & 0x1f)) - 1U & uVar7) * 4
                               );
            local_34 = uVar13;
            local_30 = pbVar11;
            local_8 = pbVar4;
          } while (uVar10 < ((uint)pbVar4 >> 8 & 0xff));
        }
        cVar2 = (char)pbVar4;
        local_8 = pbVar4;
        if ((cVar2 != '\0') && (((uint)pbVar4 & 0xf0) == 0)) {
          bVar9 = (byte)((uint)pbVar4 >> 8);
          local_8 = *(byte **)(puVar1[0x11] +
                              ((((1 << (cVar2 + bVar9 & 0x1f)) - 1U & uVar7) >> (bVar9 & 0x1f)) +
                              ((uint)pbVar4 >> 0x10)) * 4);
          uVar5 = (uint)pbVar4 >> 8 & 0xff;
          local_18 = pbVar4;
          if (uVar10 < ((uint)local_8 >> 8 & 0xff) + uVar5) {
            do {
              uVar12 = 0;
              if (uVar13 == 0) goto LAB_00c03717;
              uVar13 = uVar13 - 1;
              uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
              pbVar11 = pbVar11 + 1;
              uVar10 = uVar10 + 8;
              local_8 = *(byte **)(puVar1[0x11] +
                                  ((((1 << (cVar2 + bVar9 & 0x1f)) - 1U & uVar7) >> (bVar9 & 0x1f))
                                  + ((uint)pbVar4 >> 0x10)) * 4);
              local_34 = uVar13;
              local_30 = pbVar11;
            } while (uVar10 < ((uint)local_8 >> 8 & 0xff) + uVar5);
          }
          uVar7 = uVar7 >> (bVar9 & 0x1f);
          uVar10 = uVar10 - uVar5;
        }
        uVar7 = uVar7 >> ((byte)((uint)local_8 >> 8) & 0x1f);
        uVar10 = uVar10 - ((uint)local_8 >> 8 & 0xff);
        puVar1[0xe] = (uint)local_8 >> 0x10;
        local_38 = uVar7;
        if ((char)local_8 == '\0') {
          *puVar1 = 0x17;
          goto LAB_00c036b5;
        }
        if (((uint)local_8 & 0x20) == 0) {
          if (((uint)local_8 & 0x40) == 0) {
            puVar1[0x10] = (uint)local_8 & 0xf;
            *puVar1 = 0x13;
            goto switchD_00c024f4_caseD_13;
          }
          param_1[6] = (int)"invalid literal/length code";
          uVar5 = uVar7;
          break;
        }
        goto LAB_00c0324c;
      }
      param_1[3] = (int)local_20;
      param_1[4] = local_28;
      *param_1 = (int)pbVar11;
      param_1[1] = uVar13;
      puVar1[0xc] = uVar7;
      puVar1[0xd] = uVar10;
      _inflate_fast(param_1,local_1c);
      local_28 = param_1[4];
      local_20 = (byte *)param_1[3];
      pbVar11 = (byte *)*param_1;
      uVar13 = param_1[1];
      uVar10 = puVar1[0xd];
      uVar7 = puVar1[0xc];
      local_38 = puVar1[0xc];
      local_34 = uVar13;
      local_30 = pbVar11;
      goto LAB_00c036b5;
    case 0x13:
switchD_00c024f4_caseD_13:
      if (puVar1[0x10] != 0) {
        if (uVar10 < puVar1[0x10]) {
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            uVar10 = uVar10 + 8;
            uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
            pbVar11 = pbVar11 + 1;
            local_34 = uVar13;
            local_30 = pbVar11;
          } while (uVar10 < puVar1[0x10]);
        }
        puVar1[0xe] = puVar1[0xe] + ((1 << ((byte)puVar1[0x10] & 0x1f)) - 1U & uVar7);
        uVar7 = uVar7 >> ((byte)puVar1[0x10] & 0x1f);
        uVar10 = uVar10 - puVar1[0x10];
      }
      *puVar1 = 0x14;
    case 0x14:
      pbVar4 = *(byte **)(puVar1[0x12] + ((1 << ((byte)puVar1[0x14] & 0x1f)) - 1U & uVar7) * 4);
      local_8 = pbVar4;
      if (uVar10 < ((uint)pbVar4 >> 8 & 0xff)) {
        do {
          uVar12 = 0;
          if (uVar13 == 0) goto LAB_00c03717;
          bVar9 = (byte)uVar10;
          uVar13 = uVar13 - 1;
          uVar10 = uVar10 + 8;
          uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
          pbVar11 = pbVar11 + 1;
          pbVar4 = *(byte **)(puVar1[0x12] + ((1 << ((byte)puVar1[0x14] & 0x1f)) - 1U & uVar7) * 4);
          local_34 = uVar13;
          local_30 = pbVar11;
          local_8 = pbVar4;
        } while (uVar10 < ((uint)pbVar4 >> 8 & 0xff));
      }
      local_8 = pbVar4;
      if (((uint)pbVar4 & 0xf0) == 0) {
        bVar9 = (byte)((uint)pbVar4 >> 8);
        local_8 = *(byte **)(puVar1[0x12] +
                            ((((1 << ((char)pbVar4 + bVar9 & 0x1f)) - 1U & uVar7) >> (bVar9 & 0x1f))
                            + ((uint)pbVar4 >> 0x10)) * 4);
        uVar5 = (uint)pbVar4 >> 8 & 0xff;
        local_18 = pbVar4;
        if (uVar10 < ((uint)local_8 >> 8 & 0xff) + uVar5) {
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            uVar13 = uVar13 - 1;
            uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
            pbVar11 = pbVar11 + 1;
            uVar10 = uVar10 + 8;
            local_8 = *(byte **)(puVar1[0x12] +
                                ((((1 << ((char)pbVar4 + bVar9 & 0x1f)) - 1U & uVar7) >>
                                 (bVar9 & 0x1f)) + ((uint)pbVar4 >> 0x10)) * 4);
            local_34 = uVar13;
            local_30 = pbVar11;
          } while (uVar10 < ((uint)local_8 >> 8 & 0xff) + uVar5);
        }
        uVar7 = uVar7 >> (bVar9 & 0x1f);
        uVar10 = uVar10 - uVar5;
      }
      uVar7 = uVar7 >> ((byte)((uint)local_8 >> 8) & 0x1f);
      uVar10 = uVar10 - ((uint)local_8 >> 8 & 0xff);
      local_38 = uVar7;
      if (((uint)local_8 & 0x40) != 0) {
        param_1[6] = (int)"invalid distance code";
        uVar5 = uVar7;
        break;
      }
      puVar1[0xf] = (uint)local_8 >> 0x10;
      puVar1[0x10] = (uint)local_8 & 0xf;
      *puVar1 = 0x15;
switchD_00c024f4_caseD_15:
      if (puVar1[0x10] != 0) {
        if (uVar10 < puVar1[0x10]) {
          do {
            uVar12 = 0;
            if (uVar13 == 0) goto LAB_00c03717;
            bVar9 = (byte)uVar10;
            uVar13 = uVar13 - 1;
            uVar10 = uVar10 + 8;
            uVar7 = uVar7 + ((uint)*pbVar11 << (bVar9 & 0x1f));
            pbVar11 = pbVar11 + 1;
            local_34 = uVar13;
            local_30 = pbVar11;
          } while (uVar10 < puVar1[0x10]);
        }
        puVar1[0xf] = puVar1[0xf] + ((1 << ((byte)puVar1[0x10] & 0x1f)) - 1U & uVar7);
        uVar7 = uVar7 >> ((byte)puVar1[0x10] & 0x1f);
        uVar10 = uVar10 - puVar1[0x10];
        local_38 = uVar7;
      }
      if ((puVar1[9] - local_28) + local_1c < puVar1[0xf]) {
        param_1[6] = (int)"invalid distance too far back";
        uVar5 = uVar7;
        break;
      }
      *puVar1 = 0x16;
switchD_00c024f4_caseD_16:
      uVar12 = uVar13;
      if (local_28 != 0) {
        local_2c = puVar1[0xf];
        if (local_1c - local_28 < local_2c) {
          local_2c = local_2c - (local_1c - local_28);
          if (puVar1[10] < local_2c) {
            local_2c = local_2c - puVar1[10];
            local_18 = (byte *)((puVar1[0xb] + puVar1[8]) - local_2c);
          }
          else {
            local_18 = (byte *)((puVar1[0xb] - local_2c) + puVar1[10]);
          }
          local_10 = puVar1[0xe];
          if (local_10 < local_2c) goto LAB_00c034f9;
        }
        else {
          local_18 = local_20 + -local_2c;
          local_10 = puVar1[0xe];
LAB_00c034f9:
          local_2c = local_10;
        }
        if (local_28 < local_2c) {
          local_2c = local_28;
        }
        local_28 = local_28 - local_2c;
        puVar1[0xe] = local_10 - local_2c;
        do {
          *local_20 = *local_18;
          local_20 = local_20 + 1;
          local_18 = local_18 + 1;
          local_2c = local_2c - 1;
        } while (local_2c != 0);
        if (puVar1[0xe] == 0) {
          *puVar1 = 0x12;
        }
        goto LAB_00c036b5;
      }
      goto LAB_00c03717;
    case 0x15:
      goto switchD_00c024f4_caseD_15;
    case 0x16:
      goto switchD_00c024f4_caseD_16;
    case 0x17:
      if (local_28 == 0) goto LAB_00c03717;
      *local_20 = (byte)puVar1[0xe];
      local_20 = local_20 + 1;
      local_28 = local_28 - 1;
      *puVar1 = 0x12;
      goto LAB_00c036b5;
    case 0x18:
      if (puVar1[2] != 0) {
        for (; uVar10 < 0x20; uVar10 = uVar10 + 8) {
          uVar12 = 0;
          if (uVar13 == 0) goto LAB_00c03717;
          uVar13 = uVar13 - 1;
          uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
          pbVar11 = pbVar11 + 1;
          local_38 = uVar7;
          local_30 = pbVar11;
          local_34 = uVar13;
        }
        local_1c = local_1c - local_28;
        param_1[5] = param_1[5] + local_1c;
        puVar1[6] = puVar1[6] + local_1c;
        if (local_1c != 0) {
          if (puVar1[4] == 0) {
            uVar7 = _z_adler32(puVar1[5],(int)local_20 - local_1c,local_1c);
          }
          else {
            uVar7 = _z_crc32(puVar1[5],(int)local_20 - local_1c);
          }
          puVar1[5] = uVar7;
          param_1[0xc] = uVar7;
          uVar7 = local_38;
        }
        uVar5 = uVar7;
        if (puVar1[4] == 0) {
          uVar5 = ((uVar7 & 0xff00) + uVar7 * 0x10000) * 0x100 + (local_38 >> 0x10 & 0xff) * 0x100 +
                  (uVar7 >> 0x18);
        }
        local_1c = local_28;
        if (uVar5 != puVar1[5]) {
          param_1[6] = (int)"incorrect data check";
          uVar5 = uVar7;
          break;
        }
        uVar10 = 0;
        local_38 = 0;
        uVar7 = 0;
      }
      *puVar1 = 0x19;
    case 0x19:
      if ((puVar1[2] != 0) && (puVar1[4] != 0)) {
        for (; uVar10 < 0x20; uVar10 = uVar10 + 8) {
          uVar12 = 0;
          if (uVar13 == 0) goto LAB_00c03717;
          uVar13 = uVar13 - 1;
          uVar7 = uVar7 + ((uint)*pbVar11 << ((byte)uVar10 & 0x1f));
          pbVar11 = pbVar11 + 1;
          local_38 = uVar7;
          local_34 = uVar13;
          local_30 = pbVar11;
        }
        if (uVar7 != puVar1[6]) {
          param_1[6] = (int)"incorrect length check";
          uVar5 = uVar7;
          break;
        }
        uVar10 = 0;
        uVar7 = 0;
      }
      *puVar1 = 0x1a;
switchD_00c024f4_caseD_1a:
      local_14 = 1;
      uVar12 = uVar13;
      goto LAB_00c03717;
    case 0x1a:
      goto switchD_00c024f4_caseD_1a;
    case 0x1b:
      local_14 = -3;
      goto LAB_00c03717;
    case 0x1c:
      goto switchD_00c024f4_caseD_1c;
    default:
      return -2;
    }
    *puVar1 = 0x1b;
    uVar7 = uVar5;
LAB_00c036b5:
    uVar5 = *puVar1;
  } while( true );
}



//=== _z_inflateEnd @ 00c038d0 ===

/* [bsim sim=0.9999999999999999 <- ego_r] */

undefined4 _z_inflateEnd(int param_1)

{
  int iVar1;
  
  if (((param_1 != 0) && (*(int *)(param_1 + 0x1c) != 0)) &&
     (*(code **)(param_1 + 0x24) != (code *)0x0)) {
    iVar1 = *(int *)(*(int *)(param_1 + 0x1c) + 0x2c);
    if (iVar1 != 0) {
      (**(code **)(param_1 + 0x24))(*(undefined4 *)(param_1 + 0x28),iVar1);
    }
    (**(code **)(param_1 + 0x24))(*(undefined4 *)(param_1 + 0x28),*(undefined4 *)(param_1 + 0x1c));
    *(undefined4 *)(param_1 + 0x1c) = 0;
    return 0;
  }
  return 0xfffffffe;
}



//=== _z_inflateCopy @ 00c03b80 ===

/* [bsim sim=1.0000000000000002 <- donor] */

undefined4 _z_inflateCopy(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  
  if ((((param_1 == (undefined4 *)0x0) || (param_2 == (undefined4 *)0x0)) ||
      (puVar7 = (undefined4 *)param_2[7], puVar7 == (undefined4 *)0x0)) ||
     (((code *)param_2[8] == (code *)0x0 || (param_2[9] == 0)))) {
    return 0xfffffffe;
  }
  puVar1 = (undefined4 *)(*(code *)param_2[8])(param_2[10],1,0x1ba8);
  if (puVar1 == (undefined4 *)0x0) {
    return 0xfffffffc;
  }
  puVar2 = (undefined4 *)0x0;
  if ((puVar7[0xb] != 0) &&
     (puVar2 = (undefined4 *)(*(code *)param_2[8])(param_2[10],1 << ((byte)puVar7[7] & 0x1f),1),
     puVar2 == (undefined4 *)0x0)) {
    (*(code *)param_2[9])(param_2[10],puVar1);
    return 0xfffffffc;
  }
  puVar6 = param_1;
  for (iVar3 = 0xe; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = *param_2;
    param_2 = param_2 + 1;
    puVar6 = puVar6 + 1;
  }
  puVar6 = puVar7;
  puVar8 = puVar1;
  for (iVar3 = 0x6ea; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar8 = *puVar6;
    puVar6 = puVar6 + 1;
    puVar8 = puVar8 + 1;
  }
  puVar1[0x11] = puVar1 + ((puVar7[0x11] - (int)puVar7) + -0x528 >> 2) + 0x14a;
  puVar1[0x12] = puVar1 + ((puVar7[0x12] - (int)puVar7) + -0x528 >> 2) + 0x14a;
  puVar1[0x19] = puVar1 + ((puVar7[0x19] - (int)puVar7) + -0x528 >> 2) + 0x14a;
  if (puVar2 != (undefined4 *)0x0) {
    uVar5 = 1 << ((byte)puVar7[7] & 0x1f);
    puVar7 = (undefined4 *)puVar7[0xb];
    puVar6 = puVar2;
    for (uVar4 = uVar5 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *puVar6 = *puVar7;
      puVar7 = puVar7 + 1;
      puVar6 = puVar6 + 1;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined1 *)puVar6 = *(undefined1 *)puVar7;
      puVar7 = (undefined4 *)((int)puVar7 + 1);
      puVar6 = (undefined4 *)((int)puVar6 + 1);
    }
  }
  puVar1[0xb] = puVar2;
  param_1[7] = puVar1;
  return 0;
}



//=== _inflate_fast @ 00c06060 ===

/* [bsim sim=1.0000000000000002 <- ego_r] */

void _inflate_fast(int *param_1,uint param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  uint uVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  int iVar18;
  uint uVar19;
  uint uVar20;
  int iVar21;
  int iVar22;
  undefined1 *puVar23;
  undefined1 *puVar24;
  undefined1 *puVar25;
  undefined1 *puVar26;
  uint uVar27;
  uint local_38;
  
  uVar16 = *param_1 - 1;
  uVar3 = param_1[1] + -5 + uVar16;
  puVar4 = (undefined4 *)param_1[7];
  iVar5 = param_1[4];
  iVar14 = iVar5 - param_2;
  puVar23 = (undefined1 *)(param_1[3] + -1);
  uVar6 = puVar4[9];
  iVar18 = puVar4[8];
  iVar21 = puVar4[0xb];
  uVar19 = puVar4[0xd];
  uVar7 = puVar4[10];
  iVar8 = puVar4[0x12];
  iVar9 = puVar4[0x11];
  uVar10 = puVar4[0x13];
  uVar11 = puVar4[0x14];
  uVar27 = puVar4[0xc];
  puVar24 = puVar23;
  local_38 = uVar16;
LAB_00c060f2:
  if (uVar19 < 0xf) {
    pbVar1 = (byte *)(uVar16 + 1);
    pbVar2 = (byte *)(uVar16 + 2);
    uVar16 = uVar16 + 2;
    uVar27 = uVar27 + ((uint)*pbVar1 << ((byte)uVar19 & 0x1f)) +
             ((uint)*pbVar2 << ((byte)uVar19 + 8 & 0x1f));
    uVar19 = uVar19 + 0x10;
    local_38 = uVar16;
  }
  uVar12 = *(uint *)(iVar9 + ((1 << ((byte)uVar10 & 0x1f)) - 1U & uVar27) * 4);
  uVar27 = uVar27 >> ((byte)(uVar12 >> 8) & 0x1f);
  uVar19 = uVar19 - (uVar12 >> 8 & 0xff);
  while ((uVar12 & 0xff) != 0) {
    puVar25 = puVar24;
    if ((uVar12 & 0x10) != 0) {
      param_2 = uVar12 >> 0x10;
      uVar12 = uVar12 & 0xf;
      uVar17 = uVar16;
      if (uVar12 != 0) {
        if (uVar19 < uVar12) {
          pbVar1 = (byte *)(uVar16 + 1);
          uVar16 = uVar16 + 1;
          uVar27 = uVar27 + ((uint)*pbVar1 << ((byte)uVar19 & 0x1f));
          uVar19 = uVar19 + 8;
          local_38 = uVar16;
        }
        param_2 = param_2 + ((1 << (sbyte)uVar12) - 1U & uVar27);
        uVar27 = uVar27 >> (sbyte)uVar12;
        uVar19 = uVar19 - uVar12;
        uVar17 = uVar16;
      }
      if (uVar19 < 0xf) {
        pbVar1 = (byte *)(uVar17 + 1);
        pbVar2 = (byte *)(uVar17 + 2);
        uVar17 = uVar17 + 2;
        uVar27 = uVar27 + ((uint)*pbVar1 << ((byte)uVar19 & 0x1f)) +
                 ((uint)*pbVar2 << ((byte)uVar19 + 8 & 0x1f));
        uVar19 = uVar19 + 0x10;
        local_38 = uVar17;
      }
      uVar12 = *(uint *)(iVar8 + ((1 << ((byte)uVar11 & 0x1f)) - 1U & uVar27) * 4);
      uVar27 = uVar27 >> ((byte)(uVar12 >> 8) & 0x1f);
      uVar19 = uVar19 - (uVar12 >> 8 & 0xff);
      goto joined_r0x00c0620f;
    }
    if ((uVar12 & 0x40) != 0) {
      if ((uVar12 & 0x20) == 0) {
        param_1[6] = (int)"invalid literal/length code";
        goto LAB_00c06471;
      }
      *puVar4 = 0xb;
      goto LAB_00c06478;
    }
    uVar12 = *(uint *)(iVar9 + (((1 << ((byte)uVar12 & 0x1f)) - 1U & uVar27) + (uVar12 >> 0x10)) * 4
                      );
    uVar27 = uVar27 >> ((byte)(uVar12 >> 8) & 0x1f);
    uVar19 = uVar19 - (uVar12 >> 8 & 0xff);
  }
  puVar24[1] = (char)(uVar12 >> 0x10);
  puVar25 = puVar24 + 1;
  goto LAB_00c06421;
joined_r0x00c0620f:
  uVar16 = uVar17;
  if ((uVar12 & 0x10) != 0) goto LAB_00c0624f;
  if ((uVar12 & 0x40) != 0) {
    param_1[6] = (int)"invalid distance code";
LAB_00c06471:
    *puVar4 = 0x1b;
    goto LAB_00c06478;
  }
  uVar12 = *(uint *)(iVar8 + (((1 << ((byte)uVar12 & 0x1f)) - 1U & uVar27) + (uVar12 >> 0x10)) * 4);
  uVar27 = uVar27 >> ((byte)(uVar12 >> 8) & 0x1f);
  uVar19 = uVar19 - (uVar12 >> 8 & 0xff);
  goto joined_r0x00c0620f;
LAB_00c0624f:
  uVar13 = uVar12 & 0xf;
  uVar20 = uVar19;
  if (uVar19 < uVar13) {
    uVar16 = uVar17 + 1;
    uVar20 = uVar19 + 8;
    uVar27 = uVar27 + ((uint)*(byte *)(uVar17 + 1) << ((byte)uVar19 & 0x1f));
    local_38 = uVar16;
    if (uVar20 < uVar13) {
      uVar16 = uVar17 + 2;
      uVar27 = uVar27 + ((uint)*(byte *)(uVar17 + 2) << ((byte)uVar20 & 0x1f));
      uVar20 = uVar19 + 0x10;
      local_38 = uVar16;
    }
  }
  uVar19 = uVar20 - uVar13;
  uVar12 = (uVar12 >> 0x10) + ((1 << (sbyte)uVar13) - 1U & uVar27);
  uVar27 = uVar27 >> (sbyte)uVar13;
  if (uVar12 <= (uint)((int)puVar24 - (int)(puVar23 + iVar14))) {
    iVar15 = (int)puVar24 - uVar12;
    do {
      puVar26 = puVar24;
      iVar22 = iVar15;
      puVar26[1] = *(undefined1 *)(iVar22 + 1);
      puVar26[2] = *(undefined1 *)(iVar22 + 2);
      puVar25 = puVar26 + 3;
      *puVar25 = *(undefined1 *)(iVar22 + 3);
      param_2 = param_2 - 3;
      iVar15 = iVar22 + 3;
      puVar24 = puVar25;
    } while (2 < param_2);
    if (param_2 != 0) {
      puVar25 = puVar26 + 4;
      *puVar25 = *(undefined1 *)(iVar22 + 4);
      if (1 < param_2) {
        puVar25 = puVar26 + 5;
        *puVar25 = *(undefined1 *)(iVar22 + 5);
      }
    }
    goto LAB_00c06421;
  }
  uVar17 = uVar12 - ((int)puVar24 - (int)(puVar23 + iVar14));
  if (uVar6 < uVar17) {
    param_1[6] = (int)"invalid distance too far back";
    *puVar4 = 0x1b;
LAB_00c06478:
    iVar18 = uVar16 - (uVar19 >> 3);
    iVar21 = uVar19 + (uVar19 >> 3) * -8;
    *param_1 = iVar18 + 1;
    param_1[3] = (int)(puVar25 + 1);
    param_1[1] = (uVar3 - iVar18) + 5;
    param_1[4] = (int)(puVar23 + iVar5 + -0x101 + (0x101 - (int)puVar25));
    puVar4[0xc] = uVar27 & (1 << ((byte)iVar21 & 0x1f)) - 1U;
    puVar4[0xd] = iVar21;
    return;
  }
  iVar15 = iVar21 + -1;
  if (uVar7 == 0) {
    iVar15 = iVar15 + (iVar18 - uVar17);
    if (uVar17 < param_2) {
      param_2 = param_2 - uVar17;
      do {
        puVar25 = (undefined1 *)(iVar15 + 1);
        iVar15 = iVar15 + 1;
        puVar24 = puVar24 + 1;
        uVar17 = uVar17 - 1;
        *puVar24 = *puVar25;
      } while (uVar17 != 0);
LAB_00c0636f:
      iVar15 = (int)puVar24 - uVar12;
    }
  }
  else if (uVar7 < uVar17) {
    iVar15 = iVar15 + (uVar7 - uVar17) + iVar18;
    uVar17 = uVar17 - uVar7;
    if (uVar17 < param_2) {
      param_2 = param_2 - uVar17;
      do {
        puVar25 = (undefined1 *)(iVar15 + 1);
        iVar15 = iVar15 + 1;
        puVar24 = puVar24 + 1;
        uVar17 = uVar17 - 1;
        *puVar24 = *puVar25;
      } while (uVar17 != 0);
      iVar15 = iVar21 + -1;
      if (uVar7 < param_2) {
        param_2 = param_2 - uVar7;
        uVar17 = uVar7;
        do {
          puVar25 = (undefined1 *)(iVar15 + 1);
          iVar15 = iVar15 + 1;
          puVar24 = puVar24 + 1;
          uVar17 = uVar17 - 1;
          *puVar24 = *puVar25;
        } while (uVar17 != 0);
        goto LAB_00c0636f;
      }
    }
  }
  else {
    iVar15 = iVar15 + (uVar7 - uVar17);
    if (uVar17 < param_2) {
      param_2 = param_2 - uVar17;
      do {
        puVar25 = (undefined1 *)(iVar15 + 1);
        iVar15 = iVar15 + 1;
        puVar24 = puVar24 + 1;
        uVar17 = uVar17 - 1;
        *puVar24 = *puVar25;
      } while (uVar17 != 0);
      goto LAB_00c0636f;
    }
  }
  if (2 < param_2) {
    iVar22 = (param_2 - 3) / 3 + 1;
    do {
      puVar24[1] = *(undefined1 *)(iVar15 + 1);
      puVar24[2] = *(undefined1 *)(iVar15 + 2);
      puVar25 = (undefined1 *)(iVar15 + 3);
      iVar15 = iVar15 + 3;
      puVar24 = puVar24 + 3;
      param_2 = param_2 - 3;
      iVar22 = iVar22 + -1;
      *puVar24 = *puVar25;
      uVar16 = local_38;
    } while (iVar22 != 0);
  }
  puVar25 = puVar24;
  if (param_2 != 0) {
    puVar25 = puVar24 + 1;
    *puVar25 = *(undefined1 *)(iVar15 + 1);
    if (1 < param_2) {
      puVar25 = puVar24 + 2;
      *puVar25 = *(undefined1 *)(iVar15 + 2);
    }
  }
LAB_00c06421:
  if ((uVar3 <= uVar16) || (puVar24 = puVar25, puVar23 + iVar5 + -0x101 <= puVar25))
  goto LAB_00c06478;
  goto LAB_00c060f2;
}



//=== _inflate_table @ 00c064d0 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.9672455093046319 <- ego_r] */

undefined4
_inflate_table(int param_1,int param_2,uint param_3,int *param_4,uint *param_5,ushort *param_6)

{
  short *psVar1;
  short sVar2;
  ushort uVar3;
  uint uVar4;
  uint uVar5;
  byte bVar6;
  byte bVar7;
  int *piVar8;
  int iVar9;
  short sVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  byte bVar14;
  uint uVar15;
  uint uVar16;
  ushort *puVar17;
  undefined4 local_7c;
  uint local_78;
  uint local_74;
  int local_6c;
  ushort *local_68;
  uint local_64;
  int local_60;
  uint local_5c;
  uint local_58;
  ushort *local_54;
  ushort *local_50;
  undefined4 local_44;
  ushort local_40 [30];
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  uVar4 = 0;
  local_44 = 0;
  local_40[0] = 0;
  local_40[1] = 0;
  local_40[2] = 0;
  local_40[3] = 0;
  local_40[4] = 0;
  local_40[5] = 0;
  local_40[6] = 0;
  local_40[7] = 0;
  local_40[8] = 0;
  local_40[9] = 0;
  local_40[10] = 0;
  local_40[0xb] = 0;
  local_40[0xc] = 0;
  local_40[0xd] = 0;
  if (param_3 != 0) {
    do {
      psVar1 = (short *)((int)&local_44 + (uint)*(ushort *)(param_2 + uVar4 * 2) * 2);
      *psVar1 = *psVar1 + 1;
      uVar4 = uVar4 + 1;
    } while (uVar4 < param_3);
  }
  uVar4 = 0xf;
  do {
    if (*(short *)((int)&local_44 + uVar4 * 2) != 0) break;
    uVar4 = uVar4 - 1;
  } while (uVar4 != 0);
  local_78 = *param_5;
  if (uVar4 < *param_5) {
    local_78 = uVar4;
  }
  if (uVar4 == 0) {
    return 0xffffffff;
  }
  local_74 = 1;
  do {
    if (*(short *)((int)&local_44 + local_74 * 2) != 0) break;
    if (*(short *)((int)&local_44 + local_74 * 2 + 2) != 0) {
      local_74 = local_74 + 1;
      break;
    }
    if (local_40[local_74] != 0) {
      local_74 = local_74 + 2;
      break;
    }
    if (local_40[local_74 + 1] != 0) {
      local_74 = local_74 + 3;
      break;
    }
    if (local_40[local_74 + 2] != 0) {
      local_74 = local_74 + 4;
      break;
    }
    local_74 = local_74 + 5;
  } while (local_74 < 0x10);
  if (local_78 < local_74) {
    local_78 = local_74;
  }
  iVar11 = 1;
  uVar5 = 1;
  do {
    iVar11 = iVar11 * 2 - (uint)*(ushort *)((int)&local_44 + uVar5 * 2);
    if (iVar11 < 0) {
      return 0xffffffff;
    }
    uVar5 = uVar5 + 1;
  } while (uVar5 < 0x10);
  if ((0 < iVar11) && ((param_1 == 0 || (param_3 - (local_44 & 0xffff) != 1)))) {
    return 0xffffffff;
  }
  local_40[0xf] = 0;
  uVar5 = 2;
  do {
    sVar10 = *(short *)((int)local_40 + uVar5 + 0x1c) + *(short *)((int)&local_44 + uVar5);
    sVar2 = *(short *)((int)&local_44 + uVar5 + 2);
    *(short *)((int)local_40 + uVar5 + 0x1e) = sVar10;
    *(short *)((int)local_40 + uVar5 + 0x20) = sVar2 + sVar10;
    uVar5 = uVar5 + 4;
  } while (uVar5 < 0x1e);
  uVar5 = 0;
  if (param_3 != 0) {
    do {
      if (*(short *)(param_2 + uVar5 * 2) != 0) {
        param_6[local_40[*(ushort *)(param_2 + uVar5 * 2) + 0xe]] = (ushort)uVar5;
        local_40[*(ushort *)(param_2 + uVar5 * 2) + 0xe] =
             local_40[*(ushort *)(param_2 + uVar5 * 2) + 0xe] + 1;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < param_3);
  }
  if (param_1 == 0) {
    local_54 = param_6;
    local_60 = 0x13;
    local_50 = param_6;
  }
  else if (param_1 == 1) {
    local_54 = (ushort *)&DAT_012b1656;
    local_50 = (ushort *)&DAT_012b1696;
    local_60 = 0x100;
  }
  else {
    local_54 = (ushort *)&DAT_012b18d8;
    local_50 = (ushort *)&DAT_012b1918;
    local_60 = -1;
  }
  local_6c = *param_4;
  bVar6 = (byte)local_78;
  local_64 = 1 << (bVar6 & 0x1f);
  local_5c = 0xffffffff;
  uVar16 = 0;
  uVar15 = 0;
  uVar5 = local_64 - 1;
  if ((param_1 != 1) || (local_64 < 0x506)) {
    local_68 = param_6;
    local_58 = local_64;
LAB_00c06750:
    do {
      uVar3 = *local_68;
      bVar14 = (byte)uVar15;
      bVar7 = (char)local_74 - bVar14;
      if ((int)(uint)uVar3 < local_60) {
        local_7c = (uint)CONCAT21(uVar3,bVar7) << 8;
      }
      else if (local_60 < (int)(uint)uVar3) {
        local_7c = CONCAT22(local_54[*local_68],CONCAT11(bVar7,(char)local_50[*local_68]));
      }
      else {
        local_7c = CONCAT31((uint3)bVar7,0x60);
      }
      iVar11 = 1 << ((char)local_74 - bVar14 & 0x1f);
      piVar8 = (int *)(local_6c + ((uVar16 >> (bVar14 & 0x1f)) + local_58) * 4);
      uVar12 = local_58;
      do {
        uVar12 = uVar12 - iVar11;
        piVar8 = piVar8 + -iVar11;
        *piVar8 = local_7c;
      } while (uVar12 != 0);
      for (uVar12 = 1 << ((char)local_74 - 1U & 0x1f); (uVar16 & uVar12) != 0; uVar12 = uVar12 >> 1)
      {
      }
      if (uVar12 == 0) {
        uVar16 = 0;
      }
      else {
        uVar16 = (uVar12 - 1 & uVar16) + uVar12;
      }
      local_68 = local_68 + 1;
      psVar1 = (short *)((int)&local_44 + local_74 * 2);
      *psVar1 = *psVar1 + -1;
      if (*(short *)((int)&local_44 + local_74 * 2) == 0) {
        if (local_74 == uVar4) {
          local_7c._0_2_ = CONCAT11((char)local_74 - bVar14,0x40);
          local_7c = (uint)(ushort)local_7c;
          goto joined_r0x00c06926;
        }
        local_74 = (uint)*(ushort *)(param_2 + (uint)*local_68 * 2);
      }
    } while ((local_74 <= local_78) || (uVar12 = uVar5 & uVar16, uVar12 == local_5c));
    if (uVar15 == 0) {
      uVar15 = local_78;
    }
    local_6c = local_6c + local_58 * 4;
    iVar9 = local_74 - uVar15;
    uVar13 = uVar15 + iVar9;
    iVar11 = 1 << ((byte)iVar9 & 0x1f);
    if (uVar13 < uVar4) {
      puVar17 = (ushort *)((int)&local_44 + uVar13 * 2);
      do {
        uVar3 = *puVar17;
        if ((int)(iVar11 - (uint)uVar3) < 1) break;
        iVar9 = iVar9 + 1;
        uVar13 = uVar13 + 1;
        puVar17 = puVar17 + 1;
        iVar11 = (iVar11 - (uint)uVar3) * 2;
      } while (uVar13 < uVar4);
    }
    local_58 = 1 << ((byte)iVar9 & 0x1f);
    local_64 = local_64 + local_58;
    if ((param_1 != 1) || (local_64 < 0x506)) {
      *(byte *)(*param_4 + uVar12 * 4) = (byte)iVar9;
      *(byte *)(*param_4 + 1 + uVar12 * 4) = bVar6;
      *(short *)(*param_4 + 2 + uVar12 * 4) = (short)(local_6c - *param_4 >> 2);
      local_5c = uVar12;
      goto LAB_00c06750;
    }
  }
  return 1;
joined_r0x00c06926:
  if (uVar16 == 0) {
LAB_00c06985:
    *param_4 = *param_4 + local_64 * 4;
    *param_5 = local_78;
    return 0;
  }
  if ((uVar15 != 0) && ((uVar5 & uVar16) != local_5c)) {
    local_6c = *param_4;
    uVar15 = 0;
    local_7c._0_2_ = CONCAT11(bVar6,(undefined1)local_7c);
    local_7c = (uint)(ushort)local_7c;
    local_74 = local_78;
  }
  *(uint *)(local_6c + (uVar16 >> ((byte)uVar15 & 0x1f)) * 4) = local_7c;
  for (uVar4 = 1 << ((char)local_74 - 1U & 0x1f); (uVar16 & uVar4) != 0; uVar4 = uVar4 >> 1) {
  }
  if (uVar4 == 0) goto LAB_00c06985;
  uVar16 = (uVar4 - 1 & uVar16) + uVar4;
  goto joined_r0x00c06926;
}



//=== _lzo1x_decompress @ 00c06b90 ===

/* [bsim sim=1.0000000000000002 <- ego_r] */

int _lzo1x_decompress(ushort *param_1,int param_2,byte *param_3,int *param_4)

{
  ushort *puVar1;
  byte bVar2;
  byte *pbVar3;
  byte *pbVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  ushort *puVar8;
  
  *param_4 = 0;
  puVar1 = (ushort *)((int)param_1 + param_2);
  pbVar4 = param_3;
  if ((byte)*param_1 < 0x12) goto LAB_00c06bcf;
  uVar5 = (byte)*param_1 - 0x11;
  param_1 = (ushort *)((int)param_1 + 1);
  pbVar3 = param_3;
  if (uVar5 < 4) goto LAB_00c06c80;
  do {
    *pbVar3 = (byte)*param_1;
    pbVar3 = pbVar3 + 1;
    param_1 = (ushort *)((int)param_1 + 1);
    uVar5 = uVar5 - 1;
  } while (uVar5 != 0);
LAB_00c06c3a:
  uVar5 = (uint)(byte)*param_1;
  puVar8 = (ushort *)((int)param_1 + 1);
  pbVar4 = pbVar3;
  if (0xf < uVar5) goto LAB_00c06c8d;
  iVar7 = (uint)*(byte *)puVar8 * -4 - (uint)(byte)((byte)*param_1 >> 2);
  param_1 = param_1 + 1;
  *pbVar3 = pbVar3[iVar7 + -0x801];
  pbVar4 = pbVar3 + 1;
  pbVar3 = pbVar3 + iVar7 + -0x800;
LAB_00c06c63:
  *pbVar4 = *pbVar3;
  pbVar4[1] = pbVar3[1];
  pbVar4 = pbVar4 + 2;
LAB_00c06c6e:
  uVar5 = (byte)param_1[-1] & 3;
  if (((byte)param_1[-1] & 3) == 0) {
LAB_00c06bcf:
    uVar5 = (uint)(byte)*param_1;
    puVar8 = (ushort *)((int)param_1 + 1);
    if (uVar5 < 0x10) goto code_r0x00c06bdc;
  }
  else {
LAB_00c06c80:
    do {
      puVar8 = param_1;
      *pbVar4 = (byte)*puVar8;
      pbVar4 = pbVar4 + 1;
      uVar5 = uVar5 - 1;
      param_1 = (ushort *)((int)puVar8 + 1);
    } while (uVar5 != 0);
    uVar5 = (uint)*(byte *)((int)puVar8 + 1);
    puVar8 = puVar8 + 1;
  }
LAB_00c06c8d:
  if (0x3f < uVar5) {
    pbVar3 = pbVar4 + ((uint)(byte)*puVar8 * -8 - (uVar5 >> 2 & 7)) + -1;
    param_1 = (ushort *)((int)puVar8 + 1);
    uVar6 = (uVar5 >> 5) - 1;
LAB_00c06cac:
    *pbVar4 = *pbVar3;
    pbVar4[1] = pbVar3[1];
    pbVar4 = pbVar4 + 2;
    pbVar3 = pbVar3 + 2;
    do {
      *pbVar4 = *pbVar3;
      pbVar4 = pbVar4 + 1;
      pbVar3 = pbVar3 + 1;
      uVar6 = uVar6 - 1;
    } while (uVar6 != 0);
    goto LAB_00c06c6e;
  }
  if (uVar5 < 0x20) {
    if (uVar5 < 0x10) goto LAB_00c06d9e;
    uVar6 = uVar5 & 7;
    if (uVar6 == 0) {
      iVar7 = 0;
      bVar2 = (byte)*puVar8;
      while (bVar2 == 0) {
        pbVar3 = (byte *)((int)puVar8 + 1);
        iVar7 = iVar7 + 0xff;
        puVar8 = (ushort *)((int)puVar8 + 1);
        bVar2 = *pbVar3;
      }
      uVar6 = iVar7 + 7 + (uint)(byte)*puVar8;
      puVar8 = (ushort *)((int)puVar8 + 1);
    }
    param_1 = puVar8 + 1;
    if (pbVar4 + ((uVar5 & 8) * -0x800 - (uint)(*puVar8 >> 2)) == pbVar4) {
      *param_4 = (int)pbVar4 - (int)param_3;
      if (param_1 == puVar1) {
        return 0;
      }
      return (-(uint)(param_1 < puVar1) & 0xfffffffc) - 4;
    }
    pbVar3 = pbVar4 + ((uVar5 & 8) * -0x800 - (uint)(*puVar8 >> 2)) + -0x4000;
  }
  else {
    uVar6 = uVar5 & 0x1f;
    if (uVar6 == 0) {
      iVar7 = 0;
      if ((byte)*puVar8 == 0) {
        iVar7 = 0;
        do {
          pbVar3 = (byte *)((int)puVar8 + 1);
          iVar7 = iVar7 + 0xff;
          puVar8 = (ushort *)((int)puVar8 + 1);
        } while (*pbVar3 == 0);
      }
      uVar6 = iVar7 + 0x1f + (uint)(byte)*puVar8;
      puVar8 = (ushort *)((int)puVar8 + 1);
    }
    pbVar3 = pbVar4 + (-1 - (uint)(*puVar8 >> 2));
    param_1 = puVar8 + 1;
  }
  if ((uVar6 < 6) || ((int)pbVar4 - (int)pbVar3 < 4)) goto LAB_00c06cac;
  *(undefined4 *)pbVar4 = *(undefined4 *)pbVar3;
  pbVar4 = pbVar4 + 4;
  pbVar3 = pbVar3 + 4;
  uVar6 = uVar6 - 2;
  do {
    *(undefined4 *)pbVar4 = *(undefined4 *)pbVar3;
    uVar6 = uVar6 - 4;
    pbVar4 = pbVar4 + 4;
    pbVar3 = pbVar3 + 4;
  } while (3 < uVar6);
  for (; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pbVar4 = *pbVar3;
    pbVar4 = pbVar4 + 1;
    pbVar3 = pbVar3 + 1;
  }
  goto LAB_00c06c6e;
code_r0x00c06bdc:
  if (uVar5 == 0) {
    iVar7 = 0;
    bVar2 = *(byte *)puVar8;
    while (bVar2 == 0) {
      pbVar3 = (byte *)((int)puVar8 + 1);
      iVar7 = iVar7 + 0xff;
      puVar8 = (ushort *)((int)puVar8 + 1);
      bVar2 = *pbVar3;
    }
    uVar5 = iVar7 + 0xf + (uint)(byte)*puVar8;
    puVar8 = (ushort *)((int)puVar8 + 1);
  }
  *(undefined4 *)pbVar4 = *(undefined4 *)puVar8;
  pbVar3 = pbVar4 + 4;
  param_1 = puVar8 + 2;
  uVar5 = uVar5 - 1;
  if (uVar5 != 0) {
    if (uVar5 < 4) {
      do {
        *pbVar3 = (byte)*param_1;
        pbVar3 = pbVar3 + 1;
        param_1 = (ushort *)((int)param_1 + 1);
        uVar5 = uVar5 - 1;
      } while (uVar5 != 0);
    }
    else {
      do {
        *(undefined4 *)pbVar3 = *(undefined4 *)param_1;
        uVar5 = uVar5 - 4;
        pbVar3 = pbVar3 + 4;
        param_1 = param_1 + 2;
      } while (3 < uVar5);
      for (; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pbVar3 = (byte)*param_1;
        pbVar3 = pbVar3 + 1;
        param_1 = (ushort *)((int)param_1 + 1);
      }
    }
  }
  goto LAB_00c06c3a;
LAB_00c06d9e:
  pbVar3 = pbVar4 + ((uint)(byte)*puVar8 * -4 - (uVar5 >> 2)) + -1;
  param_1 = (ushort *)((int)puVar8 + 1);
  goto LAB_00c06c63;
}



//=== _lzo1x_999_compress_internal @ 00c07af0 ===

/* [bsim sim=1.0000000000000002 <- ego_r] */

int _lzo1x_999_compress_internal
              (int param_1,int param_2,undefined1 *param_3,undefined4 *param_4,int param_5,
              undefined4 param_6,undefined4 param_7,code *param_8,uint param_9,uint param_10,
              uint param_11,int param_12,uint param_13)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  undefined4 *puVar8;
  uint local_90;
  undefined1 *local_8c;
  int local_80;
  undefined4 local_78;
  uint local_74;
  uint local_70;
  uint local_6c;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  undefined1 *local_50;
  code *local_4c;
  uint local_48;
  undefined1 *local_44;
  uint local_40;
  int iStack_30;
  uint local_2c;
  uint local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  int local_4;
  
  iVar3 = _lzo_assert(1);
  if (iVar3 == 0) {
    return -1;
  }
  uVar7 = 0;
  if ((int)param_9 < 0) {
    param_9 = 1;
  }
  if (param_10 == 0) {
    param_10 = 0x20;
  }
  if (param_11 == 0) {
    param_11 = 0x20;
  }
  if (param_12 == 0) {
    param_12 = 0;
  }
  if (param_13 == 0) {
    param_13 = 0x800;
  }
  local_54 = param_1 + param_2;
  local_58 = param_1;
  local_5c = param_1;
  local_80 = param_1;
  local_50 = param_3;
  local_4c = param_8;
  local_8c = param_3;
  local_78 = 0;
  local_14 = 0;
  local_18 = 0;
  local_1c = 0;
  local_20 = 0;
  local_24 = 0;
  local_8 = 0;
  local_c = 0;
  local_10 = 0;
  local_28 = 0;
  local_2c = 0;
  iVar3 = init_match(param_5,param_6);
  if (iVar3 == 0) {
    if (param_13 != 0) {
      *(uint *)(param_5 + 0xc) = param_13;
    }
    if (param_12 != 0) {
      *(int *)(param_5 + 0x10) = param_12;
    }
    iVar3 = find_match(0);
    if (iVar3 == 0) {
      if (local_74 != 0) {
        do {
          uVar2 = local_6c;
          uVar1 = local_70;
          local_44 = (undefined1 *)((int)local_8c - (int)param_3);
          if (uVar7 == 0) {
            local_80 = local_60;
          }
          if (local_70 < 2) {
LAB_00c07d8a:
            uVar7 = uVar7 + 1;
            *(uint *)(param_5 + 0xc) = param_13;
            find_match(0);
          }
          else {
            if (local_70 == 2) {
              if ((((local_6c < 0x401) && (uVar7 != 0)) && (uVar7 < 4)) && (local_8c != param_3))
              goto LAB_00c07cc3;
              goto LAB_00c07d8a;
            }
            if ((local_8c == param_3) && (uVar7 == 0)) goto LAB_00c07d8a;
            if (local_70 == 3) {
              if ((0xc00 < local_6c) && (3 < uVar7)) goto LAB_00c07d8a;
            }
            else if (local_70 == 0) goto LAB_00c07d8a;
LAB_00c07cc3:
            if (*(int *)(param_5 + 0x14) != 0) {
              better_match();
            }
            local_90 = 0;
            if ((0 < (int)param_9) && (uVar1 < param_11)) {
              local_4 = len_of_coded_match();
              uVar4 = local_4 - 1U;
              if ((int)param_9 <= (int)(local_4 - 1U)) {
                uVar4 = param_9;
              }
              if (uVar4 != 0) {
                do {
                  if (local_74 <= uVar1) break;
                  if (uVar1 < param_10) {
                    *(uint *)(param_5 + 0xc) = param_13;
                  }
                  else {
                    *(uint *)(param_5 + 0xc) = param_13 >> 2;
                  }
                  local_48 = local_48 + 1;
                  *(undefined4 *)(param_5 + 0x1c) = 1;
                  if (*(int *)(param_5 + 0x14) != 0) {
                    puVar8 = (undefined4 *)(param_5 + 0xbc);
                    for (iVar3 = 0x22; iVar3 != 0; iVar3 = iVar3 + -1) {
                      *puVar8 = 0;
                      puVar8 = puVar8 + 1;
                    }
                  }
                  swd_findbest();
                  local_70 = *(uint *)(param_5 + 0x1c);
                  local_6c = *(uint *)(param_5 + 0x20);
                  swd_getbyte();
                  if (*(int *)(param_5 + 0x28) < 0) {
                    local_74 = 0;
                    local_70 = 0;
                  }
                  else {
                    local_74 = *(int *)(param_5 + 0x24) + 1;
                  }
                  local_60 = local_5c - local_74;
                  if ((local_4c != (code *)0x0) && (local_40 < local_48)) {
                    (*local_4c)(local_48,local_44);
                    local_40 = local_40 + 0x400;
                  }
                  local_90 = local_90 + 1;
                  if ((uVar1 <= local_70) && ((local_70 != uVar1 || (local_6c < uVar2)))) {
                    if (*(int *)(param_5 + 0x14) != 0) {
                      better_match();
                    }
                    iVar3 = len_of_coded_match();
                    if (-1 < iVar3) {
                      if (local_8c == param_3) {
                        iVar5 = -1;
                      }
                      else {
                        iVar5 = len_of_coded_match();
                      }
                      if (uVar7 < 4) {
                        uVar6 = -(uint)(3 < local_90 + uVar7) & 2;
LAB_00c07f24:
                        uVar6 = uVar6 + local_90;
                      }
                      else {
                        uVar6 = local_90;
                        if (uVar7 < 0x13) {
                          uVar6 = (uint)(0x12 < local_90 + uVar7);
                          goto LAB_00c07f24;
                        }
                      }
                      iVar3 = uVar6 + (iVar3 - local_4) * 2;
                      if (0 < iVar5) {
                        iVar3 = iVar3 + (iVar5 - local_90) * 2;
                      }
                      if (iVar3 < 0) {
                        iVar3 = 0;
                      }
                      if (iVar3 + uVar1 <= local_70) {
                        iStack_30 = iStack_30 + 1;
                        if (iVar5 < 1) {
                          uVar7 = uVar7 + local_90;
                        }
                        else {
                          if (uVar7 == 0) {
                            local_28 = 0;
                            local_2c = 0;
                          }
                          else {
                            STORE_RUN(&local_78,local_80,uVar7);
                            local_2c = uVar7;
                            local_28 = local_90;
                          }
                          uVar7 = 0;
                          local_8c = (undefined1 *)code_match(&local_78,uVar2);
                        }
                        goto LAB_00c0800e;
                      }
                    }
                  }
                } while (local_90 < uVar4);
              }
            }
            uVar4 = 0;
            if (uVar7 == 0) {
              local_2c = 0;
            }
            else {
              STORE_RUN(&local_78,local_80,uVar7);
              uVar4 = uVar1;
              local_2c = uVar7;
            }
            uVar7 = 0;
            local_28 = uVar4;
            local_8c = (undefined1 *)code_match(&local_78,uVar2);
            *(uint *)(param_5 + 0xc) = param_13;
            find_match(local_90 + 1);
          }
LAB_00c0800e:
        } while (local_74 != 0);
        if (uVar7 != 0) {
          local_8c = (undefined1 *)STORE_RUN(&local_78,local_80,uVar7);
        }
      }
      *local_8c = 0x11;
      local_8c[1] = 0;
      local_8c[2] = 0;
      local_44 = local_8c + (3 - (int)param_3);
      *param_4 = local_44;
      if (local_4c != (code *)0x0) {
        (*local_4c)(local_48,local_44);
      }
      iVar3 = 0;
    }
  }
  return iVar3;
}



//=== _lzo1x_999_compress @ 00c08130 ===

/* [bsim sim=0.9999999999999998 <- ego_r] */

void _lzo1x_999_compress(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4
                        ,undefined4 param_5)

{
  _lzo1x_999_compress_internal(param_1,param_2,param_3,param_4,param_5,0,0,0,2,0x20,0x80,0,0x800,1);
  return;
}



//=== _lzo1x_decompress_safe @ 00c08170 ===

/* [bsim sim=1.0 <- ego_r] */

int _lzo1x_decompress_safe(ushort *param_1,int param_2,byte *param_3,int *param_4)

{
  ushort *puVar1;
  byte bVar2;
  ushort uVar3;
  byte *pbVar4;
  ushort *puVar5;
  byte *pbVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  byte *pbVar11;
  
  puVar1 = (ushort *)((int)param_1 + param_2);
  pbVar4 = (byte *)*param_4;
  *param_4 = 0;
  bVar2 = (byte)*param_1;
  pbVar11 = param_3;
  if (bVar2 < 0x12) goto LAB_00c081d5;
  pbVar6 = (byte *)(bVar2 - 0x11);
  puVar5 = (ushort *)((int)param_1 + 1);
  param_1 = puVar5;
  if (pbVar6 < (byte *)0x4) goto LAB_00c082de;
  if (pbVar4 < pbVar6) {
LAB_00c084ff:
    *param_4 = (int)pbVar11 - (int)param_3;
    return -5;
  }
  if ((uint)((int)puVar1 - (int)puVar5) < bVar2 - 0x10) {
LAB_00c084b7:
    *param_4 = (int)pbVar11 - (int)param_3;
    return -4;
  }
  do {
    *pbVar11 = (byte)*puVar5;
    pbVar11 = pbVar11 + 1;
    puVar5 = (ushort *)((int)puVar5 + 1);
    pbVar6 = pbVar6 + -1;
  } while (pbVar6 != (byte *)0x0);
LAB_00c0827a:
  uVar8 = (uint)(byte)*puVar5;
  param_1 = (ushort *)((int)puVar5 + 1);
  if (0xf < uVar8) goto LAB_00c0830c;
  iVar10 = (uint)*(byte *)param_1 * -4 - (uint)(byte)((byte)*puVar5 >> 2);
  param_1 = puVar5 + 1;
  if (pbVar11 + iVar10 + -0x801 < param_3) {
LAB_00c084e9:
    *param_4 = (int)pbVar11 - (int)param_3;
    return -6;
  }
  if ((byte *)0x2 < param_3 + ((int)pbVar4 - (int)pbVar11)) {
    *pbVar11 = pbVar11[iVar10 + -0x801];
    pbVar11[1] = pbVar11[iVar10 + -0x800];
    pbVar11[2] = pbVar11[iVar10 + -0x7ff];
    pbVar11 = pbVar11 + 3;
LAB_00c082cc:
    do {
      pbVar6 = (byte *)((byte)param_1[-1] & 3);
      if (((byte)param_1[-1] & 3) == 0) {
LAB_00c081d5:
        if (puVar1 <= param_1) {
          *param_4 = (int)pbVar11 - (int)param_3;
          return -7;
        }
        uVar3 = *param_1;
        uVar8 = (uint)(byte)uVar3;
        param_1 = (ushort *)((int)param_1 + 1);
        if (uVar8 < 0x10) goto code_r0x00c081ea;
      }
      else {
LAB_00c082de:
        if (param_3 + ((int)pbVar4 - (int)pbVar11) < pbVar6) break;
        if ((byte *)((int)puVar1 - (int)param_1) < pbVar6 + 1) goto LAB_00c084b7;
        do {
          puVar5 = param_1;
          *pbVar11 = (byte)*puVar5;
          pbVar11 = pbVar11 + 1;
          pbVar6 = pbVar6 + -1;
          param_1 = (ushort *)((int)puVar5 + 1);
        } while (pbVar6 != (byte *)0x0);
        uVar8 = (uint)*(byte *)((int)puVar5 + 1);
        param_1 = puVar5 + 1;
        if (puVar1 <= param_1) goto LAB_00c081d5;
      }
LAB_00c0830c:
      if (uVar8 < 0x40) {
        if (uVar8 < 0x20) {
          if (uVar8 < 0x10) {
            pbVar6 = pbVar11 + ((uint)(byte)*param_1 * -4 - (uVar8 >> 2));
            param_1 = (ushort *)((int)param_1 + 1);
            if (pbVar6 + -1 < param_3) goto LAB_00c084e9;
            if (param_3 + ((int)pbVar4 - (int)pbVar11) < (byte *)0x2) break;
            *pbVar11 = pbVar6[-1];
            pbVar11[1] = *pbVar6;
            pbVar11 = pbVar11 + 2;
            goto LAB_00c082cc;
          }
          uVar9 = uVar8 & 7;
          if (uVar9 == 0) {
            if (puVar1 == param_1) goto LAB_00c084b7;
            iVar10 = 0;
            if ((byte)*param_1 == 0) {
              iVar10 = 0;
              do {
                iVar10 = iVar10 + 0xff;
                param_1 = (ushort *)((int)param_1 + 1);
                if (puVar1 == param_1) goto LAB_00c084b7;
              } while (*(byte *)param_1 == 0);
            }
            uVar9 = iVar10 + 7 + (uint)(byte)*param_1;
            param_1 = (ushort *)((int)param_1 + 1);
          }
          uVar3 = *param_1;
          param_1 = param_1 + 1;
          if (pbVar11 + ((uVar8 & 8) * -0x800 - (uint)(uVar3 >> 2)) == pbVar11) {
            *param_4 = (int)pbVar11 - (int)param_3;
            if (param_1 != puVar1) {
              return (-(uint)(param_1 < puVar1) & 0xfffffffc) - 4;
            }
            return 0;
          }
          pbVar6 = pbVar11 + ((uVar8 & 8) * -0x800 - (uint)(uVar3 >> 2)) + -0x4000;
        }
        else {
          uVar9 = uVar8 & 0x1f;
          if (uVar9 == 0) {
            if (puVar1 == param_1) goto LAB_00c084b7;
            iVar10 = 0;
            if ((byte)*param_1 == 0) {
              iVar10 = 0;
              do {
                iVar10 = iVar10 + 0xff;
                param_1 = (ushort *)((int)param_1 + 1);
                if (puVar1 == param_1) goto LAB_00c084b7;
              } while (*(byte *)param_1 == 0);
            }
            uVar9 = iVar10 + 0x1f + (uint)(byte)*param_1;
            param_1 = (ushort *)((int)param_1 + 1);
          }
          pbVar6 = pbVar11 + (-1 - (uint)(*param_1 >> 2));
          param_1 = param_1 + 1;
        }
        if (pbVar6 < param_3) goto LAB_00c084e9;
        if (param_3 + ((int)pbVar4 - (int)pbVar11) < (byte *)(uVar9 + 2)) break;
        if ((5 < uVar9) && (3 < (int)pbVar11 - (int)pbVar6)) {
          *(undefined4 *)pbVar11 = *(undefined4 *)pbVar6;
          pbVar11 = pbVar11 + 4;
          pbVar6 = pbVar6 + 4;
          uVar9 = uVar9 - 2;
          do {
            *(undefined4 *)pbVar11 = *(undefined4 *)pbVar6;
            uVar9 = uVar9 - 4;
            pbVar11 = pbVar11 + 4;
            pbVar6 = pbVar6 + 4;
          } while (3 < uVar9);
          for (; uVar9 != 0; uVar9 = uVar9 - 1) {
            *pbVar11 = *pbVar6;
            pbVar11 = pbVar11 + 1;
            pbVar6 = pbVar6 + 1;
          }
          goto LAB_00c082cc;
        }
      }
      else {
        pbVar6 = pbVar11 + ((uint)(byte)*param_1 * -8 - (uVar8 >> 2 & 7)) + -1;
        param_1 = (ushort *)((int)param_1 + 1);
        uVar9 = (uVar8 >> 5) - 1;
        if (pbVar6 < param_3) goto LAB_00c084e9;
        if (param_3 + ((int)pbVar4 - (int)pbVar11) < (byte *)((uVar8 >> 5) + 1)) break;
      }
      *pbVar11 = *pbVar6;
      pbVar11[1] = pbVar6[1];
      pbVar11 = pbVar11 + 2;
      pbVar6 = pbVar6 + 2;
      do {
        *pbVar11 = *pbVar6;
        pbVar11 = pbVar11 + 1;
        pbVar6 = pbVar6 + 1;
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    } while( true );
  }
  goto LAB_00c084ff;
code_r0x00c081ea:
  if (uVar8 == 0) {
    if (puVar1 == param_1) goto LAB_00c084b7;
    iVar10 = 0;
    iVar7 = 0;
    if (*(byte *)param_1 == (byte)uVar3) {
      do {
        iVar10 = iVar10 + 0xff;
        param_1 = (ushort *)((int)param_1 + 1);
        if (puVar1 == param_1) goto LAB_00c084b7;
        iVar7 = iVar10;
      } while (*(byte *)param_1 == 0);
    }
    uVar8 = iVar7 + 0xf + (uint)(byte)*param_1;
    param_1 = (ushort *)((int)param_1 + 1);
  }
  if (param_3 + ((int)pbVar4 - (int)pbVar11) < (byte *)(uVar8 + 3)) goto LAB_00c084ff;
  if ((uint)((int)puVar1 - (int)param_1) < uVar8 + 4) goto LAB_00c084b7;
  *(undefined4 *)pbVar11 = *(undefined4 *)param_1;
  pbVar11 = pbVar11 + 4;
  puVar5 = param_1 + 2;
  uVar8 = uVar8 - 1;
  if (uVar8 != 0) {
    if (uVar8 < 4) {
      do {
        *pbVar11 = (byte)*puVar5;
        pbVar11 = pbVar11 + 1;
        puVar5 = (ushort *)((int)puVar5 + 1);
        uVar8 = uVar8 - 1;
      } while (uVar8 != 0);
    }
    else {
      do {
        *(undefined4 *)pbVar11 = *(undefined4 *)puVar5;
        uVar8 = uVar8 - 4;
        pbVar11 = pbVar11 + 4;
        puVar5 = puVar5 + 2;
      } while (3 < uVar8);
      for (; uVar8 != 0; uVar8 = uVar8 - 1) {
        *pbVar11 = (byte)*puVar5;
        pbVar11 = pbVar11 + 1;
        puVar5 = (ushort *)((int)puVar5 + 1);
      }
    }
  }
  goto LAB_00c0827a;
}



//=== _lzo_assert @ 00c08530 ===

/* [bsim sim=1.0 <- ego_r] */

bool _lzo_assert(int param_1)

{
  return param_1 != 0;
}



//=== Decompress @ 00f39ed0 ===

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=0.9292312011330016 <- ego_r]
   public: void __thiscall CRangeCompressor::Decompress(unsigned char const *,long,long,void *) */

void __thiscall
CRangeCompressor::Decompress
          (CRangeCompressor *this,uchar *param_1,long param_2,long param_3,void *param_4)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  int iVar9;
  uchar *puVar10;
  int iVar11;
  byte *pbVar12;
  uint *puVar13;
  double dVar14;
  int local_b0;
  uint local_ac;
  uint local_a8;
  int local_a4;
  uint local_9c;
  long local_8c;
  ulong local_80 [16];
  ulong local_40 [16];
  
  dVar14 = GFGetTime();
  _DAT_0143e970 = _DAT_0143e970 - dVar14;
  uVar5 = param_2 * param_3;
  DAT_0143e978 = DAT_0143e978 + uVar5;
  if (*param_1 == '\0') {
    puVar10 = param_1 + 1;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)param_4 = *(undefined4 *)puVar10;
      puVar10 = puVar10 + 4;
      param_4 = (uchar *)((int)param_4 + 4);
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(uchar *)param_4 = *puVar10;
      puVar10 = puVar10 + 1;
      param_4 = (uchar *)((int)param_4 + 1);
    }
    dVar14 = GFGetTime();
    _DAT_0143e970 = dVar14 + _DAT_0143e970;
    return;
  }
  bVar1 = param_1[1];
  pbVar12 = param_1 + 2;
  while( true ) {
    if ((char)bVar1 < '\0') {
      dVar14 = GFGetTime();
      _DAT_0143e970 = dVar14 + _DAT_0143e970;
      return;
    }
    if ((bVar1 & 0x40) == 0) {
      local_b0 = ((bVar1 & 0x20) != 0) + 1;
    }
    else {
      local_b0 = 4;
    }
    if (0x7fffffff < (uint)(local_b0 * 8)) break;
    bVar2 = *pbVar12;
    uVar5 = (uint)bVar2;
    puVar13 = (uint *)(pbVar12 + 1);
    local_ac = 0;
    bVar3 = 0;
    local_a8 = 0;
    local_a4 = 0;
    if ((bVar1 & 0x11) != 0) {
      if (local_b0 == 4) {
        local_ac = *puVar13;
      }
      else if (local_b0 == 2) {
        local_ac = (uint)(ushort)*puVar13;
      }
      else {
        local_ac = (uint)(byte)*puVar13;
      }
      puVar13 = (uint *)((int)puVar13 + local_b0);
    }
    if ((bVar1 & 2) != 0) {
      bVar3 = (byte)*puVar13;
      puVar13 = (uint *)((int)puVar13 + 1);
    }
    if ((bVar1 & 8) != 0) {
      if (local_b0 == 4) {
        local_a8 = *puVar13;
      }
      else if (local_b0 == 2) {
        local_a8 = (uint)(ushort)*puVar13;
      }
      else {
        local_a8 = (uint)(byte)*puVar13;
      }
      puVar13 = (uint *)((int)puVar13 + local_b0);
    }
    if ((bVar1 & 4) != 0) {
      if (local_b0 == 4) {
        uVar6 = *puVar13;
      }
      else if (local_b0 == 2) {
        uVar6 = (uint)(ushort)*puVar13;
      }
      else {
        uVar6 = (uint)(byte)*puVar13;
      }
      local_a4 = CalcShuffleOperations(this,uVar6,local_40,local_80);
      puVar13 = (uint *)((int)puVar13 + local_b0);
    }
    iVar9 = 0x20;
    uVar6 = (1 << (bVar2 & 0x1f)) - 1;
    if (uVar5 == 0) {
      iVar9 = 0;
    }
    local_9c = 0;
    if (0 < param_2) {
      local_8c = param_2;
      puVar8 = param_4;
      do {
        if (uVar5 == 0x20) {
          uVar7 = *puVar13;
          puVar13 = puVar13 + 1;
        }
        else {
          if (iVar9 == 0x20) {
            local_9c = *puVar13;
            puVar13 = puVar13 + 1;
            iVar9 = 0;
          }
          uVar4 = local_9c >> ((byte)iVar9 & 0x1f);
          iVar9 = iVar9 + uVar5;
          uVar7 = uVar4 & uVar6;
          if (0x20 < iVar9) {
            local_9c = *puVar13;
            puVar13 = puVar13 + 1;
            uVar7 = uVar6 & (uVar4 | local_9c << ((bVar2 - (char)iVar9) + 0x20 & 0x1f));
            iVar9 = iVar9 + -0x20;
          }
        }
        if ((bVar1 & 1) != 0) {
          uVar7 = uVar7 + local_ac & *(uint *)(&DAT_013ac944 + local_b0 * 4);
        }
        uVar7 = uVar7 << (bVar3 & 0x1f);
        uVar4 = uVar7;
        if ((bVar1 & 4) != 0) {
          uVar4 = 0;
          iVar11 = 0;
          if (0 < local_a4) {
            do {
              uVar4 = uVar4 | uVar7 << ((byte)local_80[iVar11] & 0x1f) & local_40[iVar11];
              iVar11 = iVar11 + 1;
            } while (iVar11 < local_a4);
          }
        }
        uVar4 = uVar4 | local_a8;
        if ((bVar1 & 0x10) != 0) {
          uVar4 = uVar4 + local_ac & *(uint *)(&DAT_013ac944 + local_b0 * 4);
        }
        if (local_b0 == 4) {
          *puVar8 = uVar4;
        }
        else if (local_b0 == 2) {
          *(short *)puVar8 = (short)uVar4;
        }
        else {
          *(char *)puVar8 = (char)uVar4;
        }
        puVar8 = (uint *)((int)puVar8 + param_3);
        local_8c = local_8c + -1;
      } while (local_8c != 0);
    }
    bVar1 = (byte)*puVar13;
    param_4 = (void *)((int)param_4 + local_b0);
    pbVar12 = (byte *)((int)puVar13 + 1);
  }
  return;
}



