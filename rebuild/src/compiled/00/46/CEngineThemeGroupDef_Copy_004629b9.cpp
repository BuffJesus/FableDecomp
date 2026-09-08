struct CDefClassBase;
struct CEngineThemeGroupDefRetail {
    unsigned char _retailDefinitionBase[0x39];
    bool Dummy; // Ego_r PDB +0x3d; retail definition base is four bytes smaller.
    void CopyBase(const CDefClassBase* source);
    void Copy(const CDefClassBase* source);
};

void CEngineThemeGroupDefRetail::Copy(const CDefClassBase* source) {
    CopyBase(source);
    const CEngineThemeGroupDefRetail* themeGroup =
        reinterpret_cast<const CEngineThemeGroupDefRetail*>(source);
    Dummy = themeGroup->Dummy;
}