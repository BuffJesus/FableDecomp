//=== ProcessEvent @ 0053673b (seed 0053673b) ===

/* [bsim sim=0.961701703866198 <- ego_r]
   public: virtual void __thiscall NUISystem::CList::ProcessEvent(enum NUISystem::EEvent) */

void __thiscall NUISystem::CList::ProcessEvent(CList *this,EEvent param_1)

{
  char cVar1;
  CFrontEndManager *pCVar2;
  int *piVar3;
  CList *pCVar4;
  int *local_c;
  undefined4 uStack_8;
  
  local_c = (int *)0x0;
  if (*(float *)(this + 0x1e8) <= *(float *)(this + 0x2c) - *(float *)(this + 0x15c)) {
    uStack_8 = this;
    pCVar2 = CFrontEndManager::GetInstance();
    cVar1 = (**(code **)(*(int *)pCVar2 + 0x2c))();
    if ((cVar1 == '\0') || (param_1 != 0x19)) {
      pCVar2 = CFrontEndManager::GetInstance();
      if (pCVar2[0xa4] == (CFrontEndManager)0x0) {
        pCVar4 = this + -4;
        piVar3 = (int *)(**(code **)(*(int *)pCVar4 + 0x1b0))(&local_c);
        if ((*(char *)(*piVar3 + 0x20a) != '\0') ||
           (uStack_8 = (CList *)CONCAT13(1,(undefined3)uStack_8),
           (int *)*DAT_013b8a98 == DAT_013b8a98)) {
          uStack_8 = (CList *)((uint)uStack_8 & 0xffffff);
        }
        if (local_c != (int *)0x0) {
          piVar3 = local_c + 1;
          *piVar3 = *piVar3 + -1;
          if (*piVar3 == 0) {
            (**(code **)(*local_c + 4))();
          }
        }
        if (uStack_8._3_1_ == '\0') {
          if (param_1 == 0) {
            (**(code **)(*(int *)pCVar4 + 0x2d8))();
          }
          else if (param_1 == 1) {
            (**(code **)(*(int *)pCVar4 + 0x2dc))();
          }
        }
      }
    }
  }
  return;
}


// CALLEES of ProcessEvent:
//   0041e5f2  GetInstance

//=== ScrollDownProcess @ 00537302 (seed 00537302) ===

/* [bsim sim=0.9999344946049739 <- ego_r]
   protected: virtual void __thiscall NUISystem::CList::ScrollDownProcess(void) */

void __thiscall NUISystem::CList::ScrollDownProcess(CList *this)

{
  char cVar1;
  CFrontEndManager *pCVar2;
  CFrontEndManager *pCVar3;
  int *piVar4;
  int iVar5;
  CList *pCVar6;
  uint uVar7;
  CList *local_8;
  
  if (*(int *)(this + 0x168) - *(int *)(this + 0x164) >> 2 != 0) {
    uVar7 = 0;
    local_8 = this;
    if (*(int *)(this + 0x1bc) != 0) {
      pCVar2 = CFrontEndManager::GetInstance();
      (**(code **)(*(int *)pCVar2 + 0x38))(this + 0x1bc);
      return;
    }
    cVar1 = (**(code **)(*(int *)this + 0x2a4))();
    if (cVar1 != '\0') {
      return;
    }
    iVar5 = *(int *)(this + 0x15c);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + iVar5 * 4) + 4) + 0x10))(2);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(3);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(4);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(5);
    cVar1 = (**(code **)(*(int *)this + 0x2b0))();
    if (cVar1 == '\0') {
      (**(code **)(*(int *)this + 0x298))(0,1);
    }
    else {
      (**(code **)(*(int *)this + 0x2a0))();
    }
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (2);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (3);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (4);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (5);
    if (*(int *)(this + 0x174) - *(int *)(this + 0x170) >> 2 != 0) {
      do {
        cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xb0))
                          (*(int *)(this + 0x15c) + 8);
        if ((cVar1 != '\0') ||
           (cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xb4))
                              (*(int *)(this + 0x15c) + 8), cVar1 != '\0')) {
          (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xc0))
                    (*(int *)(this + 0x15c) + 8);
        }
        uVar7 = uVar7 + 1;
      } while (uVar7 < (uint)(*(int *)(this + 0x174) - *(int *)(this + 0x170) >> 2));
    }
    if (*(int *)(this + 0x15c) == iVar5) {
      pCVar3 = CFrontEndManager::GetInstance();
      pCVar2 = CFrontEndManager::GetInstance();
      piVar4 = (int *)(**(code **)(*(int *)pCVar3 + 0xa8))(&local_8);
      iVar5 = *piVar4 + 0x1a4;
    }
    else {
      pCVar3 = CFrontEndManager::GetInstance();
      pCVar2 = CFrontEndManager::GetInstance();
      piVar4 = (int *)(**(code **)(*(int *)pCVar3 + 0xa8))(&local_8);
      iVar5 = *piVar4 + 0x194;
    }
    (**(code **)(*(int *)pCVar2 + 0xa0))(iVar5,1);
    if (local_8 != (CList *)0x0) {
      pCVar6 = local_8 + 4;
      *(int *)pCVar6 = *(int *)pCVar6 + -1;
      if (*(int *)pCVar6 == 0) {
        (**(code **)(*(int *)local_8 + 4))();
      }
    }
  }
  *(undefined4 *)(this + 0x160) = *(undefined4 *)(this + 0x30);
  return;
}


