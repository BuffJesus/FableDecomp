//=== `vector_deleting_destructor' @ 0059b641 (seed 0059b641) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void * __thiscall CLandscapeBackgroundPatch::`vector deleting
   destructor'(unsigned int) */

void * __thiscall
CLandscapeBackgroundPatch::_vector_deleting_destructor_
          (CLandscapeBackgroundPatch *this,uint param_1)

{
  CEngineScreenEffectDisplacementRenderer___CEngineScreenEffectDisplacementRenderer();
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// CALLEES of `vector_deleting_destructor':
//   005954ad  CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer
//   00bfe9bc  operator_delete

//=== 0059b5c2 : NO FUNCTION ===
//=== 0052d900 : NO FUNCTION ===
//=== ObserveEvent @ 0052da20 (seed 0052da20) ===

/* [bsim sim=0.765686397889247 <- ego_r]
   public: virtual void __thiscall NUISystem::CObserver::ObserveEvent(enum NUISystem::EEvent) */

void __thiscall NUISystem::CObserver::ObserveEvent(CObserver *this,EEvent param_1)

{
  int iVar1;
  int local_c;
  undefined1 local_8 [8];
  
  BinaryTree_LowerBound_Int(&local_c,&param_1);
  iVar1 = *(int *)(this + 4);
  CRedBlackTree__Insert(local_8,&param_1);
  if ((param_1 == 0x19) && (local_c == iVar1)) {
    (**(code **)(*(int *)this + 4))(0x19);
  }
  return;
}


// CALLEES of ObserveEvent:
//   0052e230  CRedBlackTree::Insert
//   0052df20  BinaryTree_LowerBound_Int

//=== 0052d940 : NO FUNCTION ===
//=== 0052d9a0 : NO FUNCTION ===
//=== ObserveAllEvents @ 0052d7b0 (seed 0052d7b0) ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall NUISystem::CObserver::ObserveAllEvents(void) */

void __thiscall NUISystem::CObserver::ObserveAllEvents(CObserver *this)

{
  (**(code **)(*(int *)this + 0xc))(0);
  (**(code **)(*(int *)this + 0xc))(1);
  (**(code **)(*(int *)this + 0xc))(2);
  (**(code **)(*(int *)this + 0xc))(3);
  (**(code **)(*(int *)this + 0xc))(4);
  (**(code **)(*(int *)this + 0xc))(5);
  (**(code **)(*(int *)this + 0xc))(6);
  (**(code **)(*(int *)this + 0xc))(7);
  (**(code **)(*(int *)this + 0xc))(8);
  (**(code **)(*(int *)this + 0xc))(9);
  (**(code **)(*(int *)this + 0xc))(10);
  (**(code **)(*(int *)this + 0xc))(0xb);
  (**(code **)(*(int *)this + 0xc))(0xc);
  (**(code **)(*(int *)this + 0xc))(0xd);
  (**(code **)(*(int *)this + 0xc))(0xe);
  (**(code **)(*(int *)this + 0xc))(0xf);
  (**(code **)(*(int *)this + 0xc))(0x10);
  (**(code **)(*(int *)this + 0xc))(0x11);
  (**(code **)(*(int *)this + 0xc))(0x12);
  (**(code **)(*(int *)this + 0xc))(0x13);
  (**(code **)(*(int *)this + 0xc))(0x14);
  (**(code **)(*(int *)this + 0xc))(0x15);
  (**(code **)(*(int *)this + 0xc))(0x16);
  (**(code **)(*(int *)this + 0xc))(0x17);
  (**(code **)(*(int *)this + 0xc))(0x18);
  (**(code **)(*(int *)this + 0xc))(0x19);
  (**(code **)(*(int *)this + 0xc))(0x1a);
  (**(code **)(*(int *)this + 0xc))(0x1b);
  (**(code **)(*(int *)this + 0xc))(0x1c);
  (**(code **)(*(int *)this + 0xc))(0x1d);
  (**(code **)(*(int *)this + 0xc))(0x1e);
  (**(code **)(*(int *)this + 0xc))(0x1f);
  (**(code **)(*(int *)this + 0xc))(0x20);
  (**(code **)(*(int *)this + 0xc))(0x21);
  (**(code **)(*(int *)this + 0xc))(0x24);
  (**(code **)(*(int *)this + 0xc))(0x25);
  return;
}


// CALLEES of ObserveAllEvents:

//=== 0041c580 : NO FUNCTION ===
