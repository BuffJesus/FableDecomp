// Behaviour fixture for CAFile::CopyFileW (0x00998E70).
// Links the compiled source object; two in-memory CAFile implementations
// record every virtual call, and the progress display and the CCharString
// temporary it receives are recording stand-ins.
#define private public
#include "fable_filesystem.h"
#include "fable_progress_display.h"
#undef private

#include <stdio.h>
#include <string.h>
#include "test_support/base_class_stubs.h"

class CFileInformation
{
public:
    unsigned char bytes[0x58];
};

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}

CFileInformation CAFile::GetFileInformation() { CFileInformation info; return info; }

static char g_log[4096];
static void Log(const char* text)
{
    strcat(g_log, text);
}

static int g_strCtor, g_strDtor, g_progress;
static const char* g_progressText;
static float g_progressFraction;
static bool g_progressForce, g_progressDebug;

CCharString::CCharString(const char* text, long length)
{
    storage_ = reinterpret_cast<CCharStringData*>(const_cast<char*>(text));
    if (length != -1)
        Log("BADLEN;");
    ++g_strCtor;
}

CCharString::~CCharString()
{
    ++g_strDtor;
}

void FABLE_FASTCALL NProgressDisplay::DisplayProgress(
    const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    ++g_progress;
    g_progressText = reinterpret_cast<const char*>(text.storage_);
    g_progressFraction = fraction;
    g_progressForce = forceRender;
    g_progressDebug = isDebug;
    Log("P;");
}

class CTestFile : public CAFile
{
public:
    CTestFile(const char* name) : Name(name), Position(0), Length(0) {}

    virtual void Open(const CWideString&, FileAccessType, unsigned long) {}
    virtual void Close() {}
    virtual void Read(void* data, long size, CASuspendableProcess* process)
    {
        char line[64];
        sprintf(line, "%s.R%ld;", Name, size);
        Log(line);
        if (LastBuffer == 0)
        {
            // The very first chunk lands in a freshly zeroed buffer.
            for (long i = 0; i < 0x1000; ++i)
                if (static_cast<unsigned char*>(data)[i] != 0)
                    FirstBufferDirty = true;
        }
        LastBuffer = data;
        if (process != 0)
            Log("PROC;");
        memcpy(data, Bytes + Position, size);
        Position += size;
    }
    virtual void Write(const void* data, long size, CASuspendableProcess* process)
    {
        char line[64];
        sprintf(line, "%s.W%ld;", Name, size);
        Log(line);
        if (data != LastBuffer)
            Log("OTHERBUF;");
        if (process != 0)
            Log("PROC;");
        memcpy(Bytes + Position, data, size);
        Position += size;
    }
    virtual void SetPosition(long position)
    {
        char line[64];
        sprintf(line, "%s.SP%ld;", Name, position);
        Log(line);
        Position = position;
    }
    virtual void MovePosition(long) { Log("MOVE;"); }
    virtual long GetPosition() const { return Position; }
    virtual void SetLength(long length)
    {
        char line[64];
        sprintf(line, "%s.SL%ld;", Name, length);
        Log(line);
        Length = length;
    }
    virtual long GetLength() const { return Length; }
    virtual bool IsOpen() const { return true; }
    virtual CWideString GetPathname() const { Log("PATH;"); return Pathname; }
    virtual CAFile* Clone() const { return 0; }

    const char* Name;
    CWideString Pathname;
    long Position;
    long Length;
    unsigned char Bytes[12000];
    static void* LastBuffer;
    static bool FirstBufferDirty;
};

void* CTestFile::LastBuffer;
bool CTestFile::FirstBufferDirty;

static CTestFile g_src("S");
static CTestFile g_dst("D");

int main()
{
    int fail = 0;

    // 10000 bytes: chunks of 4096, 4096, 1808, one progress pump after each.
    for (int i = 0; i < 10000; ++i)
        g_src.Bytes[i] = (unsigned char)(i * 7 + 3);
    g_src.Length = 10000;
    g_src.Position = 55;
    g_dst.Position = 99;
    g_log[0] = 0;
    CTestFile::LastBuffer = 0;
    CTestFile::FirstBufferDirty = false;
    CAFile::CopyFileW(g_src, g_dst);
    const char* expected =
        "D.SL10000;D.SP0;S.SP0;"
        "S.R4096;D.W4096;P;"
        "S.R4096;D.W4096;P;"
        "S.R1808;D.W1808;P;";
    if (strcmp(g_log, expected) != 0) fail |= 1;
    if (memcmp(g_src.Bytes, g_dst.Bytes, 10000) != 0 || g_dst.Length != 10000) fail |= 2;
    if (CTestFile::FirstBufferDirty) fail |= 4;
    if (g_progress != 3 || g_strCtor != 3 || g_strDtor != 3) fail |= 8;
    if (g_progressText == 0 || g_progressText[0] != 0 || g_progressFraction != -1.0f ||
        g_progressForce || g_progressDebug) fail |= 16;

    // Exactly one buffer: a single chunk.
    g_src.Length = 4096;
    g_log[0] = 0;
    g_progress = 0;
    CAFile::CopyFileW(g_src, g_dst);
    if (strcmp(g_log, "D.SL4096;D.SP0;S.SP0;S.R4096;D.W4096;P;") != 0 || g_progress != 1) fail |= 32;

    // Empty source: sized and rewound, nothing streamed, no progress.
    g_src.Length = 0;
    g_log[0] = 0;
    g_progress = 0;
    CAFile::CopyFileW(g_src, g_dst);
    if (strcmp(g_log, "D.SL0;D.SP0;S.SP0;") != 0 || g_progress != 0) fail |= 64;

    if (fail == 0)
        printf("CAFILE_COPYFILE_PASS\n");
    else
        printf("CAFILE_COPYFILE_FAIL %d [%s]\n", fail, g_log);
    return 0;
}