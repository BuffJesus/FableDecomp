#pragma once
// Fixture-only virtual slot bodies for the retail compressor model.
#include "engine/CCompressorZlib.h"
void CACompressor::CompressAsImage(void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) {}
void CACompressor::Compress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::Decompress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::SetQuality(float) {}
void CCompressorZlib::Compress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CCompressorZlib::CompressAsImage(const void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) {}
void CCompressorZlib::Decompress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
