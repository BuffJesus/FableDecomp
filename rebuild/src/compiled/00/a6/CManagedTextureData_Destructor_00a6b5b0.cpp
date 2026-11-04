#define FABLE_RESOURCE_DESTRUCTOR_DECLARATION_ONLY
#include "fable_managed_texture_data.h"
CManagedTextureData::~CManagedTextureData()
{
    operator delete(D3DVolumeHeader_ptr);
}
