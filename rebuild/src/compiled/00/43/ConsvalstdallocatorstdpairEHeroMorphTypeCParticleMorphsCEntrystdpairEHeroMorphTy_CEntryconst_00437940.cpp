namespace NPlayerGui
{
class CDrawDamage
{
public:
    void AddDamageChange(float amount, bool isDamage);
};
}

class CPlayerGui
{
public:
    unsigned char unused00[0x1C0];
    NPlayerGui::CDrawDamage* drawDamage;

    void AddDamageChange(float amount, bool isDamage);
};

void CPlayerGui::AddDamageChange(float amount, bool isDamage)
{
    drawDamage->AddDamageChange(amount, isDamage);
}
