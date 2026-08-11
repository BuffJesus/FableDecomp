// CEngineShadowRenderer::GetConsoleEnableFunctionName
// Returns a CCharString by value, constructed from a constant C-string
// literal with length -1 (i.e. strlen). The engine method never touches
// 'this'; it just builds the constant name string in the hidden return
// buffer and returns that buffer pointer.
//
// CCharString: only the (const char*, long) ctor is declared here, kept
// out-of-line so the compiler emits it as the external call 0x99ebf0.
class CCharString {
public:
    CCharString(const char* str, long len);
private:
    char* m_data;
};

// The renderer class. Enough virtuals that GetConsoleEnableFunctionName is
// a real virtual member reached through the vtable; the body is constant.
class CEngineShadowRenderer {
public:
    virtual CCharString GetConsoleEnableFunctionName();
};

CCharString CEngineShadowRenderer::GetConsoleEnableFunctionName() {
    return CCharString("EnableShadows", -1);
}