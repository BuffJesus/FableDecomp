#include <stdio.h>
struct CTCRandomAppearanceMorph {
    char m_leading[0x18];
    void* m_pInterface;
    int updates;
    void UpdateMorphSet();
    void AddToInterface(void* pInterface);
};
void CTCRandomAppearanceMorph::UpdateMorphSet() { updates++; }
void CTCRandomAppearanceMorph::AddToInterface(void* pInterface) {
    m_pInterface = pInterface;
    UpdateMorphSet();
}

int main() {
    CTCRandomAppearanceMorph morph;
    morph.m_pInterface = 0;
    morph.updates = 0;
    morph.AddToInterface((void*)0x5678);
    if (morph.m_pInterface != (void*)0x5678) return 1;
    if (morph.updates != 1) return 2;
    printf("ADD_TO_INTERFACE PASS\n");
    return 0;
}