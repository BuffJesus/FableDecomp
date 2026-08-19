class CTCCreditsUI
{
public:
    char pad0[0xC];
    char flagC;
    char pad2[0x1C - 0xC - 1];
    char flag1C;

    void Deactivate();
};

void CTCCreditsUI::Deactivate()
{
    if (flagC)
    {
        flag1C = 1;
    }
}