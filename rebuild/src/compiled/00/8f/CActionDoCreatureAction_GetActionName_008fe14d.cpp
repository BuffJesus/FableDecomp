// Byte-exact reconstruction of CActionDoCreatureAction::GetActionName @ 0x008fe14d
//
// Retail:
//   push esi; push -1; push 0x128a268; mov esi,ecx; call 0x99ebf0;
//   mov eax,esi; pop esi; ret
//
// The callee at 0x99ebf0 is a __fastcall member (ecx=this, two stack args
// key/idx pushed right-to-left). GetActionName saves 'this' in esi across the
// call and returns it (eax=this). Model the callee as a real C++ member method
// (native __fastcall: ecx=this + stack args), and return self.

struct CCharString { void* p; };

struct CActionDoCreatureAction {
    int dummy;
    // Native member => ecx=this, key & idx pushed on the stack right-to-left.
    void LookupActionName(const char* key, int idx);
};

static const char* const kActionNameKey = (const char* const)0x128a268;

CCharString __fastcall GetActionName(CActionDoCreatureAction* self)
{
    self->LookupActionName(kActionNameKey, -1);
    CCharString r;
    r.p = self;   // eax = this
    return r;
}