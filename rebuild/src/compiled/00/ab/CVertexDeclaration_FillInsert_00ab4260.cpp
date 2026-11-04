#include <new>
#include "fable_vertex_shader_input.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {

// Typed STLport helper forms observed at AB3E60, AB3CD0 and AB3F30.
__declspec(noinline) D3DVERTEXELEMENT9* FABLE_FASTCALL CopyVertexDeclarationsBackward(D3DVERTEXELEMENT9* first,D3DVERTEXELEMENT9* last,D3DVERTEXELEMENT9* result,const __false_type&)
{
 for(ptrdiff_t n=last-first;n>0;--n)*--result=*--last;
 return result;
}
__declspec(noinline) void FABLE_FASTCALL FillVertexDeclarations(D3DVERTEXELEMENT9* first,D3DVERTEXELEMENT9* last,const D3DVERTEXELEMENT9& value)
{
 for(;first!=last;++first)*first=value;
}
__declspec(noinline) D3DVERTEXELEMENT9* FABLE_FASTCALL ConstructVertexDeclarations(D3DVERTEXELEMENT9* first,size_t count,const D3DVERTEXELEMENT9& value)
{
 for(;count>0;--count,++first)_Construct(first,value);
 return first;
}

    template<>
    void vector<D3DVERTEXELEMENT9>::_M_fill_insert(D3DVERTEXELEMENT9* __position, size_t __n, const D3DVERTEXELEMENT9& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                D3DVERTEXELEMENT9 __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                D3DVERTEXELEMENT9* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    CopyVertexDeclarationsBackward(__position, __old_finish - __n, __old_finish, __false_type());
                    FillVertexDeclarations(__position, __position + __n, __x_copy);
                }
                else
                {
                    ConstructVertexDeclarations(this->_M_finish, __n - __elems_after, __x_copy);
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                    this->_M_finish += __elems_after;
                    FillVertexDeclarations(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }

}
