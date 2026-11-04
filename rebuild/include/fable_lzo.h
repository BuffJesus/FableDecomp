#pragma once

// LZO's i386 assembly decompressor (Ego_r public _lzo1x_decompress_asm_fast;
// retail 0x00C069D0: `push ebp/edi/esi/ebx/ecx/edx; cld`), a third-party
// boundary. cdecl, C linkage; returns an LZO status the callers ignore.
extern "C" int lzo1x_decompress_asm_fast(const unsigned char* src, unsigned long srcLength,
                                         unsigned char* dst, unsigned long* dstLength,
                                         void* workMemory);

// LZO's best-ratio compressor (Ego_r public _lzo1x_999_compress, retail
// 0x00C08130), the same third-party boundary; its work memory is
// LZO1X_999_MEM_COMPRESS bytes (14 * 16384 shorts).
enum { LZO1X_999_MEM_COMPRESS = 14 * 16384 * 2 };
extern "C" int lzo1x_999_compress(const unsigned char* src, unsigned long srcLength,
                                  unsigned char* dst, unsigned long* dstLength,
                                  void* workMemory);
