struct CTCCreatureOpinionOfHero {
    char pad[0x33c];
    int m_opinionValue;   // +0x33c
    char pad2[0x342 - 0x33c - 4];
    char m_isEnemy;       // +0x342

    void SetHeroOpinionEnemy(char bEnemy);
};

void CTCCreatureOpinionOfHero::SetHeroOpinionEnemy(char bEnemy)
{
    this->m_isEnemy = bEnemy;
    this->m_opinionValue = bEnemy ? 3 : 18;
}