#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"

void CBankFileAsyncData::ChangeLoadingPriority(EThreadedFilePriority priority)
{
    if (ThreadedFileUID)
        BankFile.ThreadedFile->SetObjectPriority(ThreadedFileUID, priority);
}
