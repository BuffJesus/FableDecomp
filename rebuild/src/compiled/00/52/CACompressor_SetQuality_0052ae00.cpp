// CACompressor::SetQuality @ 0x0052AE00 (Ego_r ?SetQuality@CACompressor@@...; slot 4):
// the base compressor has no quality setting (vtable 0x0129B424; CCompressorZlib's
// vtable 0x01245974 inherits it).
#include "engine/CACompressor.h"

void CACompressor::SetQuality(float)
{
}
