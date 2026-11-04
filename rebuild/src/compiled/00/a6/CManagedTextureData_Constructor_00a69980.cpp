#include "fable_managed_texture_data.h"
CManagedTextureData::CManagedTextureData()
 : Width(0),Height(0),Owner(0),D3DHeader(0),D3DVolumeHeader_ptr(0),Data(0),
   AllocSize(0),PoolIndex(0),Allocated(false),Locked(false),BusyFlag(false)
{
}
