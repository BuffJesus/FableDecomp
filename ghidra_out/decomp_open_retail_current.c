//=== OpenRetailBank @ 009a8840 (seed 009a8840) ===

/* [bsim sim=0.7545462912605256 <- ego_r]
   public: void __thiscall CBankFileManager::OpenRetailBank(class CWideString const &,bool) */

void __thiscall
CBankFileManager::OpenRetailBank(CBankFileManager *this,CWideString *param_1,bool param_2)

{
  int iVar1;
  bool bVar2;
  CBase *this_00;
  CEntitySoundDef *this_01;
  CFileInfo *pCVar3;
  CCharString *pCVar4;
  CAFile *pCVar5;
  C2DVector *pCVar6;
  CMultiStatBar *this_02;
  CIPhysicsFacesSegmentBase *pCVar7;
  uint uVar8;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  uint unaff_EBP;
  uint unaff_ESI;
  CAFile *pCVar9;
  uint uVar10;
  uint unaff_EDI;
  map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
  *this_03;
  CCountedPointer<CSpawnedFuncBase> *pCVar11;
  uint uStack_78;
  uint uStack_74;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppStack_70;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_6c [4];
  int iStack_68;
  int *piStack_64;
  allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> aaStack_60 [4];
  int *piStack_5c;
  CAFile *pCStack_58;
  CAFile *local_54 [2];
  CMemoryDataInputStream aCStack_4c [4];
  uint uStack_48;
  uint *puStack_40;
  int iStack_38;
  CFileDataInputStream aCStack_30 [4];
  uint uStack_2c;
  CAFile *pCStack_24;
  int iStack_1c;
  CWideString *pCStack_8;
  
  bVar2 = CAFile::PathExists(param_1);
  if (bVar2) {
    this_00 = ::operator_new(0x1c);
    if (this_00 == (CBase *)0x0) {
      this_00 = (CBase *)0x0;
    }
    else {
      NHeroInformationScreens::CBase::CBase(this_00);
      *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0122d06c;
      CCharString::CCharString((CCharString *)(this_00 + 0x18));
      this_00[0x14] = (CBase)0x0;
    }
    ::CCountedPointer<CFileInstaller::CFileInfo_const_>::
    CCountedPointer<CFileInstaller::CFileInfo_const_>
              ((CCountedPointer<CFileInstaller::CFileInfo_const_> *)local_54,(CFileInfo *)this_00);
    pCVar5 = local_54[0];
    pCVar11 = (CCountedPointer<CSpawnedFuncBase> *)0x1;
    (**(code **)(*(int *)local_54[0] + 4))(param_1,1,2);
    CFileDataInputStream::CFileDataInputStream(aCStack_30,pCVar5,0x4000);
    if (uStack_2c + 4 < 0x80000000) {
      if (iStack_1c < 4) {
        CDataInputStream::ReadWithSrcChunkOverflow
                  ((CDataInputStream *)aCStack_30,&stack0xffffff7c,4);
      }
      else {
        unaff_ESI = *(uint *)pCStack_24;
        pCStack_24 = pCStack_24 + 4;
        iStack_1c = iStack_1c + -4;
        uStack_2c = uStack_2c + 4;
      }
    }
    if (unaff_ESI == 0x42474942) {
      if (uStack_2c + 4 < 0x80000000) {
        if (iStack_1c < 4) {
          CDataInputStream::ReadWithSrcChunkOverflow
                    ((CDataInputStream *)aCStack_30,&stack0xffffff80,4);
        }
        else {
          unaff_EDI = *(uint *)pCStack_24;
          pCStack_24 = pCStack_24 + 4;
          iStack_1c = iStack_1c + -4;
          uStack_2c = uStack_2c + 4;
        }
      }
      if (unaff_EDI != *(uint *)(this + 0x34)) {
        CFileDataInputStream::~CFileDataInputStream(aCStack_30);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  (aaStack_60,extraout_EDX);
        return;
      }
      this_01 = ::operator_new(0x28);
      if (this_01 == (CEntitySoundDef *)0x0) {
        pCVar3 = (CFileInfo *)0x0;
      }
      else {
        pCVar3 = (CFileInfo *)CEntitySoundDef::CEntitySoundDef(this_01);
      }
      ::CCountedPointer<CFileInstaller::CFileInfo_const_>::
      CCountedPointer<CFileInstaller::CFileInfo_const_>
                ((CCountedPointer<CFileInstaller::CFileInfo_const_> *)&iStack_68,pCVar3);
      std::
      _List_val<CCountedPointer<CSpawnedFuncBase>,std::allocator<CCountedPointer<CSpawnedFuncBase>_>_>
      ::_Buynode<CCountedPointer<CSpawnedFuncBase>_>
                ((_List_val<CCountedPointer<CSpawnedFuncBase>,std::allocator<CCountedPointer<CSpawnedFuncBase>_>_>
                  *)(this + 0x10),(_Node *)&iStack_68,(_Node *)param_1,pCVar11);
      pCVar4 = (CCharString *)CWideString__ConvertToCharString(aaStack_6c);
      iVar1 = iStack_68;
      CCharString::operator=((CCharString *)(iStack_68 + 0x24),pCVar4);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_6c,extraout_EDX_00,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
      CCountedPointer<CDiskFileWin32>::operator=
                ((CCountedPointer<CDiskFileWin32> *)(iVar1 + 4),
                 (CCountedPointer<class_CDiskFileWin32> *)aaStack_60);
      if (uStack_2c + 4 < 0x80000000) {
        if (iStack_1c < 4) {
          CDataInputStream::ReadWithSrcChunkOverflow
                    ((CDataInputStream *)aCStack_30,&stack0xffffff84,4);
        }
        else {
          unaff_EBP = *(uint *)pCStack_24;
          pCStack_24 = pCStack_24 + 4;
          iStack_1c = iStack_1c + -4;
          uStack_2c = uStack_2c + 4;
        }
      }
      uVar10 = unaff_EDI;
      if (uStack_2c + 4 < 0x80000000) {
        if (iStack_1c < 4) {
          CDataInputStream::ReadWithSrcChunkOverflow
                    ((CDataInputStream *)aCStack_30,&stack0xffffff80,4);
        }
        else {
          uVar10 = *(uint *)pCStack_24;
          pCStack_24 = pCStack_24 + 4;
          iStack_1c = iStack_1c + -4;
          uStack_2c = uStack_2c + 4;
        }
      }
      if (uVar10 != 0) {
        Buffer_AllocateAndZero(uVar10);
        CDataInputStream::_SetPosition_CDataInputStream__UAEXK_Z
                  ((CDataInputStream *)aCStack_30,unaff_EBP);
        pCVar5 = pCStack_58;
        if (local_54[0] == pCStack_58) {
          pCVar5 = (CAFile *)&pCStack_58;
        }
        if ((0 < (int)uVar10) && (uStack_2c + uVar10 < 0x80000000)) {
          if (iStack_1c < (int)uVar10) {
            CDataInputStream::ReadWithSrcChunkOverflow
                      ((CDataInputStream *)aCStack_30,(uchar *)pCVar5,uVar10);
          }
          else {
            pCVar9 = pCStack_24;
            for (uVar8 = uVar10 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
              *(CAFile **)pCVar5 = *(CAFile **)pCVar9;
              pCVar9 = pCVar9 + 4;
              pCVar5 = pCVar5 + 4;
            }
            for (uVar8 = uVar10 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
              *pCVar5 = *pCVar9;
              pCVar9 = pCVar9 + 1;
              pCVar5 = pCVar5 + 1;
            }
            pCStack_24 = pCStack_24 + uVar10;
            iStack_1c = iStack_1c - uVar10;
            uStack_2c = uStack_2c + uVar10;
          }
        }
        pCVar5 = pCStack_58;
        if (local_54[0] == pCStack_58) {
          pCVar5 = (CAFile *)&pCStack_58;
        }
        CMemoryDataInputStream::CMemoryDataInputStream(aCStack_4c,pCVar5,uVar10);
        uVar10 = unaff_EBP;
        if (uStack_48 + 4 < 0x80000000) {
          if (iStack_38 < 4) {
            CDataInputStream::ReadWithSrcChunkOverflow
                      ((CDataInputStream *)aCStack_4c,&stack0xffffff84,4);
          }
          else {
            uVar10 = *puStack_40;
            puStack_40 = puStack_40 + 1;
            iStack_38 = iStack_38 + -4;
            uStack_48 = uStack_48 + 4;
          }
        }
        if (uVar10 != 0) {
          this_03 = (map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
                     *)(iStack_68 + 0x18);
          do {
            CMemoryDataInputStream::
            _ReadNullTerminatedString_CMemoryDataInputStream__UAE_AVCCharString__XZ(aCStack_4c);
            if (uStack_48 + 4 < 0x80000000) {
              if (iStack_38 < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)aCStack_4c,&stack0xffffff84,4);
              }
              else {
                unaff_EBP = *puStack_40;
                puStack_40 = puStack_40 + 1;
                iStack_38 = iStack_38 + -4;
                uStack_48 = uStack_48 + 4;
              }
            }
            if (uStack_48 + 4 < 0x80000000) {
              if (iStack_38 < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)aCStack_4c,&stack0xffffff80,4);
              }
              else {
                unaff_EDI = *puStack_40;
                puStack_40 = puStack_40 + 1;
                iStack_38 = iStack_38 + -4;
                uStack_48 = uStack_48 + 4;
              }
            }
            if (uStack_48 + 4 < 0x80000000) {
              if (iStack_38 < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)aCStack_4c,(uchar *)&uStack_78,4);
              }
              else {
                uStack_78 = *puStack_40;
                puStack_40 = puStack_40 + 1;
                iStack_38 = iStack_38 + -4;
                uStack_48 = uStack_48 + 4;
              }
            }
            if (uStack_48 + 4 < 0x80000000) {
              if (iStack_38 < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)aCStack_4c,(uchar *)&uStack_74,4);
              }
              else {
                uStack_74 = *puStack_40;
                puStack_40 = puStack_40 + 1;
                iStack_38 = iStack_38 + -4;
                uStack_48 = uStack_48 + 4;
              }
            }
            if (uStack_48 + 4 < 0x80000000) {
              if (iStack_38 < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)aCStack_4c,(uchar *)&ppStack_70,4);
              }
              else {
                ppStack_70 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)*puStack_40;
                puStack_40 = puStack_40 + 1;
                iStack_38 = iStack_38 + -4;
                uStack_48 = uStack_48 + 4;
              }
            }
            pCVar6 = std::
                     map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
                     ::operator[](this_03,(CCharString *)aaStack_6c);
            *(uint *)pCVar6 = unaff_EBP;
            *(uint *)(pCVar6 + 4) = unaff_EDI;
            *(uint *)(pCVar6 + 8) = uStack_78;
            *(uint *)(pCVar6 + 0xc) = uStack_74;
            *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(pCVar6 + 0x10) = ppStack_70;
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (aaStack_6c,ppStack_70,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
            uVar10 = uVar10 - 1;
          } while (uVar10 != 0);
        }
        CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)aCStack_4c);
        if (pCStack_58 != (CAFile *)0x0) {
          free(pCStack_58);
        }
      }
      this_02 = ::operator_new(0x1c);
      if (this_02 == (CMultiStatBar *)0x0) {
        pCVar7 = (CIPhysicsFacesSegmentBase *)0x0;
      }
      else {
        pCVar7 = (CIPhysicsFacesSegmentBase *)
                 NPlayerGui::CMultiStatBar::CMultiStatBar(this_02,(CPlayerGuiDef *)param_1);
      }
      CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
                ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(iStack_68 + 0xc),pCVar7);
      *(undefined1 *)(iStack_68 + 0x14) = 0;
      CThreadedFile::Open(*(CThreadedFile **)(iStack_68 + 0xc),pCStack_8,false);
      if ((piStack_64 != (int *)0x0) && (*piStack_64 = *piStack_64 + -1, *piStack_64 == 0)) {
        (*(code *)piStack_64[1])();
        operator_delete(piStack_64);
      }
    }
    CFileDataInputStream::~CFileDataInputStream(aCStack_30);
    if ((piStack_5c != (int *)0x0) && (*piStack_5c = *piStack_5c + -1, *piStack_5c == 0)) {
      (*(code *)piStack_5c[1])();
      operator_delete(piStack_5c);
    }
  }
  return;
}


