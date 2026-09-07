#include <stdio.h>
struct CTargetingFlags {
    char m_leading[0x0C];
    unsigned int m_targetingFlags;
    void AddTargetingFlag(unsigned int flag);
};
void CTargetingFlags::AddTargetingFlag(unsigned int flag) {
    m_targetingFlags = m_targetingFlags | flag;
}
int main() {
    CTargetingFlags flags;
    flags.m_targetingFlags = 0x01;
    flags.AddTargetingFlag(0x04);
    if (flags.m_targetingFlags != 0x05) return 1;
    flags.AddTargetingFlag(0x04);
    if (flags.m_targetingFlags != 0x05) return 2;
    printf("ADD_TARGETING_FLAG PASS\n");
    return 0;
}