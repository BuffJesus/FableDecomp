// CIndexBuffer movable-resource hooks (retail 0x00AC1F50 RelocateData, 0x00AC1F60 OnPostRelocation; Ego_r 0x00BCEC90 /
// 0x00BCECA0). The index data lives in a D3D buffer, not in the movable pool, so a pool move needs no copy or fix-up.
#include "fable_index_buffer.h"

void CIndexBuffer::RelocateData(long, void*, void*, long)
{
}

void CIndexBuffer::OnPostRelocation()
{
}
