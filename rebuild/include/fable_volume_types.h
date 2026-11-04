#pragma once
#include "fable_base_class.h"
#include "fable_surface_types.h"

// Original PDB: CVolume (8 bytes), C3DExtentsI (12 bytes).
class C3DBoxI;
class CAFile;
class CDataInputStream;
class CDataOutputStream;

class CVolumeLockedBox
{
public:
    CVolumeLockedBox() {}
    C3DExtentsI Dimensions;
    long RowPitch;
    long SlicePitch;
    unsigned char* PMem;
};

class CVolume : public CBaseClass
{
public:
    CVolume();
    explicit CVolume(IDirect3DVolume9* volume);
    CVolume(const CVolume& source);
    virtual ~CVolume();
    void Unlock() const;
    void Uninitialise(bool);
    CPixelFormat GetPixelFormat() const;
    C3DExtentsI GetDimensions() const;
    unsigned long GetByteLength() const;
    CVolumeLockedBox Lock(unsigned long flags);
    void InitialiseFromVolumeTextureMipmap(IDirect3DVolume9* volume);
    void Swizzle(CVolumeLockedBox& box);
    void Unswizzle(CVolumeLockedBox& box);
    void CopyFromVolume(const C3DBoxI* destinationBox, const CVolume& source,
        const C3DBoxI* sourceBox, unsigned long filter);
    static unsigned long TotalMemory;
protected:
    void ReleaseVolume();
    IDirect3DVolume9* PD3DVolume;
};

enum EVolumePool
{
 VOLUME_POOL_DEFAULT_LOCATION=0,
 VOLUME_POOL_MANAGED=1,
 VOLUME_POOL_SYSTEM_MEM=2
};

class CVolumeTexture : public CBaseClass
{
public:
#ifdef FABLE_VOLUME_TEXTURE_INLINE_DEFAULT_CONSTRUCTOR
    // Original managed-volume creation inlines these stores and the base call.
    CVolumeTexture() : PD3DVolumeTexture(0),PData(0) {}
#else
    CVolumeTexture();
#endif
    CVolumeTexture(const CVolumeTexture& source);
    CVolumeTexture& operator=(const CVolumeTexture& source);
    void CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter);
    void SaveToFile(CAFile& file);
    void LoadFromFile(CAFile& file);
    void SaveToDataStream(CDataOutputStream& stream);
    void LoadFromDataStream(CDataInputStream& stream);
    virtual ~CVolumeTexture();
    CVolume GetVolumeLevel(long level) const;
    unsigned int GetNoLevels() const;
    unsigned long GetPriority() const;
    unsigned long Sizeof() const;
    bool Initialise(const C3DExtentsI& dimensions, unsigned int levels, const CPixelFormat& format,
        unsigned long usage, EVolumePool pool, bool);
    bool InitialiseFromPreallocatedVolumeTexture(IDirect3DVolumeTexture9* texture);
    void Uninitialise();
    void PreLoad();
    bool IsBusy() const;
    bool Compress();
    void GenerateMipmaps(unsigned long first, unsigned long second);
    void Unswizzle(unsigned int level, CVolumeLockedBox& box);
    void SetPriority(unsigned long priority);
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);
    void Unlock(unsigned long level);
    C3DExtentsI GetDimensions(unsigned long level) const;
    CPixelFormat GetPixelFormat() const;
    unsigned int GetLevelByteLength(unsigned int level) const;
    static unsigned long TotalMemory;
protected:
    void ReleaseTexture();
    void CalcByteLength();
    bool CreateTexture(const C3DExtentsI& dimensions, unsigned int levels, unsigned long usage,
        const CPixelFormat& format, EVolumePool pool, bool);
    IDirect3DVolumeTexture9* PD3DVolumeTexture;
    unsigned long ByteLength;
    void* PData;
};

FABLE_STATIC_ASSERT(sizeof(CVolume) == 8);
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI) == 12);
FABLE_STATIC_ASSERT(sizeof(D3DVOLUME_DESC) == 28);
FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox) == 24);
FABLE_STATIC_ASSERT(sizeof(CVolumeTexture) == 16);
