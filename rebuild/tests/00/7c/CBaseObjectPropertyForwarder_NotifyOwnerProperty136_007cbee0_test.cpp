#include <stdio.h>
struct CBaseObject {
    int lastProperty;
    void OnPropertyChanged(int propertyId);
};
void CBaseObject::OnPropertyChanged(int propertyId) { lastProperty = propertyId; }

struct CFlashPropertySender136 {
    void* m_pUnknown0;
    CBaseObject* m_pOwner;
    void NotifyOwnerProperty136();
};
void CFlashPropertySender136::NotifyOwnerProperty136() { m_pOwner->OnPropertyChanged(136); }

int main() {
    CBaseObject owner;
    owner.lastProperty = -1;
    CFlashPropertySender136 sender;
    sender.m_pUnknown0 = 0;
    sender.m_pOwner = &owner;
    sender.NotifyOwnerProperty136();
    if (owner.lastProperty != 136) return 1;
    printf("NOTIFY_PROPERTY_136 PASS\n");
    return 0;
}