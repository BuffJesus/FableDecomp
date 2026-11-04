#include <stdio.h>
#include <string.h>
#include "fable_font_vertex_buffer.h"
#include "fable_stlport_vector.h"
static int insertCalls;static unsigned lastCount;
template<> void _STL::vector<CVertexBufferWin32*>::_M_fill_insert(
    CVertexBufferWin32** position, size_t count, CVertexBufferWin32* const& value)
{
    ++insertCalls;lastCount=count;
    if(position!=_M_finish)return;
    for(unsigned i=0;i<count;++i) _M_finish[i]=value;
    _M_finish+=count;
}
int main() {
    unsigned char storage[sizeof(_STL::vector<CVertexBufferWin32*>)];memset(storage,0xA5,sizeof(storage));
    _STL::vector<CVertexBufferWin32*>& values=*reinterpret_cast<_STL::vector<CVertexBufferWin32*>*>(storage);
    CVertexBufferWin32* slots[5]={
      reinterpret_cast<CVertexBufferWin32*>(0x1000),reinterpret_cast<CVertexBufferWin32*>(0x2000),
      reinterpret_cast<CVertexBufferWin32*>(0x3000),reinterpret_cast<CVertexBufferWin32*>(0x4000),0};
    values._M_start=slots;values._M_finish=slots+4;values._M_end_of_storage._M_data=slots+5;
    CVertexBufferWin32* value=reinterpret_cast<CVertexBufferWin32*>(0x5000);
    values.resize(2,value);
    if(values._M_finish!=slots+2||insertCalls||slots[0]!=(CVertexBufferWin32*)0x1000||slots[1]!=(CVertexBufferWin32*)0x2000)return 1;
    values.resize(5,value);
    if(values._M_finish!=slots+5||insertCalls!=1||lastCount!=3||slots[2]!=value||slots[3]!=value||slots[4]!=value)return 2;
    values.resize(5,value);
    if(values._M_finish!=slots+5||insertCalls!=2||lastCount!=0)return 3;
    puts("FONT_VB_RESIZE_009e3580_PASS");return 0;
}