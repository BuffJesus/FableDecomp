#include <stdio.h>
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* GetPItem();
};
void* CBaseIntelligentPointer::GetPItem() { return m_pItem; }

struct CParticleMorphsHolder {
    char m_leading[0x100];
    CBaseIntelligentPointer m_item;
};

struct CParticleMorphsAccessor {
    char m_leading[0x40];
    CParticleMorphsHolder* m_pHolder;
    void* GetMorphEntryItem();
};
void* CParticleMorphsAccessor::GetMorphEntryItem() { return m_pHolder->m_item.GetPItem(); }

int main() {
    CParticleMorphsHolder holder;
    holder.m_item.m_pItem = (void*)0x1234;
    CParticleMorphsAccessor accessor;
    accessor.m_pHolder = &holder;
    if (accessor.GetMorphEntryItem() != (void*)0x1234) return 1;
    printf("GET_MORPH_ENTRY_ITEM PASS\n");
    return 0;
}