// CALLEES of OpenRetailBank:
//   00993b10  ~CChunkedFileChunk
//   0099efb0  operator=
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099aed0  CCharString
//   00bfe9bc  operator_delete
//   00994930  CMemoryDataInputStream
//   00995d30  ?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ
//   0099a2f0  CBase
//   00993ca0  ReadWithSrcChunkOverflow
//   009a9d10  CCountedPointer<CFileInstaller::CFileInfo_const_>
//   0098dfd0  CMultiStatBar
//   00999230  PathExists
//   0099b2c0  CWideString::ConvertToCharString
//   009a9bf0  operator=
//   00994780  ~CFileDataInputStream
//   00bfea14  free
//   0098e1e0  Open
//   00411910  Buffer_AllocateAndZero
//   009ac530  operator[]
//   009a9bb0  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   00994700  CFileDataInputStream
//   009ac4d0  CEntitySoundDef
//   00bfea1a  operator_new
//   009a9c80  Reset
//   00993bc0  ?SetPosition@CDataInputStream@@UAEXK@Z
//   009aaaa0  _Buynode<CCountedPointer<CSpawnedFuncBase>_>
//   009a9b60  CCountedPointer<CFileInstaller::CFileInfo_const_>

//=== OpenIniFile @ 009a8170 (seed 009a8170) ===

