inline void SwapLong(long& left, long& right)
{
    long value = left;
    left = right;
    right = value;
}
struct C2DBoxI {
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    void ValidateOrientationAndFlip();
};
void C2DBoxI::ValidateOrientationAndFlip()
{
    if (BRX < TLX) SwapLong(TLX, BRX);
    if (BRY < TLY) SwapLong(TLY, BRY);
}