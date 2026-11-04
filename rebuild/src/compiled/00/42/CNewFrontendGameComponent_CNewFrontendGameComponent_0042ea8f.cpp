// CNewFrontendGameComponent::CNewFrontendGameComponent @ 0x0042EA8F
// Size-optimised in retail: /O2 gives 324 bytes; the fldz stores and the
// out-of-line CMainGameComponentInit ctor call only appear under /Os.
// FileNames/FileTimes are STLport vectors; their construction path
// (fable_stlport_vector.h) is what fixes the scheduling around SaveGame.
#pragma optimize("s", on)

#include "fable_game.h"

CNewFrontendGameComponent::CNewFrontendGameComponent(
    CGame& game,
    const CNewFrontendGameComponentInit&)
    : CGameComponent(game),
      CurrentMovement(CURSOR_MOVEMENT_NONE),
      StartedRepeatingMovement(false),
      TimeLastMovementHappened(GFGetTime()),
      m_bAllowInputs(true),
      m_bCreateGame(false),
      SaveGame(L""),
      PAVITextureShow(0),
      PAVITextureDecode(0),
      AVIPlaying(false),
      XMVCodeLoaded(false),
      m_pFrontEndManager(0),
      m_fLastRealUpdateTime(GFGetTime()),
      m_fRepeatDelay(0.333f),
      m_dwDPadTriggerState(0),
      LastActiveJoystickDeviceNumber(0),
      unknown144_(true)
{
    m_bInitMainGame.InitialWorldName = L"";
    m_bInitMainGame.SaveGameName = L"";
    CreatedMarkerFile = false;
    CreateDevFrontEnd = false;
}