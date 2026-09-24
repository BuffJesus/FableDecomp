#include "fable_ui_bank_open.h"
bool __fastcall FableUiOpenBankReadOnly(FableUiBankFileView* bank,void*,const FableUiStringValue* name,unsigned flags)
{
    bank->ReadOnly=true; bank->OpenFlags=flags;
    FableUiAssignString(&bank->BankHandle,0,name);
    bank->ChangeCount=0; bank->FileValid=false; bank->WrittenToFile=false;
    bank->Solid=false; bank->UpdatingFlag=false;
    if(FableUiGraphicsBankOpenMode && !(flags&4))
    {
        bank->RetailMode=true;
        FableUiRegisteredBankHeader header={}; FableUiBankReference found={};
        if(!FableUiFindRegisteredBank(&FableUiBankRegistryState,0,name,&header,&found))
        { FableUiReleaseRegisteredBank(&found,0); return false; }
        FableUiBankOpenVtable* table=static_cast<FableUiBankOpenVtable*>(bank->Vtable);
        if(header.Type!=table->GetType(bank,0))
        { FableUiReleaseRegisteredBank(&found,0); return false; }
        bank->Alignment=header.Alignment;
        FableUiBankReference disk=static_cast<FableUiRegisteredBank*>(found.Data)->DiskFile;
        if(disk.Info) ++static_cast<FableReferenceCount*>(disk.Info)->owners;
        FableUiAssignDiskFile(&bank->BankFile,0,&disk);
        FableUiReleaseDiskFile(&disk,0);
        CFileDataInputStream stream;
        FableUiConstructBankStream(&stream,0,bank->BankFile.Data,0x4000);
        FableUiSeekBankStream(&stream,0,header.Offset);
        FableUiReadBankEntries(bank,0,&stream,header.EntryCount+1,header.EntryCount);
        table=static_cast<FableUiBankOpenVtable*>(bank->Vtable);
        table->FinishRead(bank,0);
        FableUiDestroyBankStream(&stream,0);
        FableUiReleaseRegisteredBank(&found,0);
        return true;
    }
    FableUiWideStringValue path;
    FableUiFindBankPath(&FableUiBankRegistryState,0,&path,name);
    FableUiBankOpenVtable* table=static_cast<FableUiBankOpenVtable*>(bank->Vtable);
    bool result=table->OpenPath(bank,0,&path,flags);
    FableUiDestroyWideString(&path,0);
    return result;
}
