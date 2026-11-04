#pragma once
#include "fable_base_class.h"
#include "fable_iv_counted_pointer.h"
#include "fable_managed_texture_data.h"
class CManagedTexture : public CBaseClass {
public:
    CManagedTexture() {}
#ifdef FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#ifdef FABLE_INLINE_PROVIDER_ANCHOR
    __declspec(noinline) virtual inline ~CManagedTexture();
    __declspec(noinline) inline const CManagedTexture& operator=(const CManagedTexture&);
#else
    virtual inline ~CManagedTexture();
    inline const CManagedTexture& operator=(const CManagedTexture&);
#endif
#else
    virtual ~CManagedTexture();
    const CManagedTexture& operator=(const CManagedTexture&);
#endif
    unsigned long Sizeof() const;
#ifdef FABLE_MANAGED_TEXTURE_INLINE_MEMBERS
#ifdef FABLE_INLINE_PROVIDER_ANCHOR
    __declspec(noinline) inline bool Release();
#else
    inline bool Release();
#endif
#else
    bool Release();
#endif
    CIVCountedPointer<CManagedTextureData> Data;
};
FABLE_STATIC_ASSERT(sizeof(CManagedTexture)==8);
