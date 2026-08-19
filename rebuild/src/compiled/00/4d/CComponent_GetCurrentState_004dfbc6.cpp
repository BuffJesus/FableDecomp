struct CUIState;

extern CUIState* __stdcall NUISystem_GetState(char* p);

CUIState* __fastcall CComponent_GetCurrentState(void* self)
{
    char local;
    return NUISystem_GetState(&local);
}