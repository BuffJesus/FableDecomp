//=== TeleportHero @ 007dfcf0 (seed 0x007dfcf0) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.970650675795008 <- ego_r]
   public: void __thiscall CTCTeleporter::TeleportHero(void) */

void __thiscall CTCTeleporter::TeleportHero(CTCTeleporter *this)

{
  CDisplayEngine *pCVar1;
  float *pfVar2;
  ulong uVar3;
  CIDrawEnvironment *pCVar4;
  float10 fVar5;
  CGameEvent *pCVar6;
  undefined1 auStack_34 [4];
  float local_30;
  undefined4 local_2c;
  undefined1 local_28;
  undefined1 local_7;
  undefined1 local_6;
  undefined1 local_5;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCVar1[0x12f] = (CDisplayEngine)0x1;
  local_2c = 0x2c;
  local_28 = 0xff;
  local_7 = 0;
  local_6 = 1;
  local_5 = 0;
  pfVar2 = (float *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
  local_30 = *pfVar2;
  CMemoryBuffer__Copy(4,&local_30);
  uVar3 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
  local_30 = *(float *)(uVar3 + 4);
  CMemoryBuffer__Copy(4,&local_30);
  uVar3 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
  local_30 = *(float *)(uVar3 + 8);
  CMemoryBuffer__Copy(4,&local_30);
  fVar5 = (float10)(**(code **)(**(int **)(*(int *)(this + 4) + 0x60) + 0xfc))();
  local_30 = (float)fVar5;
  CMemoryBuffer__Copy(4,&local_30);
  auStack_34[0] = 1;
  CMemoryBuffer__Copy(1,auStack_34);
  auStack_34[0] = 0;
  CMemoryBuffer__Copy(1,auStack_34);
  auStack_34[0] = 0;
  CMemoryBuffer__Copy(1,auStack_34);
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCVar4 = CWorld::DrawGetEnvironment((CWorld *)pCVar1);
  pCVar6 = (CGameEvent *)&local_2c;
  local_28 = SUB41(pCVar4,0);
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  CWorld::HandleMoveHeroToRegionGameEvent((CWorld *)pCVar1,pCVar6);
  return;
}


// CALLEES of TeleportHero:
//   009f1760  CMemoryBuffer::Copy
//   00686a70  GetDisplayEngine
//   00686d50  GetDisplayEngine
//   00449700  DrawGetEnvironment
//   00bfe9f9  __security_check_cookie
//   004c73d0  DoSizeof
//   0049eaf0  HandleMoveHeroToRegionGameEvent

//=== StartTeleporting @ 007dff90 (seed 0x007dff90) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.6955747754113022 <- ego_r]
   private: void __thiscall CTCTeleporter::StartTeleporting(class CThingPlayerCreature &) */

void __thiscall CTCTeleporter::StartTeleporting(CTCTeleporter *this,CThingPlayerCreature *param_1)

{
  CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_> *this_00;
  CThingPlayerCreature *pCVar1;
  int *piVar2;
  CDisplayEngine *pCVar3;
  CIDrawEnvironment *pCVar4;
  int iVar5;
  int local_30;
  undefined4 local_2c;
  undefined1 local_28;
  undefined1 local_7;
  undefined1 local_6;
  undefined1 local_5;
  undefined4 local_4;
  
  pCVar1 = param_1;
  local_4 = DAT_0139c8a8;
  iVar5 = local_30;
  if ((*(uint *)(param_1 + 0x20) & 0x8000000) != 0) {
    this_00 = (CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
               *)(param_1 + 0x44);
    param_1 = (CThingPlayerCreature *)0x1b;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound(this_00,(ETCInterfaceType *)&param_1);
    if ((piVar2 == *(int **)(pCVar1 + 0x48)) || (0x1b < *piVar2)) {
      piVar2 = *(int **)(pCVar1 + 0x48);
    }
    iVar5 = piVar2[1];
  }
  *(undefined1 *)(iVar5 + 0x240) = 0;
  *(undefined1 *)(iVar5 + 0x241) = 0;
  if ((*(char *)(DAT_013b8790 + 0xfd) == '\0') &&
     (pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this),
     *(int *)(pCVar3 + 0x104) == 0)) {
    param_1 = (CThingPlayerCreature *)((uint)param_1 & 0xffffff00);
    CTCInventoryMap__BuildMapTeleportUI(0,1,&param_1);
    if (((char)param_1 != '\0') &&
       ((((byte)pCVar1[0x91] & 1) == 0 && (0.0001 < *(float *)(pCVar1 + 0xb4))))) {
      local_2c = 0x1a;
      local_28 = 0xff;
      local_7 = 0;
      local_6 = 1;
      local_5 = 0;
      local_30 = 8;
      CMemoryBuffer__Copy(4,&local_30);
      pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      pCVar4 = CWorld::DrawGetEnvironment((CWorld *)pCVar3);
      local_28 = SUB41(pCVar4,0);
      CMainGameComponent::AddGameEvent(DAT_013b86a0,(CGameEvent *)&local_2c);
      return;
    }
  }
  *(undefined1 *)(iVar5 + 0x240) = 1;
  return;
}


