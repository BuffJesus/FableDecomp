// Retail 0x009FD3F0. Select the eight-byte blank-texture record for the
// active texture index in the frame's graphics data bank.
struct CTexture { unsigned char m_storage[8]; };
struct CGraphicDataBank {
    char m_leading[0x1C];
    unsigned int m_blankTextureIndex;
    char* m_pTextureStorage;
};
struct CGraphicInfoFrame {
    char m_leading[0x0C];
    CGraphicDataBank* m_pDataBank;
    CTexture* GetBlankTexture() const;
};
CTexture* CGraphicInfoFrame::GetBlankTexture() const {
    return reinterpret_cast<CTexture*>(
        m_pDataBank->m_pTextureStorage + 0x2B4) + m_pDataBank->m_blankTextureIndex;
}