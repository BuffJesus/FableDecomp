// CBankFile::NotifyEntryRemoved @ 0x009CBC30 (FableWin ?NotifyEntryRemoved@CBankFile@@...; vtable 0x0129B54C
// slot 14): the base bank ignores the notification; derived banks override it.
#include "fable_game_text_bank.h"

void CBankFile::NotifyEntryRemoved(unsigned long)
{
}
