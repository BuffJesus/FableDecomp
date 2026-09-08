struct CCombatRing {
    unsigned char State[0x1c];
    void Reset();
};
struct CCombatRingVector {
    CCombatRing* Begin;
    CCombatRing* End;
    CCombatRing* CapacityEnd;
};
struct CCombatWheel {
    CCombatRingVector CombatRings;
    void ResetRings();
};
void CCombatWheel::ResetRings()
{
    for (CCombatRing* ring = CombatRings.Begin; ring != CombatRings.End; ++ring)
        ring->Reset();
}