#pragma once
#include "fable_camera.h"
class CCameraModeDef;
template<class T> class CDefPointer;
namespace NCameraModes {
// FableWin enum, partial values sufficient to type the original virtual ABI.
enum EMode { CAMERA_MODE_NULL = 0 };
// FableWin/retail virtual prefix through Recentre; fields and later slots
// remain unmodelled. CBase is not a CBaseClass subclass.
class CBase {
public:
    CBase(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&);
    virtual ~CBase(); // [0]
    virtual void Update() = 0; // [1]
    virtual EMode GetModeType() const = 0; // [2], const verified separately
    virtual void OnCameraInsideObservedThing(); // [3]
    virtual bool IsUseRightStickLookAround() const; // [4]
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const; // [5]
    virtual bool IsShortenOnOcclusion() const; // [6]
    virtual bool IsRaiseUpOnHeroNearby() const; // [7]
    virtual bool IsStretchAroundHero() const; // [8]
    virtual bool IsAvoidLandscape() const; // [9]
    virtual bool IsFOVChange() const; // [10]
    virtual bool IsTrackMeleeOpponents() const; // [11]
    virtual bool IsTrackMeleeOpponentsAutoFOVChange() const; // [12]
    virtual bool IsFinished() const; // [13]
    virtual bool IsStringAndCage() const; // [14]
    virtual float GetObservedThingCurrentMeshHeight() const; // [15]
    virtual void Recentre(); // [16]
};
}
