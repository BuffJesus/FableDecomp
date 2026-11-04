// CBankFile::NotifyPostEntriesLoaded @ 0x0050B440 (FableWin ?NotifyPostEntriesLoaded@CBankFile@@...; vtable 0x0129B54C
// slot 15): the base bank ignores the notification; derived banks override it.
// The linker folded this empty body with other empty members (/OPT:ICF;
// 12 vtable references in retail).
#include "fable_game_text_bank.h"

void CBankFile::NotifyPostEntriesLoaded()
{
}
