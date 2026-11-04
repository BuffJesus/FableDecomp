// CIndexBuffer::Unlock (retail 0x00AC1F00), GetSize (0x00AC1F10) and Sizeof (0x00AC1F30); Ego_r lib_index_buffer
// 0x00BCEC40 / 0x00BCEC50 / 0x00BCEC70. GetSize asks the D3D buffer for its description; Sizeof adds the constant 0x18
// that both retail and Ego_r add to it.
#include "fable_index_buffer.h"
#include "fable_d3d9_index_buffer.h"

void CIndexBuffer::Unlock()
{
    PD3DIndexBuffer->Unlock();
}

long CIndexBuffer::GetSize() const
{
    D3DINDEXBUFFER_DESC desc;
    PD3DIndexBuffer->GetDesc(&desc);
    return desc.Size;
}

unsigned long CIndexBuffer::Sizeof() const
{
    return GetSize() + 0x18;
}
