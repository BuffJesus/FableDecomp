struct CTCTurncoatSpell {
    char pad[0x3a]; unsigned char b3a; char pad2[0x11]; long f4c;
    void SetAICast(long v);
};
void CTCTurncoatSpell::SetAICast(long v){ this->b3a = 1; this->f4c = v; }