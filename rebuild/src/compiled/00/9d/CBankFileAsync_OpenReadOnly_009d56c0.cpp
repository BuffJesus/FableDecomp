#include "fable_ui_bank_open.h"
bool __fastcall FableUiOpenAsyncBankReadOnly(FableUiBankFileAsyncView* bank,void*,const FableUiStringValue* name,unsigned flags)
{
    if(!FableUiOpenBankReadOnly(&bank->Base,0,name,flags)) return false;
    FableUiStringValue progress;
    FableUiConstructBankName(&progress,0,"Open Bank File Async",-1);
    FableUiStartBankProgress(&progress,false,-1.0f,false);
    FableUiDestroyBankName(&progress,0);
    if(bank->Base.RetailMode)
    {
        FableUiRegisteredBankHeader header={}; FableUiBankReference found={};
        if(!FableUiFindRegisteredBank(&FableUiBankRegistryState,0,name,&header,&found))
        { FableUiReleaseRegisteredBank(&found,0); return false; }
        FableUiBankReference threaded=static_cast<FableUiRegisteredBank*>(found.Data)->ThreadedFile;
        if(threaded.Info) ++static_cast<FableReferenceCount*>(threaded.Info)->owners;
        FableUiAssignThreadedFile(&bank->ThreadedFile,0,&threaded);
        FableUiReleaseThreadedFile(&threaded,0);
        bank->NonCached=static_cast<FableUiRegisteredBank*>(found.Data)->NonCached;
        bank->DisableAsync=false;
        FableUiReleaseRegisteredBank(&found,0);
        return true;
    }
    CThreadedFile* file=static_cast<CThreadedFile*>(FableUiAllocateGraphicsBank(28));
    FableUiResetThreadedFile(&bank->ThreadedFile,0,file ? FableUiConstructThreadedFile(file,0) : 0);
    file=static_cast<CThreadedFile*>(bank->ThreadedFile.Data);
    bool nonCached=bank->NonCached;
    FableUiWideStringValue path;
    FableUiGetBankPath(&bank->Base,0,&path);
    FableUiOpenThreadedFile(file,0,&path,nonCached);
    FableUiDestroyWideString(&path,0);
    bank->DisableAsync=false;
    return true;
}
