#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_memory_allocator_deallocate.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "fable_surface_types.h"
#include "fable_font_vertex_buffer.h"
#include "engine/CIndexBuffer.h"

// FableWin.exe: each entry goes through Delete*BlockWhileBusy (destroy, then
// FreeVirtual, whose `if (allocation) Deallocate` is expanded here). The test
// folds away after the buffers' virtual destructor calls.
void CEngineLandscapeMemoryManager::FreeDisposalList()
{
    for (long i=0;i<3;++i) {
        while (TextureDisposalList[i].size()) {
            CTexture* texture=TextureDisposalList[i].front();
            texture->~CTexture();
            if (texture)
                CMemoryAllocator::Deallocate(texture);
            TextureDisposalList[i].pop_front();
        }
        while (VertexBufferDisposalList[i].size()) {
            CVertexBufferWin32* buffer=VertexBufferDisposalList[i].front();
            buffer->~CVertexBufferWin32();
            if (buffer)
                CMemoryAllocator::Deallocate(buffer);
            VertexBufferDisposalList[i].pop_front();
        }
        while (IndexBufferDisposalList[i].size()) {
            CIndexBuffer* buffer=IndexBufferDisposalList[i].front();
            buffer->~CIndexBuffer();
            if (buffer)
                CMemoryAllocator::Deallocate(buffer);
            IndexBufferDisposalList[i].pop_front();
        }
    }
}
