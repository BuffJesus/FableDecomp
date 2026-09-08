// These offsets and member names are shared with Ego_r.pdb and the generated
// CTCHeroExperience header. The method removes half the currently spendable
// experience from both the spendable and lifetime totals.
struct CTCHeroExperience {
    unsigned char _base_0x00[0x10];
    long TotalExperiencePoints;
    long ExperiencePointsAvailableToSpend;

    void ApplyPenaltyForDeath();
};

void CTCHeroExperience::ApplyPenaltyForDeath() {
    const long penalty = ExperiencePointsAvailableToSpend / 2;
    ExperiencePointsAvailableToSpend -= penalty;
    TotalExperiencePoints -= penalty;
}