#pragma once
#include "fable_resource_list.h"
// Ego_r original base identity and 28B extent. No additional owner fields.
class CManagedTextureOwner : public CResource {
public:
    virtual void ReleaseTexture()=0; // original slot 10
};
FABLE_STATIC_ASSERT(sizeof(CManagedTextureOwner)==28);
