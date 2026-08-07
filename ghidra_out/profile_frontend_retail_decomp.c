//=== CreateNewProfile @ 004945b0 (seed 004945b0) ===

/* [bsim sim=0.5510982443647429 <- ego_r]
   private: class CWideString __thiscall CFrontendGameComponent::CreateNewProfile(void) */

CWideString * __thiscall CFrontendGameComponent::CreateNewProfile(CFrontendGameComponent *this)

{
  int iVar1;
  int iVar2;
  CCodeSectionManager *pCVar3;
  ulong unaff_ESI;
  CWideString *in_stack_00000004;
  CTVertexRHWColSpecTex1 *pCVar4;
  CTVertexRHWColSpecTex1 *pCVar5;
  CTexture *in_stack_fffffff4;
  CCharString local_8 [4];
  CWideString local_4 [4];
  
  iVar1 = *(int *)(this + 0x1e0c);
  iVar2 = *(int *)(this + 0x1e10);
  CCharString::CCharString(local_8);
  CCharString__SetFromFormatV(local_8,&PTR_LAB_0122de44,(iVar2 - iVar1 >> 2) + 1);
  CWideString::CWideString(local_4,(CWideString *)local_8);
  CCharString::~CCharString(local_8);
  CCharString__SetFromEnum(local_4);
  CCharString::~CCharString((CCharString *)local_4);
  CTBaseSingleton<CCodeSectionManager>::Get();
  CPersistContext::GetDefaultVal<CSpecialEffectsStringMap>();
  CCharString::~CCharString((CCharString *)local_4);
  pCVar5 = (CTVertexRHWColSpecTex1 *)0x0;
  pCVar4 = (CTVertexRHWColSpecTex1 *)&stack0xfffffff4;
  pCVar3 = CTBaseSingleton<CCodeSectionManager>::Get();
  CRenderManager2D::AddLineToDrawBuffer
            ((CRenderManager2D *)pCVar3,pCVar4,pCVar5,unaff_ESI,in_stack_fffffff4);
  pCVar3 = CTBaseSingleton<CCodeSectionManager>::Get();
  CUserProfileManager::SetDefaultValues((CUserProfileManager *)pCVar3);
  CWideString::CWideString(in_stack_00000004,(CWideString *)&stack0xfffffff4);
  CCharString::~CCharString((CCharString *)&stack0xfffffff4);
  return in_stack_00000004;
}


// CALLEES of CreateNewProfile:
//   0040c790  SetDefaultValues
//   0099aed0  CCharString
//   0099b720  CWideString
//   0099bff0  CCharString::SetFromEnum
//   004082d0  GetDefaultVal<CSpecialEffectsStringMap>
//   0040d2a0  Get
//   0099ba70  CCharString::SetFromFormatV
//   0040d400  AddLineToDrawBuffer
//   0099b510  ~CCharString

//=== GetProfileNames @ 004959e0 (seed 004959e0) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.5800668951061372 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::GetProfileNames(class std::vector<class
   CWideString,class std::allocator<class CWideString> > &) */

void __thiscall
NUISystem::CFrontEndManager::GetProfileNames
          (CFrontEndManager *this,vector<CWideString,std::allocator<CWideString>_> *param_1)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> pVar1;
  CWideString *this_00;
  CFrontEndManager *pCVar2;
  char cVar3;
  undefined4 *puVar4;
  int iVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  undefined1 *puVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  CCharString *pCVar9;
  char *pcVar10;
  CWideString *pCVar11;
  int iVar12;
  undefined3 uVar13;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar14;
  int *piVar15;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int *piVar16;
  bool bVar17;
  undefined *puVar18;
  wchar_t *pwVar19;
  CCharString local_528 [4];
  undefined4 *local_524;
  CWideString local_520 [4];
  CCharString local_51c [4];
  CCharString aCStack_518 [4];
  CCharString local_514 [7];
  undefined1 uStack_50d;
  CWideString local_50c [4];
  CFrontEndManager *local_508;
  undefined4 local_504;
  undefined4 *local_500;
  CCharString aCStack_4fc [4];
  undefined4 local_4f8;
  CCharString aCStack_4f4 [4];
  CFileDataInputStream local_4f0 [4];
  uint local_4ec;
  undefined4 *local_4e4;
  int local_4dc;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_4cc [4];
  CWideString local_4c8 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_4c4 [4];
  CCharString local_4c0 [4];
  undefined4 local_4bc;
  undefined **appuStack_4b8 [3];
  CCharString aCStack_4ac [4];
  CCharString aCStack_4a8 [4];
  int *piStack_4a4;
  undefined **local_490 [5];
  char local_47c;
  CCharString aCStack_478 [20];
  CPersistContext aCStack_464 [44];
  int local_438 [2];
  undefined1 local_430 [528];
  undefined1 local_220 [532];
  undefined4 local_c;
  
  local_c = DAT_0139c8a8;
  local_508 = this;
  Std_Vector_Erase_Range(*(undefined4 *)this,*(undefined4 *)(this + 4));
  puVar18 = &DAT_0122de6c;
  NResourceDirectoryNames::GetSaveDir();
  CCharString__AssignFromValue(puVar18);
  CCharString::~CCharString(local_51c);
  cVar3 = CAFile_FindFirstFile(local_220);
  do {
    if (cVar3 == '\0') {
      StdSort_Introsort(local_4f8);
      CCharString::~CCharString((CCharString *)local_520);
      return;
    }
    CCharString__AssignFromWide(local_220);
    CCharString::CCharString((CCharString *)&local_500,".",-1);
    puVar4 = (undefined4 *)CWideString__ConvertToCharString(local_4cc);
    puVar4 = (undefined4 *)*puVar4;
    if (puVar4 == (undefined4 *)0x0) {
      iVar5 = 0;
    }
    else {
      iVar5 = puVar4[1];
    }
    if (local_500 == (undefined4 *)0x0) {
      iVar12 = 0;
    }
    else {
      iVar12 = local_500[1];
    }
    ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(iVar5 - iVar12);
    if ((int)ppVar6 < 0) {
      bVar17 = false;
    }
    else {
      if (local_500 == (undefined4 *)0x0) {
        ppVar14 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)&DAT_0129aaf4;
      }
      else {
        ppVar14 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)*local_500;
      }
      puVar7 = &DAT_0129aaf4;
      if (puVar4 != (undefined4 *)0x0) {
        puVar7 = (undefined1 *)*puVar4;
      }
      ppVar8 = ppVar6 + (int)puVar7;
      do {
        pVar1 = *ppVar8;
        uVar13 = (undefined3)((uint)ppVar6 >> 8);
        ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)CONCAT31(uVar13,pVar1);
        bVar17 = (byte)pVar1 < (byte)*ppVar14;
        if (pVar1 != *ppVar14) {
LAB_00495aed:
          bVar17 = 1 - bVar17 == (uint)(bVar17 != 0);
          goto LAB_00495afb;
        }
        if (pVar1 == (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0) break;
        pVar1 = ppVar8[1];
        ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)CONCAT31(uVar13,pVar1);
        bVar17 = (byte)pVar1 < (byte)ppVar14[1];
        if (pVar1 != ppVar14[1]) goto LAB_00495aed;
        ppVar8 = ppVar8 + 2;
        ppVar14 = ppVar14 + 2;
      } while (pVar1 != (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0);
      bVar17 = true;
    }
