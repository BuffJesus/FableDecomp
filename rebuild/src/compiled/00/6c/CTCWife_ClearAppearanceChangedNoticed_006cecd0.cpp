// Retail 0x006CECD0. The donor places FrameNoticedChangeInAppearance at
// +0xAC; TLC retail's smaller base places the same 16-entry PDB-named array at +0xA4.
enum EWifeAppearanceCategories {
    WIFE_APPEARANCE_CATEGORY_FIRST = 0
};
struct CTCWifeRetail {
    char m_leading[0xA4];
    long FrameNoticedChangeInAppearance[16];
    void ClearAppearanceChangedNoticed(const EWifeAppearanceCategories& category);
};
void CTCWifeRetail::ClearAppearanceChangedNoticed(
    const EWifeAppearanceCategories& category) {
    FrameNoticedChangeInAppearance[category] = 0;
}