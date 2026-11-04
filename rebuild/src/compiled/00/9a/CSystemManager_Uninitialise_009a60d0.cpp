#include "fable_system_manager.h"
#include "fable_pending_loaded_glyph.h"
extern "C" __declspec(dllimport) int FABLE_STDCALL DestroyWindow(void* window);
extern "C" __declspec(dllimport) void FABLE_STDCALL DeleteCriticalSection(void* section);

template<typename T>
inline void CCountedPointer<T>::ReleaseData()
{
    Release();
    Info=0;
    Data=0;
}

void CSystemManager::Uninitialise()
{
    if (!IsValid()) return;
    PInputManager.ReleaseData();
    PSoundManager.ReleaseData();
    CTBaseSingletonNoDefaultAlloc<CDriveManager>::Release();
    for(unsigned long i=0;i!=FablePendingGlyphCount_0129ab30;++i) {
        PendingLoadedGlyph& glyph=FablePendingGlyphs_0138e074[i];
        glyph.data.ReleaseData();
        glyph.index=0;
    }
    FablePendingGlyphRead_013c5a74=0;
    FablePendingGlyphWrite_013c5a78=0;
    PFontManager.ReleaseData();
    PGraphicBankManager.ReleaseData();
    if(PDisplayManager.Data)
        PDisplayManager->RemoveDeviceResetCallback(PMeshBankManager.Data);
    PMeshBankManager.ReleaseData();
    if(GScratchBuffer.IsValid()) GScratchBuffer.Uninitialise();
    LibDebugManager.ReleaseData();
    PDisplayManager.ReleaseData();
    if(!AttachedToExternalWindow) {
        if(WindowValid==true) {
            DestroyWindow(ApplicationWinHandle);
            WindowValid=false;
            ApplicationWinHandle=0;
        }
    } else ApplicationWinHandle=0;
    ProfileManager.ReleaseData();
    DeleteCriticalSection(CriticalSection);
    --NoInstances;
    CInitBaseClass::Uninitialise();
}
