// Retail 0x007A5420. Manifest/BSim labels this row `_Dest_val<...>`; the body is a
// property-change notification forwarded to the owner object held at +4.
// 0x004C9840 is CBaseObject::OnPropertyChanged.
struct CBaseObject {
    void OnPropertyChanged(int propertyId);
};

struct CFlashPropertySender150 {
    void* m_pUnknown0;
    CBaseObject* m_pOwner;
    void NotifyOwnerProperty150();
};

void CFlashPropertySender150::NotifyOwnerProperty150() {
    m_pOwner->OnPropertyChanged(150);
}