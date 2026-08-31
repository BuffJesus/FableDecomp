//=== SetText @ 00cbe87f (seed 0x00CBE87F) ===

/* [bsim sim=0.5079268553761639 <- ego_r]
   public: void __thiscall CSubtitleRenderer::SetText(class CFontBank const &,class CCharString
   const &,class CCharString const &,class CCharString const &,class CCharString const &,unsigned
   long,float,bool,bool) */

void __thiscall
CSubtitleRenderer::SetText
          (CSubtitleRenderer *this,CFontBank *param_1,CCharString *param_2,CCharString *param_3,
          CCharString *param_4,CCharString *param_5,ulong param_6,float param_7,bool param_8,
          bool param_9)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  undefined *puVar5;
  undefined4 uVar6;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_1c [4];
  CCharString local_18 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_14 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_10 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8 [4];
  
  puVar5 = &DAT_01244db4;
  uVar2 = GFIntToCharString_API(&DAT_01244db4);
  CCharString::CCharString(local_18,"TEXT_QST_LOG_STORY_",-1);
  CCharString__AppendData(uVar2);
  CCharString__AppendCString(puVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_14,extraout_EDX,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18,
             extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_1c,extraout_EDX_01,unaff_ESI);
  CCharString__AppendCString(&DAT_012c174c);
  CCharString__AppendCString(&DAT_012c1744);
  iVar1 = *DAT_0143e8f8;
  uVar6 = 1;
  uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
  uVar3 = CPersistContext::GetDefaultVal<CAnimationSet>();
  ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
           CPersistContext::GetDefaultVal<CAnimationSet>();
  (**(code **)(iVar1 + 0x4d0))(ppVar4,uVar3,uVar2,uVar6);
  CCharString::~CCharString((CCharString *)local_1c);
  CCharString::~CCharString(local_18);
  CCharString::~CCharString((CCharString *)local_14);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_10,extraout_EDX_02,ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_8,extraout_EDX_03,ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_c,extraout_EDX_04,ppVar4);
  return;
}


// CALLEES of SetText:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   0099f570  CCharString::AppendData
//   0099f600  CCharString::AppendCString
//   0099f830  GFIntToCharString_API
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099b510  ~CCharString

