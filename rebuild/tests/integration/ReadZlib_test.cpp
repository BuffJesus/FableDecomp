// Behaviour fixture for CDataInputStream::ReadZlibCompressed 0x00996230: a
// 32-bit compressed size, that many bytes into a temporary buffer, a stack
// CCompressorZlib (workspace size, base constructor, Initialise) inflating
// them into the caller's buffer, CAFile's error callback when the inflated
// size is not `size`, then the compressor and the buffer are destroyed. The
// compressor members, the chunk-overflow read and vector(n) are fixture-side
// recorders.
#include <new>
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_filesystem.h"
#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char g_source[128];
static unsigned g_sourceLength;
static char g_log[64];
static unsigned g_logLength;
static unsigned long g_decompressReturn, g_workspace, g_seenSize, g_seenCapacity;
static const void* g_seenData;
static void* g_seenOutput;
static void Log(char c) { if (g_logLength < sizeof(g_log) - 1) g_log[g_logLength++] = c; g_log[g_logLength] = 0; }

void CDataInputStream::ReadWithSrcChunkOverflow(void* out, long size)
{
    memcpy(out, g_source + StreamPos, size);
    StreamPos += size;
    CurrentSourcePtr = g_source + StreamPos;
    SourceChunkBytesRemaining = 0;
    Log('o');
}
namespace _STL
{
    template <> vector<unsigned char>::vector(size_t n)
        : _Vector_base<unsigned char, allocator<unsigned char> >(allocator<unsigned char>())
    {
        _M_start = static_cast<unsigned char*>(malloc(n ? n : 1));
        memset(_M_start, 0, n);
        _M_finish = _M_start + n;
        _M_end_of_storage._M_data = _M_finish;
        Log('v');
    }
}
unsigned long FABLE_FASTCALL CCompressorZlib::GetRequiredWorkspaceSize() { Log('w'); return g_workspace; }
CACompressor::CACompressor(long workspaceSize) : Workspace(0), WorkspaceSize(workspaceSize), CompressionOn(false) { Log('b'); }
void CCompressorZlib::Initialise() { Log('i'); ZlibInitialised = false; }
unsigned long CCompressorZlib::InternalDecompress(const void* data, unsigned long size, void* output, unsigned long capacity)
{
    Log('d');
    g_seenData = data; g_seenSize = size; g_seenOutput = output; g_seenCapacity = capacity;
    const unsigned char* in = static_cast<const unsigned char*>(data);
    unsigned char* out = static_cast<unsigned char*>(output);
    for (unsigned long i = 0; i < size && i < capacity; ++i) out[i] = (unsigned char)(in[i] ^ 0x5a);
    return g_decompressReturn;
}
CCompressorZlib::~CCompressorZlib() { Log('x'); }
// The vtable's other slots (never reached here).
void CCompressorZlib::Compress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CCompressorZlib::CompressAsImage(const void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) {}
void CCompressorZlib::Decompress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::CompressAsImage(void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) {}
void CACompressor::Compress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::Decompress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::SetQuality(float) {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
void (FABLE_FASTCALL* CAFile::m_errorCallback)() = 0;
static void FABLE_FASTCALL OnError() { Log('!'); }

int main()
{
    int cases = 0;
    unsigned char raw[sizeof(CDataInputStream) + 8];
    static const long sizes[] = { 1, 7, 20 };
    for (int k = 0; k < 3; ++k) for (unsigned n = 0; n < 24; n += 5) for (int good = 0; good < 2; ++good)
        for (int callback = 0; callback < 2; ++callback) for (unsigned available = 0; available < 40; available += 13)
    {
        const long size = sizes[k];
        memcpy(g_source, &n, 4);
        for (unsigned i = 0; i < n; ++i) g_source[4 + i] = (unsigned char)(0x30 + 3 * i);
        g_sourceLength = 4 + n;
        g_decompressReturn = good ? size : size + 1;
        g_workspace = 0x1000 + n;
        CAFile::m_errorCallback = callback ? &OnError : 0;
        g_logLength = 0; g_log[0] = 0;
        memset(raw, 0xa5, sizeof(raw));
        CDataInputStream* s = reinterpret_cast<CDataInputStream*>(raw + 4);
        s->StreamPos = 0; s->StreamSize = g_sourceLength; s->CurrentSourcePtr = g_source;
        s->SourceChunkStreamPos = 0; s->SourceChunkBytesRemaining = available < g_sourceLength ? available : g_sourceLength;
        const unsigned long chunk = s->SourceChunkBytesRemaining;
        unsigned char out[32];
        memset(out, 0xee, sizeof(out));
        s->CDataInputStream::ReadZlibCompressed(out, size);
        // Expected call order: [o] size, v, [o] bytes, w b i d [!] x.
        char want[16]; unsigned w = 0;
        if (chunk < 4) want[w++] = 'o';
        want[w++] = 'v';
        if (n > 0 && (chunk < 4 || chunk - 4 < n)) want[w++] = 'o';
        want[w++] = 'w'; want[w++] = 'b'; want[w++] = 'i'; want[w++] = 'd';
        if (callback && !good) want[w++] = '!';
        want[w++] = 'x'; want[w] = 0;
        bool ok = !strcmp(g_log, want) && g_seenSize == n && g_seenOutput == out && g_seenCapacity == (unsigned long)size
            && s->StreamPos == g_sourceLength && raw[3] == 0xa5 && raw[sizeof(raw) - 4] == 0xa5;
        for (unsigned long i = 0; ok && i < (unsigned long)size && i < n; ++i) ok = out[i] == (unsigned char)(g_source[4 + i] ^ 0x5a);
        if (!ok) { printf("READ_ZLIB_FAIL size=%ld n=%u good=%d cb=%d avail=%u log=%s want=%s\n", size, n, good, callback, available, g_log, want); return 1; }
        ++cases;
    }
    for (long size = -1; size <= 0; ++size)
    {
        g_logLength = 0; g_log[0] = 0;
        CDataInputStream* s = reinterpret_cast<CDataInputStream*>(raw + 4);
        s->StreamPos = 0; s->SourceChunkBytesRemaining = 4;
        s->CDataInputStream::ReadZlibCompressed(raw, size);
        if (g_logLength || s->StreamPos != 0) { printf("READ_ZLIB_FAIL empty size=%ld\n", size); return 1; }
        ++cases;
    }
    printf("READ_ZLIB_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
