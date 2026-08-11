// CEngineWeatherRenderer::GetConsoleEnableFunctionName
// retail 0x00b56650 : CCharString __fastcall (returns by value)
//   push esi ; esi = hidden return-buffer ptr [esp+8]
//   push -1 ; push <literal> ; ecx = retbuf ; call CCharString::CCharString(const char*, int)
//   eax = retbuf ; ret 4

class CCharString
{
public:
    CCharString(const char* s, int len);
private:
    char* m_data;
};

// A leaf class that carries this virtual getter.
class CEngineWeatherRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineWeatherRenderer::GetConsoleEnableFunctionName()
{
    return CCharString("EnableWeather", -1);
}