#include "fable_frontend_startup.h"

// Retail 0x0042EA8F. The unused frontend initializer is part of the ABI.
CNewFrontendGameComponent::CNewFrontendGameComponent(
    CGame& game, const CNewFrontendGameComponentInit&)
    : CGameComponent(game),
      CurrentMovement(0),
      StartedRepeatingMovement(false),
      TimeLastMovementHappened(GFGetTime()),
      m_bAllowInputs(true),
      m_bCreateGame(false),
      m_pFrontEndDef(0),
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
      WindowsMediaPlayerInstalled(true)
{
    m_bInitMainGame.InitialWorldName = L"";
    m_bInitMainGame.SaveGameName = L"";
    CreatedMarkerFile = false;
    CreateDevFrontEnd = false;
    // Retail leaves input, AVI rectangles and the two float frame timestamps
    // untouched here. Their initialization belongs to later startup phases.
}
