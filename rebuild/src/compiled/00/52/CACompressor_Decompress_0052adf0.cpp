// CACompressor::Decompress @ 0x0052ADF0 (Ego_r ?Decompress@CACompressor@@...; slot 3):
// the base compressor decompresses nothing (vtable 0x0129B424; CCompressorZlib's
// vtable 0x01245974 inherits it).
#include "engine/CACompressor.h"

void CACompressor::Decompress(void*, unsigned long, void*, unsigned long, CCompressionInfo&)
{
}
