#pragma once

// CChecksum (Ego_r.pdb layout, 0x10: Checksum1..4).

#include "rebuild_abi.h"

class CChecksum
{
public:
    // FableWin ??0CChecksum@@QAE@JJJ@Z; inline in retail
    // (CMainGameComponent::GetInputs 0x00416E78 stores all four fields).
    CChecksum(long checksum1, long checksum2, long checksum3)
        : Checksum1(checksum1),
          Checksum2(checksum2),
          Checksum3(checksum3),
          Checksum4(0)
    {
    }

    long Checksum1;
    long Checksum2;
    long Checksum3;
    long Checksum4;
};

FABLE_STATIC_ASSERT(sizeof(CChecksum) == 0x10);
