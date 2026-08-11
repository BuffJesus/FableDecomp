// CUserProfileManager::CaptureBackBufferForUIBackgroundIfNonExistant  @ 0x00c32b80
// Retail is a tiny __fastcall(this, unsigned int pos) that subtracts a member
// span at [this+8] and tail-jumps to a sibling routine at 0x00c32950.

struct CUserProfileManager
{
    int   m_0;    // +0x00
    int   m_4;    // +0x04
    unsigned int m_8;    // +0x08

    // sibling routine reached by the tail jump; a __fastcall member so no edx
    // parameter is emitted (matches retail: ecx=this, one stacked arg).
    void Sibling_00c32950(unsigned int pos);

    void CaptureBackBufferForUIBackgroundIfNonExistant(unsigned int pos);
};

void CUserProfileManager::CaptureBackBufferForUIBackgroundIfNonExistant(unsigned int pos)
{
    unsigned int span = this->m_8;
    if (pos <= span)
        return;
    this->Sibling_00c32950(pos - span);
}