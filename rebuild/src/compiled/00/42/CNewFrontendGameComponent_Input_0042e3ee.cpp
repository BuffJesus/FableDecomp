// CNewFrontendGameComponent::Input @ 0x0042E3EE
// ?Input@CNewFrontendGameComponent@@AAEXXZ. VC7.1, x86. The retail body is
// size-optimised with a standard ebp frame (`and dword ptr [ebp-4],0`,
// compare-tree switches, `push ebp; mov ebp, esp`), hence the pragmas.
// Retail constructs and destroys a second CInputEvent it never reads.

#pragma optimize("s", on)
#pragma optimize("y", off)

#include "fable_game.h"
#include "fable_console.h"
#include "fable_input_event.h"
#include "fable_nui_manager.h"
#include "fable_system_manager.h"

using NUISystem::CManager;

// Frontend input request bits gathered from one frame's events (low word:
// directions + face buttons; the D-pad state in m_dwDPadTriggerState uses
// bits 0x10-0x80 alongside them).
enum
{
    FRONTEND_INPUT_UP = 0x1,
    FRONTEND_INPUT_DOWN = 0x2,
    FRONTEND_INPUT_LEFT = 0x4,
    FRONTEND_INPUT_RIGHT = 0x8,
    FRONTEND_INPUT_DPAD_UP = 0x10,
    FRONTEND_INPUT_DPAD_DOWN = 0x20,
    FRONTEND_INPUT_DPAD_LEFT = 0x40,
    FRONTEND_INPUT_DPAD_RIGHT = 0x80,
    FRONTEND_INPUT_A = 0x100,
    FRONTEND_INPUT_B = 0x200,
    FRONTEND_INPUT_START = 0x400,
    FRONTEND_INPUT_BACK = 0x800,
    FRONTEND_INPUT_KEYBOARD_ARROWLEFT = 0x1000,
    FRONTEND_INPUT_KEYBOARD_ARROWRIGHT = 0x2000,
    FRONTEND_INPUT_KEYBOARD_SPACE = 0x4000,
    FRONTEND_INPUT_KEYBOARD_BACKSPACE = 0x8000,
    FRONTEND_INPUT_X = 0x10000,
    FRONTEND_INPUT_Y = 0x20000
};

