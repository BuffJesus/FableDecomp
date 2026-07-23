#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifndef WINVER
#define WINVER 0x0501
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <d3d9.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    IDirect3D9* direct3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3D != 0)
    {
        direct3D->Release();
    }
    return GetCurrentProcessId() == 0 ? 1 : 0;
}
