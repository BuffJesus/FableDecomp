#include "fable_font_vertex_buffer.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
#include <string.h>

bool CVertexBufferWin32::Initialise(EVertexType type, long count, long stride,
                                    unsigned long flags, EVertexBufferPool pool)
{
    CSystemManager* manager = GFGetSystemManager();
    long bytes = count * stride;
    HRESULT result = manager->GetDisplayManager()->D3DDevice->CreateVertexBuffer(
        bytes, flags, type, static_cast<D3DPOOL>(pool), &Buffer, 0);
    ByteSize = bytes;
    VertexStride = static_cast<unsigned char>(stride);
    VertexType = type;
    float defaults[8] = {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    memcpy(&ScaleX, defaults, sizeof(defaults));
    return result >= 0;
}