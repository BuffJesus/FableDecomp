//=== PlayAttachedSample @ 0066b300 ===

/* [bsim sim=0.5145951607009488 <- ego_r]
   public: unsigned long __thiscall CTCSoundPlayer::PlayAttachedSample(class CASoundBank &,class
   CSoundPair const &,class CSoundInit const &,bool,bool,bool) */

ulong __thiscall
CTCSoundPlayer::PlayAttachedSample
          (CTCSoundPlayer *this,CASoundBank *param_1,CSoundPair *param_2,CSoundInit *param_3,
          bool param_4,bool param_5,bool param_6)

{
  CDisplayEngine *pCVar1;
  uint uVar2;
  ulong uVar3;
  uint uVar4;
  uint uVar5;
  undefined3 in_stack_00000011;
  undefined3 in_stack_00000015;
  undefined3 in_stack_00000019;
  
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  uVar3 = *(int *)(pCVar1 + 0x16140) * 0x24a1 + 0x24df;
  uVar5 = (*(int *)(param_2 + 4) - *(int *)param_2) + 1;
  *(ulong *)(pCVar1 + 0x16140) = uVar3;
  uVar2 = GFROR13(uVar3);
  uVar4 = 0;
  *(uint *)(pCVar1 + 0x16140) = uVar2;
  if (uVar5 != 0) {
    uVar4 = uVar2 % uVar5;
  }
  uVar3 = FUN_0066b080(param_1,*(int *)param_2 + uVar4,param_3,_param_4,_param_5,_param_6);
  return uVar3;
}



