// Retail 009D06F0: bank-handle dispatch. Preserve the retail spare slot and
// ignored entry-reader result; the dev branch delegates through the virtual.
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
bool CBankFile::OpenReadOnly(const CCharString& handle,unsigned long flags)
{
    ReadOnly=true;
    OpenFlags=flags;
    BankHandle=handle;
    ChangeCount=0;
    FileValid=false;
    WrittenToFile=false;
    Solid=false;
    UpdatingFlag=false;
    if(g_FableBankFileManager_013CA79C.UseRetailBanks && !(flags&4)) {
        RetailMode=true;
        NBankFileManager::CContainedBank info;
        CCountedPointer<NBankFileManager::CRetailBank> retail;
        if(!g_FableBankFileManager_013CA79C.FindContainedBank(handle,info,retail))return false;
        if(info.dataVersion_!=GetDataFormatVersion())return false;
        Alignment=info.alignment_;
        BankFile=CCountedPointer<CDiskFileWin32>(retail->diskFile_);
        CFileDataInputStream stream(BankFile.Data,0x4000);
        stream.SetPosition(info.infoOffset_);
        ReadEntryList(stream,info.entryCount_+1,info.entryCount_);
        NotifyBankOpened();
        return true;
    }
    return OpenFileReadOnly(g_FableBankFileManager_013CA79C.GetDevBankFilePath(handle),flags);
}
