// Native window callback at 009A5B60, registered by InitWindow at 009A64D9.
// FableWin provides the private static stdcall signature. Keep full switch
// dispatch semantics, including return values and reloads after callbacks.
#include "fable_window_procedure.h"
extern "C" __declspec(dllimport) long FABLE_STDCALL DefWindowProcW(HWND__*,unsigned int,unsigned int,long);

long FABLE_STDCALL CSystemManager::WinProcedure(HWND__* window,unsigned int message,unsigned int wParam,long lParam)
{
    if (FableWindowSystemManager_013ca798) {
        FableIMEMessageFunction handler=FableWindowSystemManager_013ca798->IMEMessageFunc;
        if (handler) {
            long result=0;
            if (handler(window,message,wParam,lParam,result))
                return result;
        }
    }
    switch (message) {
    case 1: // WM_CREATE: lpCreateParams is the first CREATESTRUCT member.
        FableWindowSystemManager_013ca798=*reinterpret_cast<CSystemManager**>(lParam);
        break;
    case 2:
        FableWindowSystemManager_013ca798->WindowValid=false;
        FableWindowSystemManager_013ca798->WindowsQuit=true;
        return 0;
    case 6:
        ActivateWindow(FableWindowSystemManager_013ca798,static_cast<unsigned short>(wParam)!=0);
        return 0;
    case 5:
        switch (wParam) {
        case 0:
        case 2:
            if (FableWindowSystemManager_013ca798->PDisplayManager.Data)
                FableWindowSystemManager_013ca798->PDisplayManager.Data->SetGamma(FableWindowGamma_0138e0b8,false);
            return 0;
        case 1:
            ActivateWindow(FableWindowSystemManager_013ca798,false);
            if (FableWindowSystemManager_013ca798->PDisplayManager.Data) {
                FableWindowGamma_0138e0b8=FableWindowSystemManager_013ca798->PDisplayManager.Data->GetGamma();
                FableWindowSystemManager_013ca798->PDisplayManager.Data->SetGamma(1.0f,false);
            }
            return 0;
        }
        break;
    case 7:
        FableWindowSystemManager_013ca798->SetHasInputFocus(true);
        if (FableWindowSystemManager_013ca798->PInputManager.Data)
            FableWindowSystemManager_013ca798->PInputManager.Data->ClearEvents();
        if (FableWindowSystemManager_013ca798->PSoundManager.Data && FableSoundDeactivated_013ca610) {
            FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSlot46(0x8000,FableSavedSoundVolume_013ca60c,0.0f);
            FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSetEnabled_009915e0(true);
            FableWindowSystemManager_013ca798->ForceMusicPlay=true;
            FableSoundDeactivated_013ca610=false;
        }
        return 0;
    case 8:
        FableWindowSystemManager_013ca798->SetHasInputFocus(false);
        if (FableWindowSystemManager_013ca798->PSoundManager.Data && !FableSoundDeactivated_013ca610) {
            FableSavedSoundVolume_013ca60c=FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSlot47(0x8000);
            FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSlot46(0x8000,0.0f,0.0f);
            if (!FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSlot49(FableWindowSystemManager_013ca798->ForceMusicPlayOffset))
                FableWindowSystemManager_013ca798->ForceMusicPlayOffset=0.0f;
            FableWindowSystemManager_013ca798->PSoundManager.Data->UnknownSetEnabled_009915e0(false);
            FableSoundDeactivated_013ca610=true;
        }
        if (FableWindowSystemManager_013ca798->PInputManager.Data)
            FableWindowSystemManager_013ca798->PInputManager.Data->ClearEvents();
        return 0;
    case 0xf:
        if (FableWindowSystemManager_013ca798->PDisplayManager.Data)
            FableWindowSystemManager_013ca798->PDisplayManager.Data->SwapScreens();
        break;
    case 0x102:
        if (FableWindowSystemManager_013ca798->IsApplicationActive()) {
            if (lParam<0) GInkeyBuffer.AddChar(static_cast<wchar_t>(wParam),false);
            else GInkeyBuffer.AddChar(static_cast<wchar_t>(wParam),true);
            return 0;
        }
        break;
    case 0x1c:
        FableWindowSystemManager_013ca798->SetAsActive(wParam!=0);
        return 0;
    case 0x104:
    case 0x105:
        if (wParam==0x12 || wParam==0x79) return 0;
        break;
    case 0x112:
        if (wParam==0xf140 || wParam==0xf170) {
            FableWindowSystemManager_013ca798->ApplicationActive=false;
            FableWindowSystemManager_013ca798->EverBeenActive=true;
            return 0;
        }
        break;
    case 0x218:
        if (wParam==0 || wParam==4) {
            FableWindowSystemManager_013ca798->ApplicationActive=false;
            FableWindowSystemManager_013ca798->EverBeenActive=true;
            return 0;
        }
        break;
    case 0x201: FableWindowSystemManager_013ca798->MouseButton0Pressed=true; return 0;
    case 0x202: FableWindowSystemManager_013ca798->MouseButton0Pressed=false; return 0;
    case 0x204: FableWindowSystemManager_013ca798->MouseButton2Pressed=true; return 0;
    case 0x205: FableWindowSystemManager_013ca798->MouseButton2Pressed=false; return 0;
    case 0x207: FableWindowSystemManager_013ca798->MouseButton1Pressed=true; return 0;
    case 0x208: FableWindowSystemManager_013ca798->MouseButton1Pressed=false; return 0;
    case 0x20b:
        if (static_cast<unsigned short>(wParam>>16)==1) FableWindowSystemManager_013ca798->MouseButton3Pressed=true;
        else if (static_cast<unsigned short>(wParam>>16)==2) FableWindowSystemManager_013ca798->MouseButton4Pressed=true;
        return 1;
    case 0x20c:
        if (static_cast<unsigned short>(wParam>>16)==1) FableWindowSystemManager_013ca798->MouseButton3Pressed=false;
        else if (static_cast<unsigned short>(wParam>>16)==2) FableWindowSystemManager_013ca798->MouseButton4Pressed=false;
        return 1;
    case 0x20a:
        FableWindowSystemManager_013ca798->MouseZMove+=static_cast<short>(wParam>>16);
        return 0;
    }
    return DefWindowProcW(window,message,wParam,lParam);
}
