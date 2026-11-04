#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_progress_display.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
bool CBankFileAsync::OpenReadOnly(const CCharString& handle,unsigned long flags)
{
    if(!CBankFile::OpenReadOnly(handle,flags))return false;
    NProgressDisplay::DisplayProgress(CCharString("Open Bank File Async"),-1.0f,false,false);
    if(RetailMode){
        NBankFileManager::CContainedBank info;
        CCountedPointer<NBankFileManager::CRetailBank> retail;
        if(!g_FableBankFileManager_013CA79C.FindContainedBank(handle,info,retail))return false;
        ThreadedFile=CCountedPointer<CThreadedFile>(retail->threadedFile_);
        NonCached=retail->useDma_;
        DisableAsync=false;
        return true;
    }
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(),NonCached);
    DisableAsync=false;
    return true;
}