/* [bsim sim=0.9094861760252965 <- ego_r]
   public: void __thiscall CBankFileManager::OpenIniFile(class CWideString const &) */

void __thiscall CBankFileManager::OpenIniFile(CBankFileManager *this,CWideString *param_1)

{
  LTextTreeWalkThrough *this_00;
  bool bVar1;
  undefined4 uVar2;
  CWideString *pCVar3;
  CBankInfo *this_01;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar7;
  pair<long,CMusicManager::CRegisteredMusicEntry> *extraout_EDX;
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
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  char local_ea;
  uchar local_e9;
  int local_e8;
  undefined4 *local_e4;
  CBankFileManager *local_e0;
  CCharString local_dc [4];
  CCharString local_d8 [4];
  CCharString local_d4 [4];
  CCharString local_d0 [4];
  allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_> local_cc [4];
  CCharString local_c8 [4];
  CCharString aCStack_c4 [4];
  CCharString local_c0 [4];
  CCharString local_bc [4];
  CCharString local_b8 [4];
  CCharString local_b4 [4];
  CWideString local_b0 [4];
  undefined4 *local_ac;
  CCharString local_a8 [4];
  undefined4 *local_a4;
  CWideString local_a0 [4];
  undefined4 *local_9c;
  undefined4 *local_98;
  CWideString local_94 [4];
  CWideString local_90 [4];
  CWideString local_8c [4];
  CWideString local_88 [4];
  undefined4 local_84;
  CMemoryDataInputStream local_80 [4];
  uint local_7c;
  uchar *local_74;
  int local_6c;
  CCharString local_64 [4];
  CCharString local_60 [4];
  CCharString local_5c [4];
  CCharString local_58 [4];
  CCharString local_54 [4];
  CCharString local_50 [4];
  CCharString local_4c [4];
  undefined **local_48 [4];
  void *local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined1 local_2c;
  CCharString local_28 [4];
  CFileDataInputStream local_24 [4];
  int local_20;
  int local_1c;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  
  this_00 = (LTextTreeWalkThrough *)(this + 4);
  local_e0 = this;
  if (*(int *)(this + 8) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_00,*(LTextBinNode **)(*(int *)this_00 + 4));
    *(int *)(*(int *)this_00 + 8) = *(int *)this_00;
    *(undefined4 *)(*(int *)this_00 + 4) = 0;
    *(int *)(*(int *)this_00 + 0xc) = *(int *)this_00;
    *(undefined4 *)(this + 8) = 0;
  }
  NHeroInformationScreens::CBase::CBase((CBase *)local_48);
  local_48[0] = &PTR__vector_deleting_destructor__0122e0fc;
  local_38 = (void *)0x0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  CCharString::CCharString(local_28);
  CMemoryFile::_Open_CMemoryFile__UAEXABVCWideString__W4FileAccessType__K_Z
            ((CMemoryFile *)local_48,param_1,1,2);
  CCharString::CCharString(local_dc);
  CCharString::CCharString(local_b4);
  CCharString::CCharString(local_bc);
  CFileDataInputStream::CFileDataInputStream(local_24,(CAFile *)local_48,0x4000);
  if (local_20 != local_1c) {
    do {
      CDataInputStream::ReadLine((CDataInputStream *)local_24);
      std::
      _Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>
                (local_cc,extraout_EDX);
      CMemoryDataInputStream::CMemoryDataInputStream(local_80,(CCharString *)local_cc);
      CDataInputStream::ReadUntilCharacter
                ((CDataInputStream *)local_80,(char)local_a8,(bool *)&DAT_0000003d);
      if (local_ea == '\0') {
        CDataInputStream::_SetPosition_CDataInputStream__UAEXK_Z((CDataInputStream *)local_80,0);
        CDataInputStream::ReadUntilWhiteSpace((CDataInputStream *)local_80);
        CCharString::CCharString((CCharString *)&local_98,"BeginRetailFile",-1);
        pcVar6 = "";
        if (local_98 != (undefined4 *)0x0) {
          pcVar6 = (char *)*local_98;
        }
        if (local_e4 == (undefined4 *)0x0) {
          pcVar4 = "";
        }
        else {
          pcVar4 = (char *)*local_e4;
        }
        iVar5 = _stricmp(pcVar4,pcVar6);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_98,
                   extraout_EDX_04,unaff_ESI);
        if (iVar5 == 0) {
          CCharString::CCharString(local_c8);
          ReadQuotedString(local_e0,local_c8,(CDataInputStream *)local_80);
          pCVar3 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          CWideString::operator=((CWideString *)local_dc,pCVar3);
          CCharString::~CCharString(local_58);
          paVar7 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c8;
          ppVar8 = extraout_EDX_05;
LAB_009a8731:
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar7,ppVar8,unaff_ESI);
          ppVar8 = extraout_EDX_13;
        }
        else {
          CCharString::CCharString((CCharString *)&local_a4,"EndRetailFile",-1);
          pcVar6 = "";
          if (local_a4 != (undefined4 *)0x0) {
            pcVar6 = (char *)*local_a4;
          }
          if (local_e4 == (undefined4 *)0x0) {
            pcVar4 = "";
          }
          else {
            pcVar4 = (char *)*local_e4;
          }
          iVar5 = _stricmp(pcVar4,pcVar6);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_a4,
                     extraout_EDX_06,unaff_ESI);
          if (iVar5 != 0) {
            CCharString::CCharString((CCharString *)&local_9c,"DevHeaderDirectory",-1);
            pcVar6 = "";
            if (local_9c != (undefined4 *)0x0) {
              pcVar6 = (char *)*local_9c;
            }
            if (local_e4 == (undefined4 *)0x0) {
              pcVar4 = "";
            }
            else {
              pcVar4 = (char *)*local_e4;
            }
            iVar5 = _stricmp(pcVar4,pcVar6);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_9c,
                       extraout_EDX_08,unaff_ESI);
            if (iVar5 == 0) {
              CCharString::CCharString(local_b8);
              ReadQuotedString(local_e0,local_b8,(CDataInputStream *)local_80);
              pCVar3 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
              CWideString::operator=((CWideString *)local_b4,pCVar3);
              CCharString::~CCharString(local_4c);
              paVar7 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_b8;
              ppVar8 = extraout_EDX_09;
            }
            else {
              CCharString::CCharString((CCharString *)&local_ac,"RetailHeaderDirectory",-1);
              pcVar6 = "";
              if (local_ac != (undefined4 *)0x0) {
                pcVar6 = (char *)*local_ac;
              }
              if (local_e4 == (undefined4 *)0x0) {
                pcVar4 = "";
              }
              else {
                pcVar4 = (char *)*local_e4;
              }
              iVar5 = _stricmp(pcVar4,pcVar6);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_ac,
                         extraout_EDX_10,unaff_ESI);
              ppVar8 = extraout_EDX_11;
              if (iVar5 != 0) goto LAB_009a8736;
              CCharString::CCharString(local_d4);
              ReadQuotedString(local_e0,local_d4,(CDataInputStream *)local_80);
              pCVar3 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
              CWideString::operator=((CWideString *)local_bc,pCVar3);
              CCharString::~CCharString(local_60);
              paVar7 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_d4;
              ppVar8 = extraout_EDX_12;
            }
            goto LAB_009a8731;
          }
          CCharString__AssignFromWide(&DAT_0122d70c);
          ppVar8 = extraout_EDX_07;
        }
