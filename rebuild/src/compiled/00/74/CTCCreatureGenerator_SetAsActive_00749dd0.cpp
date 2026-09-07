struct CTCCreatureGenerator {
    char pad00[0x4c];
    bool active;
    char pad4d[0x0f];
    unsigned char activationState;
    void SetAsActive(bool value);
};
void CTCCreatureGenerator::SetAsActive(bool value) {
    active = value;
    activationState = 0;
}