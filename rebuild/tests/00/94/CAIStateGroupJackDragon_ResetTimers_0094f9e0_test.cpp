#include <stdio.h>

int  GetCurrentTimeValue(void);
void ResetHelper(void);

struct CAIStateGroup_JackDragon
{
    char pad[0x4c];
    int  timerA;
    int  timerB;
    char pad2[0x58 - 0x54];
    int  flag;
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

static int g_helperCalled = 0;
void ResetHelper(void) { g_helperCalled = 1; }
int  GetCurrentTimeValue(void) { return 12345; }

int main()
{
    CAIStateGroup_JackDragon obj;
    obj.timerA = -1; obj.timerB = -1; obj.flag = 99;
    obj.ResetTimers();
    if (obj.flag == 0 && obj.timerA == 12345 && obj.timerB == 12345 && g_helperCalled == 1)
        printf("RESETTIMERS_OK\n");
    else
        printf("RESETTIMERS_FAIL\n");
    return (obj.flag == 0 && obj.timerA == 12345 && obj.timerB == 12345 && g_helperCalled) ? 0 : 1;
}