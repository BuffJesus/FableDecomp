#include "fable_game_text_bank.h"

void CBankFileAsync::AsyncUpdatePending()
{
    ProcessDisposalList();
    RestartAbortedReads();
}