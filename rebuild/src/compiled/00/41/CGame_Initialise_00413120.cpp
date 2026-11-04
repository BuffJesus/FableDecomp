#include "fable_boot.h"
#include "fable_cgame.h"

bool CGame::Initialise()
{
    if (CInitBaseClass::Initialise())
    {
        Quit = false;
        CurrentGameComponent = 0;

        CCountedProgressDisplay countedDisplay(new CProgressDisplay);
        SetProgressDisplay(&countedDisplay);
        return true;
    }

    return false;
}
