#pragma optimize("s",on)
#include "engine/CTattooDef.h"

struct CAppearanceModifierScalingDefRetail {
    unsigned char EarlierFields[0x40];
    float Attractiveness;
    float Scariness;
};
struct CAppearanceModifierScalingVectorRetail {
    CAppearanceModifierScalingDefRetail* Begin;
    CAppearanceModifierScalingDefRetail* End;
};
// Retail scaling entries are 0x48 bytes rather than the donor's 0x4c.
struct CTattooDefRetail {
    unsigned char EarlierFields[0x58];
    float Attractiveness;
    float Scariness;
    float GetTypeBalancedScariness(
        const CAppearanceModifierScalingVectorRetail& scaling) const;
};
float CTattooDefRetail::GetTypeBalancedScariness(
    const CAppearanceModifierScalingVectorRetail& scaling) const
{
    if (scaling.End - scaling.Begin == 5)
        return scaling.Begin[4].Scariness * Scariness;
    return Scariness;
}
