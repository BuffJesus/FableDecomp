// CMainGameComponent's static data (Ego_r.pdb names). ConstantFPS is
// initialised retail .data (0x01375550 = 15); the rest lie in the zero-filled
// tail of retail .data: AlwaysUpdateGameFrame 0x013B8688, ProfileUpdateTime
// 0x013B8690, ProfileRenderTime 0x013B8698, PMainGameComponent 0x013B86A0,
// UseOldInterpolationTiming 0x013B86A4.
#include "fable_game.h"

long CMainGameComponent::ConstantFPS = 15;
bool CMainGameComponent::AlwaysUpdateGameFrame = false;
double CMainGameComponent::ProfileUpdateTime = 0;
double CMainGameComponent::ProfileRenderTime = 0;
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
bool CMainGameComponent::UseOldInterpolationTiming = false;
