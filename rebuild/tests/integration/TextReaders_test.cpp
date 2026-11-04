// Behaviour fixture for CDataInputStream's text readers: ReadLine 0x00994B50,
// ReadUntilCharacter 0x00994DC0, ReadUntilWhiteSpace 0x00994E90 and
// ReadWhiteSpace 0x00994F60. Each is the real compiled provider. The stream
// is raw storage whose vtable has fixture SetPosition/GetPosition slots that
// seek within the test text; CCharString's members and the chunk-overflow
// read are fixture-side (strings are tracked by object address).
#include "fable_data_input_stream.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static const char* g_text;
static unsigned g_length;
struct Text { const void* owner; char chars[64]; unsigned length; };
static Text g_strings[256];
static unsigned g_stringCount, g_dtors;
static Text* Find(const void* owner)
{
    for (unsigned i = g_stringCount; i-- > 0;) if (g_strings[i].owner == owner) return &g_strings[i];
    return 0;
}
static Text* Make(const void* owner) { Text* t = &g_strings[g_stringCount < 255 ? g_stringCount++ : 255]; t->owner = owner; t->length = 0; return t; }
CCharString::CCharString() : storage_(0) { Make(this); }
CCharString::CCharString(const CCharString& other) : storage_(0) { Text* from = Find(&other); Text* to = Make(this); if (from) { memcpy(to->chars, from->chars, from->length); to->length = from->length; } }
CCharString::~CCharString() { ++g_dtors; }
const CCharString& CCharString::operator+=(char character) { Text* t = Find(this); if (t && t->length < 64) t->chars[t->length++] = character; return *this; }

void CDataInputStream::ReadWithSrcChunkOverflow(void* out, long size)
{
    memcpy(out, g_text + StreamPos, size);
    StreamPos += size;
    CurrentSourcePtr = reinterpret_cast<unsigned char*>(const_cast<char*>(g_text)) + StreamPos;
    SourceChunkBytesRemaining = 0;
}
// The vtable slots the readers call: [1] SetPosition, [2] GetPosition. A seek
// makes the rest of the text the current chunk again.
struct FakeStream
{
    void SetPos(unsigned long position)
    {
        CDataInputStream* s = reinterpret_cast<CDataInputStream*>(this);
        s->StreamPos = position;
        s->CurrentSourcePtr = reinterpret_cast<unsigned char*>(const_cast<char*>(g_text)) + position;
        s->SourceChunkBytesRemaining = g_length - position;
    }
    unsigned long GetPos() const { return reinterpret_cast<const CDataInputStream*>(this)->StreamPos; }
};
union SetSlot { void (FakeStream::*member)(unsigned long); void* address; };
union GetSlot { unsigned long (FakeStream::*member)() const; void* address; };
static void* g_vtable[3];

static CDataInputStream* Stream(unsigned char* raw, const char* text, unsigned length, unsigned position, unsigned chunk)
{
    g_text = text; g_length = length;
    g_stringCount = 0;
    memset(raw, 0xa5, sizeof(CDataInputStream) + 8);
    CDataInputStream* s = reinterpret_cast<CDataInputStream*>(raw + 4);
    *reinterpret_cast<void**>(s) = g_vtable;
    s->StreamPos = position; s->StreamSize = length;
    s->CurrentSourcePtr = reinterpret_cast<unsigned char*>(const_cast<char*>(text)) + position;
    s->SourceChunkStreamPos = position;
    s->SourceChunkBytesRemaining = chunk < length - position ? chunk : length - position;
    return s;
}
static int g_fail;
static void Expect(const CCharString& value, const char* want, int code, unsigned long position, unsigned long wantPosition)
{
    Text* t = Find(&value);
    const unsigned n = (unsigned)strlen(want);
    if (!t || t->length != n || memcmp(t->chars, want, n) || position != wantPosition)
    {
        printf("TEXT_READERS_FAIL %d got=%.*s pos=%lu want=%s pos=%lu\n", code, t ? (int)t->length : 0, t ? t->chars : "", position, want, wantPosition);
        g_fail = 1;
    }
}

int main()
{
    SetSlot set; set.member = &FakeStream::SetPos;
    GetSlot get; get.member = &FakeStream::GetPos;
    g_vtable[1] = set.address; g_vtable[2] = get.address;
    unsigned char raw[sizeof(CDataInputStream) + 8];
    int cases = 0;
    struct Line { const char* text; unsigned length; const char* want; unsigned end; };
    static const Line lines[] = {
        { "abc\r\ndef", 8, "abc", 5 }, { "abc\rdef", 7, "abc", 4 }, { "abc\ndef", 7, "abc", 4 },
        { "abc\0def", 7, "abc", 4 }, { "abc", 3, "abc", 3 }, { "ab\r", 3, "ab", 3 }, { "\r\n", 2, "", 2 }, { "", 0, "", 0 } };
    for (unsigned chunk = 0; chunk < 10; chunk += 3) for (unsigned i = 0; i < 8; ++i)
    {
        CDataInputStream* s = Stream(raw, lines[i].text, lines[i].length, 0, chunk);
        CCharString line = s->CDataInputStream::ReadLine();
        Expect(line, lines[i].want, 1, s->StreamPos, lines[i].end);
        ++cases;
    }
    for (unsigned chunk = 0; chunk < 10; chunk += 3)
    {
        bool found = true;
        CDataInputStream* s = Stream(raw, "key:value", 9, 0, chunk);
        CCharString key = s->CDataInputStream::ReadUntilCharacter(':', found);
        Expect(key, "key", 2, s->StreamPos, 3); if (!found) { puts("TEXT_READERS_FAIL found"); g_fail = 1; }
        s = Stream(raw, "novalue", 7, 2, chunk);
        found = true;
        CCharString rest = s->CDataInputStream::ReadUntilCharacter(':', found);
        Expect(rest, "value", 3, s->StreamPos, 7); if (found) { puts("TEXT_READERS_FAIL not found"); g_fail = 1; }
        s = Stream(raw, ":x", 2, 0, chunk);
        CCharString none = s->CDataInputStream::ReadUntilCharacter(':', found);
        Expect(none, "", 4, s->StreamPos, 0); if (!found) { puts("TEXT_READERS_FAIL immediate"); g_fail = 1; }
        s = Stream(raw, "word \tnext", 10, 0, chunk);
        CCharString word = s->CDataInputStream::ReadUntilWhiteSpace();
        Expect(word, "word", 5, s->StreamPos, 4);
        CCharString gap = s->CDataInputStream::ReadWhiteSpace();
        Expect(gap, " \t", 6, s->StreamPos, 6);
        CCharString next = s->CDataInputStream::ReadUntilWhiteSpace();
        Expect(next, "next", 7, s->StreamPos, 10);
        CCharString tail = s->CDataInputStream::ReadWhiteSpace();
        Expect(tail, "", 8, s->StreamPos, 10);
        s = Stream(raw, "\r\n\n x", 5, 0, chunk);
        CCharString blank = s->CDataInputStream::ReadWhiteSpace();
        Expect(blank, "\r\n\n ", 9, s->StreamPos, 4);
        cases += 9;
    }
    if (g_fail) return 1;
    printf("TEXT_READERS_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
