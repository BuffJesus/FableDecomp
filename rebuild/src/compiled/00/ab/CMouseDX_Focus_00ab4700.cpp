// CMouseDX's window and focus members (retail 0x00AB4700..0x00AB4907): placing the Windows cursor, reading the
// position in screen space, and acquiring the DirectInput device only while the application has the input focus.
// In exclusive mode the device owns the mouse and the Windows cursor is left alone.
#include "fable_dinput8.h"
#include "fable_mouse_dx.h"
#include "fable_gfuninitialise.h"
#include "fable_win32_cursor.h"

// 0x00AB4700
void CMouseDX::SetPos(const C2DVector* position)
{
    Pos = *position;
    NonExclusiveOldMouseX = Pos.X;
    NonExclusiveOldMouseY = Pos.Y;
    if (!ExclusiveMode)
    {
        void* window = GFGetSystemManager()->GetApplicationWinHandle();
        FableWin32Point origin = { 0, 0 };
        ClientToScreen(window, &origin);
        FableWin32Rect client;
        GetClientRect(window, &client);
        OffsetRect(&client, origin.x, origin.y);
        FableWin32Point cursor;                      // built first: retail converts x before y
        cursor.x = long(client.left + Pos.X);
        cursor.y = long(client.top + Pos.Y);
        SetCursorPos(cursor.x, cursor.y);
    }
}

// 0x00AB47D0: losing focus releases the device, gaining it acquires it; Acquired follows what DirectInput accepted
// (losing focus also counts DI_NOEFFECT, "was not acquired").
void CMouseDX::SetHasInputFocus(bool hasFocus)
{
    if (DMouse)
    {
        if (!hasFocus)
        {
            if (Acquired == true)
            {
                const HRESULT result = DMouse->Unacquire();
                if (result == DI_OK || result == DI_NOEFFECT)
                    Acquired = false;
            }
        }
        else if (!Acquired && DMouse->Acquire() == DI_OK)
            Acquired = true;
    }
}

// 0x00AB4830
void CMouseDX::UpdateAcquired()
{
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->IsApplicationInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
}

// 0x00AB4870
C2DVector CMouseDX::PeekScreenPos() const
{
    if (ExclusiveMode)
        return Pos;
    void* window = GFGetSystemManager()->GetApplicationWinHandle();
    FableWin32Point origin = { 0, 0 };
    ClientToScreen(window, &origin);
    FableWin32Rect client;
    GetClientRect(window, &client);
    OffsetRect(&client, origin.x, origin.y);
    return C2DVector(client.left + Pos.X, client.top + Pos.Y);
}
