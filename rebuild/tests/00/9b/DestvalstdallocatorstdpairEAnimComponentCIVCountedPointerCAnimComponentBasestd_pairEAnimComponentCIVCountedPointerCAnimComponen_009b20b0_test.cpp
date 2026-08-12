#include <stdio.h>

struct CAnimComponentBase
{
    virtual void v0() = 0;
    virtual void Release() = 0;
    int m_refCount;
};

struct DestPair
{
    int          m_first;
    CAnimComponentBase* m_ptr;
    void _Dest_val();
};

void DestPair::_Dest_val()
{
    CAnimComponentBase* p = m_ptr;
    if (p != 0)
    {
        if (--p->m_refCount == 0)
        {
            p->Release();
        }
        m_ptr = 0;
    }
}

static int g_released = 0;

struct Impl : public CAnimComponentBase
{
    virtual void v0() {}
    virtual void Release() { g_released++; }
};

int main()
{
    // Case 1: refcount 2 -> decremented to 1, no release, ptr nulled.
    Impl a; a.m_refCount = 2;
    DestPair p1; p1.m_first = 7; p1.m_ptr = &a;
    p1._Dest_val();
    bool ok1 = (a.m_refCount == 1) && (g_released == 0) && (p1.m_ptr == 0);

    // Case 2: refcount 1 -> decremented to 0, release called, ptr nulled.
    Impl b; b.m_refCount = 1;
    DestPair p2; p2.m_first = 9; p2.m_ptr = &b;
    p2._Dest_val();
    bool ok2 = (b.m_refCount == 0) && (g_released == 1) && (p2.m_ptr == 0);

    // Case 3: null ptr -> no crash, stays null.
    DestPair p3; p3.m_first = 3; p3.m_ptr = 0;
    p3._Dest_val();
    bool ok3 = (p3.m_ptr == 0) && (g_released == 1);

    if (ok1 && ok2 && ok3)
    {
        printf("DESTVAL_OK\n");
        return 0;
    }
    printf("DESTVAL_FAIL\n");
    return 1;
}