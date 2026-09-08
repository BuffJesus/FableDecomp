// Retail 0x00A8CA00. Relocation is allowed exactly when the byte at +0xE5 is clear.
struct C3DMesh2 {
    char m_leading[0xE5];
    bool m_bRelocationBlocked;
    bool CanRelocateData(long) const;
};

bool C3DMesh2::CanRelocateData(long) const {
    return !m_bRelocationBlocked;
}