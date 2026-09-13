#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <cstdio>
// Read the same interface used by retail C53260/C53340. No SetSpeakerConfig.
int main() {
    HRESULT init = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(init)) return 1;
    IDirectSound* sound = nullptr;
    HRESULT create = CoCreateInstance(CLSID_DirectSound, nullptr, CLSCTX_INPROC_SERVER,
                                      IID_IDirectSound, reinterpret_cast<void**>(&sound));
    HRESULT initialize = E_FAIL, query = E_FAIL, priority = E_FAIL, second = E_FAIL;
    DWORD config = 0, after = 0;
    HWND window = CreateWindowExW(0, L"STATIC", L"Read-only retail audio probe", WS_POPUP,
                                  0, 0, 1, 1, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    if (SUCCEEDED(create)) {
        initialize = sound->Initialize(nullptr);
        if (SUCCEEDED(initialize)) {
            query = sound->GetSpeakerConfig(&config);
            // Retail C532DD uses DSSCL_PRIORITY before querying; hidden own window.
            priority = sound->SetCooperativeLevel(window, DSSCL_PRIORITY);
            second = sound->GetSpeakerConfig(&after);
        }
        sound->Release();
    }
    std::printf("{\"create_hr\":%ld,\"initialize_hr\":%ld,\"query_hr\":%ld,\"speaker_config\":%lu,\"config_low_byte\":%lu,\"priority_hr\":%ld,\"second_query_hr\":%ld,\"after_priority_config\":%lu}\n",
                create, initialize, query, config, DWORD(DSSPEAKER_CONFIG(config)), priority, second, after);
    if (window) DestroyWindow(window);
    CoUninitialize();
    return FAILED(query) ? 1 : 0;
}
