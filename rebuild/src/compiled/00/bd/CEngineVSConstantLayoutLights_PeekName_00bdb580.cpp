// CEngineVSConstantLayoutLights::PeekName  @ 00bdb580
// Struct-return __fastcall: returns a CCharString by value built from a
// literal string pointer (0x012a7d2c) and length -1 (null-terminated).

struct CCharString {
    char* m_data;
    // __fastcall ctor(const char*, int len)
    CCharString(const char* s, int len);
};

class CEngineVSConstantLayoutLights {
public:
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutLights::PeekName()
{
    return CCharString((const char*)0x012a7d2c, -1);
}