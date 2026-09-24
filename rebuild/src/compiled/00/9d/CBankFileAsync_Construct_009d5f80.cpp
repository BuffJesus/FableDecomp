#include "fable_ui_bank_file.h"
void* __fastcall FableUiConstructBankFileBase(void* receiver,void*)
{
    FableUiBankFileAsyncView* bank=static_cast<FableUiBankFileAsyncView*>(receiver);
    FableUiConstructBankFile(&bank->Base,0);
    bank->Base.Vtable=FableUiBankFileAsyncVtable;
    bank->ThreadedFile.Data=0; bank->ThreadedFile.Info=0;
    bank->LoadingMemoryPool.Data=0; bank->LoadingMemoryPool.Info=0;
    bank->MemoryFailureHandler.Data=0; bank->MemoryFailureHandler.Info=0;
    FableUiConstructBankTree(&bank->AsyncDataInstances,20);
    FableUiConstructBankTree(&bank->ReadsToRestart,20);
    bank->DisposalList=0;
    FableUiBankDisposalNode* head=static_cast<FableUiBankDisposalNode*>(FableUiAllocateGraphicsBankNode(16));
    head->Next=head; head->Previous=head;
    bank->DisposalList=head;
    bank->NonCached=false; bank->DisableAsync=false;
    bank->LastRestartingData=0;
    FableUiInitialiseBankCriticalSection(bank->CriticalSection);
    return bank;
}