// CALLEES of StartTeleporting:
//   0041649c  AddGameEvent
//   00686a80  GetDisplayEngine
//   009f1760  CMemoryBuffer::Copy
//   00686d50  GetDisplayEngine
//   00449700  DrawGetEnvironment
//   00bfe9f9  __security_check_cookie
//   0060c900  CTCInventoryMap::BuildMapTeleportUI
//   0040f020  LowerBound

//=== FrameUpdate @ 007e00d0 (seed 0x007e00d0) ===

/* [bsim sim=0.8102064627579963 <- ego_r]
   public: virtual void __thiscall CTCTeleporter::FrameUpdate(void) */

void __thiscall CTCTeleporter::FrameUpdate(CTCTeleporter *this)

{
  CBaseIntelligentPointer *this_00;
  float fVar1;
  char cVar2;
  bool bVar3;
  CDisplayEngine *pCVar4;
  CThingPlayerCreature *extraout_EAX;
  int iVar5;
  CIDrawEnvironment *pCVar6;
  CIDrawEnvironment *pCVar7;
  CVertexBufferWin32 *this_01;
  float *pfVar8;
  float *pfVar9;
  int *piVar10;
  CBaseObject *pCVar11;
  C3DVector *pCVar12;
  CThing *pCVar13;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBP;
  CTCDParticleEmitter *this_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CIntelligentPointer<CThingPhysical_const_> *this_03;
  undefined8 uVar15;
  CThingBuilding *apCStack_8 [2];
  
  if (this[0x1b] == (CTCTeleporter)0x0) {
    iVar5 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
    pCVar4 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar6 = (CIDrawEnvironment *)CWorldMap::GetRegionNumberMapIsIn((CWorldMap *)pCVar4,iVar5);
    pCVar4 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar7 = CWorld::DrawGetEnvironment((CWorld *)pCVar4);
    if (pCVar7 != pCVar6) {
      this[0x12] = (CTCTeleporter)0x1;
      return;
    }
    if ((((this[0x11] == (CTCTeleporter)0x0) && (*(char *)(*(int *)(this + 0xc) + 0x30) == '\0')) &&
        (cVar2 = CCharString__NotEqual("HERO_GUILD_TELEPORT_MARKER"), cVar2 != '\0')) &&
       (this[0x10] != (CTCTeleporter)0x1)) {
      this[0x10] = (CTCTeleporter)0x1;
      this[0x11] = (CTCTeleporter)0x1;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (DAT_013b8790,extraout_EDX,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0);
    }
    if (*(int *)(this + 0xc) != 0) {
      pCVar4 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      ppVar14 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                (int)*(short *)(*(int *)(this + 4) + 0x8e);
      cVar2 = (**(code **)(*(int *)pCVar4 + 0x28))();
      if (cVar2 != '\0') {
        pCVar4 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        if (pCVar4[0xd5] == (CDisplayEngine)0x0) {
          this_03 = (CIntelligentPointer<CThingPhysical_const_> *)(this + 0x13);
          bVar3 = ::IS_THING_ALIVE<CIntelligentPointer<CThingPhysical_const_>_>(this_03);
          if (bVar3) {
            bVar3 = true;
            pCVar11 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)this_03);
            CThing::Kill((CThing *)pCVar11,bVar3);
            ::CIntelligentPointer<CThing_const_>::CIntelligentPointer<CThing_const_>
                      ((CIntelligentPointer<CThing_const_> *)&stack0xfffffff4,(CThing *)0x0);
            CIntelligentPointer<CThingCreatureBase>::operator=
                      ((CIntelligentPointer<CThingCreatureBase> *)this_03,
                       (CIntelligentPointer<class_CThingCreatureBase> *)&stack0xfffffff4);
            CBaseIntelligentPointer::~CBaseIntelligentPointer
                      ((CBaseIntelligentPointer *)&stack0xfffffff4);
            return;
          }
        }
        else {
          NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
          uVar15 = Catch_0040d959();
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar15,
                     (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar15 >> 0x20),
                     unaff_EBP);
          apCStack_8[0] = (CThingBuilding *)this_01;
          bVar3 = IS_THING_ALIVE<CThingBuilding*>(apCStack_8);
          if (bVar3) {
            pfVar8 = (float *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
            pfVar9 = (float *)CVertexBufferWin32::DoSizeof(this_01);
            fVar1 = *(float *)(*(int *)(this + 0xc) + 0x28);
            if (fVar1 * fVar1 <=
                (*pfVar9 - *pfVar8) * (*pfVar9 - *pfVar8) +
                (pfVar9[1] - pfVar8[1]) * (pfVar9[1] - pfVar8[1])) {
              this[0x12] = (CTCTeleporter)0x0;
            }
            else {
              if (((this[0x11] == (CTCTeleporter)0x0) &&
                  (*(char *)(*(int *)(this + 0xc) + 0x30) != '\0')) &&
                 ((cVar2 = CCharString__NotEqual("HERO_GUILD_TELEPORT_MARKER"), cVar2 != '\0' &&
                  (this[0x10] != (CTCTeleporter)0x1)))) {
                this[0x10] = (CTCTeleporter)0x1;
                this[0x11] = (CTCTeleporter)0x1;
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          (DAT_013b8790,extraout_EDX_00,
                           (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0);
              }
              if (this[0x10] == (CTCTeleporter)0x0) {
                return;
              }
              if (this[0x12] == (CTCTeleporter)0x0) {
                this[0x12] = (CTCTeleporter)0x1;
                if ((*(uint *)(this_01 + 0x24) & 0x200) != 0) {
                  apCStack_8[0] = (CThingBuilding *)0x29;
                  piVar10 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                   ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                                 *)(this_01 + 0x44),(ETCInterfaceType *)apCStack_8);
                  if ((piVar10 == *(int **)(this_01 + 0x48)) || (0x29 < *piVar10)) {
                    piVar10 = *(int **)(this_01 + 0x48);
                  }
                  if (((CTCHero *)piVar10[1] != (CTCHero *)0x0) &&
                     (bVar3 = CTCHero::GiveTutorialForCategory((CTCHero *)piVar10[1],0x1f,false),
                     bVar3)) {
                    this[0x1b] = (CTCTeleporter)0x1;
                    return;
                  }
                }
                StartTeleporting(this,(CThingPlayerCreature *)this_01);
              }
            }
          }
          if (this[0x10] != (CTCTeleporter)0x0) {
            this_00 = (CBaseIntelligentPointer *)(this + 0x13);
            pCVar11 = CBaseIntelligentPointer::GetPItem(this_00);
            if (pCVar11 == (CBaseObject *)0x0) {
              iVar5 = *(int *)(this + 0xc);
              pCVar12 = (C3DVector *)
                        CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
              pCVar13 = CTCDParticleEmitter::Create
                                  (*(long *)(iVar5 + 0x2c),pCVar12,SUB41(pCVar11,0));
              ::CIntelligentPointer<CThing_const_>::CIntelligentPointer<CThing_const_>
                        ((CIntelligentPointer<CThing_const_> *)apCStack_8,pCVar13);
              CIntelligentPointer<CThingCreatureBase>::operator=
                        ((CIntelligentPointer<CThingCreatureBase> *)this_00,
                         (CIntelligentPointer<class_CThingCreatureBase> *)apCStack_8);
              CBaseIntelligentPointer::~CBaseIntelligentPointer
                        ((CBaseIntelligentPointer *)apCStack_8);
              pCVar11 = CBaseIntelligentPointer::GetPItem(this_00);
              if (pCVar11 != (CBaseObject *)0x0) {
                this_02 = (CTCDParticleEmitter *)0x0;
                pCVar11 = CBaseIntelligentPointer::GetPItem(this_00);
                if ((*(uint *)(pCVar11 + 0x28) & 0x800) != 0) {
                  apCStack_8[0] = (CThingBuilding *)0x4b;
                  piVar10 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                   ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                                 *)(pCVar11 + 0x44),(ETCInterfaceType *)apCStack_8);
                  if ((piVar10 == *(int **)(pCVar11 + 0x48)) || (0x4b < *piVar10)) {
                    piVar10 = *(int **)(pCVar11 + 0x48);
                  }
                  this_02 = (CTCDParticleEmitter *)piVar10[1];
                }
                CCharString::CCharString((CCharString *)apCStack_8,"",-1);
                CTCDParticleEmitter::AttachToThing
                          (this_02,*(CThing **)(this + 4),1,(CCharString *)apCStack_8,0,0.0);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           apCStack_8,extraout_EDX_01,ppVar14);
                CTCDParticleEmitter::SetAlwaysUpdate(this_02,true);
                return;
              }
            }
          }
        }
      }
    }
  }
  else if ((DAT_013b8790[0xfd] == (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0
           ) && (pCVar4 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this),
                pCVar4[0xa4] == (CDisplayEngine)0x0)) {
    this[0x1b] = (CTCTeleporter)0x0;
    NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    uVar15 = Catch_0040d959();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar15,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar15 >> 0x20),unaff_ESI
              );
    if ((extraout_EAX != (CThingPlayerCreature *)0x0) && (((byte)extraout_EAX[0x91] & 1) == 0)) {
      StartTeleporting(this,extraout_EAX);
      return;
    }
  }
  return;
}


