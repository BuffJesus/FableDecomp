#include "fable_game_text_bank.h"
CBankFile::CBankFile() : ChecksumCache(0x10000)
{
    HeaderSize = 0;
    FileInformationSize = 0;
    FileSize = 0;
    ProgressCallback = 0;
    Size = 0;
    BankFormatVersion = 0x73;
    Alignment = 1;
}