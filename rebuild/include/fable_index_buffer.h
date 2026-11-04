#pragma once
// Hand-owned (retail model): CIndexBuffer, an engine index buffer wrapping an IDirect3DIndexBuffer9 (lib_index_buffer).
// Retail RTTI .?AVCIndexBuffer@@ : CMovableResource, vtable 0x0129EF2C:
//   [0] scalar deleting destructor 0x00AC2070, [1] RelocateData 0x00AC1F50 (empty: the D3D buffer does not move),
//   [2] CanRelocateData 0x009D50F0 and [3] Verify 0x009D5100 (CMovableResource's).
// Ego_r.pdb names and members, 0x14 bytes.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_movable_resource.h"
#include "fable_render_state_manager.h"   // EPrimitiveType

struct IDirect3DIndexBuffer9;

// Ego_r enums; the values are the D3D9 ones (D3DFMT_INDEX16 / D3DFMT_INDEX32, D3DPOOL_*), passed straight through.
enum EIndexBufferFormat { IB_FORMAT_16 = 101, IB_FORMAT_32 = 102 };
enum EIndexBufferPool { IB_POOL_DEFAULT = 0, IB_POOL_MANAGED = 1, IB_POOL_SYSTEM_MEM = 2 };

class CIndexBuffer : public CMovableResource
{
public:
    // Ego_r ??0CIndexBuffer@@QAE@XZ (retail 0x00AC1EC0): no buffer yet.
    CIndexBuffer();
    // Ego_r ??0CIndexBuffer@@QAE@KKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z (retail 0x00AC2010):
    // construct and Initialise (expanded inline: same retail unit).
    CIndexBuffer(unsigned long length, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool,
                 EPrimitiveType primitiveType);
    // Ego_r ~CIndexBuffer has no body of its own: inline, expanded into the scalar deleting destructor 0x00AC2070.
    virtual ~CIndexBuffer() { Uninitialise(); }
    // Ego_r ?RelocateData@CIndexBuffer@@UAEXJPAX0J@Z (retail 0x00AC1F50).
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);

    // Ego_r ?Uninitialise@CIndexBuffer@@QAEXXZ (retail 0x00AC1EE0): release the D3D buffer.
    void Uninitialise();
    // Ego_r ?OnPostRelocation@CIndexBuffer@@QAEXXZ (retail 0x00AC1F60): nothing to fix up.
    void OnPostRelocation();
    // Ego_r ?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z (retail
    // 0x00AC1F70): create the D3D index buffer on the display manager's device.
    void Initialise(unsigned long length, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool,
                    EPrimitiveType primitiveType);
    // Ego_r ?Lock@CIndexBuffer@@QAEPAEKKK@Z (retail 0x00AC1FB0): wait until the GPU is done with the buffer, then lock
    // size bytes at offset (0: the whole buffer).
    unsigned char* Lock(unsigned long flags, unsigned long offset, unsigned long size);
    // Ego_r ?Unlock@CIndexBuffer@@QAEXXZ (retail 0x00AC1F00).
    void Unlock();
    // Ego_r ?GetSize@CIndexBuffer@@QBEJXZ (retail 0x00AC1F10): the D3D buffer's size from its description.
    long GetSize() const;
    // Ego_r ?Sizeof@CIndexBuffer@@QBEKXZ (retail 0x00AC1F30): the buffer's size plus 0x18 (Ego_r adds the same).
    unsigned long Sizeof() const;

    IDirect3DIndexBuffer9* PD3DIndexBuffer;   // +0x04
    unsigned long ByteLength;                 // +0x08
    EPrimitiveType PrimitiveType;             // +0x0c
    void* PAllocatedBuffer;                   // +0x10
};

FABLE_STATIC_ASSERT(sizeof(CIndexBuffer) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CIndexBuffer, PD3DIndexBuffer) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CIndexBuffer, ByteLength) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CIndexBuffer, PrimitiveType) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CIndexBuffer, PAllocatedBuffer) == 0x10);
