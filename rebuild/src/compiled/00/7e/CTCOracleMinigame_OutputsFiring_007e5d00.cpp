struct CTCOracleMinigame {
    char pad[0xC0];
    int outputs[4];
    bool OutputsFiring();
};

bool CTCOracleMinigame::OutputsFiring()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->outputs[i] != 0)
            return true;
    }
    return false;
}