LAB_00495afb:
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (local_4cc,ppVar6,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_500,
               extraout_EDX,unaff_EDI);
    if ((!bVar17) && (bVar17 = CAFile::IsPathFile(local_520), !bVar17)) {
      CCharString__SetFromUnicodeString(&DAT_0122de78);
      CCharString::CCharString(local_51c);
      CAFile::SplitPath(local_528,(CCharString *)0x0,local_51c,(CCharString *)0x0,(CCharString *)0x0
                       );
      pwVar19 = L"*.bin";
      CWideString::CWideString(local_4c8,(CWideString *)local_528);
      CCharString__AssignFromValue(pwVar19);
      CCharString::~CCharString((CCharString *)local_4c8);
      cVar3 = CAFile_FindFirstFile(local_430);
      while (cVar3 != '\0') {
        CCharString__AssignFromWide(local_430);
        CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_490,local_50c,1,2);
        CCharString::~CCharString((CCharString *)local_50c);
        if (local_47c != '\0') {
          CCharString::CCharString((CCharString *)&local_524);
          CDiskFileWin32::_Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z
                    ((CDiskFileWin32 *)local_490,local_438,8,(CASuspendableProcess *)0x0);
          iVar5 = 2;
          bVar17 = true;
          piVar15 = local_438;
          piVar16 = (int *)"FablePro";
          do {
            if (iVar5 == 0) break;
            iVar5 = iVar5 + -1;
            bVar17 = *piVar15 == *piVar16;
            piVar15 = piVar15 + 1;
            piVar16 = piVar16 + 1;
          } while (bVar17);
          if (bVar17) {
            CFileDataInputStream::CFileDataInputStream(local_4f0,(CAFile *)local_490,0x4000);
            if (local_4ec + 4 < 0x80000000) {
              if (local_4dc < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)local_4f0,(uchar *)&local_4bc,4);
              }
              else {
                local_4bc = *local_4e4;
                local_4e4 = local_4e4 + 1;
                local_4dc = local_4dc + -4;
                local_4ec = local_4ec + 4;
              }
            }
            if (local_4ec + 4 < 0x80000000) {
              if (local_4dc < 4) {
                CDataInputStream::ReadWithSrcChunkOverflow
                          ((CDataInputStream *)local_4f0,(uchar *)&local_504,4);
              }
              else {
                local_504 = *local_4e4;
                local_4e4 = local_4e4 + 1;
                local_4dc = local_4dc + -4;
                local_4ec = local_4ec + 4;
              }
            }
            pCVar9 = (CCharString *)
                     CDataInputStream::ReadString((CDataInputStream *)local_4f0,(ulong)local_4c4);
            CCharString::operator=((CCharString *)&local_524,pCVar9);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      (local_4c4,extraout_EDX_00,unaff_EDI);
            CFileDataInputStream::~CFileDataInputStream(local_4f0);
          }
          else {
            CDiskFileWin32::_SetPosition_CDiskFileWin32__UAEXJ_Z((CDiskFileWin32 *)local_490,0);
            CCharString::LoadFromFile
                      ((CCharString *)&local_524,(CAFile *)local_490,(CASuspendableProcess *)0x0,-1)
            ;
          }
          pCVar9 = (CCharString *)CCharString::CCharString(local_4c0,"",-1);
          NHeroInformationScreens::CBase::CBase((CBase *)appuStack_4b8);
          appuStack_4b8[0] = &PTR__vector_deleting_destructor__0122d874;
          CCharString::CCharString(aCStack_4ac);
          CCharString::CCharString(aCStack_4a8);
          piStack_4a4 = (int *)0x0;
          piStack_4a4 = malloc(0x10);
          *piStack_4a4 = (int)piStack_4a4;
          piStack_4a4[1] = (int)piStack_4a4;
          CCharString::operator=(aCStack_4a8,(CCharString *)&local_524);
          if (local_524 == (undefined4 *)0x0) {
            pcVar10 = "";
          }
          else {
            pcVar10 = (char *)*local_524;
          }
          CStringParser::Init((CStringParser *)appuStack_4b8,pcVar10,
                              (_func_void_CCharString_ptr *)0x0,pCVar9,0);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4c0,
                     extraout_EDX_01,unaff_EDI);
          CPersistContext::CPersistContext
                    (aCStack_464,(CStringParser *)appuStack_4b8,(_func_void_CCharString_ptr *)0x0,
                     true,(CDefinitionManager *)0x0,(CSymbolMap *)0x0,(CCharString *)0x0);
          CCharString::CCharString(aCStack_518);
          CCharString::CCharString(aCStack_4f4);
          CCharString::CCharString(aCStack_4fc,aCStack_4f4);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4f4,
                     extraout_EDX_02,unaff_EDI);
          CPersistContext::Transfer<CIntelligentPointer<CThing>_>
                    (aCStack_464,"ProfileName",(CIntelligentPointer<CThing> *)aCStack_518,
                     (CIntelligentPointer<class_CThing> *)aCStack_4fc);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4fc,
                     extraout_EDX_03,unaff_EDI);
          pCVar11 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar2 = local_508;
          this_00 = *(CWideString **)(local_508 + 4);
          if (this_00 == *(CWideString **)(local_508 + 8)) {
            Std_Vector_Insert_Range_Copies(this_00,pCVar11,&uStack_50d,1,1);
          }
          else {
            if (this_00 != (CWideString *)0x0) {
              CWideString::CWideString(this_00,pCVar11);
            }
            *(int *)(pCVar2 + 4) = *(int *)(pCVar2 + 4) + 4;
          }
          CCharString::~CCharString((CCharString *)&local_4f8);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_518,
                     extraout_EDX_04,unaff_EDI);
          piVar15 = (int *)*piStack_4a4;
          ppVar6 = extraout_EDX_05;
          if (piVar15 != piStack_4a4) {
            do {
              piVar16 = (int *)*piVar15;
              free(piVar15);
              ppVar6 = extraout_EDX_06;
              piVar15 = piVar16;
            } while (piVar16 != piStack_4a4);
          }
          *piStack_4a4 = (int)piStack_4a4;
          piStack_4a4[1] = (int)piStack_4a4;
          if (piStack_4a4 != (int *)0x0) {
            free(piStack_4a4);
            ppVar6 = extraout_EDX_07;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4a8,
                     ppVar6,unaff_EDI);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4ac,
                     extraout_EDX_08,unaff_EDI);
          NHeroInformationScreens::CBase::CBase((CBase *)appuStack_4b8);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_524,
                     extraout_EDX_09,unaff_EDI);
        }
        cVar3 = CAFile__FindNextFileSequential(local_430);
        local_490[0] = &PTR__vector_deleting_destructor__0122d06c;
        CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_490);
        CCharString::~CCharString(aCStack_478);
        NHeroInformationScreens::CBase::CBase((CBase *)local_490);
      }
      CCharString::~CCharString(local_514);
      CCharString::~CCharString(local_51c);
    }
    cVar3 = CAFile__FindNextFileSequential(local_220);
    CCharString::~CCharString(local_528);
  } while( true );
}


