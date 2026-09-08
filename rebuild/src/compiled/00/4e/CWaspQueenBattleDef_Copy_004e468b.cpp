struct CDefClassBase;
struct CWaspQueenBattleDefRetail {
    unsigned char _retailDefinitionBase[0x3c];
    long NumFramesPauseBetweenAttacks; // Ego_r PDB +0x40; retail base is four bytes smaller.
    void CopyBase(const CDefClassBase* source);
    void Copy(const CDefClassBase* source);
};

void CWaspQueenBattleDefRetail::Copy(const CDefClassBase* source) {
    CopyBase(source);
    const CWaspQueenBattleDefRetail* battle =
        reinterpret_cast<const CWaspQueenBattleDefRetail*>(source);
    NumFramesPauseBetweenAttacks = battle->NumFramesPauseBetweenAttacks;
}