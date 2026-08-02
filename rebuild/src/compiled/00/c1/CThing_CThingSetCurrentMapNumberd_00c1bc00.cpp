class CThing {
public:
    char pad[0xc];
    short currentMapNumber;

    void SetCurrentMapNumber(short mapNumber);
};

void CThing::SetCurrentMapNumber(short mapNumber)
{
    currentMapNumber = mapNumber;
}