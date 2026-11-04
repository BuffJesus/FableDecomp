#include "fable_game.h"

// CGame::Play: build the first game component (the main game when a save or
// world was requested on the command line, otherwise one of the two front
// ends), then run components until one reports that the game should quit.
void CGame::Play()
{
    CGameComponent* component = 0;

    if (g_FableCompileFrontendDefinitions_013B8648)
    {
        CNewFrontendGameComponent::CompileDefs();
    }

    if (g_FableStartMainGame_013B8605)
    {
        CMainGameComponentInit init;
        init.SaveGameName = g_FableMainGameStartupPath_013B7D5C;
        component = new CMainGameComponent(*this, init);
        component->Init();
    }
    else
    {
        // Each front end calls Init in its own arm; the compiler merges the
        // two calls, and writing them this way is what gives retail's
        // register choice for the second constructor's argument.
        if (g_FableUseLegacyFrontend_013B8642)
        {
            CFrontendGameComponentInit init;
            init.Nowt = 0;
            component = new CFrontendGameComponent(*this, init);
            component->Init();
        }
        else
        {
            CNewFrontendGameComponentInit init;
            init.Nothing = 0;
            component = new CNewFrontendGameComponent(*this, init);
            component->Init();
        }
    }

    // Definition-compile runs only build the front end's definitions.
    if (g_FableCompileFrontendDefinitions_013B8648)
    {
        delete component;
        return;
    }

    if (component)
    {
        CurrentGameComponent = component;
    }

    // Run hands back the next component through the same local.
    while (!Quit)
    {
        const bool keepRunning = CurrentGameComponent->Run(&component);
        if (g_FableRetiredGameComponent_013B7D58)
        {
            delete g_FableRetiredGameComponent_013B7D58;
            g_FableRetiredGameComponent_013B7D58 = 0;
        }
        if (!keepRunning)
        {
            Quit = true;
        }
        else
        {
            CurrentGameComponent = component;
        }
    }
}
