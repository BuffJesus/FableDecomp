#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_font_vertex_buffer.h"
#include "fable_system_manager.h"

static char g_events[16];
static unsigned g_eventCount;
static CSystemManager* g_system;
static CRenderManager2D* g_expectedRenderer;
static IDirect3DVertexBuffer9* g_expectedBuffer;
static bool g_hookArgumentsValid;
static void Record(char value)
{
    g_events[g_eventCount++] = value;
    g_events[g_eventCount] = 0;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    Record('G');
    return g_system;
}

void CRenderManagerCore::WaitWhileResourceIsBusy(
    IDirect3DResource9* buffer, float scale)
{
    Record('H');
    g_hookArgumentsValid = this == g_expectedRenderer &&
        buffer == reinterpret_cast<IDirect3DResource9*>(g_expectedBuffer) && scale == 1.0f;
}

struct FakeBuffer : IUnknown
{
    HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    unsigned long __stdcall AddRef() { return 1; }
    unsigned long __stdcall Release() { Record('R'); return 0; }
};

int main()
{
    unsigned char objectStorage[sizeof(CVertexBufferWin32)];
    unsigned char systemStorage[sizeof(CSystemManager)];
    unsigned char displayStorage[sizeof(CDisplayManager)];
    unsigned char rendererStorage[sizeof(CRenderManager2D)];
    memset(objectStorage, 0, sizeof(objectStorage));
    memset(systemStorage, 0, sizeof(systemStorage));
    memset(displayStorage, 0, sizeof(displayStorage));
    memset(rendererStorage, 0, sizeof(rendererStorage));
    CVertexBufferWin32* object = reinterpret_cast<CVertexBufferWin32*>(objectStorage);
    g_system = reinterpret_cast<CSystemManager*>(systemStorage);
    CDisplayManager* display = reinterpret_cast<CDisplayManager*>(displayStorage);
    g_expectedRenderer = reinterpret_cast<CRenderManager2D*>(rendererStorage);
    g_system->PDisplayManager.Data = display;
    new (&display->PRenderManager) boost::scoped_ptr<CRenderManager>(reinterpret_cast<CRenderManager*>(g_expectedRenderer));
    object->Uninitialise();
    if (g_eventCount || object->Buffer != 0)
        return 1;
    FakeBuffer fake;
    g_expectedBuffer = reinterpret_cast<IDirect3DVertexBuffer9*>(&fake);
    object->Buffer = g_expectedBuffer;
    object->Uninitialise();
    if (strcmp(g_events, "GHR") != 0 || !g_hookArgumentsValid || object->Buffer)
        return 2;
    g_eventCount = 0;
    g_events[0] = 0;
    object->Uninitialise();
    if (g_eventCount)
        return 3;
    puts("VERTEX_BUFFER_UNINITIALISE_00a63200_PASS");
    return 0;
}
