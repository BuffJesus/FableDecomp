#include "fable_volume_types.h"
#include "fable_counted_pointer.h"
template<>
void CCountedPointer<CVolumeTexture>::Reset(CVolumeTexture* data)
{
 Release();
 Info=0;
 Data=data;
 if(data)
  Info=new CCPPointerInfo(DeleteData,Data);
}

