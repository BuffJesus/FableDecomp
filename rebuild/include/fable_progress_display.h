#pragma once

#include "fable_string.h"
#include "fable_counted_pointer.h"

// The progress display's abstract interface (FableWin CAProgressDisplay; the
// retail implementation is CProgressDisplay, vtable 0x012388B4). Slot order
// from that vtable, confirmed by the NProgressDisplay forwarders
// 0x009E9EF0..0x009E9FB0 ([vptr+4] StartProgress ... [vptr+0x18] IsActive):
//   0 scalar deleting dtor 0x0049B7D0, 1 StartProgress 0x00499AA0,
//   2 StopProgress 0x00499D90, 3 DisplayProgress 0x00499C50,
//   4 InitialiseTextures 0x0049A0B0, 5 SetToDisplayText 0x00499A70,
//   6 IsActive 0x0049B460.
// Do not include together with the generated flat "engine/CAProgressDisplay.h".
class CAProgressDisplay
{
public:
    virtual ~CAProgressDisplay() {}
    virtual void StartProgress(bool flag0, bool flag1, bool flag2,
        bool flag3, bool flag4, bool flag5) = 0;
    virtual void StopProgress(bool flag0, bool flag1) = 0;
    virtual void DisplayProgress(const CCharString& text, float fraction,
        bool forceRender, bool isDebug) = 0;
    virtual void InitialiseTextures(bool flag0, bool flag1) = 0;
    virtual void SetToDisplayText(bool displayText) = 0;
    virtual bool IsActive() const = 0;
};

// FableWin ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z,
// retail 0x009E9F40: fastcall, text in ECX, the first bool in DL, the
// fraction and the second bool on the stack. Boot-time callers pass -1.0f
// (no bar position) and false/false.
namespace NProgressDisplay
{
    void FABLE_FASTCALL StopProgress(bool flag0,bool flag1); // retail009E9F20
    // ?PProgressDisplay@NProgressDisplay@@3V?$CCountedPointer@VCAProgressDisplay@@@@A,
    // retail 0x013CAA38 (the same object fable_boot.h models as
    // g_FableProgressDisplay_013CAA38 for the boot path).
    extern CCountedPointer<CAProgressDisplay> PProgressDisplay;

    void FABLE_FASTCALL DisplayProgress(
        const CCharString& text,
        float fraction,
        bool forceRender,
        bool isDebug);

    // FableWin ?StartProgress@NProgressDisplay@@YIX_N00000@Z, retail
    // 0x009E9EF0 (first two bools in CL/DL, the other four on the stack).
    // Parameter names not recovered.
    void FABLE_FASTCALL StartProgress(
        bool flag0,
        bool flag1,
        bool flag2,
        bool flag3,
        bool flag4,
        bool flag5);

    // FableWin ?SetToDisplayText@NProgressDisplay@@YIX_N@Z, retail 0x009E9F90.
    void FABLE_FASTCALL SetToDisplayText(bool displayText);

    // FableWin ?IsActive@NProgressDisplay@@YI_NXZ, retail 0x009E9FB0 (the
    // progress-display singleton exists and its slot 6 says it is active).
    bool FABLE_FASTCALL IsActive();
}
