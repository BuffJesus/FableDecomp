#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
void CBankFileAsync::Close()
{
    if (ThreadedFile.data_ && !RetailMode) ThreadedFile->Close();
    CBankFile::Close();
}
