#include <cstdio>

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

static void* g_capturedSelf = 0;
static void* g_capturedOwner = 0;

void __fastcall NotifyOwnerStub(void* self, void* edx, void* owner)
{
    g_capturedSelf = self;
    g_capturedOwner = owner;
}

int main()
{
    IErrorNotifierVtbl vt;
    vt.slot0 = 0;
    vt.slot1 = 0;
    vt.slot2 = 0;
    vt.NotifyOwner = NotifyOwnerStub;

    IErrorNotifier helper;
    helper.vt = &vt;

    CManager mgr;
    mgr.field0 = 0;
    mgr.notifier = &helper;

    CManager_UpdateErrorMessage_c(&mgr);

    if (g_capturedSelf == (void*)&helper && g_capturedOwner == (void*)&mgr)
    {
        printf("UEM_FORWARD_OK\n");
    }
    else
    {
        printf("UEM_FORWARD_FAIL\n");
    }
    return 0;
}