// CALLEES of FrameUpdate:
//   00449970  Catch@0040d959
//   0099ebf0  CCharString
//   004a96f0  CIntelligentPointer<CThing_const_>
//   006e0be0  AttachToThing
//   00686d40  GetDisplayEngine
//   004378c0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   004c73d0  DoSizeof
//   004fb150  DrawGetEnvironment
//   00a01c10  ~CBaseIntelligentPointer
//   00686d50  GetDisplayEngine
//   00686d30  GetDisplayEngine
//   004a9760  operator=
//   006e0590  SetAlwaysUpdate
//   00686a80  GetDisplayEngine
//   00487dc0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   006e0880  Create
//   00449960  Catch@0040d959
//   00a01b50  GetPItem
//   0040f020  LowerBound
//   004cc340  IS_THING_ALIVE<CIntelligentPointer<CThingPhysical_const_>_>
//   007dff90  StartTeleporting
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   004c9b80  Kill
//   004fc190  GetRegionNumberMapIsIn
//   00686d20  GetDisplayEngine
//   0043dbd0  IS_THING_ALIVE<CThingBuilding*>
//   0099e960  CCharString::NotEqual
//   006c9fa0  GiveTutorialForCategory

//=== GetGuildSealRecallRegion @ 005fa990 (seed 0x005fa990) ===

