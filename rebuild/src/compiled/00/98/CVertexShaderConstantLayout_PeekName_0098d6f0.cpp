// CVertexShaderConstantLayout::PeekName  @ 0098d6f0
// __fastcall, returns CCharString by value from a string literal.

class CCharString {
public:
    CCharString(const char* s, int count);
private:
    char* m_data;
};

class CVertexShaderConstantLayout {
public:
    CCharString PeekName();
};

CCharString CVertexShaderConstantLayout::PeekName()
{
    return CCharString("PeekName", -1);
}