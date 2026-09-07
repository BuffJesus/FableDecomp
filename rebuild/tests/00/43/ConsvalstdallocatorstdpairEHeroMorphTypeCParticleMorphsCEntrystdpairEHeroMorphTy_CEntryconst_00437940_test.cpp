#include <cstdio>

struct DrawDamage
{
    float amount;
    bool isDamage;
    int calls;
    void AddDamageChange(float value, bool damage)
    {
        amount = value;
        isDamage = damage;
        ++calls;
    }
};
struct PlayerGui
{
    DrawDamage* drawDamage;
    void AddDamageChange(float amount, bool isDamage)
    {
        drawDamage->AddDamageChange(amount, isDamage);
    }
};
int main()
{
    DrawDamage draw = {0.0f, false, 0};
    PlayerGui gui = {&draw};
    gui.AddDamageChange(-17.25f, true);
    if (draw.calls != 1 || draw.amount != -17.25f || !draw.isDamage) return 1;
    gui.AddDamageChange(4.5f, false);
    if (draw.calls != 2 || draw.amount != 4.5f || draw.isDamage) return 2;
    std::puts("PASS_00437940");
    return 0;
}
