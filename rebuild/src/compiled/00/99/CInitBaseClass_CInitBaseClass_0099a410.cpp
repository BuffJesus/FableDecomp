// ~CInitBaseClass 0x0099a410
// Virtual destructor: stores derived vtable, conditionally deregisters, stores base vtable.
// Modeled as a struct method so ecx=self (__fastcall). The two vtable pointers and the
// global registration counter are the module's absolute addresses (relocated at link time).

extern int g_initCount; // 0x013bca14 (extern symbol -> relocated, folds to dec [mem])

struct CInitBaseClass
{
    void* m_vtable;       // +0
    char  m_registered;   // +4
    void dtor_CInitBaseClass();
};

// Absolute vtable addresses from the retail image (immediates in the store).
#define VT_DERIVED ((void*)0x0129a7cc)
#define VT_BASE    ((void*)0x01231710)

void CInitBaseClass::dtor_CInitBaseClass()
{
    m_vtable = VT_DERIVED;
    if (m_registered == 1)
    {
        --g_initCount;
        m_registered = 0;
    }
    m_vtable = VT_BASE;
}