// CACompressor::CompressAsImage @ 0x0052ADD0 (Ego_r ?CompressAsImage@CACompressor@@...; slot 1):
// the base compressor compresses nothing (vtable 0x0129B424; CCompressorZlib's
// vtable 0x01245974 inherits it).
#include "engine/CACompressor.h"

void CACompressor::CompressAsImage(void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&)
{
}