// CALLEES of GetProfileNames:
//   0099a300  CBase
//   0099efb0  operator=
//   0099b720  CWideString
//   0099a960  ?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z
//   0099bf30  CCharString::AssignFromValue
//   00994700  CFileDataInputStream
//   004109a0  Transfer<CIntelligentPointer<CThing>_>
//   00999760  CAFile_FindFirstFile
//   009badd0  CPersistContext
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099f2e0  LoadFromFile
//   00bfea0e  malloc
//   00996be0  ReadString
//   0099abf0  ?SetPosition@CDiskFileWin32@@UAEXJ@Z
//   00bfe9f9  __security_check_cookie
//   0099ad80  CDiskFileWin32
//   0099b510  ~CCharString
//   0099b6b0  CCharString::AssignFromWide
//   00999850  CAFile::FindNextFileSequential
//   0099a2d0  CBase
//   004977d0  StdSort_Introsort
//   0099b2c0  CWideString::ConvertToCharString
//   00997620  SplitPath
//   00bfea14  free
//   00994780  ~CFileDataInputStream
//   0099a2e0  CBase
//   00993ca0  ReadWithSrcChunkOverflow
//   009b9530  Init
//   0099ec30  CCharString
//   0099b940  CCharString::SetFromUnicodeString
//   0099a920  ?Close@CDiskFileWin32@@UAEXXZ
//   0099e4b0  CCharString
//   00412330  Std_Vector_Insert_Range_Copies
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099ebf0  CCharString
//   0099aed0  CCharString
//   00412450  Std_Vector_Erase_Range
//   00999a30  IsPathFile
//   0041a540  GetSaveDir

//=== GetProfileNames @ 005955ab (seed 005955ab) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.979114156275753 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::GetProfileNames(class std::vector<class
   CWideString,class std::allocator<class CWideString> > &) */

void __thiscall
NUISystem::CFrontEndManager::GetProfileNames
          (CFrontEndManager *this,vector<CWideString,std::allocator<CWideString>_> *param_1)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> pVar1;
  char cVar2;
  undefined4 *puVar3;
  int iVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar5;
  undefined1 *puVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar7;
  int iVar8;
  undefined3 uVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar10;
  bool bVar11;
  wchar_t *pwVar12;
  undefined *puVar13;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_444 [4];
  CWideString local_440 [4];
  CCharString local_43c [4];
  undefined4 *local_438;
  CCharString local_434 [4];
  CCharString local_430 [4];
  CWideString local_42c [4];
  CCharString local_428 [4];
  CCharString local_424 [4];
  undefined1 local_420 [524];
  undefined1 local_214 [524];
  undefined4 local_8;
  
  local_8 = DAT_0139c8a8;
  Std_Vector_Erase_Range(*(undefined4 *)param_1,*(undefined4 *)(param_1 + 4));
  puVar13 = &DAT_0122de6c;
  NResourceDirectoryNames::GetSaveDir();
  CCharString__AssignFromValue(puVar13);
  CCharString::~CCharString(local_428);
  cVar2 = CAFile_FindFirstFile(local_214);
  do {
    if (cVar2 == '\0') {
      CCharString::~CCharString((CCharString *)local_42c);
      return;
    }
    CCharString__AssignFromWide(local_214);
    CCharString::CCharString((CCharString *)&local_438,".",-1);
    puVar3 = (undefined4 *)CWideString__ConvertToCharString(local_444);
    puVar3 = (undefined4 *)*puVar3;
    if (puVar3 == (undefined4 *)0x0) {
      iVar4 = 0;
    }
    else {
      iVar4 = puVar3[1];
    }
    if (local_438 == (undefined4 *)0x0) {
      iVar8 = 0;
    }
    else {
      iVar8 = local_438[1];
    }
    ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(iVar4 - iVar8);
    if ((int)ppVar5 < 0) {
      bVar11 = false;
    }
    else {
      if (local_438 == (undefined4 *)0x0) {
        ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)&DAT_0129aaf4;
      }
      else {
        ppVar10 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)*local_438;
      }
      puVar6 = &DAT_0129aaf4;
      if (puVar3 != (undefined4 *)0x0) {
        puVar6 = (undefined1 *)*puVar3;
      }
      ppVar7 = ppVar5 + (int)puVar6;
      do {
        pVar1 = *ppVar7;
        uVar9 = (undefined3)((uint)ppVar5 >> 8);
        ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)CONCAT31(uVar9,pVar1);
        bVar11 = (byte)pVar1 < (byte)*ppVar10;
        if (pVar1 != *ppVar10) {
LAB_00595696:
          iVar4 = (1 - (uint)bVar11) - (uint)(bVar11 != 0);
          goto LAB_0059569b;
        }
        if (pVar1 == (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0) break;
        pVar1 = ppVar7[1];
        ppVar5 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)CONCAT31(uVar9,pVar1);
        bVar11 = (byte)pVar1 < (byte)ppVar10[1];
        if (pVar1 != ppVar10[1]) goto LAB_00595696;
        ppVar7 = ppVar7 + 2;
        ppVar10 = ppVar10 + 2;
      } while (pVar1 != (pair<EHeroMorphType,CParticleMorphs::CEntry>)0x0);
      iVar4 = 0;
LAB_0059569b:
      bVar11 = iVar4 == 0;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (local_444,ppVar5,unaff_ESI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_438,
               extraout_EDX,unaff_ESI);
    if ((!bVar11) && (bVar11 = CAFile::IsPathFile(local_42c), !bVar11)) {
      CCharString__SetFromUnicodeString(&DAT_0122de78);
      CCharString::CCharString(local_428);
      CAFile::SplitPath(local_424,(CCharString *)0x0,local_428,(CCharString *)0x0,(CCharString *)0x0
                       );
      pwVar12 = L"*.bin";
      CWideString::CWideString(local_440,(CWideString *)local_424);
      CCharString__AssignFromValue(pwVar12);
      CCharString::~CCharString((CCharString *)local_440);
      cVar2 = CAFile_FindFirstFile(local_420);
      while (cVar2 != '\0') {
        CCharString::CCharString(local_434);
        CCharString__AssignFromWide(local_214);
        CAFile::SplitPath(local_43c,(CCharString *)0x0,(CCharString *)0x0,local_434,
                          (CCharString *)0x0);
        CCharString::~CCharString(local_43c);
        Vector_PushBack(local_434);
        cVar2 = CAFile__FindNextFileSequential(local_420);
        CCharString::~CCharString(local_434);
      }
      CCharString::~CCharString(local_430);
      CCharString::~CCharString(local_428);
    }
    cVar2 = CAFile__FindNextFileSequential(local_214);
    CCharString::~CCharString(local_424);
  } while( true );
}


// CALLEES of GetProfileNames:
//   0099b2c0  CWideString::ConvertToCharString
//   00997620  SplitPath
//   0099b720  CWideString
//   0099bf30  CCharString::AssignFromValue
//   004128a0  Vector_PushBack
//   00999760  CAFile_FindFirstFile
//   0099b940  CCharString::SetFromUnicodeString
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   0099aed0  CCharString
//   00412450  Std_Vector_Erase_Range
//   00999a30  IsPathFile
//   00bfe9f9  __security_check_cookie
//   0099b510  ~CCharString
//   0099b6b0  CCharString::AssignFromWide
//   0041a540  GetSaveDir
//   00999850  CAFile::FindNextFileSequential

//=== GotoNewProfileScreen @ 00595845 (seed 00595845) ===

/* [bsim sim=0.9987145426832219 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::GotoNewProfileScreen(void) */

void __thiscall NUISystem::CFrontEndManager::GotoNewProfileScreen(CFrontEndManager *this)

