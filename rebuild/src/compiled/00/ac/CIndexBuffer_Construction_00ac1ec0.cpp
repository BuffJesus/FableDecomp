// CIndexBuffer construction (Ego_r names; retail 0x00AC1EC0 default ctor, 0x00AC1F70 Initialise, 0x00AC1FB0 Lock,
// 0x00AC2010 ctor). Initialise creates the D3D index buffer on the display manager's device and remembers its length and
// primitive type; the full constructor expands it (same retail unit). Lock waits until the render manager is done with
// the buffer, then locks size bytes at offset - 0 meaning the buffer's whole size from its description.
#include "fable_index_buffer.h"
#include "fable_d3d9_index_buffer.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"

CIndexBuffer::CIndexBuffer()
{
    PAllocatedBuffer = 0;
    PD3DIndexBuffer = 0;
}

void CIndexBuffer::Initialise(unsigned long length, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool,
                              EPrimitiveType primitiveType)
{
    IDirect3DDevice9* device = GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    device->CreateIndexBuffer(length, usage, (D3DFORMAT)format, (D3DPOOL)pool, &PD3DIndexBuffer, 0);
    ByteLength = length;
    PrimitiveType = primitiveType;
}

unsigned char* CIndexBuffer::Lock(unsigned long flags, unsigned long offset, unsigned long size)
{
    if (size == 0)
    {
        D3DINDEXBUFFER_DESC desc;
        PD3DIndexBuffer->GetDesc(&desc);
        size = desc.Size;
    }
    GFGetSystemManager()->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DIndexBuffer, 1.0f);
    unsigned char* data;
    PD3DIndexBuffer->Lock(offset, size, reinterpret_cast<void**>(&data), flags);
    return data;
}

CIndexBuffer::CIndexBuffer(unsigned long length, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool,
                           EPrimitiveType primitiveType)
{
    PAllocatedBuffer = 0;
    PD3DIndexBuffer = 0;
    Initialise(length, usage, format, pool, primitiveType);
}
