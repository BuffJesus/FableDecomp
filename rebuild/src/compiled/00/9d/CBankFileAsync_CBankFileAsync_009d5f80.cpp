#include "fable_game_text_bank.h"

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void*);

CBankFileAsync::CBankFileAsync()
{
    NonCached = false;
    DisableAsync = false;
    LastRestartingData = 0;
    InitializeCriticalSection(CriticalSection);
}