{
  CCodeSectionManager *pCVar1;
  CFrontEndManager *pCVar2;
  CFrontEndManager *pCVar3;
  int *piVar4;
  CThing *pCVar5;
  int *extraout_EAX;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  int *local_8;
  
  pCVar1 = CTBaseSingleton<CCodeSectionManager>::Get();
  pCVar1[0xc] = (CCodeSectionManager)0x1;
  local_14 = 0;
  local_10 = 0;
  local_c = 0;
  GetProfileNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&local_14);
  this[100] = (CFrontEndManager)0x1;
  this[0xa0] = (CFrontEndManager)0x1;
  pCVar2 = GetInstance();
  pCVar3 = GetInstance();
  piVar4 = (int *)(**(code **)(*(int *)pCVar2 + 0xa8))(&local_8);
  pCVar5 = (CThing *)(*piVar4 + 0x1a0);
  (**(code **)(*(int *)pCVar3 + 0xa0))(pCVar5,1);
  if (local_8 != (int *)0x0) {
    piVar4 = local_8 + 1;
    *piVar4 = *piVar4 + -1;
    if (*piVar4 == 0) {
      (**(code **)(*local_8 + 4))();
    }
  }
  CTCTrophy::OnShowTrophyInitialise((CTCTrophy *)(this + 0x20),pCVar5);
  piVar4 = (int *)*extraout_EAX;
  (**(code **)(*piVar4 + 0xc0))(6);
  pCVar2 = GetInstance();
  (**(code **)(*(int *)pCVar2 + 0x14))(piVar4 + 1);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_14);
  return;
}


// CALLEES of GotoNewProfileScreen:
//   005955ab  GetProfileNames
//   0041e5f2  GetInstance
//   0040d2a0  Get
//   00412130  ResetRings
//   0059b039  OnShowTrophyInitialise

//=== AddProfileNameToScreens @ 00595b24 (seed 00595b24) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::AddProfileNameToScreens(class CWideString
   const &) */

