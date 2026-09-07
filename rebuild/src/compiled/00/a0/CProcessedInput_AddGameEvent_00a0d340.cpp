struct CGameEvent
{
    long type;
    unsigned char player;
    unsigned char data[0x23];
};

class CProcessedInput
{
public:
    unsigned char player;
    unsigned char unused01[3];
    long type;
    CGameEvent gameEvents[4];
    unsigned char noGameEvents;
    unsigned char unusedA9[3];
    long priority;

    void AddGameEvent(const CGameEvent& event);
};

void CProcessedInput::AddGameEvent(const CGameEvent& event)
{
    type = 1;
    unsigned char index = noGameEvents;
    gameEvents[index] = event;
    gameEvents[noGameEvents].player = player;
    ++noGameEvents;
}