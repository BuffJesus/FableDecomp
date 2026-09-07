// Retail 0x007CBEE0. Manifest/BSim labels this row `_Dest_val<...>`; the body is a
// property-change notification forwarded to the owner object held at +4.
// 0x004C9840 is CBaseObject::OnPropertyChanged.
struct CBaseObject {
    void OnPropertyChanged(int propertyId);
};

struct CFlashPropertySender136 {
    void* m_pUnknown0;
    CBaseObject* m_pOwner;
    void NotifyOwnerProperty136();
};

void CFlashPropertySender136::NotifyOwnerProperty136() {
    m_pOwner->OnPropertyChanged(136);
}