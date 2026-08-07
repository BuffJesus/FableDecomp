#include "rebuild_abi.h"
// CLipSyncDataBank::IsDiskFile @ 0x00a427c0
// xor al,al; ret 8  -> always returns false (2 stack args, thiscall)
struct CLipSyncDataBank { bool IsDiskFile(unsigned long a, unsigned long b); };
bool CLipSyncDataBank::IsDiskFile(unsigned long a, unsigned long b)
{
    return false;
}