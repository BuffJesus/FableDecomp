#include "fable_ui_manager_configuration.h"
int* __fastcall FableUiLookupInputKey(FableUiIntegerMapStorage* map,void*,const int* key)
{
    return FableUiFindOrInsertIntegerMap(map,key);
}
