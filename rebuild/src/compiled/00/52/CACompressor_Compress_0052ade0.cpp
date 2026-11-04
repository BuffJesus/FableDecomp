// CACompressor::Compress @ 0x0052ADE0 (Ego_r ?Compress@CACompressor@@...; slot 2):
// the base compressor compresses nothing (vtable 0x0129B424; CCompressorZlib's
// vtable 0x01245974 inherits it).
#include "engine/CACompressor.h"

void CACompressor::Compress(void*, unsigned long, void*, unsigned long, CCompressionInfo&)
{
}
