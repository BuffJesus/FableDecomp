//=== SetAsActive @ 007dfeb0 (seed 0x007dfeb0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCTeleporter::SetAsActive(bool) */

void __thiscall CTCTeleporter::SetAsActive(CTCTeleporter *this,bool param_1)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  
  if ((CTCTeleporter)param_1 != this[0x10]) {
    this[0x10] = (CTCTeleporter)param_1;
    if (param_1) {
      this[0x11] = (CTCTeleporter)0x1;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (DAT_013b8790,in_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0);
    return;
  }
  return;
}


// CALLEES of SetAsActive:
//   004378c0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>

//=== CTCTeleporter @ 007dfee0 (seed 0x007dfee0) ===

/* [bsim sim=0.8014649960951367 <- ego_r]
   public: __thiscall CTCTeleporter::CTCTeleporter(class CThing &) */

CTCTeleporter * __thiscall CTCTeleporter::CTCTeleporter(CTCTeleporter *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126fda4;
  *(undefined4 *)(this + 0xc) = 0;
  this[0x10] = (CTCTeleporter)0x0;
  this[0x11] = (CTCTeleporter)0x0;
  this[0x12] = (CTCTeleporter)0x1;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x13));
  *(undefined ***)(this + 0x13) = &PTR__vector_deleting_destructor__01238c6c;
  this[0x1b] = (CTCTeleporter)0x0;
  CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
            (*(CDefClassBase **)(*(int *)(this + 4) + 0x70),
             (CDefPointer<CAICreatureWillPowerIndicatorDef_const_> *)(this + 0xc));
  return this;
}


// CALLEES of CTCTeleporter:
//   00a01b10  CBaseIntelligentPointer
//   007e0540  GetSubDef<CAICreatureWillPowerIndicatorDef>
//   00686800  CEngineSubPrimitive2DClockSprite

//=== 0x007e04f0 : NO FUNCTION ===
//=== 0x007e0530 : NO FUNCTION ===
