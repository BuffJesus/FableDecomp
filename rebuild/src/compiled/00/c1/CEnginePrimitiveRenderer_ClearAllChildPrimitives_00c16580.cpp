// CEnginePrimitiveRenderer::ClearAllChildPrimitives  @ 0x00c16580
// void __fastcall(this)  -- ends in plain 'ret' (no stack cleanup)

struct IChild {
    virtual void slot0() = 0;
};

struct CEnginePrimitiveRenderer {
    void*   m_vtbl;   // +0x00
    int     m_pad4;   // +0x04
    IChild* m_child;  // +0x08

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