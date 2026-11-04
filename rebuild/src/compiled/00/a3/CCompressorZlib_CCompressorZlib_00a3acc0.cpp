#include "engine/CCompressorZlib.h"
inline void CCompressorZlib::FinishCompressStream() { if(ZlibInitialised) { z_deflateEnd(&Stream); ZlibInitialised=false; } }
inline void CCompressorZlib::FinishDecompressStream() { if(ZlibInitialised) { z_inflateEnd(&Stream); SetupStream(Stream,0,0,0,0); ZlibInitialised=false; } }
CCompressorZlib::~CCompressorZlib(){if(ZlibInitialised) { if(Compressing) FinishCompressStream(); else FinishDecompressStream(); }}