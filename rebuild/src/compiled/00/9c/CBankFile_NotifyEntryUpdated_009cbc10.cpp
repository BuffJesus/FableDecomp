// CBankFile::NotifyEntryUpdated @ 0x009CBC10 (FableWin ?NotifyEntryUpdated@CBankFile@@...; vtable 0x0129B54C
// slot 13): the base bank ignores the notification; derived banks override it.
#include "fable_game_text_bank.h"

void CBankFile::NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&)
{
}
