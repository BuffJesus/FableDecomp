//=== operator[] @ 009ac530 (seed 009ac530) ===

/* [bsim sim=0.754554316669569 <- ego_r]
   public: class C2DVector & __thiscall std::map<class CCharString,class C2DVector,struct
   std::less<class CCharString>,class std::allocator<struct std::pair<class CCharString const ,class
   C2DVector> > >::operator[](class CCharString const &) */

C2DVector * __thiscall
std::
map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
::operator[](map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_>
             *this,CCharString *param_1)

{
  CBasicString<char> *pCVar1;
  CBasicString<char> *this_00;
  CCharString *pCVar2;
  bool bVar3;
  CCharString *pCVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString local_18 [4];
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  pCVar2 = param_1;
  pCVar4 = (CCharString *)
           _Tree<std::_Tmap_traits<CCharString,float,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,float>_>,1>_>
           ::_Lbound((_Tree<std::_Tmap_traits<CCharString,float,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,float>_>,1>_>
                      *)this,param_1);
  if (pCVar4 != (CCharString *)*(_Node **)this) {
    pCVar1 = *(CBasicString<char> **)(pCVar4 + 0x10);
    this_00 = *(CBasicString<char> **)pCVar2;
    if (pCVar1 == this_00) goto LAB_009ac5bf;
    if (this_00 != (CBasicString<char> *)0x0) {
      if (pCVar1 == (CBasicString<char> *)0x0) goto LAB_009ac5bf;
      bVar3 = CBasicString<char>::operator<(this_00,pCVar1);
      if (!bVar3) goto LAB_009ac5bf;
    }
  }
  CCharString::CCharString(local_18,param_1);
  local_8 = 0;
  local_4 = 0;
  local_14 = 0;
  local_10 = 0;
  local_c = 0;
  map_operator_square_bracket_CCharString_1(&param_1,pCVar4,local_18);
  pCVar4 = param_1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18,extraout_EDX,
             unaff_EDI);
LAB_009ac5bf:
  return (C2DVector *)(pCVar4 + 0x14);
}


// CALLEES of operator[]:
//   009ab870  map_operator_square_bracket_CCharString_1
//   0099ec30  CCharString
//   00429950  operator<
//   009aad60  _Lbound
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>

