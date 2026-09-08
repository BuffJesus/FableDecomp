#pragma optimize("s",on)
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);
};
struct CDrunkennessDefRetail {
    unsigned char Base[0x28];
    float DrunkennessThresholdMult;
    void Transfer(CPersistContext& context);
};
void CDrunkennessDefRetail::Transfer(CPersistContext& context) {
    const float defaultValue = 0.0f;
    context.Transfer(
        "DrunkennessThresholdMult",
        DrunkennessThresholdMult,
        defaultValue);
}