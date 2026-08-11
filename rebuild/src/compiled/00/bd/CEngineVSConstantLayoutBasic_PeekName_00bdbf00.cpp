// CEngineVSConstantLayoutBasic::PeekName  @ 0x00bdbf00
// __fastcall, returns CCharString by value (hidden ret buffer as stack arg).
// Body: construct a CCharString from a fixed string literal with length -1.

class CCharString {
public:
    CCharString(const char* text, long length);
    // one pointer member so sizeof matches the engine's small string handle
    void* storage_;
};

class CEngineVSConstantLayoutBasic {
public:
    CCharString PeekName();
};

// The fixed literal the engine passes (address 0x012a7d48 in retail).
static const char* const kName = "CEngineVSConstantLayoutBasic";

CCharString CEngineVSConstantLayoutBasic::PeekName()
{
    return CCharString(kName, -1);
}