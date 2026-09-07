#include <stdio.h>
struct CBaseObject {
    int lastProperty;
    void OnPropertyChanged(int propertyId);
};
void CBaseObject::OnPropertyChanged(int propertyId) { lastProperty = propertyId; }

struct CFlashPropertySender150 {
    void* m_pUnknown0;
    CBaseObject* m_pOwner;
    void NotifyOwnerProperty150();
};
void CFlashPropertySender150::NotifyOwnerProperty150() { m_pOwner->OnPropertyChanged(150); }

int main() {
    CBaseObject owner;
    owner.lastProperty = -1;
    CFlashPropertySender150 sender;
    sender.m_pUnknown0 = 0;
    sender.m_pOwner = &owner;
    sender.NotifyOwnerProperty150();
    if (owner.lastProperty != 150) return 1;
    printf("NOTIFY_PROPERTY_150 PASS\n");
    return 0;
}