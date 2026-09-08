// The owner and virtual signature are confirmed independently by retail RTTI.
// Retail stores the canonical layout name at 0x0129b9b8.
struct CCharString {
    int Data;
    CCharString(const char* text, int length);
};

extern char kVertexShaderConstantLayoutEmptyName[];

struct CVertexShaderConstantLayoutEmpty {
    CCharString PeekName() const;
};

CCharString CVertexShaderConstantLayoutEmpty::PeekName() const {
    return CCharString(kVertexShaderConstantLayoutEmptyName, -1);
}