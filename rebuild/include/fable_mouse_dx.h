#pragma once
// Hand-owned (retail model): CMouseDX, the DirectInput mouse. Ego_r.pdb names, members and access; retail RTTI vtable
// 0x0129EA14 fills CMouse's slots (fable_input_devices.h): [1] GetPos 0x00AB46E0, [2] PeekPos 0x00AB5F00,
// [3] PeekScreenPos 0x00AB4870, [4] SetPos 0x00AB4700, [5] Update 0x00AB58E0, [6] SetExclusiveMode 0x00AB5710,
// [7..12] SetInputOn .. SetMouseFiltering 0x00AB5F10..0x00AB5F60, [13] SetMouseCursor 0x00AB5390,
// [14] SetHasInputFocus 0x00AB47D0, [15..17] IsLMBDown/IsMMBDown/IsRMBDown 0x00AB5F70..0x00AB5F90.
// Ego_r CMouseDX is 0x4868 over a 0x341C CMouse; retail's STLport list is one pointer, so CMouse is 0x3414 and every
// member below sits 8 bytes lower than in Ego_r (retail 0x4860).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_counted_pointer.h"
#include "fable_input_devices.h"

struct IDirectInputDevice8W;
class CInputManagerDX;

class CMouseDX : public CMouse
{
public:
    virtual ~CMouseDX();
    virtual void GetPos(C2DVector* position);
    virtual const C2DVector& PeekPos() const;
    virtual C2DVector PeekScreenPos() const;
    virtual void SetPos(const C2DVector* position);
    virtual void Update();
    virtual bool SetExclusiveMode(bool exclusive);
    virtual void SetInputOn(bool on);
    virtual void SetRecentringOn(bool on);
    virtual bool IsRecentringOn() const;
    virtual void SetDrawCursor(bool draw);
    virtual bool IsDrawingCursor() const;
    virtual void SetMouseFiltering(bool on);
    virtual void SetMouseCursor(const CSurface& cursor);
    virtual void SetHasInputFocus(bool hasFocus);
    virtual bool IsLMBDown() const;
    virtual bool IsMMBDown() const;
    virtual bool IsRMBDown() const;

    // Ego_r ?UpdateAcquired@CMouseDX@@QAEXXZ (retail 0x00AB4830): hold the device only while the application is
    // active and has input focus.
    void UpdateAcquired();

protected:
    friend struct CMouseDXLayoutCheck;
    C2DVector Pos;                                   // +0x3414
    IDirectInputDevice8W* DMouse;                    // +0x341C
    CInputManagerDX* InputManagerDX;                 // +0x3420 (Ego_r: CInputManagerDX&)
    bool Acquired;                                   // +0x3424
    float MouseStep;                                 // +0x3428
    bool NonExclusiveOldButton0Pressed;              // +0x342C
    bool NonExclusiveOldButton1Pressed;              // +0x342D
    bool NonExclusiveOldButton2Pressed;              // +0x342E
    bool NonExclusiveOldButton3Pressed;              // +0x342F
    bool NonExclusiveOldButton4Pressed;              // +0x3430
    float NonExclusiveOldMouseX;                     // +0x3434
    float NonExclusiveOldMouseY;                     // +0x3438
    bool ExclusiveMode;                              // +0x343C
    bool InputEnabled;                               // +0x343D
    bool ForceRecentre;                              // +0x343E
    bool DrawingCursor;                              // +0x343F
    unsigned char ExclusiveModeData[0x1400];         // +0x3440 DIDEVICEOBJECTDATA[256]
    unsigned long ExclusiveModeEventsCount;          // +0x4840
    unsigned long NoExclusiveModeEvents;             // +0x4844
    bool LMBOn;                                      // +0x4848
    bool MMBOn;                                      // +0x4849
    bool RMBOn;                                      // +0x484A
    bool Filtering;                                  // +0x484B
    C3DVector LastUpdateMovement;                    // +0x484C
    CCountedPointer<CSurface> CursorGraphic;         // +0x4858
};

struct CMouseDXLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, Pos) == 0x3414);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, MouseStep) == 0x3428);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, NonExclusiveOldMouseX) == 0x3434);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, ExclusiveMode) == 0x343C);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, DrawingCursor) == 0x343F);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, ExclusiveModeEventsCount) == 0x4840);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, LMBOn) == 0x4848);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, Filtering) == 0x484B);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, LastUpdateMovement) == 0x484C);
    FABLE_STATIC_ASSERT(offsetof(CMouseDX, CursorGraphic) == 0x4858);
};
FABLE_STATIC_ASSERT(sizeof(CMouseDX) == 0x4860);
