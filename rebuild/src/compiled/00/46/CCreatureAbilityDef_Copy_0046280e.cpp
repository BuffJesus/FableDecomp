struct CDefClassBase;
struct CCreatureAbilityDefRetail {
    unsigned char _retailDefinitionBase[0x3c];
    long Type; // Ego_r PDB +0x40; retail definition base is four bytes smaller.
    void CopyBase(const CDefClassBase* source);
    void Copy(const CDefClassBase* source);
};

void CCreatureAbilityDefRetail::Copy(const CDefClassBase* source) {
    CopyBase(source);
    const CCreatureAbilityDefRetail* ability =
        reinterpret_cast<const CCreatureAbilityDefRetail*>(source);
    Type = ability->Type;
}