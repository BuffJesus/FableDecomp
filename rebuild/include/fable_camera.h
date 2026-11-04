#pragma once

// CCamera and the polymorphic CAIGameCameraBase as the reconstructed
// CMainGameComponent code sees them.
//
// CCamera: Ego_r.pdb layout (0x38 bytes). Only the inline accessor retail
// code uses is modelled; it is returned by value through a hidden pointer.
//
// CAIGameCameraBase: retail RTTI vtable 0x0125C1F4 (25 slots), FableWin donor
// 0x041AB29C (tools/rtti_map.py CAIGameCameraBase CGameCameraManager). Slots 0
// and 1 are pure in the base; the concrete CGameCameraManager names them
// Update (0) and GetRenderCamera(float) const (1). Only those two slots are
// initially modelled. Slots 2-10 now follow in PDB/retail order; later slots
// remain outside this partial interface.

#include "fable_2d_vector.h"
#include "rebuild_abi.h"

// Ego_r/FableWin CRightHandedSet: Up +0, Forward +C, size 18.
class CRightHandedSet
{
public:
    const C3DVector& GetUp() const { return Up; }
    const C3DVector& GetForward() const { return Forward; }
    // Original Forward cross Up; component expression of C3DVector operator^.
    C3DVector GetRight() const {
        return C3DVector(Forward.Y*Up.Z-Forward.Z*Up.Y,
                         Forward.Z*Up.X-Forward.X*Up.Z,
                         Forward.X*Up.Y-Forward.Y*Up.X);
    }
    void SetUp(const C3DVector& up) { Up=up; }
    void SetForward(const C3DVector& forward) { Forward=forward; }
    void Init(const C3DVector& up,const C3DVector& forward) { Up=up; Forward=forward; }
    void Normalise() { Up.AccurateNormalise(); Forward.AccurateNormalise(); }
    C3DVector Up;
    C3DVector Forward;
};
FABLE_STATIC_ASSERT(sizeof(CRightHandedSet)==0x18);

class CCamera
{
public:
    CCamera() {}
    CCamera(const CCamera& other);
    CCamera(const C3DVector& world_position,const C3DVector& target_position,float fov,bool is_2d);
    // FableWin ?PeekWorldPos@CCamera@@QBEABVC3DVector@@XZ; inline in retail.
    const C3DVector& PeekWorldPos() const
    {
        return WorldPosition;
    }

    void PointAt(const C3DVector& target_position); // retail 00A0C2B0
    void SetView(const C3DVector& view_direction,const C3DVector& up); // retail 00A0BE30
    void SetFOV(float h_angle,float v_angle); // retail 00A0BED0
    unsigned long GetFOVFlags() const; // retail 00A0BE80
    float GetHorizontalFOV() const; // retail 00A0BE90
    float GetVerticalFOV() const; // retail 00A0BEA0
    void MoveSideways(float world_units); // retail 00A0C1B0
    void MoveForward(float world_units); // retail 00A0C250

    float GetAngleXY() const; // retail 00A0C380
    float GetAngleYZ() const; // retail 00A0C3E0
    float GetAngleXZ() const; // retail 00A0C450

    C3DVector WorldPosition;                 // +0x00
    CRightHandedSet RHSet;                   // +0x0C
    bool HeightLocked;                       // +0x24
    bool ViewVecZLocked;                     // +0x25
    unsigned long FOVFlags;                  // +0x28
    float HorizontalFOV;                     // +0x2C
    float VerticalFOV;                       // +0x30
    float Zoom;                              // +0x34
};

FABLE_STATIC_ASSERT(sizeof(CCamera) == 0x38);

class CEnginePrimitivePrepareInfo;
class CGameCameraManager;

class CAIGameCameraBase
{
public:
    virtual void Update() = 0;                                   // [0] +0x00
    // FableWin ?GetRenderCamera@...@@UBE?AVCCamera@@M@Z.
    virtual CCamera GetRenderCamera(float interpolate) const = 0; // [1] +0x04
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const; // [2]
    virtual bool IsPosInViewingFrustrum(const C3DVector& pos) const; // [3]
    virtual bool Is2DAreaInViewingFrustrum(const C3DVector& pos, float radius) const; // [4]
    virtual bool IsVolumeInViewingFrustrum(const C3DVector& pos, float radius) const; // [5]
    virtual bool IsLineOfSightToPos(const C3DVector& pos) const; // [6]
    virtual float GetControlAngleXY() const; // [7]
    virtual void SetChangeInControlDirection(); // [8]
    // VC7.1 reverses the slots of adjacent overloads: const declaration first.
    virtual bool GetCameraManager(const CGameCameraManager** manager) const; // [10]
    virtual bool GetCameraManager(CGameCameraManager** manager); // [9]

};
