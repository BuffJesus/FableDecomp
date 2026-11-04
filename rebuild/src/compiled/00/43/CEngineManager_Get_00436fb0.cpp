#include <new>
#include "fable_engine_manager.h"
#include "fable_stlport_uninitialized.h"
CEngineManager& CTBaseSingleton<CEngineManager>::Get()
{
    if(PClass.IsNull())PClass.Reset(new CEngineManager);
    return *PClass;
}
