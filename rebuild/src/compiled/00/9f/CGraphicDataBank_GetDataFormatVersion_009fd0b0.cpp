// ?GetDataFormatVersion@CGraphicDataBank@@MAEKXZ @ 0x009FD0B0: slot 8 of the
// CGraphicDataBank vtable 0x0129C8CC (RTTI COL 0x01356944; CBankFile slot
// order puts GetDataFormatVersion at 8). First function of GraphicDataBank.cpp
// in both retail and Ego_r.pdb module order.
#include "engine/CGraphicDataBank.h"

unsigned long CGraphicDataBank::GetDataFormatVersion()
{
    return 122;
}
