#include <stdio.h>

class CWideString
{
public:
    CWideString();
    ~CWideString();

private:
    wchar_t* m_data;
};

int g_ctorCount = 0;
int g_dtorCount = 0;

CWideString::CWideString()
{
    ++g_ctorCount;
}

CWideString::~CWideString()
{
    ++g_dtorCount;
}

namespace
{
CWideString __fastcall ExpandPath(CWideString* in, CWideString* scratch);
}

class CAFile
{
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path);
};

CWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path)
{
    return ExpandPath(const_cast<CWideString*>(&path), &CWideString());
}

int g_expandCalls = 0;
CWideString* g_lastIn = 0;
CWideString* g_lastScratch = 0;

namespace
{
CWideString __fastcall ExpandPath(CWideString* in, CWideString* scratch)
{
    ++g_expandCalls;
    g_lastIn = in;
    g_lastScratch = scratch;
    return *in;
}
}

int main()
{
    CWideString pathBuf;

    int ctorBefore = g_ctorCount;
    int dtorBefore = g_dtorCount;

    CWideString result = CAFile::ConvertPathToFullPath(pathBuf);
    (void)result;

    int ok = 1;
    if (g_expandCalls != 1) ok = 0;
    if (g_lastIn != &pathBuf) ok = 0;
    if (g_lastScratch == 0) ok = 0;
    if (g_ctorCount <= ctorBefore) ok = 0;
    if (g_dtorCount <= dtorBefore) ok = 0;

    if (ok) {
        printf("CONVERTPATH_OK\n");
    } else {
        printf("CONVERTPATH_FAIL expand=%d ctor=%d dtor=%d\n", g_expandCalls, g_ctorCount, g_dtorCount);
    }
    return 0;
}