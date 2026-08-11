// CAIStateGroup_BuyFromSeller::BoughtSomething @ 007e5950
// __fastcall(this) + float stack arg; ret 4

extern int g_buyFromSellerInt; // int global @ 0x1375550 (fild = integer load)

struct CAIStateGroup_BuyFromSeller
{
    char pad[0xC];
    int  m_result; // +0xC

    void BoughtSomething(float amount);
};

void CAIStateGroup_BuyFromSeller::BoughtSomething(float amount)
{
    this->m_result = (int)((float)g_buyFromSellerInt * amount);
}