// CBankFile::NotifyEntryAdded @ 0x009CBC20 (FableWin ?NotifyEntryAdded@CBankFile@@...; vtable 0x0129B54C
// slot 12): the base bank ignores the notification; derived banks override it.
#include "fable_game_text_bank.h"

void CBankFile::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&)
{
}
