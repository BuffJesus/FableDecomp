#include <cstdio>

struct CTCCreatureOpinionOfHero {
    char pad[0x33c];
    int m_opinionValue;   // +0x33c
    char pad2[0x342 - 0x33c - 4];
    char m_isEnemy;       // +0x342

    void SetHeroOpinionEnemy(char bEnemy);
};

int main()
{
    CTCCreatureOpinionOfHero o;
    o.SetHeroOpinionEnemy(1);
    if (o.m_isEnemy != 1 || o.m_opinionValue != 3) { printf("FAIL1\n"); return 1; }
    o.SetHeroOpinionEnemy(0);
    if (o.m_isEnemy != 0 || o.m_opinionValue != 18) { printf("FAIL2\n"); return 1; }
    printf("OPINION_ENEMY_OK\n");
    return 0;
}