#include "rebuild_abi.h"
// CTCHeroMorph::SetAsTeenager @ 0x0071ba40
// dl=[this+0x60]; al=arg; [this+0x3d]=(al!=dl); [this+0x60]=al; ret4
struct CTCHeroMorph { char pad3d[0x3d]; unsigned char m_changed; char pad[0x60-0x3e]; unsigned char m_val; void SetAsTeenager(unsigned char v); };
void CTCHeroMorph::SetAsTeenager(unsigned char v){
    m_changed = (v != m_val) ? 1 : 0;
    m_val = v;
}