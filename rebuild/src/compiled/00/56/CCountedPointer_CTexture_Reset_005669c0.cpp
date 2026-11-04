#include "fable_surface_types.h"
#include "fable_counted_pointer.h"
template<>
void CCountedPointer<CTexture>::Reset(CTexture* data)
{
 Release();
 Info=0;
 Data=data;
 if(data)
  Info=new CCPPointerInfo(DeleteData,Data);
}
