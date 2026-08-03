class CStringParser
{
public:
    bool __fastcall CalcInQuotes(void* /*edx_unused*/, bool bSeed, char* p, char* pEnd);
};

bool __fastcall CStringParser::CalcInQuotes(void* /*edx_unused*/, bool bSeed, char* p, char* pEnd)
{
    bool bInQuotes = bSeed;
    while (p != pEnd)
    {
        char c = *p;
        if (c == '"' || c == '\'')
        {
            bInQuotes = !bInQuotes;
        }
        ++p;
    }
    return bInQuotes;
}