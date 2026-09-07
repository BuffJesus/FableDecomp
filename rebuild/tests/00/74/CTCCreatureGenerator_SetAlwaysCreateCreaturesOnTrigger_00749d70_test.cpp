#include <stdio.h>
struct CTCCreatureGenerator {
    char m_leading[0x50];
    bool m_bSuppressCreateOnTrigger;
    void SetAlwaysCreateCreaturesOnTrigger(bool bAlways);
};
void CTCCreatureGenerator::SetAlwaysCreateCreaturesOnTrigger(bool bAlways) {
    m_bSuppressCreateOnTrigger = !bAlways;
}
int main() {
    CTCCreatureGenerator gen;
    gen.SetAlwaysCreateCreaturesOnTrigger(true);
    if (gen.m_bSuppressCreateOnTrigger) return 1;
    gen.SetAlwaysCreateCreaturesOnTrigger(false);
    if (!gen.m_bSuppressCreateOnTrigger) return 2;
    printf("SET_ALWAYS_CREATE PASS\n");
    return 0;
}