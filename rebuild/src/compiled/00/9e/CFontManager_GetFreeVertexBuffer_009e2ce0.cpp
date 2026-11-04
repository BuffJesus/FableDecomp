#include "fable_system_manager.h"
CVertexBufferWin32& CFontManager::GetFreeVertexBuffer()
{
    unsigned long next = (static_cast<unsigned long>(CurrentVB) + 1) % VertexBuffers.size();
    CurrentVB = next;
    return *VertexBuffers[next];
}