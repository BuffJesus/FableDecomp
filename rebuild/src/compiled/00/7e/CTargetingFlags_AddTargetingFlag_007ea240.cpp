// Retail 0x007EA240. Load / or / store against the flag word at +0x0C.
struct CTargetingFlags {
    char m_leading[0x0C];
    unsigned int m_targetingFlags;
    void AddTargetingFlag(unsigned int flag);
};

void CTargetingFlags::AddTargetingFlag(unsigned int flag) {
    m_targetingFlags = m_targetingFlags | flag;
}