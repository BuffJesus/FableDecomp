inline const long& Maximum(const long& left, const long& right)
{
    return left < right ? right : left;
}

// Member names and offsets come from the Ego_r PDB layout and agree with retail.
struct CTCHeroExperience {
    unsigned char BaseSubobject[0x10];
    long TotalExperiencePoints;
    long ExperiencePointsAvailableToSpend;

    void ForceSpendableExperienceTo(long amount);
};

void CTCHeroExperience::ForceSpendableExperienceTo(long amount)
{
    if (amount < 0)
        amount = 0;

    ExperiencePointsAvailableToSpend = amount;
    TotalExperiencePoints = Maximum(TotalExperiencePoints,
                                    ExperiencePointsAvailableToSpend);
}