// Retail 0x00749D70. The stored byte is the INVERSE of the argument
// (`sete` after `test al,al`), so the field at +0x50 is the suppression flag.
struct CTCCreatureGenerator {
    char m_leading[0x50];
    bool m_bSuppressCreateOnTrigger;
    void SetAlwaysCreateCreaturesOnTrigger(bool bAlways);
};

void CTCCreatureGenerator::SetAlwaysCreateCreaturesOnTrigger(bool bAlways) {
    m_bSuppressCreateOnTrigger = !bAlways;
}