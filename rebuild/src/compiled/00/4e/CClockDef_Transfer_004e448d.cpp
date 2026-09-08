#pragma optimize("s",on)

struct CCharStringLongMapRetail { unsigned char Storage[0x0c]; };
enum EClockHandType { CLOCK_HAND_TYPE_DEFAULT = 0 };
struct CPersistContext {
    void Transfer(CCharStringLongMapRetail&);
    void Transfer(EClockHandType&);
};
struct CClockDefRetail {
    unsigned char Base[0x28];
    CCharStringLongMapRetail Sound;
    EClockHandType HandType;
    void Transfer(CPersistContext& context);
};
void CClockDefRetail::Transfer(CPersistContext& context) {
    context.Transfer(Sound);
    context.Transfer(HandType);
}