// Behaviour fixture for CDataInputStream::ReadNullTerminatedWString 0x00994D20:
// it reads 16-bit characters (from the current chunk while two bytes remain,
// through ReadWithSrcChunkOverflow otherwise) until a zero, appending each to
// the result, and returns the string without the terminator. CWideString's
// members and the overflow read are fixture-side recorders.
#define private public
#include "fable_string.h"
#undef private
#include "fable_data_input_stream.h"
#include <stdio.h>
#include <string.h>

static const wchar_t source[] = { 0xffff, L'a', 0x8000, L'z', 0, L't', 0 };
static unsigned g_overflowCalls, g_overflowBadSize;
static wchar_t g_appended[16];
static unsigned g_appendCount, g_ctors, g_copies, g_dtors;
void CDataInputStream::ReadWithSrcChunkOverflow(void* out, long size)
{
    if (size != 2 || StreamPos + 2 > sizeof(source)) { ++g_overflowBadSize; return; }
    memcpy(out, reinterpret_cast<const unsigned char*>(source) + StreamPos, size);
    StreamPos += size;
    ++g_overflowCalls;
}
CWideString::CWideString() : storage_(0) { ++g_ctors; }
CWideString::CWideString(const CWideString&) : storage_(0) { ++g_copies; }
CWideString::~CWideString() { ++g_dtors; }
const CWideString& CWideString::operator+=(wchar_t character)
{
    if (g_appendCount < 16) g_appended[g_appendCount] = character;
    ++g_appendCount;
    return *this;
}

int main()
{
    int cases = 0;
    for (unsigned start = 0; start < 3; ++start) for (unsigned available = 0; available < 12; ++available)
    {
        unsigned char raw[sizeof(CDataInputStream) + 8];
        memset(raw, 0xa5, sizeof(raw));
        CDataInputStream* stream = reinterpret_cast<CDataInputStream*>(raw + 4);
        const unsigned offset = start == 0 ? 0 : start == 1 ? 8 : 10;      // in bytes
        const unsigned count = start == 0 ? 4 : start == 1 ? 0 : 1;        // characters before the zero
        stream->StreamPos = offset;
        stream->StreamSize = sizeof(source);
        stream->CurrentSourcePtr = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(source) + offset);
        stream->SourceChunkStreamPos = offset;
        stream->SourceChunkBytesRemaining = available < sizeof(source) - offset ? available : sizeof(source) - offset;
        const unsigned initial = stream->SourceChunkBytesRemaining;
        g_overflowCalls = g_overflowBadSize = g_appendCount = g_ctors = g_copies = g_dtors = 0;
        {
            CWideString value = stream->CDataInputStream::ReadNullTerminatedWString();
        }
        // Characters taken from the chunk while two bytes remain; the rest overflow
        // (an odd byte left in the chunk is never split).
        const unsigned reads = count + 1, fromChunk = initial / 2 < reads ? initial / 2 : reads;
        bool ok = g_appendCount == count && !memcmp(g_appended, reinterpret_cast<const unsigned char*>(source) + offset, count * 2)
            && stream->StreamPos == offset + 2 * reads && g_overflowCalls == reads - fromChunk && !g_overflowBadSize
            && g_ctors == 1 && g_copies + g_dtors >= 1 && raw[3] == 0xa5 && raw[sizeof(raw) - 4] == 0xa5;
        if (!ok)
        {
            printf("WIDE_READER_FAIL start=%u available=%u appended=%u overflow=%u pos=%lu\n", start, available, g_appendCount, g_overflowCalls, (unsigned long)stream->StreamPos);
            return 1;
        }
        ++cases;
    }
    printf("WIDE_READER_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
