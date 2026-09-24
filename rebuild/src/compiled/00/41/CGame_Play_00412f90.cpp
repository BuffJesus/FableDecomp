#include "fable_game.h"

// Functional C++ reconstruction: VC7.1 /O2 /Oy emits 394 bytes, with a
// two-byte ECX/EDX allocation residue at retail +0xEC/+0xEF. See the
// 2026-09-22 native-play journal and tools/decomp_pipeline/check_cgame_play.py.
// Accepted as functional, not byte-matching; the bootstrap gate pins this
// exact residue. The prior assembly oracle lives under src/asm_bake/.
void CGame::Play()
{
    CGameComponent* nextComponent = 0;
    if (g_FableCompileFrontendDefinitions_013B8648)
        CNewFrontendGameComponent::CompileDefs();

    if (g_FableStartMainGame_013B8605)
    {
        CMainGameComponentInit init;
        init.SaveGameName = g_FableMainGameStartupPath_013B7D5C;
        nextComponent = new CMainGameComponent(*this, init);
        nextComponent->Init();
        // Init consumes the strings before their reverse-order destruction.
    }
    else
    {
        CFrontendGameComponentInit init;
        init.value = 0;
        if (g_FableUseLegacyFrontend_013B8642)
            nextComponent = new CFrontendGameComponent(*this, init);
        else
            nextComponent = new CNewFrontendGameComponent(*this, init);
        nextComponent->Init();
    }

    // Retail re-reads this flag after Init, rather than caching it at entry.
    if (g_FableCompileFrontendDefinitions_013B8648)
    {
        delete nextComponent;
        return;
    }
    if (nextComponent)
        CurrentGameComponent = nextComponent;
    while (!Quit)
    {
        const bool keepRunning = CurrentGameComponent->Run(&nextComponent);
        // A component retired during Run is released before installing its
        // successor, including when Run requests shutdown.
        if (g_FableRetiredGameComponent_013B7D58)
        {
            delete g_FableRetiredGameComponent_013B7D58;
            g_FableRetiredGameComponent_013B7D58 = 0;
        }
        if (!keepRunning)
            Quit = true;
        else
            CurrentGameComponent = nextComponent;
    }
}