// CALLEES of ScrollDownProcess:
//   0041e5f2  GetInstance

//=== ScrollDownProcess @ 0053753c (seed 0053753c) ===

/* [bsim sim=0.9999344946049739 <- ego_r]
   protected: virtual void __thiscall NUISystem::CList::ScrollDownProcess(void) */

void __thiscall NUISystem::CList::ScrollDownProcess(CList *this)

{
  char cVar1;
  CFrontEndManager *pCVar2;
  CFrontEndManager *pCVar3;
  int *piVar4;
  int iVar5;
  CList *pCVar6;
  uint uVar7;
  CList *local_8;
  
  if (*(int *)(this + 0x168) - *(int *)(this + 0x164) >> 2 != 0) {
    uVar7 = 0;
    local_8 = this;
    if (*(int *)(this + 0x1b4) != 0) {
      pCVar2 = CFrontEndManager::GetInstance();
      (**(code **)(*(int *)pCVar2 + 0x38))(this + 0x1b4);
      return;
    }
    cVar1 = (**(code **)(*(int *)this + 0x2a4))();
    if (cVar1 != '\0') {
      return;
    }
    iVar5 = *(int *)(this + 0x15c);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + iVar5 * 4) + 4) + 0x10))(2);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(3);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(4);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0x10)
    )(5);
    cVar1 = (**(code **)(*(int *)this + 0x2b0))();
    if (cVar1 == '\0') {
      (**(code **)(*(int *)this + 0x2a0))(0,1);
    }
    else {
      (**(code **)(*(int *)this + 0x298))();
    }
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (2);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (3);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (4);
    (**(code **)(*(int *)(*(int *)(*(int *)(this + 0x164) + *(int *)(this + 0x15c) * 4) + 4) + 0xc))
              (5);
    if (*(int *)(this + 0x174) - *(int *)(this + 0x170) >> 2 != 0) {
      do {
        cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xb0))
                          (*(int *)(this + 0x15c) + 8);
        if ((cVar1 != '\0') ||
           (cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xb4))
                              (*(int *)(this + 0x15c) + 8), cVar1 != '\0')) {
          (**(code **)(**(int **)(*(int *)(this + 0x170) + uVar7 * 4) + 0xc0))
                    (*(int *)(this + 0x15c) + 8);
        }
        uVar7 = uVar7 + 1;
      } while (uVar7 < (uint)(*(int *)(this + 0x174) - *(int *)(this + 0x170) >> 2));
    }
    if (*(int *)(this + 0x15c) == iVar5) {
      pCVar3 = CFrontEndManager::GetInstance();
      pCVar2 = CFrontEndManager::GetInstance();
      piVar4 = (int *)(**(code **)(*(int *)pCVar3 + 0xa8))(&local_8);
      iVar5 = *piVar4 + 0x1a4;
    }
    else {
      pCVar3 = CFrontEndManager::GetInstance();
      pCVar2 = CFrontEndManager::GetInstance();
      piVar4 = (int *)(**(code **)(*(int *)pCVar3 + 0xa8))(&local_8);
      iVar5 = *piVar4 + 0x194;
    }
    (**(code **)(*(int *)pCVar2 + 0xa0))(iVar5,1);
    if (local_8 != (CList *)0x0) {
      pCVar6 = local_8 + 4;
      *(int *)pCVar6 = *(int *)pCVar6 + -1;
      if (*(int *)pCVar6 == 0) {
        (**(code **)(*(int *)local_8 + 4))();
      }
    }
  }
  *(undefined4 *)(this + 0x160) = *(undefined4 *)(this + 0x30);
  return;
}


