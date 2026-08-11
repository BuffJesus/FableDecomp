// Retail ~CInputProcessManager @ 009a0b00
// Derived-class destructor: set vtable, destroy member at +8, tail-call base dtor.

struct MemberAt8 {
    void destroy();   // external non-inline (stands for 0x99eae0)
};

struct BaseInput {
    void baseDtor();  // external non-inline (stands for 0x99a300)
};

struct CInputProcessManager {
    void* vtbl;       // +0
    int   pad;        // +4
    MemberAt8 member; // +8
    void dtor_CInputProcessManager();
};

extern const int g_CInputProcessManager_vtbl; // its address is 0x129ab38 at link time

void CInputProcessManager::dtor_CInputProcessManager()
{
    this->vtbl = (void*)&g_CInputProcessManager_vtbl; // mov [esi], imm32
    this->member.destroy();                            // lea ecx,[esi+8]; call
    ((BaseInput*)this)->baseDtor();                    // mov ecx,esi; jmp (tail)
}