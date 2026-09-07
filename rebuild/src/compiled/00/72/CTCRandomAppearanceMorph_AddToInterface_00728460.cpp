// Retail 0x00728460. Stores the incoming interface pointer at +0x18 and then
// re-runs CTCRandomAppearanceMorph::UpdateMorphSet (0x007280C0).
struct CTCRandomAppearanceMorph {
    char m_leading[0x18];
    void* m_pInterface;
    void UpdateMorphSet();
    void AddToInterface(void* pInterface);
};

void CTCRandomAppearanceMorph::AddToInterface(void* pInterface) {
    m_pInterface = pInterface;
    UpdateMorphSet();
}