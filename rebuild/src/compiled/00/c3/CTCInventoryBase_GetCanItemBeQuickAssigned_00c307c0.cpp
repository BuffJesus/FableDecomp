#include "rebuild_abi.h"
// CTCInventoryBase::GetCanItemBeQuickAssigned @ 0x00c307c0
// mov al,[ecx+0x30]; ret  -> return this->f30 (byte)
struct CTCInventoryBase { char pad00[0x30]; unsigned char m_flag; unsigned char GetCanItemBeQuickAssigned(); };
unsigned char CTCInventoryBase::GetCanItemBeQuickAssigned(){ return m_flag; }