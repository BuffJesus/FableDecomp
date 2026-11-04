#include "fable_game.h"

// Retail 004175da; uses the shared primary/secondary dispatch layout.
bool CMainGameComponent::OnPostDeviceReset() { InitialiseFonts(); return true; }
