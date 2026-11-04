// CBankFile::NotifyBankSize @ 0x009CBBF0 (FableWin ?NotifyBankSize@CBankFile@@...; vtable 0x0129B54C
// slot 10): the base bank ignores the notification; derived banks override it.
#include "fable_game_text_bank.h"

void CBankFile::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&)
{
}
