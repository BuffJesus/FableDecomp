CRGBColour CPlayerManager::GetMultiplayerColour(long player_number) const
{
    switch (player_number)
    {
    case 0:
        return CRGBColour{0x00, 0x00, 0xFF, 0xFF};

    case 1:
        return CRGBColour{0xFF, 0x00, 0x00, 0xFF};

    case 2:
        return CRGBColour{0x00, 0xFF, 0xFF, 0xFF};

    case 3:
        return CRGBColour{0x00, 0xFF, 0x00, 0xFF};

    default:
        return CRGBColour{0xFF, 0xFF, 0xFF, 0xFF};
    }
}