void CNewFrontendGameComponent::Input()
{
    if (m_inputDelayTimer.m_bIsRunning)
    {
        g_FableInputManager_013B8388->ClearEvents();
        return;
    }

    CInputEventScanInfo scanInfo;
    CInputEvent event;
    CInputEvent unusedEvent;

    g_FableInputManager_013B8388->Update();

    unsigned long input = 0;
    bool more = g_FableInputManager_013B8388->EventScanStart(scanInfo, event);
    while (more)
    {
        switch (event.GetType())
        {
        case IE_KEY_PRESSED:
            CTBaseSingleton<CConsole>::Get().ProcessInput(event);
            if (event.GetKey() == KB_LEFT)
                input |= FRONTEND_INPUT_LEFT;
            if (event.GetKey() == KB_RIGHT)
                input |= FRONTEND_INPUT_RIGHT;
            if (event.GetKey() == KB_UP)
                input |= FRONTEND_INPUT_UP;
            if (event.GetKey() == KB_DOWN)
                input |= FRONTEND_INPUT_DOWN;
            if (event.GetKey() == KB_A)
                input |= FRONTEND_INPUT_A;
            if (event.GetKey() == KB_B)
                input |= FRONTEND_INPUT_B;
            if (event.GetKey() == KB_Y)
                input |= FRONTEND_INPUT_Y;
            CManager::GetInstance()->SetKeyPressed(event.GetKey());
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEY_DOWN);
            break;

        case IE_CHAR_PRESSED:
            CManager::GetInstance()->SetKeyPressed(event.GetChar());
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_CHAR_DOWN);
            break;

        case IE_LMB_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_LEFT_MOUSE_CLICK);
            break;

        case IE_LMB_RELEASED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_LEFT_MOUSE_RELEASE);
            break;

        case IE_MMB_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MIDDLE_MOUSE_CLICK);
            break;

        case IE_RMB_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_RIGHT_MOUSE_CLICK);
            break;

        case IE_MOUSE_MOVEMENT:
        {
            CManager::GetInstance()->MouseMovement =
                C2DVector(event.GetMouseMovement().X, event.GetMouseMovement().Y);
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
            break;
        }

        case IE_MOUSE_WHEEL_MOVEMENT:
        {
            if (event.GetMouseWheelMovement() > DELTA_NOTIONAL_ZERO)
                CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_WHEEL_UP);
            else if (event.GetMouseWheelMovement() < -DELTA_NOTIONAL_ZERO)
                CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_WHEEL_DOWN);
            break;
        }

        case IE_JOYSTICK_POSITION:
            if (m_bAllowInputs)
            {
                C2DVector position = event.GetJoystickPosition();
                if (position.Y > 0.25f)
                    input |= FRONTEND_INPUT_DOWN;
                else if (position.Y < -0.25f)
                    input |= FRONTEND_INPUT_UP;
                if (position.X > 0.25f)
                    input |= FRONTEND_INPUT_RIGHT;
                else if (position.X < -0.25f)
                    input |= FRONTEND_INPUT_LEFT;
            }
            break;

        case IE_JOYSTICK_BUTTON_PRESSED:
        {
            bool mainPlayer = true;
            if (CGameJoystickManager::Get().HasExplicitMainPlayerJoystickDeviceNumber())
                mainPlayer = CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()
                             == event.GetDeviceNumber();
            if (m_bAllowInputs && mainPlayer)
            {
                LastActiveJoystickDeviceNumber = event.GetDeviceNumber();
                switch (event.GetJoystickButton())
                {
                case XBOX_PAD_X_BUTTON:
                    input |= FRONTEND_INPUT_X;
                    break;
                case XBOX_PAD_Y_BUTTON:
                    input |= FRONTEND_INPUT_Y;
                    break;
                case XBOX_PAD_A_BUTTON:
                    input |= FRONTEND_INPUT_A;
                    break;
                case XBOX_PAD_B_BUTTON:
                    input |= FRONTEND_INPUT_B;
                    break;
                case XBOX_PAD_START_BUTTON:
                    input |= FRONTEND_INPUT_START;
                    break;
                case XBOX_PAD_BACK_BUTTON:
                    input |= FRONTEND_INPUT_BACK;
                    break;
                case XBOX_PAD_DPAD_UP_BUTTON:
                    m_dwDPadTriggerState |= FRONTEND_INPUT_DPAD_UP;
                    break;
                case XBOX_PAD_DPAD_DOWN_BUTTON:
                    m_dwDPadTriggerState |= FRONTEND_INPUT_DPAD_DOWN;
                    break;
                case XBOX_PAD_DPAD_LEFT_BUTTON:
                    m_dwDPadTriggerState |= FRONTEND_INPUT_DPAD_LEFT;
                    break;
                case XBOX_PAD_DPAD_RIGHT_BUTTON:
                    m_dwDPadTriggerState |= FRONTEND_INPUT_DPAD_RIGHT;
                    break;
                }
            }
            break;
        }

        case IE_JOYSTICK_BUTTON_RELEASED:
            if (m_bAllowInputs)
            {
                switch (event.GetJoystickButton())
                {
                case XBOX_PAD_DPAD_UP_BUTTON:
                    m_dwDPadTriggerState &= ~FRONTEND_INPUT_DPAD_UP;
                    break;
                case XBOX_PAD_DPAD_DOWN_BUTTON:
                    m_dwDPadTriggerState &= ~FRONTEND_INPUT_DPAD_DOWN;
                    break;
                case XBOX_PAD_DPAD_LEFT_BUTTON:
                    m_dwDPadTriggerState &= ~FRONTEND_INPUT_DPAD_LEFT;
                    break;
                case XBOX_PAD_DPAD_RIGHT_BUTTON:
                    m_dwDPadTriggerState &= ~FRONTEND_INPUT_DPAD_RIGHT;
                    break;
                }
            }
            break;

        case IE_MB4_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_BUTTON_4_CLICK);
            break;
        case IE_MB5_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_BUTTON_5_CLICK);
            break;
        case IE_MB6_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_BUTTON_6_CLICK);
            break;
        case IE_MB7_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_BUTTON_7_CLICK);
            break;
        case IE_MB8_PRESSED:
            CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_MOUSE_BUTTON_8_CLICK);
            break;
        }
        more = g_FableInputManager_013B8388->EventScanNext(scanInfo, event);
    }
    g_FableInputManager_013B8388->EventScanEnd(scanInfo);

    if (m_dwDPadTriggerState)
        input |= m_dwDPadTriggerState;

    if (input)
    {
        m_attractModeTimer.Reset();
        if (m_RepeatTimer.m_bIsRunning)
        {
            if (m_RepeatTimer.GetElapsedSeconds() < m_fRepeatDelay)
                return;
            m_fRepeatDelay = 0.085f;
            m_RepeatTimer.StartZero();
        }
        else
        {
            m_fRepeatDelay = 0.333f;
            m_RepeatTimer.StartZero();
        }
    }
    else
    {
        m_fRepeatDelay = 0.333f;
        m_RepeatTimer.Stop();
    }

    if (input & FRONTEND_INPUT_START)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_SELECT);
    else if (input & FRONTEND_INPUT_BACK)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_BACK);
    else if (input & FRONTEND_INPUT_A)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_SELECT);
    else if (input & FRONTEND_INPUT_B)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_BACK);
    else if (input & FRONTEND_INPUT_Y)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_DELETE);
    else if ((input & FRONTEND_INPUT_LEFT) || (input & FRONTEND_INPUT_DPAD_LEFT))
    {
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_LEFT);
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_SLIDER_LEFT);
    }
    else if ((input & FRONTEND_INPUT_RIGHT) || (input & FRONTEND_INPUT_DPAD_RIGHT))
    {
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_RIGHT);
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_SLIDER_RIGHT);
    }
    else if ((input & FRONTEND_INPUT_UP) || (input & FRONTEND_INPUT_DPAD_UP))
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_UP);
    else if ((input & FRONTEND_INPUT_DOWN) || (input & FRONTEND_INPUT_DPAD_DOWN))
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_DOWN);
    else if (input & FRONTEND_INPUT_KEYBOARD_ARROWLEFT)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEYBOARD_ARROWLEFT);
    else if (input & FRONTEND_INPUT_KEYBOARD_ARROWRIGHT)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEYBOARD_ARROWRIGHT);
    else if (input & FRONTEND_INPUT_KEYBOARD_SPACE)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEYBOARD_SPACE);
    else if (input & FRONTEND_INPUT_KEYBOARD_BACKSPACE)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_KEYBOARD_BACKSPACE);
    else if (input & FRONTEND_INPUT_X)
        CManager::GetInstance()->SetEvent(NUISystem::UI_EVENT_X);
}