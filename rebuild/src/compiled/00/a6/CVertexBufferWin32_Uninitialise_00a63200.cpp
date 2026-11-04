#include "fable_font_vertex_buffer.h"
#include "fable_system_manager.h"

void CVertexBufferWin32::Uninitialise()
{
    if (Buffer != 0)
    {
        CSystemManager* system = GFGetSystemManager();
        system->PDisplayManager.Data->PRenderManager->
            WaitWhileResourceIsBusy(reinterpret_cast<IDirect3DResource9*>(Buffer), 1.0f);
        reinterpret_cast<IUnknown*>(Buffer)->Release();
        Buffer = 0;
    }
}
