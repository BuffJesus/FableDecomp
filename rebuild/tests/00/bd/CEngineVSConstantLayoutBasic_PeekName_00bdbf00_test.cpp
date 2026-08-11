#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* text, long length);
    void* storage_;
};

class CEngineVSConstantLayoutBasic {
public:
    CCharString PeekName();
};

static const char* g_lastText = 0;
static long        g_lastLen  = 0;

CCharString::CCharString(const char* text, long length)
{
    g_lastText = text;
    g_lastLen  = length;
    storage_   = (void*)text;
}

static const char* const kName = "CEngineVSConstantLayoutBasic";

CCharString CEngineVSConstantLayoutBasic::PeekName()
{
    return CCharString(kName, -1);
}

int main()
{
    CEngineVSConstantLayoutBasic obj;
    CCharString s = obj.PeekName();

    if (g_lastText == 0 || std::strcmp(g_lastText, "CEngineVSConstantLayoutBasic") != 0) {
        std::printf("FAIL text\n");
        return 1;
    }
    if (g_lastLen != -1) {
        std::printf("FAIL len %ld\n", g_lastLen);
        return 1;
    }
    if (s.storage_ != (void*)g_lastText) {
        std::printf("FAIL storage\n");
        return 1;
    }
    std::printf("PEEKNAME_OK\n");
    return 0;
}