LAB_009a8736:
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_e4,ppVar8
                   ,unaff_ESI);
        ppVar8 = extraout_EDX_14;
        this = local_e0;
      }
      else {
        if (local_7c + 1 < 0x80000000) {
          if (local_6c < 1) {
            CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)local_80,&local_e9,1);
          }
          else {
            local_e9 = *local_74;
            local_74 = local_74 + 1;
            local_6c = local_6c + -1;
            local_7c = local_7c + 1;
          }
        }
        CCharString::CCharString(local_c0);
        bVar1 = ReadQuotedString(this,local_c0,(CDataInputStream *)local_80);
        ppVar8 = extraout_EDX_00;
        if (bVar1) {
          CCharString::CCharString((CCharString *)&local_e8);
          CCharString::CCharString(local_d8);
          CCharString::CCharString(local_d0);
          bVar1 = ReadQuotedString(this,(CCharString *)&local_e8,(CDataInputStream *)local_80);
          if (((bVar1) && (local_e8 != 0)) && (0 < *(int *)(local_e8 + 4))) {
            uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
            pCVar3 = (CWideString *)CCharString__AssignFromData(uVar2);
            CWideString::operator=((CWideString *)local_d8,pCVar3);
            CCharString::~CCharString(local_54);
            CCharString::~CCharString(local_5c);
            uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
            pCVar3 = (CWideString *)CCharString__AssignFromData(uVar2);
            CWideString::operator=((CWideString *)local_d0,pCVar3);
            CCharString::~CCharString(local_64);
            CCharString::~CCharString(local_50);
          }
          CPersistContext::GetDefaultVal<CAnimationSet>();
          CWideString::CWideString(local_94,local_b0);
          CWideString::CWideString(local_90,(CWideString *)local_dc);
          CWideString::CWideString(local_8c,(CWideString *)local_d8);
          CWideString::CWideString(local_88,(CWideString *)local_d0);
          local_84 = 0;
          this_01 = std::
                    map<CCharString,CBankFileManager::CBankInfo,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CBankFileManager::CBankInfo>_>_>
                    ::operator[]((map<CCharString,CBankFileManager::CBankInfo,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CBankFileManager::CBankInfo>_>_>
                                  *)this_00,local_a8);
          CWideString::operator=((CWideString *)this_01,local_94);
          CWideString::operator=((CWideString *)(this_01 + 4),local_90);
          CWideString::operator=((CWideString *)(this_01 + 8),local_8c);
          CWideString::operator=((CWideString *)(this_01 + 0xc),local_88);
          *(undefined4 *)(this_01 + 0x10) = local_84;
          CCharString::~CCharString((CCharString *)local_88);
          CCharString::~CCharString((CCharString *)local_8c);
          CCharString::~CCharString((CCharString *)local_90);
          CCharString::~CCharString((CCharString *)local_94);
          CCharString::~CCharString((CCharString *)local_b0);
          CCharString::~CCharString(local_d0);
          CCharString::~CCharString(local_d8);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_e8,
                     extraout_EDX_01,unaff_ESI);
          ppVar8 = extraout_EDX_02;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c0,ppVar8,
                   unaff_ESI);
        ppVar8 = extraout_EDX_03;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_a8,ppVar8,
                 unaff_ESI);
      CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)local_80);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_cc,
                 extraout_EDX_15,unaff_ESI);
    } while (local_20 != local_1c);
  }
  CWideString::CWideString(local_a0,(CWideString *)local_28);
  CCharString::CCharString(aCStack_c4);
  CAFile::SplitPath((CCharString *)local_a0,(CCharString *)0x0,aCStack_c4,(CCharString *)0x0,
                    (CCharString *)0x0);
  CWideString::operator=((CWideString *)(this + 0x24),(CWideString *)aCStack_c4);
  CCharString::~CCharString(aCStack_c4);
  CCharString::~CCharString((CCharString *)local_a0);
  CFileDataInputStream::~CFileDataInputStream(local_24);
  CCharString::~CCharString(local_bc);
  CCharString::~CCharString(local_b4);
  CCharString::~CCharString(local_dc);
  local_48[0] = &PTR__vector_deleting_destructor__0122e0fc;
  CTCGatherPointBuilding::_Close_CMemoryFile__UAEXXZ((CTCGatherPointBuilding *)local_48);
  CCharString::~CCharString(local_28);
  if (local_38 != (void *)0x0) {
    free(local_38);
  }
  NHeroInformationScreens::CBase::CBase((CBase *)local_48);
  return;
}


