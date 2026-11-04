#include "fable_font_vertex_buffer.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
unsigned char* CVertexBufferBase::Lock(unsigned long flags,unsigned long offset,unsigned long bytes) {return DoLock(flags,offset,bytes);}
void CVertexBufferBase::Unlock() {DoUnlock();}
bool CVertexBufferBase::IsBusy() const {return DoIsBusy();}
unsigned long CVertexBufferBase::Sizeof() const {return DoSizeof();}
unsigned char* CVertexBufferWin32::DoLock(unsigned long flags,unsigned long offset,unsigned long bytes)
{
    if(bytes==0)bytes=ByteSize;
    GFGetSystemManager()->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(Buffer,1.0f);
    unsigned char* data=0;
    Buffer->Lock(offset,bytes,reinterpret_cast<void**>(&data),flags);
    return data;
}
void CVertexBufferWin32::DoUnlock(){Buffer->Unlock();}
bool CVertexBufferWin32::DoIsBusy() const{return false;}
unsigned long CVertexBufferWin32::DoSizeof() const{return static_cast<unsigned long>(ByteSize)+56;}
