#pragma optimize("s",on)

struct CQuakeStrengthFloatMapRetail { unsigned char Storage[0x0c]; };
struct CQuakeLengthFloatMapRetail { unsigned char Storage[0x0c]; };
struct CPersistContext {
    void Transfer(CQuakeStrengthFloatMapRetail&);
    void Transfer(CQuakeLengthFloatMapRetail&);
};
struct CRumbleDefRetail {
    unsigned char Base[0x28];
    CQuakeStrengthFloatMapRetail QuakeIntensities;
    CQuakeLengthFloatMapRetail QuakeDurations;
    void Transfer(CPersistContext& context);
};
void CRumbleDefRetail::Transfer(CPersistContext& context) {
    context.Transfer(QuakeIntensities);
    context.Transfer(QuakeDurations);
}