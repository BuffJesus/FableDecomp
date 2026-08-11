// CAIStateGroup_JackDragon::ResetTimers  @ 0x0094f9e0
// Faithful VC7.1 reconstruction.

int  GetCurrentTimeValue(void);        // 0x0049d870 : returns int in eax
void ResetHelper(void);                // 0x008fcfc0 : return discarded

struct CAIStateGroup_JackDragon
{
    char pad[0x4c];
    int  timerA;   // +0x4c
    int  timerB;   // +0x50
    char pad2[0x58 - 0x54];
    int  flag;     // +0x58

    void ResetTimers();
};

void CAIStateGroup_JackDragon::ResetTimers()
{
    this->flag = 0;
    ResetHelper();
    int t = GetCurrentTimeValue();
    this->timerA = t;
    this->timerB = t;
}