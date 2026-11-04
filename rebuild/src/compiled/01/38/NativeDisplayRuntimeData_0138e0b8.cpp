// Typed display and window runtime state. The original .data initializer is
// gamma=1; the remaining fields occupy the zero-filled tail of retail .data.
#include "fable_system_activation.h"
#include "fable_virtual_pixels.h"
#include "fable_window_procedure.h"

float FableWindowGamma_0138e0b8 = 1.0f;
float FableSavedSoundVolume_013ca60c = 0.0f;
bool FableSoundDeactivated_013ca610 = false;
CSystemManager* FableWindowSystemManager_013ca798 = 0;
double FableVirtualPixelScaleX_013ca800 = 0.0;
double FableVirtualPixelScaleY_013ca808 = 0.0;
