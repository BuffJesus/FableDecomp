// Retail 0x008F09C0. Commits the highlighted index at +0x30C into the selected
// index at +0x308 and unconditionally reports success.
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