struct C3DVector {
    float x;
    float y;
    float z;
};

struct CCombatAction_GenericStrikeResponseBase {
    char pad[0x11c];
    C3DVector hitPos;

    void SetHitPosition(C3DVector pos);
};

void CCombatAction_GenericStrikeResponseBase::SetHitPosition(C3DVector pos)
{
    this->hitPos = pos;
}