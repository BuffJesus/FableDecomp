struct CTCTrophy {
    char pad[0x54];
    int showInitialiseCount;
    void PrepareTrophy();
    void OnShowTrophyInitialise();
};
void CTCTrophy::OnShowTrophyInitialise() {
    PrepareTrophy();
    ++showInitialiseCount;
}