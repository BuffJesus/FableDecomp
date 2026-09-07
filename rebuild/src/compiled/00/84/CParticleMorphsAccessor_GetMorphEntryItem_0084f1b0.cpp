// Retail 0x0084F1B0. Manifest/BSim labels this row `_Cons_val<std::allocator<...>>`;
// the body reads the object held at +0x40, steps to the CBaseIntelligentPointer
// subobject at +0x100 inside it, and tail-calls
// CBaseIntelligentPointer::GetPItem (0x00A01B50).
struct CBaseIntelligentPointer {
    void* GetPItem();
};

struct CParticleMorphsHolder {
    char m_leading[0x100];
    CBaseIntelligentPointer m_item;
};

struct CParticleMorphsAccessor {
    char m_leading[0x40];
    CParticleMorphsHolder* m_pHolder;
    void* GetMorphEntryItem();
};

void* CParticleMorphsAccessor::GetMorphEntryItem() {
    return m_pHolder->m_item.GetPItem();
}