void __thiscall
NUISystem::CFrontEndManager::AddProfileNameToScreens(CFrontEndManager *this,CWideString *param_1)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CFrontEndManager *local_8;
  
  local_8 = this;
  CCharString__AssignFromValue(&DAT_0122e070);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_NEW_GAME",-1);
  AddPrefixToTitleBar(this,0,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,extraout_EDX,
             unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_LOAD_GAME",-1);
  AddPrefixToTitleBar(this,0,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_OPTIONS_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,0x18,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_01,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_OPTIONS_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,1,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_02,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_GAME_OPTIONS_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,1,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_03,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_VIDEO_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,5,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_04,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_SCOREBOARD_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,0x19,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_05,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_REDEFINE_KEYS_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,0x16,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_06,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_TEXT_AUDIO_OPTIONS_MENU_TITLE",-1);
  AddPrefixToTitleBar(this,4,(CCharString *)&local_8,(CWideString *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_07,unaff_EDI);
  CCharString::~CCharString((CCharString *)&param_1);
  return;
}


// CALLEES of AddProfileNameToScreens:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   00595ad9  AddPrefixToTitleBar
//   0099bf30  CCharString::AssignFromValue
//   0099b510  ~CCharString

//=== CreateNewProfile @ 0059697a (seed 0059697a) ===

/* [bsim sim=1.0 <- ego_r]
   protected: bool __thiscall NUISystem::CFrontEndManager::CreateNewProfile(class CWideString &) */

bool __thiscall
NUISystem::CFrontEndManager::CreateNewProfile(CFrontEndManager *this,CWideString *param_1)

{
  bool bVar1;
  CCodeSectionManager *pCVar2;
  CCharString *this_00;
  long lVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  bool bVar4;
  CCodeSectionManager CVar5;
  CWideString *pCVar6;
  CTVertexRHWColSpecTex1 *pCVar7;
  CFrontEndManager *pCVar8;
  
  pCVar8 = this;
  pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
  bVar1 = CUserProfileManager::IsEnoughSpaceForProfile((CUserProfileManager *)pCVar2);
  bVar4 = false;
  if (bVar1) {
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    pCVar2[0xc] = (CCodeSectionManager)0x0;
    CTBaseSingleton<CCodeSectionManager>::Get();
    this_00 = (CCharString *)CPersistContext::GetDefaultVal<CSpecialEffectsStringMap>();
    lVar3 = CCharString::GetLength(this_00);
    bVar4 = lVar3 != 0;
    CCharString::~CCharString((CCharString *)&stack0xfffffff4);
    pCVar7 = (CTVertexRHWColSpecTex1 *)0x0;
    pCVar6 = param_1;
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    CRenderManager2D::AddLineToDrawBuffer
              ((CRenderManager2D *)pCVar2,(CTVertexRHWColSpecTex1 *)pCVar6,pCVar7,(ulong)unaff_ESI,
               (CTexture *)pCVar8);
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    CUserProfileManager::SetDefaultValues((CUserProfileManager *)pCVar2);
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    CUserProfileManager::ApplyNewProfileValues((CUserProfileManager *)pCVar2);
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    if (pCVar2[0xc] == (CCodeSectionManager)0x0) {
      CVar5 = pCVar2[0x10];
    }
    else {
      CVar5 = pCVar2[0xd0];
    }
    bVar1 = true;
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    CUserProfileManager::SetCameraBowYZInverted((CUserProfileManager *)pCVar2,(bool)CVar5,bVar1);
    pCVar2 = CTBaseSingleton<CCodeSectionManager>::Get();
    CUserProfileManager::Save((CUserProfileManager *)pCVar2);
    CCharString::CCharString
              ((CCharString *)&stack0xfffffff4,"UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE",-1);
    SetMainMenuDefName(this,(CCharString *)&stack0xfffffff4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff4,
               extraout_EDX,unaff_ESI);
    AddProfileNameToScreens(this,param_1);
  }
  return bVar4;
}


// CALLEES of CreateNewProfile:
//   0040d2f0  SetCameraBowYZInverted
//   0040cca0  ApplyNewProfileValues
//   0040d400  AddLineToDrawBuffer
//   004067c0  IsEnoughSpaceForProfile
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0040a740  Save
//   0099b220  GetLength
//   0040c790  SetDefaultValues
//   0099ebf0  CCharString
//   00595b24  AddProfileNameToScreens
//   004082d0  GetDefaultVal<CSpecialEffectsStringMap>
//   0040d2a0  Get
//   00595a06  SetMainMenuDefName
//   0099b510  ~CCharString

//=== RefreshAvailableProfiles @ 00596cce (seed 00596cce) ===

/* [bsim sim=0.5039661763012944 <- ego_r]
   protected: long __thiscall NUISystem::CFrontEndManager::RefreshAvailableProfiles(void) */

long __thiscall NUISystem::CFrontEndManager::RefreshAvailableProfiles(CFrontEndManager *this)

{
  int *piVar1;
  int iVar2;
  char cVar3;
  long *plVar4;
  int *piVar5;
  CComponent *pCVar6;
  CRumbleDef *pCVar7;
  CAction *pCVar8;
  undefined4 uVar9;
  CActionParamString *this_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_06;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_07;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar10;
  CWideString *local_68;
  int local_64;
  undefined4 local_60;
  CAction *local_5c;
  CCPPointerInfo *local_58;
  int *local_54 [2];
  undefined4 *local_4c [2];
  int *local_44;
  CActiveFile local_40 [4];
  int *local_3c;
  undefined4 local_38;
  int *local_34 [2];
  int *local_2c;
  CCharString local_28 [4];
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  local_24 [4];
  int *local_20;
  CManager *local_1c;
  CCharString local_18 [4];
  CCharString local_14 [4];
  float local_10;
  CFrontEndManager *local_c;
  CWideString *local_8;
  
  local_68 = (CWideString *)0x0;
  local_64 = 0;
  local_60 = 0;
  local_c = this;
  GetProfileNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&local_68);
  local_1c = (CManager *)GetInstance();
  local_20 = (int *)0x7;
  plVar4 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_20);
  piVar1 = (int *)*plVar4;
  iVar2 = *piVar1;
  ppVar10 = extraout_ECX;
  CCharString::CCharString((CCharString *)&stack0xffffff88,"UI_FRONTEND_LIST_FOR_PROFILES",-1);
  piVar5 = (int *)(**(code **)(iVar2 + 0xc))();
  local_20 = piVar5;
  if (piVar5 != (int *)0x0) {
    CBankFile::GetBankHandle((CBankFile *)&local_44);
    iVar2 = *piVar5;
    local_44 = (int *)0x1;
    std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
              ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffff80,
               (pair<unsigned_long,class_CAnimationEntry> *)&local_44);
    (**(code **)(iVar2 + 0xf8))();
    CFileInstaller::CActiveFile::OnReadFinished(local_40);
  }
  CCharString::CCharString((CCharString *)&local_10,"UI_FRONTEND_LIST_FOR_PROFILES",-1);
  pCVar6 = CManager::CreateComponent(local_1c,(CCharString *)&local_10,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_5c,(CAction *)pCVar6);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,extraout_EDX
             ,ppVar10);
  local_3c = (int *)0x0;
  local_38 = 0;
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_3c,local_5c,local_58);
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_5c;
  (**(code **)(*piVar1 + 0xec))();
  CCharString::CCharString((CCharString *)&local_10,"TEXT_GUI_MENU_NEW_PROFILE",-1);
  NScript::
  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
            (local_24,(CIsThingAlive *)&local_10);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_00,ppVar10);
  CCharString::CCharString((CCharString *)&local_8,"",-1);
  CCharString::CCharString((CCharString *)&local_10,"UI_FRONTEND_BUTTON_FOR_PROFILES_LIST",-1);
  pCVar6 = CreateFrontEndButtonComponent
                     (local_c,(CWideString *)local_24,0.0,(CCharString *)&local_10,
                      (CCharString *)&local_8,true);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_54,(CAction *)pCVar6);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_01,ppVar10);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_02,ppVar10);
  pCVar7 = ::operator_new(0x10);
  if (pCVar7 == (CRumbleDef *)0x0) {
    pCVar8 = (CAction *)0x0;
  }
  else {
    pCVar8 = (CAction *)CRumbleDef::CRumbleDef(pCVar7);
  }
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_4c,pCVar8);
  *local_4c[0] = 0x125;
  (**(code **)(*local_54[0] + 0x108))();
  (**(code **)(*local_54[0] + 0x11c))(local_4c);
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_54;
  (**(code **)(*local_3c + 0xf0))();
  local_2c = (int *)(local_64 - (int)local_68 >> 2);
  local_10 = 50.0;
  if (local_2c != (int *)0x0) {
    local_8 = local_68;
    local_20 = local_2c;
    do {
      CCharString::CCharString(local_18,"",-1);
      CCharString::CCharString(local_14,"UI_FRONTEND_BUTTON_FOR_PROFILES_LIST",-1);
      pCVar6 = CreateFrontEndButtonComponent(local_c,local_8,local_10,local_14,local_18,false);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_44,(CAction *)pCVar6);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_14,
                 extraout_EDX_03,ppVar10);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18,
                 extraout_EDX_04,ppVar10);
      pCVar7 = ::operator_new(0x10);
      if (pCVar7 == (CRumbleDef *)0x0) {
        pCVar8 = (CAction *)0x0;
      }
      else {
        pCVar8 = (CAction *)CRumbleDef::CRumbleDef(pCVar7);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)local_34,pCVar8);
      CTBaseSingleton<CCodeSectionManager>::Get();
      uVar9 = CPersistContext::GetDefaultVal<CSpecialEffectsStringMap>();
      cVar3 = CUserProfileManager_VerifyAndLoadProfile(uVar9);
      CCharString::~CCharString(local_28);
      *local_34[0] = (-(uint)(cVar3 != '\0') & 0x49) + 0xdb;
      this_00 = ::operator_new(0xc);
      if (this_00 == (CActionParamString *)0x0) {
        local_1c = (CManager *)0x0;
      }
      else {
        local_1c = (CManager *)NUISystem::CActionParamString::CActionParamString(this_00,local_8);
      }
      CUIState__operator_(&local_1c);
      (**(code **)(*local_44 + 0x108))(local_34);
      (**(code **)(*local_44 + 0x11c))(local_34);
      (**(code **)(*local_3c + 0xf0))(&local_44);
      local_10 = local_10 + 30.0;
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 local_34,extraout_EDX_05);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_44,extraout_EDX_06);
      local_8 = local_8 + 4;
      local_20 = (int *)((int)local_20 + -1);
    } while (local_20 != (int *)0x0);
    local_20 = (int *)0x0;
  }
  piVar5 = local_2c;
  (**(code **)(*piVar1 + 0xac))();
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_4c
             ,extraout_EDX_07);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_54
             ,extraout_EDX_08);
  CCharString::~CCharString((CCharString *)local_24);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_3c,extraout_EDX_09);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_5c,extraout_EDX_10);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_68);
  return (long)piVar5;
}


// CALLEES of RefreshAvailableProfiles:
//   0099b510  ~CCharString
//   0059a994  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   005957d9  CUserProfileManager_VerifyAndLoadProfile
//   0040d2a0  Get
//   0041db1d  CreateComponent
//   0041e5f2  GetInstance
//   0042be50  CRumbleDef
//   00429c15  CCountedPointer<NUISystem::CAction>
//   00596a68  CreateFrontEndButtonComponent
//   0042cd84  pair<unsigned_long,CAnimationEntry>
//   00412130  ResetRings
//   0059b5d7  operator[]
//   0059aa86  ShareData
//   004291de  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00429160  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099b6a0  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
//   00bfea1a  operator_new
//   0042abca  OnReadFinished
//   0099ebf0  CCharString
//   004082d0  GetDefaultVal<CSpecialEffectsStringMap>
//   0054dae0  CActionParamString
//   0042bed4  GetBankHandle
//   0042aa29  CCountedPointer<NUISystem::CAction>
//   0042b646  CUIState::operator=
//   005955ab  GetProfileNames

//=== RefreshAvailableProfilesForDelete @ 00597006 (seed 00597006) ===

/* [bsim sim=0.5068560120970722 <- ego_r]
   protected: long __thiscall NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete(void)
    */

long __thiscall
NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete(CFrontEndManager *this)

