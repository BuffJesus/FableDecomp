#include <new>
#include "fable_engine_manager.h"
template<> void CCountedPointer<CEngineManager>::Reset(CEngineManager* data)
{
    Release();Info=0;Data=data;
    if(Data)Info=new CCPPointerInfo(&DeleteData,Data);
}
