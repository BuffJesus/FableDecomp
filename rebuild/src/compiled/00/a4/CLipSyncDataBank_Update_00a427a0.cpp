// CLipSyncDataBank::Update @ 0x00a427a0
// mov al,[ecx+0x178]; test al,al; jne ret; jmp 0x9cec60

struct CLipSyncDataBank {
    char pad[0x178];
    unsigned char m_bSuspended; // +0x178
    void DoUpdate();            // 0x9cec60, __fastcall
    void Update();
};

void CLipSyncDataBank::Update()
{
    if (this->m_bSuspended == 0)
        this->DoUpdate();
}