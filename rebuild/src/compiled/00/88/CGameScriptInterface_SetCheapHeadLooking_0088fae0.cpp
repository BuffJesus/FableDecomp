extern unsigned char g_cheapHeadLooking;
unsigned char g_cheapHeadLooking;
void __stdcall CGameScriptInterface_SetCheapHeadLooking(char val)
{
    g_cheapHeadLooking = (unsigned char)val;
}