struct CHeroCombatDefRetail {
    unsigned char Base[0x136];
    bool ProjectileTargetingUseAnalogueZoom;
};
struct CTCHeroRangedAttacks {
    unsigned char Base[0x10];
    CHeroCombatDefRetail* PHeroCombatDef;
    unsigned char State[0x2c];
    float ProjectileWeaponZoomParameter;
    void ResetProjectileTargetingZoomParameter();
};
void CTCHeroRangedAttacks::ResetProjectileTargetingZoomParameter() {
    if (PHeroCombatDef != 0 && PHeroCombatDef->ProjectileTargetingUseAnalogueZoom)
        ProjectileWeaponZoomParameter = 0.0f;
    else
        ProjectileWeaponZoomParameter = 1.0f;
}