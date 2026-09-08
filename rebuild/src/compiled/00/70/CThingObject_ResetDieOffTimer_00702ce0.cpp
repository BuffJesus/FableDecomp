struct CThingObjectDefRetail {
    unsigned char _retailDefinitionBase[0x94];
    long DieOffTimer; // Ego_r PDB +0x98; retail definition base is four bytes smaller.
};

struct CThingObjectRetail {
    unsigned char _retailThingBase[0xd0];
    CThingObjectDefRetail* PDef; // donor CThingObject::PDef is +0xe8
    long DieOffTimer;
    bool DiesOff;

    void ResetDieOffTimer();
};

void CThingObjectRetail::ResetDieOffTimer() {
    DieOffTimer = PDef->DieOffTimer;
    DiesOff = DieOffTimer > 0;
}