/* [bsim sim=0.9941912256312974 <- ego_r]
   private: long __thiscall CTCInventoryMap::GetGuildSealRecallRegion(void)const  */

long __thiscall CTCInventoryMap::GetGuildSealRecallRegion(CTCInventoryMap *this)

{
  float fVar1;
  CDisplayEngine *pCVar2;
  CDisplayEngine *pCVar3;
  CWorldMap *this_00;
  long lVar4;
  
  pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  fVar1 = *(float *)(pCVar2 + 0x154);
  if (9.999999e-09 <
      *(float *)(pCVar2 + 0x15c) * *(float *)(pCVar2 + 0x15c) +
      *(float *)(pCVar2 + 0x158) * *(float *)(pCVar2 + 0x158) + fVar1 * fVar1) {
    pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    this_00 = (CWorldMap *)(**(code **)(*(int *)pCVar3 + 0x34))();
    lVar4 = (**(code **)(*(int *)this_00 + 0x40))(pCVar2 + 0x154);
    lVar4 = CWorldMap::GetRegionNumberMapIsIn(this_00,lVar4);
    return lVar4;
  }
  return 0;
}


// CALLEES of GetGuildSealRecallRegion:
//   004fc190  GetRegionNumberMapIsIn
//   00686a80  GetDisplayEngine

