#include <stdio.h>
#include <string.h>

// --- Minimal standalone CCharString mirroring the retail 4-byte handle so the
// --- behaviour test links without the engine string runtime. ---
struct CharData
{
    char* text;
    unsigned long unknown04;
    unsigned long unknown08;
    unsigned char flags0C;
    long owners;
};

class CCharString
{
public:
    CCharString(const char* text, long length)
        : storage_(0)
    {
        if (text != 0 && text[0] != '\0')
        {
            long n = (length < 0) ? (long)strlen(text) : length;
            storage_ = new CharData;
            storage_->text = new char[n + 1];
            memcpy(storage_->text, text, n);
            storage_->text[n] = '\0';
            storage_->unknown04 = 0;
            storage_->unknown08 = 0;
            storage_->flags0C = 0;
            storage_->owners = 1;
        }
    }
    const char* c_str() const
    {
        return storage_ ? storage_->text : "";
    }

private:
    CharData* storage_;
};

// Standalone re-declaration of the leaf under test (same body as s.cpp).
class CEngineScreenEffectDisplacementRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineScreenEffectDisplacementRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("EnableScreenEffectDisplacement", -1);
}

int main()
{
    CEngineScreenEffectDisplacementRenderer r;
    CCharString name = r.GetConsoleEnableFunctionName();
    if (strcmp(name.c_str(), "EnableScreenEffectDisplacement") == 0)
    {
        printf("CONSOLE_ENABLE_NAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}