// CALLEES of ScrollDownProcess:
//   0041e5f2  GetInstance

//=== ScrollSelectionUp @ 00540e50 (seed 00540e50) ===

/* [bsim sim=0.9049785926842715 <- ego_r]
   public: virtual void __thiscall NUISystem::CDynamicList::ScrollSelectionUp(void) */

void __thiscall NUISystem::CDynamicList::ScrollSelectionUp(CDynamicList *this)

{
  int iVar1;
  
  if (*(int *)(this + 0x16c) != 0) {
    (**(code **)(**(int **)(*(int *)(this + 0x174) + *(int *)(this + 0x16c) * 4) + 0xc0))(4);
    iVar1 = *(int *)(this + 0x16c);
    *(int *)(this + 0x16c) = iVar1 + -1;
    (**(code **)(**(int **)(*(int *)(this + 0x174) + (iVar1 + -1) * 4) + 0xc0))(3);
    if (*(int *)(this + 0x16c) < *(int *)(this + 0x168)) {
      *(int *)(this + 0x168) = *(int *)(this + 0x16c);
                    /* WARNING: Could not recover jumptable at 0x00540eab. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*(int *)this + 0x238))();
      return;
    }
  }
  return;
}


// CALLEES of ScrollSelectionUp:

//=== ScrollSelectionDown @ 00540ec0 (seed 00540ec0) ===

/* [bsim sim=0.9395570287168774 <- ego_r]
   public: virtual void __thiscall NUISystem::CDynamicList::ScrollSelectionDown(void) */

void __thiscall NUISystem::CDynamicList::ScrollSelectionDown(CDynamicList *this)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x16c);
  if ((-1 < iVar1) && (iVar1 + 1U < (uint)(*(int *)(this + 0x178) - *(int *)(this + 0x174) >> 2))) {
    (**(code **)(**(int **)(*(int *)(this + 0x174) + iVar1 * 4) + 0xc0))(4);
    iVar1 = *(int *)(this + 0x16c);
    *(int *)(this + 0x16c) = iVar1 + 1;
    (**(code **)(**(int **)(*(int *)(this + 0x174) + (iVar1 + 1) * 4) + 0xc0))(3);
    if (*(int *)(this + 0x168) + *(int *)(this + 0x170) <= *(int *)(this + 0x16c)) {
      *(int *)(this + 0x168) = (*(int *)(this + 0x16c) - *(int *)(this + 0x170)) + 1;
                    /* WARNING: Could not recover jumptable at 0x00540f40. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*(int *)this + 0x238))();
      return;
    }
  }
  return;
}


// CALLEES of ScrollSelectionDown:

//=== Update @ 00555220 (seed 00555220) ===

/* [bsim sim=0.9398896691361488 <- ego_r]
   public: virtual void __thiscall NUISystem::CScrollableList::Update(float) */

void __thiscall NUISystem::CScrollableList::Update(CScrollableList *this,float param_1)

{
  CScrollableList *pCVar1;
  float fVar2;
  CScrollableList **ppCVar3;
  CFrontEndManager *pCVar4;
  CScrollableList *pCVar5;
  int iVar6;
  CScrollableList *local_4;
  
  local_4 = this;
  CChangingStateComponent::Update((CChangingStateComponent *)this,param_1);
  if (0.0 < *(float *)(this + 0x16c)) {
    fVar2 = *(float *)(this + 0x16c) - param_1;
    *(float *)(this + 0x16c) = fVar2;
    if (fVar2 < 0.0 != (fVar2 == 0.0)) {
      pCVar5 = *(CScrollableList **)(this + 0x15c);
      param_1 = (float)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3);
      local_4 = pCVar5 + *(int *)(this + 0x160);
      ppCVar3 = (CScrollableList **)&param_1;
      if (pCVar5 + *(int *)(this + 0x160) <= (uint)param_1) {
        ppCVar3 = &local_4;
      }
      pCVar1 = *ppCVar3;
      for (; pCVar5 < pCVar1; pCVar5 = pCVar5 + 1) {
        (**(code **)(*(int *)(*(int *)(*(int *)(this + 0xb0) + (int)pCVar5 * 8) + 4) + 0x10))(0x19);
        (**(code **)(*(int *)(*(int *)(*(int *)(this + 0xb0) + (int)pCVar5 * 8) + 4) + 0xc))(0x19);
      }
      if (this[0x178] != (CScrollableList)0x0) {
        (**(code **)(*(int *)(*(int *)(this + 0x170) + 4) + 0x10))(0x19);
        if (*(int *)(this + 0x170) == 0) {
          iVar6 = 0;
        }
        else {
          iVar6 = *(int *)(this + 0x170) + 4;
        }
        pCVar4 = CFrontEndManager::GetInstance();
        (**(code **)(*(int *)pCVar4 + 8))(iVar6);
        (**(code **)(*(int *)(*(int *)(this + 0x170) + 4) + 0xc))(0x19);
        (**(code **)(*(int *)(*(int *)(this + 0x170) + 4) + 4))(0x19);
      }
      if (this[0x179] != (CScrollableList)0x0) {
        (**(code **)(*(int *)(*(int *)(this + 0x174) + 4) + 0x10))(0x19);
        if (*(int *)(this + 0x174) == 0) {
          iVar6 = 0;
        }
        else {
          iVar6 = *(int *)(this + 0x174) + 4;
        }
        pCVar4 = CFrontEndManager::GetInstance();
        (**(code **)(*(int *)pCVar4 + 8))(iVar6);
        (**(code **)(*(int *)(*(int *)(this + 0x174) + 4) + 0xc))(0x19);
        (**(code **)(*(int *)(*(int *)(this + 0x174) + 4) + 4))(0x19);
      }
    }
  }
  return;
}


// CALLEES of Update:
//   0052c7e0  Update
//   0041e5f2  GetInstance

//=== HideList @ 005559d0 (seed 005559d0) ===

/* [bsim sim=0.803124018033408 <- ego_r]
   protected: void __thiscall NUISystem::CScrollableList::HideList(float) */

void __thiscall NUISystem::CScrollableList::HideList(CScrollableList *this,float param_1)

{
  int *piVar1;
  uint uVar2;
  undefined1 **ppuVar3;
  undefined1 *local_20;
  undefined1 auStack_1c [4];
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  uVar2 = 0;
  if (*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3 != 0) {
    do {
      piVar1 = *(int **)(*(int *)(this + 0xb0) + uVar2 * 8);
      uStack_14 = 0;
      local_20 = auStack_1c;
      (**(code **)(*piVar1 + 0x40))();
      local_20 = (undefined1 *)param_1;
      HideComponent(this,piVar1,0);
      uVar2 = uVar2 + 1;
    } while (uVar2 < (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3));
  }
  uStack_14 = 0;
  local_20 = auStack_1c;
  ppuVar3 = &local_20;
  (**(code **)(**(int **)(this + 0x170) + 0x40))();
  HideComponent(this,*(undefined4 *)(this + 0x170),0,param_1);
  uStack_18 = 0;
  (**(code **)(**(int **)(this + 0x174) + 0x40))(&local_20);
  HideComponent(this,*(undefined4 *)(this + 0x174),0,param_1,ppuVar3);
  *(undefined4 *)(this + 0x16c) = 0;
  this[0x178] = (CScrollableList)0x0;
  this[0x179] = (CScrollableList)0x0;
  return;
}


// CALLEES of HideList:
//   00555440  HideComponent

//=== UpdateVisibleList @ 00555a90 (seed 00555a90) ===

/* [bsim sim=0.926575867962825 <- ego_r]
   protected: void __thiscall NUISystem::CScrollableList::UpdateVisibleList(float,float) */

void __thiscall
NUISystem::CScrollableList::UpdateVisibleList(CScrollableList *this,float param_1,float param_2)

{
  float fVar1;
  int iVar2;
  int iVar3;
  float *pfVar4;
  undefined1 *unaff_EBX;
  undefined1 *puVar5;
  float fVar6;
  undefined4 unaff_retaddr;
  undefined1 **ppuVar7;
  undefined1 *local_2c;
  undefined1 *local_28;
  float afStack_24 [2];
  undefined1 *local_c;
  float local_8;
  undefined4 uStack_4;
  
  fVar1 = *(float *)(this + 0x168);
  fVar6 = 0.0;
  if (*(int *)(this + 0x15c) == 0) {
    afStack_24[1] = 0.0;
    local_2c = (undefined1 *)&local_28;
    (**(code **)(**(int **)(this + 0x170) + 0x40))();
    puVar5 = (undefined1 *)param_1;
    HideComponent(this,*(undefined4 *)(this + 0x170),0,param_1);
    this[0x178] = (CScrollableList)0x0;
  }
  else {
    local_28 = (undefined1 *)afStack_24;
    local_2c = (undefined1 *)0x555af3;
    (**(code **)(**(int **)(this + 0x170) + 0x40))();
    puVar5 = (undefined1 *)param_1;
    local_2c = (undefined1 *)param_1;
    UnhideComponent(this,*(undefined4 *)(this + 0x170),0);
    this[0x178] = (CScrollableList)0x1;
  }
  if (*(int *)(this + 0x15c) != 0) {
    do {
      if ((uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3) <= (uint)fVar6) break;
      afStack_24[0] = 0.0;
      local_2c = (undefined1 *)-fVar1;
      local_28 = unaff_EBX;
      HideComponent(this,*(undefined4 *)(*(int *)(this + 0xb0) + (int)fVar6 * 8),unaff_retaddr,
                    puVar5);
      fVar6 = (float)((int)fVar6 + 1);
    } while ((uint)fVar6 < (uint)*(float *)(this + 0x15c));
  }
  local_c = (undefined1 *)(-fVar1 + *(float *)(this + 0x164));
  iVar2 = *(int *)(this + 0x160);
  iVar3 = *(int *)(this + 0x15c);
  local_8 = (float)unaff_EBX + *(float *)(this + 0x168);
  pfVar4 = &param_1;
  if ((uint)(iVar2 + iVar3) <= (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3)) {
    pfVar4 = (float *)&stack0xfffffff0;
  }
  param_1 = *pfVar4;
  if ((uint)fVar6 < (uint)param_1) {
    do {
      afStack_24[0] = local_8;
      local_28 = local_c;
      local_2c = puVar5;
      UnhideComponent(this,*(undefined4 *)(*(int *)(this + 0xb0) + (int)fVar6 * 8),unaff_retaddr);
      local_c = (undefined1 *)((float)local_c + *(float *)(this + 0x164));
      fVar6 = (float)((int)fVar6 + 1);
      local_8 = local_8 + *(float *)(this + 0x168);
    } while ((uint)fVar6 < (uint)param_1);
  }
  if ((uint)fVar6 < (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3)) {
    local_2c = (undefined1 *)&local_28;
    (**(code **)(**(int **)(this + 0x174) + 0x40))();
    UnhideComponent(this,*(undefined4 *)(this + 0x174),0,puVar5);
    this[0x179] = (CScrollableList)0x1;
  }
  else {
    afStack_24[0] = 0.0;
    ppuVar7 = &local_2c;
    (**(code **)(**(int **)(this + 0x174) + 0x40))(&local_2c);
    HideComponent(this,*(undefined4 *)(this + 0x174),0,puVar5,ppuVar7);
    this[0x179] = (CScrollableList)0x0;
  }
  if ((uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3) <= (uint)fVar6) {
    *(undefined4 *)(this + 0x16c) = uStack_4;
    return;
  }
  do {
    local_28 = (undefined1 *)0x0;
    local_2c = local_c;
    HideComponent(this,*(undefined4 *)(*(int *)(this + 0xb0) + (int)fVar6 * 8),unaff_retaddr,puVar5,
                  iVar2 + iVar3);
    fVar6 = (float)((int)fVar6 + 1);
  } while ((uint)fVar6 < (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3));
  *(undefined4 *)(this + 0x16c) = uStack_4;
  return;
}


// CALLEES of UpdateVisibleList:
//   005556f0  UnhideComponent
//   00555440  HideComponent

//=== OnActivate @ 00556200 (seed 00556200) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall NUISystem::CScrollableList::OnActivate(void) */

void __thiscall NUISystem::CScrollableList::OnActivate(CScrollableList *this)

{
  CScrollableList *pCVar1;
  CFrontEndManager *pCVar2;
  int iVar3;
  
  UpdateVisibleList(this,0.0,0.2);
  pCVar1 = this + 4;
  *(undefined4 *)(this + 0x16c) = 0x3e4ccccd;
  (**(code **)(*(int *)(this + 4) + 0xc))(0x25);
  (**(code **)(*(int *)pCVar1 + 0xc))(0x24);
  (**(code **)(*(int *)pCVar1 + 0xc))(4);
  pCVar2 = CFrontEndManager::GetInstance();
  (**(code **)(*(int *)pCVar2 + 8))(pCVar1);
  if (*(int *)(this + 0x170) == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = *(int *)(this + 0x170) + 4;
  }
  pCVar2 = CFrontEndManager::GetInstance();
  (**(code **)(*(int *)pCVar2 + 0x14))(iVar3);
  iVar3 = *(int *)(this + 0x174);
  if (iVar3 != 0) {
    pCVar2 = CFrontEndManager::GetInstance();
    (**(code **)(*(int *)pCVar2 + 0x14))(iVar3 + 4);
    return;
  }
  pCVar2 = CFrontEndManager::GetInstance();
  (**(code **)(*(int *)pCVar2 + 0x14))(0);
  return;
}


// CALLEES of OnActivate:
//   00555a90  UpdateVisibleList
//   0041e5f2  GetInstance

//=== ProcessEvent @ 00557eb0 (seed 00557eb0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall NUISystem::CKeyRedefiner::ProcessEvent(enum NUISystem::EEvent) */

void __thiscall NUISystem::CKeyRedefiner::ProcessEvent(CKeyRedefiner *this,EEvent param_1)

{
  EHeroAbility EVar1;
  EUISpellCategory EVar2;
  CFrontEndManager *pCVar3;
  undefined4 extraout_EDX;
  CKeyRedefiner *this_00;
  undefined4 local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  local_14 = 0;
  local_10 = 0;
  local_c = 0;
  local_1c = *(undefined4 *)(**(int **)(this + 0x198) + 8);
  local_8 = *(undefined4 *)(this + 0x1b4);
  local_4 = *(undefined4 *)(this + 0x1b8);
  local_18 = 0;
  if (DAT_013b8ac8 == this + -4) {
    switch(param_1) {
    case 0x1a:
      local_18 = 3;
      local_c = 1;
      break;
    case 0x1b:
      local_18 = 3;
      local_c = 2;
      break;
    default:
      goto switchD_00557f24_caseD_1c;
    case 0x21:
      pCVar3 = CFrontEndManager::GetInstance();
      EVar1 = (**(code **)(*(int *)pCVar3 + 0x34))();
      if ((EVar1 & 0xffff) == 1) {
        CancelSelection(this + -4);
        return;
      }
      EVar2 = CTCInventoryAbilities::GetCategory
                        ((CTCInventoryAbilities *)(this + -4),EVar1 & 0xffff);
      if ((char)EVar2 == '\0') {
        if (local_18 == 0) goto switchD_00557f24_caseD_1c;
      }
      else {
        local_18 = 2;
        local_14 = extraout_EDX;
      }
      break;
    case 0x23:
      local_18 = 3;
      local_c = 3;
      break;
    case 0x26:
      local_18 = 3;
      local_c = 8;
      break;
    case 0x27:
      local_18 = 3;
      local_c = 9;
      break;
    case 0x28:
      local_18 = 3;
      local_c = 10;
      break;
    case 0x29:
      local_18 = 3;
      local_c = 0xb;
      break;
    case 0x2a:
      local_18 = 3;
      local_c = 0xc;
    }
    this_00 = this + -4;
    Redefine(this_00,(CActionInputControl *)&local_1c,true);
    if (DAT_013b8ac8 == this_00) {
      DAT_013b8ac8 = (CKeyRedefiner *)0x0;
      UpdateKeyText(this_00);
      (**(code **)(*(int *)this + 0x10))(0x21);
      (**(code **)(*(int *)this + 0xc))(0x19);
      pCVar3 = CFrontEndManager::GetInstance();
      (**(code **)(*(int *)pCVar3 + 0x10))(this);
      return;
    }
  }
  else {
switchD_00557f24_caseD_1c:
    CClickable::ProcessEvent((CClickable *)this,param_1);
  }
  return;
}


// CALLEES of ProcessEvent:
//   0055ad60  ProcessEvent
//   00557a10  UpdateKeyText
//   00557bd0  CancelSelection
//   0041e5f2  GetInstance
//   00557960  GetCategory
//   00557d20  Redefine

//=== ProcessEvent @ 0055ad60 (seed 0055ad60) ===

/* [bsim sim=0.7093822203134075 <- ego_r]
   public: virtual void __thiscall NUISystem::CClickable::ProcessEvent(enum NUISystem::EEvent) */

void __thiscall NUISystem::CClickable::ProcessEvent(CClickable *this,EEvent param_1)

{
  undefined4 uVar1;
  
  switch(param_1) {
  case 0x1a:
    if (this[0x15c] != (CClickable)0x0) {
      (**(code **)(*(int *)(this + -4) + 0x248))();
      *(undefined4 *)(this + 0x18c) = *(undefined4 *)(this + 0x2c);
      this[0x16c] = (CClickable)0x1;
      CHoverable::ProcessEvent((CHoverable *)this,param_1);
      return;
    }
    goto LAB_0055ae3d;
  case 0x1b:
    if (this[0x15c] != (CClickable)0x0) {
      (**(code **)(*(int *)(this + -4) + 0x250))();
      this[0x180] = (CClickable)0x1;
    }
    break;
  case 0x1c:
    if (this[0x16c] != (CClickable)0x0) {
      (**(code **)(*(int *)(this + -4) + 0x24c))();
      this[0x16c] = (CClickable)0x0;
    }
    break;
  case 0x1d:
    if (this[0x180] != (CClickable)0x0) {
      (**(code **)(*(int *)(this + -4) + 0x254))();
      this[0x180] = (CClickable)0x0;
    }
    break;
  default:
    goto switchD_0055ad74_caseD_1e;
  case 0x1f:
    if (*(float *)(this + 0x2c) - *(float *)(this + 0x18c) < *(float *)(this + 0x188))
    goto switchD_0055ad74_caseD_1e;
    if (this[0x16c] == (CClickable)0x0) goto LAB_0055ae3d;
    uVar1 = *(undefined4 *)(this + 0x174);
    goto LAB_0055ae30;
  case 0x20:
    if (this[0x180] == (CClickable)0x0) goto LAB_0055ae3d;
    uVar1 = *(undefined4 *)(this + 0x184);
LAB_0055ae30:
    (**(code **)(*(int *)(this + -4) + 0x20c))(uVar1);
LAB_0055ae3d:
    *(undefined4 *)(this + 0x18c) = *(undefined4 *)(this + 0x2c);
    CHoverable::ProcessEvent((CHoverable *)this,param_1);
    return;
  }
  *(undefined4 *)(this + 0x18c) = *(undefined4 *)(this + 0x2c);
switchD_0055ad74_caseD_1e:
  CHoverable::ProcessEvent((CHoverable *)this,param_1);
  return;
}


// CALLEES of ProcessEvent:
//   0055b9d0  ProcessEvent

