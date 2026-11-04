#include "fable_display_viewport.h"
#include "fable_gamma_ramp.h"

void CDisplayManager::SetGamma(float gamma, bool calibrate)
{
    if (gamma < 0.3f) gamma = 0.3f;
    else if (gamma > 6.0f) gamma = 6.0f;
    Gamma = gamma;
    D3DGAMMARAMP ramp;
    long index=0;
    for (long value=0; value<=0xff00; value+=0x100, ++index) {
        float scaled = value * gamma;
        if (scaled < 0.0f) scaled = 0.0f;
        else if (scaled > 65280.0f) scaled = 65280.0f;
        unsigned short entry = static_cast<unsigned short>(scaled);
        ramp.red[index] = entry;
        ramp.green[index] = entry;
        ramp.blue[index] = entry;
    }
    D3DDevice->SetGammaRamp(0, calibrate ? 1 : 0, &ramp);
}
