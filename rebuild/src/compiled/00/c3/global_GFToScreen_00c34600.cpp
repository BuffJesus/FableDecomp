struct C2DBoxF {
    float TLX;
    float TLY;
    float BRX;
    float BRY;
};
namespace NPlayerGui {
C2DBoxF __fastcall GFToScreen(const C2DBoxF& box) {
    return box;
}
}