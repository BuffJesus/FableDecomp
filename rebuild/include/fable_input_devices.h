#pragma once

// The input devices CInputManager drives: Ego_r.pdb classes with retail
// layouts (STLport's list is one node pointer) and the slot order of the
// retail base vtables CMouse 0x0129DBC4, CKeyboard 0x0129DBA8 and CJoystick
// 0x0129C0EC (pure slots are _purecall 0x00BFEA08 there). The DX
// implementations (CMouseDX, CKeyboardDX, CJoystickDX) fill the pure slots.

#include "fable_input_event.h"
#include "fable_stlport_list.h"

class C2DVector;
class CInputManager;
class CSurface;
class CWideString;

// Ego_r CMaintainedEvent (0x38): an event a device keeps re-reporting.
class CMaintainedEvent : public CBaseClass
{
public:
    CMaintainedEvent();
    CInputEvent Event;
};

class CMouse
{
public:
    virtual ~CMouse();                                              // [0]
    virtual void GetPos(C2DVector* position) = 0;                   // [1]
    virtual const C2DVector& PeekPos() const = 0;                   // [2]
    virtual C2DVector PeekScreenPos() const = 0;                    // [3]
    virtual void SetPos(const C2DVector* position) = 0;             // [4]
    virtual void Update() = 0;                                      // [5]
    virtual bool SetExclusiveMode(bool exclusive) = 0;              // [6]
    virtual void SetInputOn(bool on) = 0;                           // [7]
    virtual void SetRecentringOn(bool on) = 0;                      // [8]
    virtual bool IsRecentringOn() const = 0;                        // [9]
    virtual void SetDrawCursor(bool draw) = 0;                      // [10]
    virtual bool IsDrawingCursor() const = 0;                       // [11]
    virtual void SetMouseFiltering(bool on) = 0;                    // [12]
    virtual void SetMouseCursor(const CSurface& cursor) = 0;        // [13]
    virtual void SetHasInputFocus(bool hasFocus);                   // [14] 0x00A673A0
    virtual bool IsLMBDown() const = 0;                             // [15]
    virtual bool IsMMBDown() const = 0;                             // [16]
    virtual bool IsRMBDown() const = 0;                             // [17]

    // Ego_r ?ClearAllMaintainedEvents@CMouse@@QAEXXZ, retail 0x00A66C00.
    void ClearAllMaintainedEvents();

protected:
    // Ego_r ?EmptyEventStore@CMouse@@IAEXXZ; expanded by its callers.
    void EmptyEventStore()
    {
        NoStoredEvents = 0;
    }

    CInputEvent EventStore[256];                                    // +0x0004
    long NoStoredEvents;                                            // +0x3404
    mutable long NoEventScansRunning;                               // +0x3408
    _STL::list<CMaintainedEvent> MaintainedEventsList;              // +0x340C
    CInputManager* InputManager;                                    // +0x3410
};

class CKeyboard
{
public:
    virtual ~CKeyboard();                                           // [0]
    virtual bool Update() = 0;                                      // [1]
    virtual bool SetExclusiveMode(bool exclusive) = 0;              // [2]
    virtual void SetHasInputFocus(bool hasFocus);                   // [3] 0x00A66AC0
    virtual bool GetKeyName(EInputKey key, CWideString& name) const; // [4] 0x00A66AD0

    // Ego_r ?ClearAllMaintainedEvents@CKeyboard@@QAEXXZ, retail 0x00A66860.
    void ClearAllMaintainedEvents();

protected:
    // Ego_r ?EmptyEventStore@CKeyboard@@IAEXXZ; expanded by its callers.
    void EmptyEventStore()
    {
        NoStoredEvents = 0;
    }

    CInputEvent EventStore[256];                                    // +0x0004
    long NoStoredEvents;                                            // +0x3404
    mutable long NoEventScansRunning;                               // +0x3408
    _STL::list<CMaintainedEvent> MaintainedEventsList;              // +0x340C
    CInputManager* InputManager;                                    // +0x3410
};

class CJoystick
{
public:
    class CActiveRumble;
    class CInitRumble;

protected:
    virtual bool EventScanStart(CInputEventScanInfo* info, CInputEvent* event) const;  // [0] 0x009E4230
    virtual bool EventScanNext(CInputEventScanInfo* info, CInputEvent* event) const;   // [1] 0x009E4280
    virtual void EventScanEnd(CInputEventScanInfo* info) const;                        // [2] 0x009E41C0
    virtual float GetXAxis() = 0;                                   // [3]
    virtual float GetYAxis() = 0;                                   // [4]
    virtual float GetXAxis2() = 0;                                  // [5]
    virtual float GetYAxis2() = 0;                                  // [6]

public:
    virtual void SetAxes(const C2DVector& axes) = 0;                // [7]
    virtual long GetJoystickDeviceNumber() const = 0;               // [8]
    virtual ~CJoystick();                                           // [9]
    virtual void Update();                                          // [10] 0x009E41B0
    virtual void UpdateInsertionsAndRemovals(unsigned long inserted, unsigned long removed); // [11] 0x009E4930
    virtual bool SetExclusiveMode(bool exclusive);                  // [12] 0x009E41A0
    virtual CActiveRumble* GetRumbleWithID(long id) = 0;            // [13]
    virtual const CActiveRumble* PeekRumbleWithID(long id) const = 0; // [14]
    virtual void Clear();                                           // [15] 0x009E4940
    virtual void SetHasInputFocus(bool hasFocus);                   // [16] 0x009E4950
    virtual void AddRumble(long id, const CInitRumble& rumble, bool replace) = 0; // [17]
    virtual void ClearAllRumbles() = 0;                             // [18]

    // Ego_r ?ClearAllMaintainedEvents@CJoystick@@QAEXXZ, retail 0x009E42C0.
    void ClearAllMaintainedEvents();

protected:
    // Ego_r ?EmptyEventStore@CJoystick@@IAEXXZ (retail keeps a copy at
    // 0x009E41D0 and expands it in its callers).
    void EmptyEventStore()
    {
        NoStoredEvents = 0;
    }

    CInputEvent EventStore[64];                                     // +0x004
    CInputManager* InputManager;                                    // +0xD04
    bool ExclusiveMode;                                             // +0xD08
    _STL::list<CMaintainedEvent> MaintainedEventsList;              // +0xD0C
    long NoStoredEvents;                                            // +0xD10
    mutable long NoEventScansRunning;                               // +0xD14
};

FABLE_STATIC_ASSERT(sizeof(CMaintainedEvent) == 0x38);
FABLE_STATIC_ASSERT(sizeof(CMouse) == 0x3414);
FABLE_STATIC_ASSERT(sizeof(CKeyboard) == 0x3414);
FABLE_STATIC_ASSERT(sizeof(CJoystick) == 0xD18);
