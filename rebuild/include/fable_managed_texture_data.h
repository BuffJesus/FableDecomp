#ifndef FABLE_ENGINE_CMANAGEDTEXTUREDATA_H
#define FABLE_ENGINE_CMANAGEDTEXTUREDATA_H
// Canonical donor-verified CResource base; inherited 28B prefix is typed.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CManagedTextureData = 0x4c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_resource_list.h"
#include "fable_counted_pointer.h"

class CCPPointerInfo;
class CManagedTextureOwner;
class CTexture;
class CVolumeTexture;
class CPixelFormat;
struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9;

#pragma pack(push, 1)
class CManagedTextureData : public CResource {
public:
    CManagedTextureData();
    virtual ~CManagedTextureData();
    void Initialise(unsigned long allocationSize, unsigned long poolIndex);
    void Uninitialise();
#ifdef FABLE_MANAGED_TEXTURE_INLINE_BUSY
    bool IsBusy() {if(BusyFlag)BusyFlag=Texture->IsBusy();return BusyFlag;}
#else
    bool IsBusy();
#endif
    bool CreateTexture(unsigned long width,unsigned long height,unsigned int levels,const CPixelFormat& format);
    bool CreateVolumeTexture(unsigned long width,unsigned long height,unsigned long depth,unsigned int levels,const CPixelFormat& format);
    void ReleaseTexture();
    unsigned short           Width;                          // +0x1c
    unsigned short           Height;                         // +0x1e
    CCountedPointer<CTexture> Texture;                       // +0x20
    CCountedPointer<CVolumeTexture> VolumeTexture;           // +0x28
    CManagedTextureOwner*    Owner;                          // +0x30
    IDirect3DTexture9*       D3DHeader;                      // +0x34
    IDirect3DVolumeTexture9* D3DVolumeHeader_ptr;            // +0x38 scoped_ptr<IDirect3DVolumeTexture9>::ptr
    void*                    Data;                           // +0x3c
    unsigned long            AllocSize;                      // +0x40
    unsigned long            PoolIndex;                      // +0x44
    bool                     Allocated;                      // +0x48
    bool                     Locked;                         // +0x49
    bool                     BusyFlag;                       // +0x4a
    unsigned char            _pad_0x4b[0x1];                 // +0x4b
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CManagedTextureData) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Width) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Height) == 0x1e);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Texture) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, VolumeTexture) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Owner) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, D3DHeader) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, D3DVolumeHeader_ptr) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Data) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, AllocSize) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, PoolIndex) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Allocated) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, Locked) == 0x49);
FABLE_STATIC_ASSERT(offsetof(CManagedTextureData, BusyFlag) == 0x4a);

#endif // FABLE_ENGINE_CMANAGEDTEXTUREDATA_H
