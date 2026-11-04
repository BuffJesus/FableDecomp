#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_data_input_stream.h"
#include "../99/trim_string_fixture.h"

static int overflowReads;
static int scanCalls;
static bool forceOverflow;

class CBankFileManagerTestAccess
{
public:
    static bool Read(CBankFileManager& manager, CCharString& text, CDataInputStream& stream)
    {
        return manager.ReadQuotedString(text, stream);
    }
};

CCharString& CCharString::operator=(const CCharString& other)
{
    if (&other != this)
    {
        UnassignString();
        storage_ = other.storage_;
        if (storage_) ++storage_->owners;
    }
    return *this;
}

CCharString CDataInputStream::ReadUntilCharacter(char delimiter, bool& found)
{
    ++scanCalls;
    const char* begin = reinterpret_cast<const char*>(CurrentSourcePtr);
    const char* end = begin + SourceChunkBytesRemaining;
    const char* cursor = begin;
    while (cursor < end && *cursor != delimiter) ++cursor;
    found = cursor < end;
    unsigned long length = static_cast<unsigned long>(cursor - begin);
    CCharString result;
    if (length)
        *reinterpret_cast<CCharStringData**>(&result) = makeData(begin, length);
    CurrentSourcePtr += length;
    SourceChunkBytesRemaining -= length;
    StreamPos += length;
    if (found && forceOverflow)
    {
        forceOverflow = false;
        SourceChunkBytesRemaining = 0;
    }
    return result;
}

void CDataInputStream::ReadWithSrcChunkOverflow(void* destination, long size)
{
    ++overflowReads;
    memcpy(destination, CurrentSourcePtr, size);
    CurrentSourcePtr += size;
    StreamPos += size;
    SourceChunkBytesRemaining = StreamSize - StreamPos;
}

static void checkCase(const char* source, const char* expected, bool expectedResult,
                      unsigned long expectedPosition, bool overflow)
{
    unsigned char managerBytes[sizeof(CBankFileManager)] = {0};
    CBankFileManager& manager = *reinterpret_cast<CBankFileManager*>(managerBytes);
    unsigned char streamBytes[sizeof(CDataInputStream)] = {0};
    CDataInputStream& stream = *reinterpret_cast<CDataInputStream*>(streamBytes);
    stream.StreamPos = 0;
    stream.StreamSize = static_cast<long>(strlen(source));
    stream.CurrentSourcePtr = reinterpret_cast<fable_u8*>(const_cast<char*>(source));
    stream.SourceChunkBytesRemaining = stream.StreamSize;
    CCharString text("keep");
    forceOverflow = overflow;
    bool result = CBankFileManagerTestAccess::Read(manager, text, stream);
    requireTrim(result == expectedResult && !strcmp(text.GetConstCharPtr(), expected) &&
                stream.StreamPos == expectedPosition);
}

int main()
{
    checkCase("missing", "keep", false, 7, false);
    checkCase("prefix \"file.bin\" suffix", "file.bin", true, 17, false);
    checkCase("\"unterminated", "unterminated", true, 13, false);
    checkCase("\"\"", "", true, 2, false);
    checkCase("\"fast\"", "fast", true, 6, true);
    requireTrim(scanCalls == 9 && overflowReads == 1);
    requireTrim(storageLive == 0 && g_CCharStringInstanceCount_013BD800 == 0);
    puts("BANK_READ_QUOTED_PASS");
    return 0;
}
