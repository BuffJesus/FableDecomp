struct CTCCreditsUI
{
    char pad[0x16];
    char flag16;
    char flag17;

    void Deactivate_2();
};

void CTCCreditsUI::Deactivate_2()
{
    if (flag16)
    {
        flag17 = 1;
    }
}