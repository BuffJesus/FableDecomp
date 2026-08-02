#include <stdio.h>
#include <wchar.h>

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString&);
    ~CWideString();
    CWideString& operator=(const CWideString& other);
    operator const wchar_t*() const;

private:
    void* storage_;
};

namespace
{
    struct TestStorage
    {
        const wchar_t* text;
    };

    TestStorage g_input = {L"Data\\lang\\English\\lang_settings.txt"};
    TestStorage g_context = {L"D:\\Fable"};
    TestStorage g_expanded = {
        L"D:\\Fable\\Data\\lang\\English\\lang_settings.txt"};

    unsigned int g_constructCalls;
    unsigned int g_copyConstructCalls;
    unsigned int g_contextCalls;
    unsigned int g_expandCalls;
    unsigned int g_assignCalls;
    unsigned int g_destroyCalls;
    unsigned int g_convertCalls;
    unsigned int g_attributesCalls;
    unsigned long g_attributeResult;

    unsigned long __stdcall GetAttributesStub(const wchar_t* pathname)
    {
        ++g_attributesCalls;
        if (wcscmp(pathname, g_expanded.text) != 0)
        {
            return 0xFFFFFFFFul;
        }
        return g_attributeResult;
    }
}

typedef unsigned long (__stdcall *FableGetFileAttributesWProc)(const wchar_t*);
extern "C" FableGetFileAttributesWProc
g_FableGetFileAttributesW_0143FDBC = GetAttributesStub;

CWideString::CWideString()
    : storage_(0)
{
    ++g_constructCalls;
}

CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_)
{
    ++g_copyConstructCalls;
}

CWideString::~CWideString()
{
    ++g_destroyCalls;
    storage_ = 0;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assignCalls;
    storage_ = other.storage_;
    return *this;
}

CWideString::operator const wchar_t*() const
{
    ++g_convertCalls;
    TestStorage* storage = static_cast<TestStorage*>(storage_);
    return storage ? storage->text : L"";
}

extern "C" CWideString __fastcall
FableGetCurrentPath_00997F80()
{
    ++g_contextCalls;
    CWideString result;
    *reinterpret_cast<void**>(&result) = &g_context;
    return result;
}

extern "C" CWideString __fastcall
ExpandPath(CWideString* pathname, CWideString* context)
{
    ++g_expandCalls;
    CWideString result;
    void** pathnameStorage = reinterpret_cast<void**>(pathname);
    void** contextStorage = reinterpret_cast<void**>(context);
    if (*pathnameStorage != &g_input || *contextStorage != &g_context)
    {
        *reinterpret_cast<void**>(&result) = 0;
    }
    else
    {
        *reinterpret_cast<void**>(&result) = &g_expanded;
    }
    return result;
}

// Candidate under test (folded in directly since this harness links the
// test object alone).
extern "C" bool __fastcall
FablePathExists_00999230(const CWideString& pathname)
{
    CWideString expanded;
    expanded = ExpandPath(
        const_cast<CWideString*>(&pathname),
        &FableGetCurrentPath_00997F80());

    if (g_FableGetFileAttributesW_0143FDBC(
            static_cast<const wchar_t*>(expanded)) != 0xFFFFFFFFul)
    {
        return true;
    }

    return false;
}

int main()
{
    CWideString pathname;
    *reinterpret_cast<void**>(&pathname) = &g_input;

    g_attributeResult = 0x20;
    if (!FablePathExists_00999230(pathname))
    {
        return 1;
    }

    g_attributeResult = 0xFFFFFFFFul;
    if (FablePathExists_00999230(pathname))
    {
        return 2;
    }

    printf("FABLETLC_PATH_EXISTS_00999230 PASS\n");
    return 0;
}