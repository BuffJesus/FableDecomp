#pragma optimize("s",on)
#include "fable_font_bank.h"
#include "fable_counted_pointer.h"
template<> void CCountedPointer<CFontBank>::ShareData(CFontBank* data,CCPPointerInfo* info)
{
 if(Info!=info){ReleaseData();Data=data;Info=info;if(info)++info->RefCount;}
}
template<> void CCountedPointer<const CFontBank>::ShareData(const CFontBank* data,CCPPointerInfo* info)
{
 if(Info!=info){ReleaseData();Data=data;Info=info;if(info)++info->RefCount;}
}
template<> CCountedPointer<CFontBank>& CCountedPointer<CFontBank>::operator=(const CCountedPointer<CFontBank>& other)
{
 ShareData(other.Data,other.Info);return *this;
}
template<> template<> CCountedPointer<const CFontBank>& CCountedPointer<const CFontBank>::operator=<CFontBank>(const CCountedPointer<CFontBank>& other)
{
 ShareData(other.Data,other.Info);return *this;
}
