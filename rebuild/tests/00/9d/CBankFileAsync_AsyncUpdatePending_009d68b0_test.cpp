#include "fable_game_text_bank.h"
#include <stdio.h>

static CBankFileAsync* expected;
static int calls[2];
static int callCount;
void CBankFileAsync::ProcessDisposalList()
{
    if (this == expected && callCount < 2) calls[callCount++] = 1;
}
void CBankFileAsync::RestartAbortedReads()
{
    if (this == expected && callCount < 2) calls[callCount++] = 2;
}
int main()
{
    unsigned long storage[0x164 / 4];
    expected = reinterpret_cast<CBankFileAsync*>(storage);
    expected->AsyncUpdatePending();
    if (callCount != 2 || calls[0] != 1 || calls[1] != 2) return 1;
    puts("ASYNC_UPDATE_PENDING_PASS");
    return 0;
}