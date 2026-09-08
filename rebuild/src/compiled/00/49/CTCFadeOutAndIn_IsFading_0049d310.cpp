// Retail 0x0049D310. Original members and offsets are from Ego_r.pdb.
struct CTCFadeOutAndIn {
    void* __vftable;
    unsigned char _baseState[8];
    bool FadingOut;
    bool FadingIn;
    unsigned char _alignment[2];
    float Alpha;
    long FramesLeftForFade;
    unsigned char FadingThings[16];
    bool IsFading() const;
};

bool CTCFadeOutAndIn::IsFading() const {
    return FadingIn || FadingOut;
}