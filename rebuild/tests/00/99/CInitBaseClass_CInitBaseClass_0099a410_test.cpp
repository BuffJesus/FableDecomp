#include <cstdio>

int g_initCount = 5;

struct CInitBaseClass
{
    void* m_vtable;
    char  m_registered;
    void dtor_CInitBaseClass();
};

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

int main()
{
    // Registered instance: dtor deregisters (count--), clears flag, leaves base vtable.
    {
        CInitBaseClass a;
        a.m_vtable = 0;
        a.m_registered = 1;
        a.dtor_CInitBaseClass();
        if (g_initCount != 4)           { printf("FAIL count=%d\n", g_initCount); return 1; }
        if (a.m_registered != 0)        { printf("FAIL flag\n"); return 1; }
        if (a.m_vtable != VT_BASE)      { printf("FAIL vt\n"); return 1; }
    }
    // Unregistered instance: no deregister, base vtable still stored.
    {
        g_initCount = 9;
        CInitBaseClass b;
        b.m_vtable = 0;
        b.m_registered = 0;
        b.dtor_CInitBaseClass();
        if (g_initCount != 9)           { printf("FAIL noreg=%d\n", g_initCount); return 1; }
        if (b.m_registered != 0)        { printf("FAIL flag2\n"); return 1; }
        if (b.m_vtable != VT_BASE)      { printf("FAIL vt2\n"); return 1; }
    }
    printf("PASS_INITBASE_DTOR\n");
    return 0;
}