struct FadeEntry {
    float min_;   // +0x3c relative
    float scale;  // +0x40 relative
};

struct CEngineLocalDetailGenerator {
    char pad[0x3c];
    FadeEntry entries[64]; // starts at +0x3c

    float ModifyFadeDistanceForVideoOptions(unsigned long index, float value);
};

float CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions(unsigned long index, float value)
{
    float product = value * entries[index].scale;
    if (product > entries[index].min_)
        return product;
    return entries[index].min_;
}