// CALLEES of OpenIniFile:
//   00993b10  ~CChunkedFileChunk
//   0099a300  CBase
//   0099be70  CCharString::AssignFromData
//   0099b7d0  operator=
//   009e94a0  ?Open@CMemoryFile@@UAEXABVCWideString@@W4FileAccessType@@K@Z
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099aed0  CCharString
//   0099f560  _Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>
//   00994e90  ReadUntilWhiteSpace
//   0099a2f0  CBase
//   00993ca0  ReadWithSrcChunkOverflow
//   009aa7c0  BuildTreeArray
//   0099e4b0  CCharString
//   00997620  SplitPath
//   0099b510  ~CCharString
//   00994780  ~CFileDataInputStream
//   00bfea14  free
//   009ac5d0  operator[]
//   0099b800  CCharString::AssignFromWide
//   00994dc0  ReadUntilCharacter
//   00994700  CFileDataInputStream
//   00994b50  ReadLine
//   00bfeba8  _stricmp
//   00995f50  CMemoryDataInputStream
//   00993bc0  ?SetPosition@CDataInputStream@@UAEXK@Z
//   009e9390  ?Close@CMemoryFile@@UAEXXZ
//   0099b720  CWideString
//   009a77d0  ReadQuotedString
//   0099ebf0  CCharString
//   0099e480  GetDefaultVal<CAnimationSet>

