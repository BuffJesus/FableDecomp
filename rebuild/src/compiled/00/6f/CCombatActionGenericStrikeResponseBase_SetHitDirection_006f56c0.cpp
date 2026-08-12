struct C3DVector {
    float x, y, z;
};

struct CCombatAction_GenericStrikeResponseBase {
    char pad[0x128];
    C3DVector m_hitDir;
    void SetHitDirection(C3DVector v);
};

void CCombatAction_GenericStrikeResponseBase::SetHitDirection(C3DVector v)
{
    m_hitDir = v;
}