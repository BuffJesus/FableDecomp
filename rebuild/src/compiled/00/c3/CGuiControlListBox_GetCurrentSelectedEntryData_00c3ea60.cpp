#include "rebuild_abi.h"
// CGuiControlListBox::GetCurrentSelectedEntryData @ 0x00c3ea60
// eax=[ecx+0x188]; if(eax==0) return 0x80000000; return [eax+4];
struct Entry { char pad[4]; int m_data; };
struct CGuiControlListBox { char pad[0x188]; Entry *m_sel; int GetData(); };
int CGuiControlListBox::GetData()
{
    Entry *e = m_sel;
    if (!e) return (int)0x80000000;
    return e->m_data;
}