{
  int *piVar1;
  int iVar2;
  long *plVar3;
  int *piVar4;
  CComponent *pCVar5;
  CRumbleDef *this_00;
  CAction *pCVar6;
  CActionParamString *this_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_02;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_03;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  int *piVar7;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar8;
  CWideString *local_4c;
  int local_48;
  undefined4 local_44;
  CAction *local_40;
  CCPPointerInfo *local_3c;
  int *local_38;
  undefined4 local_34;
  int *local_30 [2];
  undefined4 *local_28 [2];
  CFrontEndManager *local_20;
  int *local_1c;
  CManager *local_18;
  CCharString local_14 [4];
  CCharString local_10 [4];
  float local_c;
  CWideString *local_8;
  
  local_4c = (CWideString *)0x0;
  local_48 = 0;
  local_44 = 0;
  local_20 = this;
  GetProfileNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&local_4c);
  local_18 = (CManager *)GetInstance();
  local_1c = (int *)0x15;
  plVar3 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_1c);
  piVar1 = (int *)*plVar3;
  iVar2 = *piVar1;
  ppVar8 = extraout_ECX;
  CCharString::CCharString
            ((CCharString *)&stack0xffffffa4,"UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE",-1);
  piVar4 = (int *)(**(code **)(iVar2 + 0xc))();
  local_1c = piVar4;
  if (piVar4 != (int *)0x0) {
    CBankFile::GetBankHandle((CBankFile *)&local_38);
    iVar2 = *piVar4;
    local_38 = (int *)0x1;
    std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
              ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffff9c,
               (pair<unsigned_long,class_CAnimationEntry> *)&local_38);
    (**(code **)(iVar2 + 0xf8))();
    CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&local_34);
  }
  CCharString::CCharString((CCharString *)&local_c,"UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE",-1);
  pCVar5 = CManager::CreateComponent(local_18,(CCharString *)&local_c,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_40,(CAction *)pCVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_c,extraout_EDX,
             ppVar8);
  local_38 = (int *)0x0;
  local_34 = 0;
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_38,local_40,local_3c);
  ppVar8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_40;
  (**(code **)(*piVar1 + 0xec))();
  piVar4 = local_38;
  local_c = 0.0;
  piVar7 = (int *)(local_48 - (int)local_4c >> 2);
  if (piVar7 != (int *)0x0) {
    local_8 = local_4c;
    local_1c = piVar7;
    do {
      CCharString::CCharString(local_14,"",-1);
      CCharString::CCharString(local_10,"UI_FRONTEND_BUTTON_FOR_PROFILES_LIST",-1);
      pCVar5 = CreateFrontEndButtonComponent(local_20,local_8,local_c,local_10,local_14,false);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)local_30,(CAction *)pCVar5);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                 extraout_EDX_00,ppVar8);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_14,
                 extraout_EDX_01,ppVar8);
      this_00 = ::operator_new(0x10);
      if (this_00 == (CRumbleDef *)0x0) {
        pCVar6 = (CAction *)0x0;
      }
      else {
        pCVar6 = (CAction *)CRumbleDef::CRumbleDef(this_00);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)local_28,pCVar6);
      this_01 = ::operator_new(0xc);
      if (this_01 == (CActionParamString *)0x0) {
        local_18 = (CManager *)0x0;
      }
      else {
        local_18 = (CManager *)NUISystem::CActionParamString::CActionParamString(this_01,local_8);
      }
      *local_28[0] = 0xd7;
      CUIState__operator_();
      (**(code **)(*local_30[0] + 0x108))();
      (**(code **)(*local_30[0] + 0x11c))(local_28);
      (**(code **)(*piVar4 + 0xf0))(local_30);
      local_c = local_c + 30.0;
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 local_28,extraout_EDX_02);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 local_30,extraout_EDX_03);
      local_8 = local_8 + 4;
      local_1c = (int *)((int)local_1c + -1);
    } while (local_1c != (int *)0x0);
  }
  (**(code **)(*piVar1 + 0xac))();
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_38,extraout_EDX_04);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_40,extraout_EDX_05);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_4c);
  return (long)piVar7;
}


// CALLEES of RefreshAvailableProfilesForDelete:
//   0059b5d7  operator[]
//   0059aa86  ShareData
//   0059a994  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   004291de  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00429160  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0041db1d  CreateComponent
//   00bfea1a  operator_new
//   0041e5f2  GetInstance
//   0042abca  OnReadFinished
//   0099ebf0  CCharString
//   0042be50  CRumbleDef
//   00429c15  CCountedPointer<NUISystem::CAction>
//   00596a68  CreateFrontEndButtonComponent
//   0054dae0  CActionParamString
//   0042bed4  GetBankHandle
//   0042cd84  pair<unsigned_long,CAnimationEntry>
//   0042aa29  CCountedPointer<NUISystem::CAction>
//   00412130  ResetRings
//   0042b646  CUIState::operator=
//   005955ab  GetProfileNames

//=== GotoProfileMenu @ 00597b20 (seed 00597b20) ===

/* [bsim sim=0.7175784189178503 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::GotoProfileMenu(void) */

void __thiscall NUISystem::CFrontEndManager::GotoProfileMenu(CFrontEndManager *this)

{
  CTCTrophy *this_00;
  int *piVar1;
  uint uVar2;
  long *plVar3;
  int *extraout_EAX;
  int *extraout_EAX_00;
  CFrontEndManager *pCVar4;
  CThing *unaff_EDI;
  bool bVar5;
  CFrontEndManager *local_c;
  CFrontEndManager *local_8;
  
  local_c = this;
  local_8 = this;
  local_c = (CFrontEndManager *)RefreshAvailableProfiles(this);
  this_00 = (CTCTrophy *)(this + 0x20);
  uVar2 = NThingMap::CThingMapManager::GetMemoryFootprint((CThingMapManager *)(this + 0x30));
  if (1 < uVar2) {
    local_8 = (CFrontEndManager *)0xb;
    plVar3 = std::
             map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
             ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                           *)(this + 0x54),(EUsedKeys *)&local_8);
    CTCTrophy::OnShowTrophyInitialise(this_00,unaff_EDI);
    if (*extraout_EAX == *plVar3) {
      CTCTrophy::OnShowTrophyInitialise(this_00,unaff_EDI);
      piVar1 = (int *)*extraout_EAX_00;
      (**(code **)(*piVar1 + 0xc0))(6);
      pCVar4 = GetInstance();
      (**(code **)(*(int *)pCVar4 + 0x14))(piVar1 + 1);
      CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
      bVar5 = local_c == (CFrontEndManager *)0x0;
      local_c = (CFrontEndManager *)0x7;
      if (bVar5) {
        local_c = (CFrontEndManager *)0xc;
      }
      plVar3 = std::
               map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
               ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                             *)(this + 0x54),(EUsedKeys *)&local_c);
      *(long *)(this + 0x9c) = *plVar3;
      *(int **)(this + 0x98) = piVar1;
      CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
      return;
    }
  }
  local_c = (CFrontEndManager *)0x7;
  plVar3 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_c);
  GotoNextScreen(this,(CComponent *)*plVar3,false);
  return;
}


// CALLEES of GotoProfileMenu:
//   0059b61c  FrameUpdate
//   0059b5d7  operator[]
//   0059a8f3  GetMemoryFootprint
//   0041e5f2  GetInstance
//   00596cce  RefreshAvailableProfiles
//   0059b039  OnShowTrophyInitialise
//   00596763  GotoNextScreen
//   0059aee5  FrameUpdate

//=== RefreshAvailableSavedGamesForProfile @ 00598463 (seed 00598463) ===

/* [bsim sim=0.7395147444611525 <- ego_r]
   protected: long __thiscall
   NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile(void) */

long __thiscall
NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile(CFrontEndManager *this)

