#include "fable_ui_manager_configuration.h"
int* __fastcall FableUiLookupLayer(FableUiIntegerMapStorage* map,void*,const int* key)
{
    return FableUiFindOrInsertIntegerMap(map,key);
}
