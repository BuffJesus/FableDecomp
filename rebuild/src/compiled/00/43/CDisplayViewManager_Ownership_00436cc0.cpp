#include <new>
#include "fable_display_view_manager.h"

CDisplayViewManager::CDisplayViewManager() {}

template<> void CCountedPointer<CDisplayViewManager>::Reset(CDisplayViewManager* data)
{
    Release();
    Info=0;
    Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