{
  CComponent *pCVar1;
  int iVar2;
  bool bVar3;
  char cVar4;
  char cVar5;
  long *plVar6;
  CCodeSectionManager *pCVar7;
  int *piVar8;
  CComponent *pCVar9;
  int *piVar10;
  CWideString *pCVar11;
  CRumbleDef *this_00;
  CAction *pCVar12;
  undefined4 uVar13;
  CActionParamString *this_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_10;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_11;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_12;
  int iVar14;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString *pCVar15;
  FileAccessType FVar16;
  ulong uVar17;
  undefined **ppuVar18;
  CCountedPointer<NUISystem::CAction> *pCVar19;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar20;
  CDiskFileWin32 local_d0 [28];
  CDiskFileWin32 local_b4 [28];
  CCountedPointer<class_CBankFileAsyncEntry> *local_98 [3];
  CCountedPointer<NUISystem::CAction> local_8c [8];
  CCountedPointer<NUISystem::CAction> local_84 [8];
  CCharString local_7c [4];
  CWideString local_78 [4];
  CWideString local_74 [4];
  CWideString local_70 [4];
  int *local_6c [2];
  CWideString local_64 [4];
  CAction *local_60;
  CCPPointerInfo *local_5c;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  local_58 [4];
  int *local_54;
  undefined4 local_50;
  CCharString local_4c [4];
  CCharString local_48 [4];
  undefined4 *local_44 [2];
  CCharString local_3c [4];
  CWideString local_38 [4];
  CComponent *local_34;
  CActiveFile local_30 [4];
  CManager *local_2c;
  int local_28;
  int local_24;
  undefined4 local_20;
  uint local_1c;
  CCharString local_18 [4];
  int *local_14;
  uint local_10;
  char local_b;
  char local_a;
  char local_9;
  float local_8;
  
  local_1c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  GetSavedGameNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&local_28);
  local_2c = (CManager *)GetInstance();
  local_14 = (int *)0x8;
  plVar6 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_14);
  pCVar1 = (CComponent *)*plVar6;
  (**(code **)(*(int *)pCVar1 + 0xd4))();
  Vector_CopyFrom_Init();
  ::CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
            ((CCountedPointer<CBankFileAsyncEntry> *)&local_60,local_98[0]);
  local_54 = (int *)0x0;
  local_50 = 0;
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_54,local_60,local_5c);
  CCharString::CCharString((CCharString *)&local_8,"TEXT_GUI_MENU_LOAD_GAME",-1);
  NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b871c + 0x60),local_3c);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,extraout_EDX,
             unaff_EDI);
  pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
  NGameText::ConstructString((CWideString *)local_3c,(CWideString *)(pCVar7 + 8));
  (**(code **)(*local_54 + 0x240))();
  iVar14 = *(int *)pCVar1;
  ppVar20 = extraout_ECX;
  CCharString::CCharString((CCharString *)&stack0xffffff14,"UI_FRONTEND_LIST_FOR_SAVES",-1);
  piVar8 = (int *)(**(code **)(iVar14 + 0xc))();
  local_14 = piVar8;
  if (piVar8 != (int *)0x0) {
    CBankFile::GetBankHandle((CBankFile *)&local_34);
    iVar14 = *piVar8;
    local_34 = (CComponent *)0x1;
    std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
              ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffff0c,
               (pair<unsigned_long,class_CAnimationEntry> *)&local_34);
    (**(code **)(iVar14 + 0xf8))();
    CFileInstaller::CActiveFile::OnReadFinished(local_30);
  }
  CCharString::CCharString((CCharString *)&local_8,"UI_FRONTEND_LIST_FOR_SAVES",-1);
  pCVar9 = CManager::CreateComponent(local_2c,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_6c,(CAction *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,ppVar20);
  ppVar20 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_6c;
  (**(code **)(*(int *)pCVar1 + 0xec))();
  CCharString::CCharString((CCharString *)&local_8,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1);
  pCVar9 = CreateComponent(this,(CCharString *)&local_8);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            (local_8c,(CAction *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_01,ppVar20);
  pCVar19 = local_8c;
  (**(code **)(*(int *)pCVar1 + 0xec))();
  CCharString::CCharString((CCharString *)&local_8,"UI_SCREENSHOT_VIEWPORT",-1);
  pCVar9 = CreateComponent(this,(CCharString *)&local_8);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            (local_84,(CAction *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar19);
  pCVar19 = local_84;
  (**(code **)(*(int *)pCVar1 + 0xec))();
  local_8 = 0.0;
  local_10 = 0;
  if (local_24 - local_28 >> 2 != 0) {
    do {
      pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
      bVar3 = CThingFilter_IsUnconscious::operator()
                        ((CThingFilter_IsUnconscious *)pCVar7,(CThing *)pCVar19);
      if (bVar3) {
LAB_00598694:
        bVar3 = false;
      }
      else {
        local_1c = local_1c | 1;
        piVar8 = (int *)(local_28 + local_10 * 4);
        pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
        piVar10 = (int *)CUserProfileManager::GetEmptySlotName((CUserProfileManager *)pCVar7);
        iVar14 = *piVar8;
        iVar2 = *piVar10;
        if (iVar14 != iVar2) {
          if ((iVar14 == 0) || (iVar2 == 0)) {
            cVar4 = '\0';
          }
          else {
            cVar4 = Compare_Unsigned16Array(iVar14,iVar2);
          }
          if (cVar4 == '\0') goto LAB_00598694;
        }
        bVar3 = true;
      }
      if ((local_1c & 1) != 0) {
        local_1c = local_1c & 0xfffffffe;
        CCharString::~CCharString(local_7c);
      }
      if (!bVar3) {
        CCharString::CCharString(local_18);
        iVar14 = local_10 * 4;
        CWideString__ConvertToCharString(&local_14);
        pCVar11 = (CWideString *)
                  NScript::
                  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            (local_58,(CIsThingAlive *)&local_14);
        CWideString::operator=((CWideString *)local_18,pCVar11);
        CCharString::~CCharString((CCharString *)local_58);
        pCVar15 = local_18;
        pCVar11 = local_70;
        pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
        pCVar11 = (CWideString *)
                  CUserProfileManager::GetSaveDisplayNameFromFileName
                            ((CUserProfileManager *)pCVar7,pCVar11,(long)pCVar15);
        CWideString::operator=((CWideString *)local_18,pCVar11);
        CCharString::~CCharString((CCharString *)local_70);
        CCharString::CCharString(local_48,"UI_BUTTON_FOR_SAVE_NAME",-1);
        CCharString::CCharString(local_4c,"UI_FRONTEND_BUTTON_FOR_SAVE_LIST",-1);
        pCVar9 = CreateFrontEndButtonComponent
                           (this,(CWideString *)local_18,local_8,local_4c,local_48,true);
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)&local_34,(CAction *)pCVar9);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_4c,
                   extraout_EDX_03,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar19);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_48,
                   extraout_EDX_04,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar19);
        this_00 = ::operator_new(0x10);
        if (this_00 == (CRumbleDef *)0x0) {
          pCVar12 = (CAction *)0x0;
        }
        else {
          pCVar12 = (CAction *)CRumbleDef::CRumbleDef(this_00);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_44,pCVar12);
        cVar4 = (char)iVar14;
        cVar5 = (char)local_28 + cVar4;
        pCVar11 = local_64;
        pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
        uVar13 = CUserProfileManager::GetPathNameFromFileName
                           ((CUserProfileManager *)pCVar7,pCVar11,(bool)cVar5);
        local_b = CUserProfileManager_VerifyAndLoadProfile(uVar13);
        CCharString::~CCharString((CCharString *)local_64);
        if (local_b != '\0') {
          uVar17 = 2;
          FVar16 = 1;
          cVar5 = (char)local_28 + cVar4;
          pCVar11 = local_78;
          pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
          pCVar11 = (CWideString *)
                    CUserProfileManager::GetPathNameFromFileName
                              ((CUserProfileManager *)pCVar7,pCVar11,(bool)cVar5);
          CDiskFileWin32::CDiskFileWin32(local_b4,pCVar11,FVar16,uVar17);
          CCharString::~CCharString((CCharString *)local_78);
          CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                    ((CAIStateGroup_CreatureUseCoverBase *)local_b4);
        }
        ppuVar18 = &PTR_LAB_01236a74;
        cVar4 = (char)local_28 + cVar4;
        pCVar11 = local_74;
        pCVar7 = CTBaseSingleton<CCodeSectionManager>::Get();
        CUserProfileManager::GetPathNameFromFileName
                  ((CUserProfileManager *)pCVar7,pCVar11,(bool)cVar4);
        CCharString__AssignFromValue(ppuVar18);
        CCharString::~CCharString((CCharString *)local_74);
        local_a = '\0';
        local_9 = CUserProfileManager_VerifyAndLoadProfile(local_38);
        bVar3 = CAFile::PathExists(local_38);
        if (bVar3) {
          local_a = '\x01';
          if (local_9 != '\0') {
            CDiskFileWin32::CDiskFileWin32(local_d0,local_38,1,2);
            CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase
                      ((CAIStateGroup_CreatureUseCoverBase *)local_d0);
          }
          *(int *)(this + 0xbc) = *(int *)(this + 0xbc) + 1;
        }
        if ((local_b == '\0') || ((local_a != '\0' && (local_9 == '\0')))) {
          *local_44[0] = 0xdc;
        }
        else {
          *local_44[0] = 0x11;
        }
        this_01 = ::operator_new(0xc);
        if (this_01 == (CActionParamString *)0x0) {
          local_2c = (CManager *)0x0;
        }
        else {
          local_2c = (CManager *)
                     NUISystem::CActionParamString::CActionParamString
                               (this_01,(CWideString *)(local_28 + iVar14));
        }
        CUIState__operator_(&local_2c);
        (**(code **)(*(int *)local_34 + 0x108))(local_44);
        (**(code **)(*(int *)local_34 + 0x11c))(local_44);
        (**(code **)(*local_6c[0] + 0xf0))(&local_34);
        local_8 = local_8 + 30.0;
        ConstructFileDescription(this,local_10,local_34,pCVar1,(CWideString *)(iVar14 + local_28));
        *(int *)(this + 0xbc) = *(int *)(this + 0xbc) + 1;
        CCharString::~CCharString((CCharString *)local_38);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_44,extraout_EDX_05);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   &local_34,extraout_EDX_06);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_14,
                   extraout_EDX_07,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar19);
        CCharString::~CCharString(local_18);
      }
      local_10 = local_10 + 1;
    } while (local_10 < (uint)(local_24 - local_28 >> 2));
  }
  (**(code **)(*(int *)pCVar1 + 0xac))();
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_84
             ,extraout_EDX_08);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_8c
             ,extraout_EDX_09);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_6c
             ,extraout_EDX_10);
  CCharString::~CCharString(local_3c);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_54,extraout_EDX_11);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_60,extraout_EDX_12);
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)local_98);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_28);
  return 0;
}


