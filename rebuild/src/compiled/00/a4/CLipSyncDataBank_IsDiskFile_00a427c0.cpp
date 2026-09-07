#include "engine/CLipSyncDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CLipSyncDataBank_Methods::IsDiskFile @ 0x00a427c0
// xor al,al; ret 8  -> always returns false (2 stack args, thiscall)
struct CLipSyncDataBank_Methods : CLipSyncDataBank {
    bool IsDiskFile(unsigned long a, unsigned long b);
};
bool CLipSyncDataBank_Methods::IsDiskFile(unsigned long a, unsigned long b)
{
    return false;
}