// CLipSyncDataBank_Methods::Update @ 0x00a427a0
// mov al,[ecx+0x178]; test al,al; jne ret; jmp 0x9cec60

#include "engine/CLipSyncDataBank.h"  // retyped onto the PDB layout; byte parity re-verified
struct CLipSyncDataBank_Methods : CLipSyncDataBank {
    // +0x178
    void DoUpdate();
    // 0x9cec60, __fastcall
    void Update();
};

void CLipSyncDataBank_Methods::Update()
{
    if (this->m_bSuspended == 0)
        this->DoUpdate();
}