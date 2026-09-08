struct CDefClassBase;
struct CThingGroupDefRetail {
    unsigned char _retailDefinitionBase[0x39];
    bool Dummy; // Ego_r PDB +0x3d; retail definition base is four bytes smaller.
    void CopyBase(const CDefClassBase* source);
    void Copy(const CDefClassBase* source);
};

void CThingGroupDefRetail::Copy(const CDefClassBase* source) {
    CopyBase(source);
    const CThingGroupDefRetail* group =
        reinterpret_cast<const CThingGroupDefRetail*>(source);
    Dummy = group->Dummy;
}