//=== TeleportHeroToGuildSealRecallLocation @ 005fad40 (seed 0x005fad40) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.9577997468437256 <- ego_r]
   public: void __thiscall CTCInventoryMap::TeleportHeroToGuildSealRecallLocation(void)const  */

void __thiscall CTCInventoryMap::TeleportHeroToGuildSealRecallLocation(CTCInventoryMap *this)

{
  ushort uVar1;
  int extraout_EAX;
  CDisplayEngine *pCVar2;
  CDisplayEngine *pCVar3;
  CIDrawEnvironment *pCVar4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar5;
  CGameEvent *pCVar6;
  undefined1 auStack_34 [4];
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined1 uStack_24;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  uVar5 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar5,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar5 >> 0x20),unaff_EDI);
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar2[0x12f] = (CDisplayEngine)0x1;
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    uStack_2c = *(undefined4 *)(pCVar3 + 0x160);
    uStack_28 = 0x2c;
    uStack_24 = 0xff;
    uVar1 = (ushort)local_4;
    local_4 = (uint)CONCAT12(1,uVar1 & 0xff);
    uStack_30 = *(undefined4 *)(pCVar2 + 0x154);
    CMemoryBuffer__Copy(4,&uStack_30);
    uStack_30 = *(undefined4 *)(pCVar2 + 0x158);
    CMemoryBuffer__Copy(4,&uStack_30);
    uStack_30 = *(undefined4 *)(pCVar2 + 0x15c);
    CMemoryBuffer__Copy(4,&uStack_30);
    CMemoryBuffer__Copy(4,&uStack_2c);
    auStack_34[0] = 1;
    CMemoryBuffer__Copy(1,auStack_34);
    auStack_34[0] = 0;
    CMemoryBuffer__Copy(1,auStack_34);
    auStack_34[0] = 0;
    CMemoryBuffer__Copy(1,auStack_34);
    pCVar4 = CWorld::DrawGetEnvironment(*(CWorld **)(DAT_013b86a0 + 0x1c));
    pCVar6 = (CGameEvent *)&uStack_28;
    uStack_24 = SUB41(pCVar4,0);
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    CWorld::HandleMoveHeroToRegionGameEvent((CWorld *)pCVar2,pCVar6);
  }
  return;
}


// CALLEES of TeleportHeroToGuildSealRecallLocation:
//   00686a80  GetDisplayEngine
//   009f1760  CMemoryBuffer::Copy
//   00686a70  GetDisplayEngine
//   00686d50  GetDisplayEngine
//   00487dd0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00449700  DrawGetEnvironment
//   00449960  Catch@0040d959
//   00bfe9f9  __security_check_cookie
//   0049eaf0  HandleMoveHeroToRegionGameEvent

