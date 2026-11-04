// CIndexBuffer::Uninitialise (retail 0x00AC1EE0, Ego_r 0x00BCEC20): release the D3D index buffer and forget it.
// fable_d3d9.h declares IDirect3DIndexBuffer9 only by name, so the release goes through its IUnknown base.
#include "fable_index_buffer.h"
#include "fable_d3d9.h"

void CIndexBuffer::Uninitialise()
{
    if (PD3DIndexBuffer)
    {
        reinterpret_cast<IUnknown*>(PD3DIndexBuffer)->Release();
        PD3DIndexBuffer = 0;
    }
}
