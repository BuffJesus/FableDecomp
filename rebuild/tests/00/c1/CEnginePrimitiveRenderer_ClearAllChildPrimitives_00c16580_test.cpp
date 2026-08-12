#include <cstdio>

struct IChild {
    virtual void slot0() = 0;
};

struct CEnginePrimitiveHandle;

struct CEnginePrimitiveRenderer {
    void*   m_vtbl;
    int     m_pad4;
    IChild* m_child;
    void ClearAllChildPrimitives();
};

void CEnginePrimitiveRenderer::ClearAllChildPrimitives()
{
    IChild* c = m_child;
    m_vtbl = (void*)0x12b2ab0;
    if (c) {
        c->slot0();
        m_child = 0;
    }
}

static int g_called = 0;
struct RealChild : public IChild {
    virtual void slot0() { g_called++; }
};

int main()
{
    // case 1: child present -> slot0 called, m_child cleared, vtbl set
    RealChild rc;
    CEnginePrimitiveRenderer r;
    r.m_vtbl = 0;
    r.m_pad4 = 0;
    r.m_child = &rc;
    r.ClearAllChildPrimitives();
    bool ok1 = (g_called == 1) && (r.m_child == 0) && (r.m_vtbl == (void*)0x12b2ab0);

    // case 2: no child -> no call, vtbl still set
    CEnginePrimitiveRenderer r2;
    r2.m_vtbl = 0;
    r2.m_pad4 = 0;
    r2.m_child = 0;
    r2.ClearAllChildPrimitives();
    bool ok2 = (g_called == 1) && (r2.m_vtbl == (void*)0x12b2ab0);

    if (ok1 && ok2) {
        printf("CLEARCHILD_OK\n");
        return 0;
    }
    printf("CLEARCHILD_FAIL c=%d\n", g_called);
    return 1;
}