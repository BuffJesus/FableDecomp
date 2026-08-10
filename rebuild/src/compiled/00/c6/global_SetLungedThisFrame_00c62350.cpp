extern int (__cdecl *g_getCount)();
extern float g_scale;
float SetLungedThisFrame()
{
    return (float)g_getCount() * g_scale;
}