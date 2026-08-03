//=== CFrontEndManager @ 005953e2 (seed 005953e2) ===

/* [bsim sim=0.7212874166685309 <- ego_r]
   public: __thiscall NUISystem::CFrontEndManager::CFrontEndManager(void) */

CFrontEndManager * __thiscall NUISystem::CFrontEndManager::CFrontEndManager(CFrontEndManager *this)

{
  uchar unaff_SI;
  undefined4 uStack_4;
  
  uStack_4 = this;
  CFrontEndManager__Initialize();
  *(undefined ***)this = &PTR__vector_deleting_destructor__012521a8;
  CCharString::CCharString((CCharString *)(this + 0x14));
  Std_DoubleLinkedList_CreateNode((int)&uStack_4 + 3);
  CGameEvent::AddUBYTE((CGameEvent *)(this + 0x20),unaff_SI);
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x54));
  *(undefined4 *)(this + 0x60) = 0;
  this[100] = (CFrontEndManager)0x0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 0;
  this[0xa0] = (CFrontEndManager)0x0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0xa8) = 0;
  CCharString::CCharString((CCharString *)(this + 0xac));
  CCharString::CCharString((CCharString *)(this + 0xb0));
  this[0xc0] = (CFrontEndManager)0x0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0xc4));
  this[0xd8] = (CFrontEndManager)0x0;
  this[0xd9] = (CFrontEndManager)0x0;
  return this;
}


// CALLEES of CFrontEndManager:
//   0052d9e0  CFrontEndManager::Initialize
//   0099aed0  CCharString
//   0059b32a  CopyBackBufferToTexture
//   0059b310  AddUBYTE
//   00486055  CopyBackBufferToTexture
//   0042ab01  Std_DoubleLinkedList_CreateNode

//=== GetInstance @ 00595582 (seed 00595582) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: static class NUISystem::CFrontEndManager * __fastcall
   NUISystem::CFrontEndManager::GetInstance(void) */

CFrontEndManager * __fastcall NUISystem::CFrontEndManager::GetInstance(void)

{
  CFrontEndManager *this;
  
  if (DAT_013b8b5c == (CFrontEndManager *)0x0) {
    this = ::operator_new(0xe0);
    if (this == (CFrontEndManager *)0x0) {
      DAT_013b8b5c = (CFrontEndManager *)0x0;
    }
    else {
      DAT_013b8b5c = (CFrontEndManager *)CFrontEndManager(this);
    }
  }
  return DAT_013b8b5c;
}


// CALLEES of GetInstance:
//   005953e2  CFrontEndManager
//   00bfea1a  operator_new

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
//   00bfe9bc  operator_delete
//   005954ad  CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer

//=== 0059b5c2 : NO FUNCTION ===
