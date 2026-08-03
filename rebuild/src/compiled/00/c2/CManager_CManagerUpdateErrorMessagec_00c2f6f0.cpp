struct IErrorNotifierVtbl
{
    void* slot0;
    void* slot1;
    void* slot2;
    void (__fastcall *NotifyOwner)(void* self, void* edx, void* owner);
};

struct IErrorNotifier
{
    IErrorNotifierVtbl* vt;
};

class CManager
{
public:
    void* field0;
    IErrorNotifier* notifier;
};

extern "C" void __fastcall CManager_UpdateErrorMessage_c(CManager* this_)
{
    IErrorNotifier* notifier = this_->notifier;
    notifier->vt->NotifyOwner(notifier, notifier->vt, this_);
}