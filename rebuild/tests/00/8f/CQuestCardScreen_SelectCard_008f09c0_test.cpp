#include <stdio.h>
struct CQuestCardScreen {
    char m_leading[0x308];
    int m_selectedIndex;
    int m_highlightedIndex;
    bool SelectCard();
};
bool CQuestCardScreen::SelectCard() {
    m_selectedIndex = m_highlightedIndex;
    return true;
}
int main() {
    CQuestCardScreen screen;
    screen.m_selectedIndex = -1;
    screen.m_highlightedIndex = 7;
    if (!screen.SelectCard()) return 1;
    if (screen.m_selectedIndex != 7) return 2;
    printf("SELECT_CARD PASS\n");
    return 0;
}