#include "fable_managed_texture_data.h"
#include "fable_surface_types.h"
bool CManagedTextureData::IsBusy()
{
 if(BusyFlag)
  BusyFlag=Texture->IsBusy();
 return BusyFlag;
}