// CALLEES of RefreshAvailableSavedGamesForProfile:
//   00429460  ShareData
//   0099b510  ~CCharString
//   00535900  OnReadFinished
//   00406a80  GetEmptySlotName
//   00407e10  GetSaveDisplayNameFromFileName
//   0059ae54  Vector_CopyFrom_Init
//   005957d9  CUserProfileManager_VerifyAndLoadProfile
//   00428928  CCountedPointer<CBankFileAsyncEntry>
//   0040d2a0  Get
//   0040f0a0  Compare_Unsigned16Array
//   0099bf30  CCharString::AssignFromValue
//   0042925c  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099b7d0  operator=
//   009c95e0  GetTextBySymbol
//   0041db1d  CreateComponent
//   0041e5f2  GetInstance
//   0042be50  CRumbleDef
//   009c7980  ConstructString
//   00429c15  CCountedPointer<NUISystem::CAction>
//   00596a68  CreateFrontEndButtonComponent
//   00407030  operator()
//   0042cd84  pair<unsigned_long,CAnimationEntry>
//   00412130  ResetRings
//   0059b5d7  operator[]
//   0099aed0  CCharString
//   004291de  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00429160  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0099b2c0  CWideString::ConvertToCharString
//   00999230  PathExists
//   0099b6a0  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
//   00597df7  GetSavedGameNames
//   00595cc1  ConstructFileDescription
//   00594f94  CreateComponent
//   00bfea1a  operator_new
//   0042abca  OnReadFinished
//   0099ebf0  CCharString
//   00401630  ~CAIStateGroup_CreatureUseCoverBase
//   0054dae0  CActionParamString
//   0042bed4  GetBankHandle
//   00408120  GetPathNameFromFileName
//   0042aa29  CCountedPointer<NUISystem::CAction>
//   0042b646  CUIState::operator=
//   0099ad80  CDiskFileWin32

//=== LoadProfile @ 0059899a (seed 0059899a) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::LoadProfile(class CWideString const &) */

void __thiscall
NUISystem::CFrontEndManager::LoadProfile(CFrontEndManager *this,CWideString *param_1)

{
  CCodeSectionManager *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CWideString *pCVar2;
  char *pcVar3;
  CTVertexRHWColSpecTex1 *pCVar4;
  CTexture *in_stack_ffffffec;
  int iVar5;
  CCharString local_8 [4];
  
  pCVar1 = CTBaseSingleton<CCodeSectionManager>::Get();
  pCVar4 = (CTVertexRHWColSpecTex1 *)0x1;
  pCVar1[0xc] = (CCodeSectionManager)0x0;
  pCVar2 = param_1;
  pCVar1 = CTBaseSingleton<CCodeSectionManager>::Get();
  CRenderManager2D::AddLineToDrawBuffer
            ((CRenderManager2D *)pCVar1,(CTVertexRHWColSpecTex1 *)pCVar2,pCVar4,(ulong)unaff_ESI,
             in_stack_ffffffec);
  iVar5 = 0;
  GetSavedGameNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&stack0xffffffec);
  if (iVar5 == 0) {
    pcVar3 = "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE";
  }
  else {
    pcVar3 = "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE";
  }
  CCharString::CCharString(local_8,pcVar3,-1);
  SetMainMenuDefName(this,local_8);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,extraout_EDX,
             unaff_ESI);
  AddProfileNameToScreens(this,param_1);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&stack0xffffffec);
  return;
}


// CALLEES of LoadProfile:
//   00597df7  GetSavedGameNames
//   0099ebf0  CCharString
//   00595b24  AddProfileNameToScreens
//   0040d400  AddLineToDrawBuffer
//   00595a06  SetMainMenuDefName
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0040d2a0  Get
//   00412130  ResetRings

