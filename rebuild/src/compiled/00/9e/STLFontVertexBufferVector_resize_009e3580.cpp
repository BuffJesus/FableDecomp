#include "fable_font_vertex_buffer.h"
#include "fable_stlport_vector.h"
template<> void _STL::vector<CVertexBufferWin32*>::resize(size_t newSize, CVertexBufferWin32* const& value)
{
    if (newSize < size())
        erase(begin() + newSize, end());
    else
        insert(end(), newSize - size(), value);
}