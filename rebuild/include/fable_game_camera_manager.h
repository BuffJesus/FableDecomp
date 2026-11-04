#pragma once
#include "fable_camera.h"
#include "fable_counted_pointer.h"
namespace NCameraModes { class CBase; }
// Primary CAIGameCameraBase ancestry and query overrides from retail RTTI
// 0125C754 / FableWin 041AB20C. Remaining fields/overrides are not modelled.
class CGameCameraManager : public CAIGameCameraBase
{
public:
    virtual bool GetCameraManager(const CGameCameraManager** manager) const;
    virtual bool GetCameraManager(CGameCameraManager** manager);
    void RecentreCamera(); // retail 006978C0, FableWin 021FD460
private:
    friend struct CGameCameraManagerTestAccess;
    fable_u8 unknown04_[0x98 - 4];
    CCountedPointer<NCameraModes::CBase> CurrentMode; // retail +98; Ego_r +9C, FableWin +A0
};
